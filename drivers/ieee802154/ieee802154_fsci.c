/*
 * Copyright (c) 2017 Linaro Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*============================================================================
                           INCLUDE FILES
=============================================================================*/
#define SYS_LOG_LEVEL CONFIG_SYS_LOG_IEEE802154_DRIVER_LEVEL
#define SYS_LOG_DOMAIN "ieee802154_fsci"

#include <logging/sys_log.h>

#include <errno.h>

#include <kernel.h>
#include <arch/cpu.h>

#include <board.h>
#include <device.h>
#include <drivers/console/uart_pipe.h>
#include <init.h>
#include <net/net_if.h>
#include <net/net_pkt.h>
#include <net/ieee802154_radio.h>

#include "../../subsys/net/ip/l2/ieee802154/ieee802154_frame.h"

#define gFsciHost_802_15_4_c        1
#define gMacUsePackedStructs_d      1
#define gPHY_802_15_4g_d            0

#include "ieee802154_fsci.h"
/*============================================================================
                           LOCAL CONSTANTS
=============================================================================*/
#define FSCI_TX_POWER_MIN		(-19)

/*============================================================================
                              LOCAL MACROS
=============================================================================*/
#define START_ADDR(ptr1, type1, member1) ((type1 *)((char *)(ptr1) - offsetof(type1, member1)))

#define SYS_SLIST_ENTRY(type)	\
struct {	\
	struct type *sle_next;	/* next element	*/ \
}

/*============================================================================
                            LOCAL TYPEDEFS
=============================================================================*/
struct fsci_context {
	struct net_if *iface;
	struct k_sem req_sem;
	struct k_mutex req_mutex;
	bool req_started;

	u8_t mac_addr[8];
	u8_t lqi;
	u16_t pan_id;
	u8_t channel;

	clientPacket_t fsci_pkt;
};

/*
 * The k_fifo interface routines uses a pointer field at the beginning of the
 * data item you send to it. We are going to be passing around FSCI packets so
 * this data structure is defined to account for the fields that would be used
 * by the k_fifo routines
 */
struct fsci_pkt_entry
{
	SYS_SLIST_ENTRY(fsci_pkt_entry) next;

	clientPacket_t  pkt;
};

#define MEM_ALIGNMENT 4
#define MEM_ALIGN_SIZE(_size,_alignment) 	(((_size) + (_alignment) - 1) & ~((_alignment) -1))
#define MEM_ALIGN_BUFFER(_size,_alignment)	(((_size) + (_alignment) - 1))
#define MEM_ALIGN(_addr,_alignment)	(void*)((uintptr_t)(_addr) + (_alignment) - 1) & ~(uintptr_t)((_alignment) - 1))

/*============================================================================
                         LOCAL FUNCTION PROTOTYPES
=============================================================================*/
/*============================================================================
                             LOCAL VARIABLES
=============================================================================*/
#define STACKSIZE   2048
static K_THREAD_STACK_DEFINE(stack, STACKSIZE);
static struct k_thread fsci_pkt_thread;

/*
 * TODO:
 * 
 * Instead of having local buffers to receive into we should use network buffers
 * to avoid a copy. Just ensure the FSCI header overhead is accounted for which 
 * would be the size of a mcpsDataInd_t.
 * 
 */
#define FSCI_PKT_CNT   6
#define FSCI_MAX_PKT_SIZE	(sizeof(clientPacketHdr_t) + gFsciMaxPayloadLen_c + gFsci_TailBytes_c)
static uint8_t pkt_buf[FSCI_PKT_CNT * MEM_ALIGN_SIZE(sizeof(struct fsci_pkt_entry), 4)] __attribute__ ((aligned (16)));

K_FIFO_DEFINE(fsci_pkts_queue);
K_FIFO_DEFINE(fsci_avail_queue);


/* Lookup table for TX_POWER */
static const u8_t tx_power_lt[22] = {
	2, 2, 2, 2, 2, 2,	/* -19:-14 dBm */
	4, 4, 4,		/* -13:-11 dBm */
	6, 6, 6,		/* -10:-8 dBm */
	8, 8,			/* -7:-6 dBm */
	10, 10,			/* -5:-4 dBm */
	12,			/* -3 dBm */
	14, 14,			/* -2:-1 dBm */
	18, 18,			/* 0:1 dBm */
	24			/* 2 dBm */
};

static struct fsci_context fsci_ctx;

/*============================================================================
                            GLOBAL VARIABLES
=============================================================================*/
/*============================================================================
                             LOCAL FUNCTIONS
=============================================================================*/
#define ENABLE_DUMPHEX	1

#ifdef ENABLE_DUMPHEX

static void dumphex(const char* msg, uint8_t *ptr, uint8_t size)
{
	int i, y;

	SYS_LOG_BACKEND_FN("%s:\n", msg);
	for (i = 0, y=0; i < size; i++)
	{
		SYS_LOG_BACKEND_FN("%02X ", ptr[i]);

		if (++y == 16)
		{
			SYS_LOG_BACKEND_FN("\n");
			y = 0;
		} else if (y == 8) {
			SYS_LOG_BACKEND_FN(" ");
		}
	}
	SYS_LOG_BACKEND_FN("\n");
}

#else

#define dumphex(_msg, _ptr, _size)

#endif

static void fsci_rx_data(clientPacket_t *fsci_pkt)
{
	struct fsci_context *fsci = &fsci_ctx;
	struct net_pkt *pkt = NULL;
	struct net_buf *frag = NULL;
	struct ieee802154_address_field *addr_field;
	struct ieee802154_fcf_seq *fs;
	u8_t *msdu;
	u8_t *buf;

	fsci_ctx.lqi = fsci_pkt->ind.data.mpduLinkQuality;
	net_pkt_print();
	pkt = net_pkt_get_reserve_rx(0, K_NO_WAIT);
	if (!pkt) {
		SYS_LOG_ERR("no RX net_buf available");
		goto out;
	}

	frag = net_pkt_get_frag(pkt, K_NO_WAIT);
	if (!frag) {
		SYS_LOG_ERR("No frag available");
		goto out;
	}

	net_pkt_frag_insert(pkt, frag);

	/*
	 * The KW40Z black-box interface is a 802.15.4 FullMAC implementation.
	 * The FSCI mcpsDataInd_t message include the information found in the 
	 * original 802.15.4 header. The following code will extract that information
	 * to recreate the 15.4 header before copying over the MAC payload and 
	 * passing the frame up to the 802.15.4 layer.
	 */
	buf = frag->data;
	fs = (struct ieee802154_fcf_seq *)buf;
	memset(fs, 0, sizeof(struct ieee802154_fcf_seq));
	buf += sizeof(struct ieee802154_fcf_seq);

	fs->fc.frame_type = IEEE802154_FRAME_TYPE_DATA;
	fs->fc.frame_version = IEEE802154_VERSION_802154_2006;
	fs->fc.pan_id_comp = 1;
	fs->sequence = fsci_pkt->ind.data.dsn;

#ifdef CONFIG_NET_L2_IEEE802154_SECURITY
	if (fsci_pkt->ind.data.securityLevel != gMacSecurityNone_c) {
		SYS_LOG_ERR("rx: MAC security is not supported");
		goto out;
	}
#endif

	addr_field = (struct ieee802154_address_field *)buf;
	fs->fc.dst_addr_mode = fsci_pkt->ind.data.dstAddrMode;
	addr_field->plain.pan_id = fsci_pkt->ind.data.dstPanId;
	buf += IEEE802154_PAN_ID_LENGTH;

	if (fsci_pkt->ind.data.dstAddrMode == gAddrModeShortAddress_c) {
		addr_field->plain.addr.short_addr = fsci_pkt->ind.data.dstAddr;
		buf += IEEE802154_SHORT_ADDR_LENGTH;
	} else { /* gAddrModeExtendedAddress_c */
		memcpy(addr_field->plain.addr.ext_addr, (u8_t *)&fsci_pkt->ind.data.dstAddr,
		       IEEE802154_EXT_ADDR_LENGTH);
		buf += IEEE802154_EXT_ADDR_LENGTH;
	}

	addr_field = (struct ieee802154_address_field *)buf;
	fs->fc.src_addr_mode = fsci_pkt->ind.data.srcAddrMode;

	if (!fs->fc.pan_id_comp) {
		addr_field->plain.pan_id = fsci_pkt->ind.data.srcPanId;
		buf += IEEE802154_PAN_ID_LENGTH;
	}

	if (fsci_pkt->ind.data.srcAddrMode == gAddrModeShortAddress_c) {
		if (fs->fc.pan_id_comp) {
			addr_field->comp.addr.short_addr = fsci_pkt->ind.data.srcAddr;
		} else {
			addr_field->plain.addr.short_addr = fsci_pkt->ind.data.srcAddr;
		}

		buf += IEEE802154_SHORT_ADDR_LENGTH;
	} else { /* gAddrModeExtendedAddress_c */
		if (fs->fc.pan_id_comp) {
			memcpy(addr_field->comp.addr.ext_addr,
			       (u8_t *)&fsci_pkt->ind.data.srcAddr,
			       IEEE802154_EXT_ADDR_LENGTH);
		} else {
			memcpy(addr_field->plain.addr.ext_addr,
			       (u8_t *)&fsci_pkt->ind.data.srcAddr,
			       IEEE802154_EXT_ADDR_LENGTH);
		}

		buf += IEEE802154_EXT_ADDR_LENGTH;
	}

	/* Copy over the MSDU Payload */
	msdu = & fsci_pkt->payload[0] + sizeof(mcpsDataInd_t);
	memcpy(buf, msdu, fsci_pkt->ind.data.msduLength);
	buf += fsci_pkt->ind.data.msduLength;

	net_buf_add(frag, (u8_t)(buf - frag->data));
	
	dumphex("rx netbuf", (uint8_t*)frag->data, 128);

	if (ieee802154_radio_handle_ack(fsci->iface, pkt) == NET_OK) {
		SYS_LOG_ERR("ACK packet handled");
		//SYS_LOG_DBG("ACK packet handled");
		goto out;
	}

	if (net_recv_data(fsci->iface, pkt) < 0) {
		SYS_LOG_DBG("Packet dropped by NET stack");
		goto out;
	}

	return;
out:
	if (pkt) {
		net_pkt_unref(pkt);
	}
}

static void fsci_rx_resp(clientPacket_t *fsci_pkt)
{
	memcpy(& fsci_ctx.fsci_pkt,(void*)fsci_pkt,
	       (sizeof(clientPacketHdr_t)) + fsci_pkt->header.len);

	dumphex("response packet", (uint8_t *)fsci_pkt, 16);
	
	fsci_ctx.req_started = false;
	
	k_sem_give(&fsci_ctx.req_sem);
}

uint32_t fsci_sync_error = 0;
static u8_t *fsci_rx(u8_t *buf, size_t *off)
{
	clientPacket_t *pkt = (void *) buf;
	struct fsci_pkt_entry  *fsci_pkt;

	if (buf[0] != 0x2)
	{
		fsci_sync_error++;
		SYS_LOG_ERR("Missing start marker (%d)... resyncing...", fsci_sync_error);
		/* TODO: need to search for it in buffer... Not going to happen with this
		 * UART because the FIFO is set to 1... i.e. we will service an interrupt
		 * for each and every byte.
		 */
		*off = 0;
		return buf;
	}

	if (*off < sizeof(clientPacketHdr_t))
	{
		return buf;
	}

	/*
	 * TODO: some instances of the FSCI have two bytes for length field.
	 */
	if (pkt->header.len > gFsciMaxPayloadLen_c)
	{
		SYS_LOG_ERR("Invalid packet length: 0x%02X", pkt->header.len);
		*off = 0;
		return buf;
	}

	/* Need to ensure that we have all of the payload and the checksum */
	if (*off < sizeof(clientPacketHdr_t) + pkt->header.len + gFsci_TailBytes_c)
	{
		return buf;
	}

	/*
	 * TODO:
	 * At this point we have a packet but we need to validate the start byte
	 * and the checksum. The simple test would start with start byte and leave
	 * the checksum to the thread.
	 *
	 * Need to think through how to resync with this style of UART interacing.
	 */
	fsci_pkt = k_fifo_get(&fsci_avail_queue, K_NO_WAIT);
	if (!fsci_pkt)
	{
		/*
		 * TODO: Change this to go ahead and pass along this particular packet
		 * and add logic to check for starved RX to replenish it with a packet.
		 * With this current logic, the current receive is going to be trashed
		 * and it is unclear what that will do... so balance the system.
		 *
		 * Check if this is a "response" and maybe we can do something about
		 * that at least.
		 */
		SYS_LOG_ERR("RX fsci_avail_queue exhaust. Dropped fsci pkt.");
		*off = 0;
		return buf;
	}

	k_fifo_put(&fsci_pkts_queue, START_ADDR(buf, struct fsci_pkt_entry, pkt));

	*off = 0;
	return (u8_t *)&fsci_pkt->pkt;
}

static int fsci_check_crc(clientPacket_t *pkt)
{
	u8_t crc, i;

	crc = pkt->header.opGroup ^
	      pkt->header.opCode ^
	      pkt->header.len;

	for (i = 0; i < pkt->header.len; i++) {
		crc ^= pkt->payload[i];
	}

	return (pkt->payload[pkt->header.len] == crc);
}

static void fsci_rx_handler(void *p1, void *p2, void *p3)
{
	while (1) {
		struct fsci_pkt_entry *fsci_pkt;

		fsci_pkt = k_fifo_get(&fsci_pkts_queue, K_FOREVER);
		SYS_LOG_DBG("Processing");

		if (fsci_pkt->pkt.header.startMarker != 0x02)
		{
			/*
			 * TODO: need to go into state of searching for 0x02
			 */
			SYS_LOG_ERR("Start marker not found (%d)!", fsci_pkt->pkt.header.startMarker);
			k_fifo_put(&fsci_avail_queue, fsci_pkt);
			continue;
		}

		/* DJLZORK */
		if (fsci_check_crc(&fsci_pkt->pkt) != TRUE)
		{
			SYS_LOG_ERR("Start marker not found (%d)!", fsci_pkt->pkt.header.startMarker);
			k_fifo_put(&fsci_avail_queue, fsci_pkt);
			continue;
		}

		if (fsci_pkt->pkt.header.opGroup == gFSCI_McpsNwkOpcodeGroup_c &&
			fsci_pkt->pkt.header.opCode == mFsciMcpsNwkDataInd_c)
		{
			fsci_rx_data(&fsci_pkt->pkt);
		} else {
			if (fsci_ctx.req_started) 
			{
				fsci_rx_resp((clientPacket_t*)&fsci_pkt->pkt);
			} else {
				SYS_LOG_ERR("Unexpected packet type: og=0x%02X oc=0x%02X",
						fsci_pkt->pkt.header.opGroup, fsci_pkt->pkt.header.opCode);
			}
		}

		k_fifo_put(&fsci_avail_queue, fsci_pkt);
	}
}

static void fsci_fill_crc(clientPacket_t *pkt)
{
	u8_t crc, i;

	crc = pkt->header.opGroup ^
	      pkt->header.opCode ^
	      pkt->header.len;

	for (i = 0; i < pkt->header.len; i++) {
		crc ^= pkt->payload[i];
	}

	pkt->payload[pkt->header.len] = crc;
}

static resultType_t fsci_send_payload(clientPacket_t *pkt)
{
	resultType_t	rslt;

	pkt->header.startMarker = 0x02;
	fsci_fill_crc(pkt);

	k_mutex_lock(&fsci_ctx.req_mutex, K_FOREVER);

	fsci_ctx.req_started = true;

	uart_pipe_send((u8_t *)&pkt->header,sizeof(clientPacketHdr_t));
	uart_pipe_send(pkt->payload, pkt->header.len + 1);

	/*
	 * Stall the thread to wait on the response for target
	 * TODO: K_FOREVER seems like a long time. Problem here is if we get an
	 * error on the response frame then we are hung up forever. So work through
	 * getting response back! Or fake a response.
	 */
#define FSCI_RSP_WAIT_TO	100
//#define FSCI_RSP_WAIT_TO	K_FOREVER
	if (k_sem_take(&fsci_ctx.req_sem, FSCI_RSP_WAIT_TO) == -EAGAIN)
	{
		SYS_LOG_ERR("TIMEOUT WAITING ON FSCI RESPONSE!");
		rslt = ~gSuccess_c;
		fsci_ctx.req_started = false;
	} else {
		/*
		 * When (or if) the interface gets more complicated and we start pushing through 
		 * more types of requests, this part will likely become more detailed to handle
		 * the various status/information types. For right now, there is only two basic
		 * types we are dealing with:
		 *     data send confirmation: [handle, status]
		 *     reset/start/set: [status]
		 */
		if ((fsci_ctx.fsci_pkt.header.opGroup == gFSCI_McpsNwkOpcodeGroup_c) &&
		    (fsci_ctx.fsci_pkt.header.opCode == mFsciMcpsNwkDataCnf_c))
		{ 
			//mcpsDataCnf_t *dcnf_p = (mcpsDataCnf_t*) &fsci_ctx.fsci_pkt.payload[0];
			rslt = fsci_ctx.fsci_pkt.cnf.data.status;
		}
		else
		{
			rslt = fsci_ctx.fsci_pkt.status;
		}
	}

	k_mutex_unlock(&fsci_ctx.req_mutex);

	return rslt;
}

static resultType_t fsci_mac_reset(bool reset_pib)
{
	resultType_t rslt;
	clientPacket_t pkt;

	pkt.header.opGroup = gFSCI_NwkMlmeOpcodeGroup_c;
	pkt.header.opCode = mFsciNwkMlmeResetReq_c;
	pkt.header.len = sizeof(mlmeResetReq_t);

	pkt.req.reset.setDefaultPIB = reset_pib;

	rslt = fsci_send_payload(&pkt);

    SYS_LOG_DBG("setDefaultPIB: %d, status: %d", reset_pib, rslt);

	return rslt;
}

static resultType_t fsci_mac_start(void)
{
	resultType_t rslt;
	clientPacket_t pkt;

	pkt.header.opGroup = gFSCI_NwkMlmeOpcodeGroup_c;
	pkt.header.opCode = mFsciNwkMlmeStartReq_c;
	pkt.header.len = sizeof(mlmeStartReq_t);

	memset(pkt.payload, 0, sizeof(mlmeStartReq_t));
	UNALIGNED_PUT(fsci_ctx.pan_id, &pkt.req.start.panId);

	pkt.req.start.logicalChannel = fsci_ctx.channel;
	pkt.req.start.beaconOrder = 0x0F;
	pkt.req.start.superframeOrder = 0x0F;
	pkt.req.start.panCoordinator = 0x01;

    SYS_LOG_DBG("logicalChannel: %d, beaconOrder: %X, superframeOrder: %X, panCoordinator: %d",
        pkt.req.start.logicalChannel,
        pkt.req.start.beaconOrder,
        pkt.req.start.superframeOrder,
        pkt.req.start.panCoordinator);

	rslt = fsci_send_payload(&pkt);

	return rslt;
}

static resultType_t fsci_set_pib_attribute(pibId_t attr_id, u8_t *val, u8_t len)
{
	resultType_t rslt;
	clientPacket_t pkt;
	u8_t *attr_value;
	u8_t i;

	pkt.header.opGroup = gFSCI_NwkMlmeOpcodeGroup_c;
	pkt.header.opCode = mFsciNwkMlmeSetReq_c;
	pkt.header.len = 2 + len;

	pkt.req.set.pibAttribute = attr_id;
	pkt.req.set.pibAttributeIndex = 0;

	attr_value = (u8_t *)&pkt.req.set.pibAttributeValue;
	for (i = 0; i < len; i++) {
		attr_value[i] = val[i];
	}

	rslt = fsci_send_payload(&pkt);

	return rslt;
}

static resultType_t fsci_set_short_addr_helper(u16_t short_addr)
{
	return fsci_set_pib_attribute(gMPibShortAddress_c, (u8_t *)&short_addr,
				      IEEE802154_SHORT_ADDR_LENGTH);
}

static resultType_t fsci_set_channel_helper(u8_t channel)
{
	return fsci_set_pib_attribute(gMPibLogicalChannel_c, &channel,
				      sizeof(u8_t));
}

static resultType_t fsci_set_pan_id_helper(u16_t pan_id)
{
	return fsci_set_pib_attribute(gMPibPanId_c, (u8_t *)&pan_id,
				      IEEE802154_PAN_ID_LENGTH);
}

static resultType_t fsci_set_ext_addr(const u8_t *ext_addr)
{
	resultType_t rslt;
	clientPacket_t pkt;

	pkt.header.opGroup = gFSCI_ReqOpcodeGroup_c;
	pkt.header.opCode = mFsciMsgWriteExtendedAdrReq_c;
	pkt.header.len = IEEE802154_EXT_ADDR_LENGTH;
	memcpy(pkt.payload, ext_addr, IEEE802154_EXT_ADDR_LENGTH);

	rslt = fsci_send_payload(&pkt);

	return rslt;
}

static resultType_t fsci_set_power_level(s16_t power_level)
{
	resultType_t rslt;
	clientPacket_t pkt;

	pkt.header.opGroup = gFSCI_AppAspOpcodeGroup_c;
	pkt.header.opCode = aspMsgTypeSetPowerLevel_c;
	pkt.header.len = sizeof(aspSetPowerLevelReq_t);

	pkt.req.pwr.powerLevel = power_level;

	rslt = fsci_send_payload(&pkt);

	return rslt;
}

static resultType_t fsci_mac_data_tx(u8_t *payload, u8_t len)
{
	resultType_t rslt;
	clientPacket_t pkt;
	struct ieee802154_address_field *addr_field;
	struct ieee802154_address *addr;
	struct ieee802154_fcf_seq *fs;
	static u8_t handle;
	u8_t *buf;

	pkt.header.opGroup = gFSCI_NwkMcpsOpcodeGroup_c;
	pkt.header.opCode = mFsciNwkMcpsDataReq_c;

	memset(&pkt.req.data, 0, sizeof(mcpsDataReq_t));

	/* Build the mcpsDataReq_t SAP structure */
	pkt.req.data.msduHandle = handle++;

	/*
	 * The KW40Z black-box interface is a 802.15.4 FullMAC implementation.
	 * The FSCI mFsciNwkMcpsDataReq_c request includes the mcpsDataReq_t
	 * structure that is populated with the information needed for the 
	 * 802.15.4 header.
	 * 
	 * The frame passed to this routine is an 802.15.4 frame so the following
	 * code has to extract the 15.4 header information to populate the 
	 * mcpsDataReq_t. The KW40Z black-box will then reconstitute the 802.15.4
	 * header on the otherside before sending the frame on the air.
	 */
	fs = (struct ieee802154_fcf_seq *)payload;
	buf = payload + sizeof(struct ieee802154_fcf_seq);

	if (fs->fc.frame_type != IEEE802154_FRAME_TYPE_DATA) {
		SYS_LOG_ERR("tx: unsupported frame type %d", fs->fc.frame_type);
		return gInvalidParameter_c;
	}

	pkt.req.data.txOptions = gMacTxOptionsNone_c;

	if (fs->fc.frame_pending) {
		pkt.req.data.txOptions |= gMacTxOptionFramePending_c;
	}

	if (fs->fc.ar) {
		pkt.req.data.txOptions |= gMacTxOptionsAck_c;
	}

	addr_field = (struct ieee802154_address_field *)buf;
	addr = (struct ieee802154_address *)&addr_field->plain.addr;
	pkt.req.data.dstAddrMode = fs->fc.dst_addr_mode;
	pkt.req.data.dstPanId = addr_field->plain.pan_id;
	buf += IEEE802154_PAN_ID_LENGTH;

	if (pkt.req.data.dstAddrMode == gAddrModeShortAddress_c) {
		pkt.req.data.dstAddr = addr->short_addr;
		buf += IEEE802154_SHORT_ADDR_LENGTH;
	} else { /* gAddrModeExtendedAddress_c */
		memcpy(&pkt.req.data.dstAddr, addr->ext_addr, IEEE802154_EXT_ADDR_LENGTH);
		buf += IEEE802154_EXT_ADDR_LENGTH;
	}

	addr_field = (struct ieee802154_address_field *)buf;
	pkt.req.data.srcAddrMode = fs->fc.src_addr_mode;

	if (!fs->fc.pan_id_comp) {
		pkt.req.data.srcPanId = addr_field->plain.pan_id;
		buf += IEEE802154_PAN_ID_LENGTH;
		addr = (struct ieee802154_address *)&addr_field->plain.addr;
	} else {
		addr = (struct ieee802154_address *)&addr_field->comp.addr;
	}

	if (pkt.req.data.srcAddrMode == gAddrModeShortAddress_c) {
		pkt.req.data.srcAddr = addr->short_addr;
		buf += IEEE802154_SHORT_ADDR_LENGTH;
	} else { /* gAddrModeExtendedAddress_c */
		memcpy(&pkt.req.data.srcAddr, addr->ext_addr, IEEE802154_EXT_ADDR_LENGTH);
		buf += IEEE802154_EXT_ADDR_LENGTH;
	}

#ifdef CONFIG_NET_L2_IEEE802154_SECURITY
	if (fs->fc.security_enabled) {
		SYS_LOG_ERR("tx: MAC security is not supported");
		return gInvalidParameter_c;
	}
#endif

	pkt.req.data.msduLength = len - (u8_t)(buf - payload);
	pkt.header.len = pkt.req.data.msduLength + sizeof(mcpsDataReq_t);
	
	/* 
	 * At this point, buf points to the MAC payload of the 802.15.4 frame.
	 * This is copied into the FSCI data request.
	 */
	memcpy(pkt.payload + sizeof(mcpsDataReq_t), buf, pkt.req.data.msduLength);
	
	rslt = fsci_send_payload(&pkt);

	SYS_LOG_DBG("send_payload result: %02X", rslt);

	return rslt;
}

static int fsci_cca(struct device *dev)
{
	//SYS_LOG_DBG("ENTRY"); // Basically called before every TX

	ARG_UNUSED(dev);

	/* No CCA command, fsci_tx will return busy if needed */
	return 0;
}

static int fsci_set_channel(struct device *dev, u16_t channel)
{
	struct fsci_context *fsci = dev->driver_data;

	SYS_LOG_DBG("channel: %d", channel);

	if (channel == fsci->channel) {
		return 0;
	}

	fsci->channel = channel;

	return fsci_set_channel_helper(channel) != gSuccess_c ? -EIO : 0;
}

static int fsci_set_pan_id(struct device *dev, u16_t pan_id)
{
	struct fsci_context *fsci = dev->driver_data;

	SYS_LOG_DBG("pan_id: %d", pan_id);

	if (pan_id == fsci->pan_id) {
		return 0;
	}

	fsci->pan_id = pan_id;

	return fsci_set_pan_id_helper(pan_id) != gSuccess_c ? -EIO : 0;
}

static int fsci_set_short_addr(struct device *dev, u16_t short_addr)
{
	SYS_LOG_DBG("short_addr: %d", short_addr);

	ARG_UNUSED(dev);

	return fsci_set_short_addr_helper(short_addr) != gSuccess_c ? -EIO : 0;
}

static int fsci_set_ieee_addr(struct device *dev, const u8_t *ieee_addr)
{
	SYS_LOG_DBG("ENTRY");

	ARG_UNUSED(dev);

	return fsci_set_ext_addr(ieee_addr) != gSuccess_c ? -EIO : 0;
}

static int fsci_set_txpower(struct device *dev, s16_t dbm)
{
	u8_t power_level;

	SYS_LOG_DBG("dbm: %d", dbm);

	ARG_UNUSED(dev);

	power_level = tx_power_lt[dbm - FSCI_TX_POWER_MIN];

	return fsci_set_power_level(power_level) != gSuccess_c ? -EIO : 0;
}

static u8_t fsci_get_lqi(struct device *dev)
{
	struct fsci_context *fsci = dev->driver_data;

	SYS_LOG_DBG("ENTRY");

	return fsci->lqi;
}

static int fsci_tx(struct device *dev,
		   struct net_pkt *pkt,
		   struct net_buf *frag)
{
	u8_t payload_len = net_pkt_ll_reserve(pkt) + frag->len;
	u8_t *payload = frag->data - net_pkt_ll_reserve(pkt);

	SYS_LOG_DBG("payload_len: %d", payload_len);
	dumphex("tx packet", payload, (payload_len<96) ? payload_len : 96);


	ARG_UNUSED(dev);

	return fsci_mac_data_tx(payload, payload_len) != gSuccess_c ? -EIO : 0;
}

static int fsci_start(struct device *dev)
{
	resultType_t ret;

	SYS_LOG_DBG("ENTRY");

	ARG_UNUSED(dev);

	ret = fsci_mac_start();
	if (ret == gNoShortAddress_c) {
		/* MAC will not start if ShortAddress is not set */
		if (fsci_set_short_addr(dev, 0x0000) != 0) {
	        SYS_LOG_ERR("MAC will not start if ShortAddress is not set");
			return -EIO;
		}

		ret = fsci_mac_start();
	}

	return ret != gSuccess_c ? -EIO : 0;
}

static int fsci_stop(struct device *dev)
{
	SYS_LOG_DBG("ENTRY");

	ARG_UNUSED(dev);

	/* No command for stop, so using reset */
	return fsci_mac_reset(false) != gSuccess_c ? -EIO : 0;
}

static int fsci_init(struct device *dev)
{
	int i;
	struct fsci_context *fsci = dev->driver_data;
	struct fsci_pkt_entry	*fsci_pkt;

	SYS_LOG_DBG("ENTRY: %d", FSCI_MAX_PKT_SIZE);

	k_sem_init(&fsci->req_sem, 0, 1);
	k_mutex_init(&fsci->req_mutex);

	for (i = 0; i < FSCI_PKT_CNT; i++) {
		k_fifo_put(&fsci_avail_queue, &pkt_buf[i * MEM_ALIGN_SIZE(sizeof(struct fsci_pkt_entry),4)]);
	}

	k_thread_create(&fsci_pkt_thread, stack, STACKSIZE, fsci_rx_handler,
			NULL, NULL, NULL, K_PRIO_COOP(6), 0, K_NO_WAIT);

	fsci->req_started = false;

	fsci->pan_id = 0;
	fsci->channel = gLogicalChannel26_c;

	SYS_LOG_DBG("pan_id: %d, channel: %d", fsci->pan_id, fsci->channel);

	fsci_pkt = k_fifo_get(&fsci_avail_queue, K_NO_WAIT);
	uart_pipe_register((uint8_t*)&fsci_pkt->pkt, FSCI_MAX_PKT_SIZE, fsci_rx);

	return fsci_mac_reset(true) != gSuccess_c ? -EIO : 0;
}

static inline u8_t *get_mac(struct device *dev)
{
	struct fsci_context *fsci = dev->driver_data;
	u32_t *ptr = (u32_t *)(fsci->mac_addr);

	UNALIGNED_PUT(sys_rand32_get(), ptr);
	ptr = (u32_t *)(fsci->mac_addr + 4);
	UNALIGNED_PUT(sys_rand32_get(), ptr);

	fsci->mac_addr[0] = (fsci->mac_addr[0] & ~0x01) | 0x02;

	return fsci->mac_addr;
}

static void fsci_iface_init(struct net_if *iface)
{
	struct device *dev = net_if_get_device(iface);
	struct fsci_context *fsci = dev->driver_data;
	u8_t *mac = get_mac(dev);

	net_if_set_link_addr(iface, mac, 8, NET_LINK_IEEE802154);
	fsci->iface = iface;
	ieee802154_init(iface);
}

static struct ieee802154_radio_api fsci_radio_api = {
	.iface_api.init     = fsci_iface_init,
	.iface_api.send     = ieee802154_radio_send,

	.cca                = fsci_cca,
	.set_channel        = fsci_set_channel,
	.set_pan_id         = fsci_set_pan_id,
	.set_short_addr     = fsci_set_short_addr,
	.set_ieee_addr      = fsci_set_ieee_addr,
	.set_txpower        = fsci_set_txpower,
	.tx                 = fsci_tx,
	.start              = fsci_start,
	.stop               = fsci_stop,
	.get_lqi            = fsci_get_lqi,
};

NET_DEVICE_INIT(fsci_802_15_4, CONFIG_IEEE802154_FSCI_DRV_NAME,
		fsci_init, &fsci_ctx, NULL,
		CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
		&fsci_radio_api, IEEE802154_L2,
		NET_L2_GET_CTX_TYPE(IEEE802154_L2), 125);
