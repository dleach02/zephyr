#ifndef __IEEE802154_FSCI_H__
#define __IEEE802154_FSCI_H__
/*!
 * @file   {...}
 * @brief  {...}
 *
 * {... longer description ...}
 *
 * @todo
 * {... paragraph ...}
 *
 * @bug {... description ...}
 * @warning
 * {... paragraph ...}
 *
 */
/*
 * Copyright (c) 2017 NXP Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
* \file
*
* This is the private header file for the MAC FSCI cmmands.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*=============================================================================
Copyright (c) 2017 NXP Semiconductors.

THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT BE USED OR
DISTRIBUTED WITHOUT WRITTEN PERMISSION OF NXP Semiconductors
=============================================================================*/
/*============================================================================
                             INCLUDE FILES
=============================================================================*/
/*============================================================================
                              CONSTANTS
=============================================================================*/
/*
 * The FSCI messaging uses a simple framing with a starting marker, group ID,
 * opcode ID, length, payload, and check sum. 
 * 
 * +------+---------+--------+-----+---------+--------+
 * |  1   +    1    +    1   +  1  +   <n>   +    1   +
 * +------+---------+--------+-----+---------+--------+
 * | 0x02 | opGroup | opCode | len | payload | chksum |
 * +------+---------+--------+-----+---------+--------+
 * 
 */

/*
 * Opcode Groups to specify which SAP Handler a packet in intended for 
 * or received from.
 */
#define gFSCI_ReservedOpGroup_c          0x52
#define gFSCI_MlmeNwkOpcodeGroup_c       0x84    /* MLME_NWK_SapHandler          */
#define gFSCI_NwkMlmeOpcodeGroup_c       0x85    /* NWK_MLME_SapHandler          */
#define gFSCI_McpsNwkOpcodeGroup_c       0x86    /* MCPS_NWK_SapHandler          */
#define gFSCI_NwkMcpsOpcodeGroup_c       0x87    /* NWK_MCPS_SapHandler          */
#define gFSCI_AspAppOpcodeGroup_c        0x94    /* ASP_APP_SapHandler           */
#define gFSCI_AppAspOpcodeGroup_c        0x95    /* APP_ASP_SapHandler           */
#define gFSCI_ReqOpcodeGroup_c           0xA3    /* FSCI utility Requests         */
#define gFSCI_CnfOpcodeGroup_c           0xA4    /* FSCI utility Confirmations/Indications    */
#define gFSCI_LoggingOpcodeGroup_c       0xB0    /* FSCI data logging utillity    */


/*
 * Operation Codes used in packets sent/received from the Serial Interface
 */
enum { /* MLME_NWK_SapHandler() OG = 0x84 */
    mFsciMlmeNwkAssociateInd_c       = 0x00,
    mFsciMlmeNwkAssociateCnf_c       = 0x01,
    mFsciMlmeNwkDisassociateInd_c    = 0x02,
    mFsciMlmeNwkDisassociateCnf_c    = 0x03,
    mFsciMlmeNwkBeaconNotifyInd_c    = 0x04,
    mFsciMlmeNwkGetCnf_c             = 0x05,
    mFsciMlmeNwkGtsInd_c             = 0x06,
    mFsciMlmeNwkGtsCnf_c             = 0x07,
    mFsciMlmeNwkOrphanInd_c          = 0x08,
    mFsciMlmwNwkResetCnf_c           = 0x09,
    mFsciMlmeNwkRxEnableCnf_c        = 0x0A,
    mFsciMlmeNwkScanCnf_c            = 0x0B,
    mFsciMlmeNwkCommStatusInd_c      = 0x0C,
    mFsciMlmeNwkSetCnf_c             = 0x0D,
    mFsciMlmeNwkStartCnf_c           = 0x0E,
    mFsciMlmeNwkSyncLossInd_c        = 0x0F,
    mFsciMlmeNwkPollCnf_c            = 0x10,
    mFsciMlmeNwkErrorCnf_c           = 0x11,
    mFsciMlmeNwkBeaconStartInd_c     = 0x12,
    mFsciMlmeNwkMaintenanceScanCnf_c = 0x13,
    mFsciMlmeNwkPollNotifyInd_c      = 0x14,
    mFsciMlmeNwkSetSlotframeCnf_c    = 0x15,
    mFsciMlmeNwkSetLinkCnf_c         = 0x16,
    mFsciMlmeNwkTschModeCnf_c        = 0x17,
    mFsciMlmeNwkKeepAliveCnf_c       = 0x18,
    mFsciMlmeNwkBeaconCnf_c          = 0x19,
};

enum { /* NWK_MLME_SapHandler() OG = 0x85*/
    mFsciNwkMlmeAssociateReq_c    = 0x00,
    mFsciNwkMlmeAssociateRes_c    = 0x01,
    mFsciNwkMlmeDisassociateReq_c = 0x02,
    mFsciNwkMlmeGetReq_c          = 0x03,
    mFsciNwkMlmeGtsReq_c          = 0x04,
    mFsciNwkMlmeOrphanRes_c       = 0x05,
    mFsciNwkMlmeResetReq_c        = 0x06,
    mFsciNwkMlmeRxEnableReq_c     = 0x07,
    mFsciNwkMlmeScanReq_c         = 0x08,
    mFsciNwkMlmeSetReq_c          = 0x09,
    mFsciNwkMlmeStartReq_c        = 0x0A,
    mFsciNwkMlmeSyncReq_c         = 0x0B,
    mFsciNwkMlmePollReq_c         = 0x0C,
    mFsciNwkMlmeSetSlotframeReq_c = 0x0D,
    mFsciNwkMlmeSetLinkReq_c      = 0x0E,
    mFsciNwkMlmeTschModeReq_c     = 0x0F,
    mFsciNwkMlmeKeepAliveReq_c    = 0x10,
    mFsciNwkMlmeBeaconReq_c       = 0x11,
};

enum { /* NWK_MCPS_SapHandler() OG = 0x87 */
    mFsciNwkMcpsDataReq_c  = 0x00,
    mFsciNwkMcpsPurgeReq_c = 0x01,
};

enum { /* MCPS_NWK_SapHandler() OG = 0x86 */
    mFsciMcpsNwkDataCnf_c  = 0x00,
    mFsciMcpsNwkDataInd_c  = 0x01,
    mFsciMcpsNwkPurgeCnf_c = 0x02,
    mFsciMcpsNwkPromInd_c  = 0x03,
};

/*! ASP messages. This enum matches with the FSCI OpCode used by ASP*/
enum { /* ASP OG = 0x95 */
    aspMsgTypeGetTimeReq_c          = 0x00,
    aspMsgTypeGetInactiveTimeReq_c  = 0x01,
    aspMsgTypeGetMacStateReq_c      = 0x02,
    aspMsgTypeDozeReq_c             = 0x03,
    aspMsgTypeAutoDozeReq_c         = 0x04,
    aspMsgTypeAcomaReq_c            = 0x05,
    aspMsgTypeHibernateReq_c        = 0x06,
    aspMsgTypeWakeReq_c             = 0x07,
    aspMsgTypeEventReq_c            = 0x08,
    aspMsgTypeClkoReq_c             = 0x09,
    aspMsgTypeSetXtalTrimReq_c      = 0x0A,
    aspMsgTypeDdrReq_c              = 0x0B,
    aspMsgTypePortReq_c             = 0x0C,
    aspMsgTypeSetMinDozeTimeReq_c   = 0x0D,
    aspMsgTypeSetNotifyReq_c        = 0x0E,
    aspMsgTypeSetPowerLevel_c       = 0x0F,
    aspMsgTypeTelecTest_c           = 0x10,
    aspMsgTypeTelecSetFreq_c        = 0x11,
    aspMsgTypeGetInactiveTimeCnf_c  = 0x12,
    aspMsgTypeGetMacStateCnf_c      = 0x13,
    aspMsgTypeDozeCnf_c             = 0x14,
    aspMsgTypeAutoDozeCnf_c         = 0x15,
    aspMsgTypeTelecSendRawData_c    = 0x16,
    aspMsgTypeSetFADState_c         = 0x17,
    aspMsgTypeSetFADThreshold_c     = 0x18,
    aspMsgTypeGetFADThreshold_c     = 0x19,
    aspMsgTypeGetFADState_c         = 0x1A,
    aspMsgTypeSetActivePromState_c  = 0x1B,
    aspMsgTypeXcvrWriteReq_c        = 0x1C,
    aspMsgTypeXcvrReadReq_c         = 0x1D,
    aspMsgTypeGetPowerLevel_c       = 0x1F,
    aspMsgTypeSetANTXState_c        = 0x20,
    aspMsgTypeGetANTXState_c        = 0x21,
    aspMsgTypeSetLQIMode_c          = 0x22,
    aspMsgTypeGetRSSILevel_c        = 0x23,
    aspMsgTypeSetMpmConfig_c        = 0x24,
    aspMsgTypeGetMpmConfig_c        = 0x25,
    aspMsgTypeSetTxInterval_c       = 0x30,
    aspMsgTypeGetXtalTrimReq_c      = 0x31
};


/* TRUE/FALSE definition*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* null pointer definition*/
#ifndef NULL
#define NULL (( void * )( 0x0UL ))
#endif

#if !defined(gFsciIncluded_c)
#define gFsciIncluded_c           0 /* Enable/Disable FSCI module */
#endif

#ifndef gFsciMaxOpGroups_c
#define gFsciMaxOpGroups_c       8
#endif

#ifndef gFsciMaxInterfaces_c
#define gFsciMaxInterfaces_c      1
#endif

#ifndef gFsciMaxVirtualInterfaces_c
#define gFsciMaxVirtualInterfaces_c      0
#endif

#ifndef gFsciMaxPayloadLen_c
#define gFsciMaxPayloadLen_c    245 /* bytes */
#endif

#ifndef gFsciLenHas2Bytes_c
#define gFsciLenHas2Bytes_c       0 /* boolean */
#endif

#ifndef gFsciUseEscapeSeq_c
#define gFsciUseEscapeSeq_c       0 /* boolean */
#endif

#ifndef gFsciUseFmtLog_c
#define gFsciUseFmtLog_c          0 /* boolean */
#endif

#ifndef gFsciUseFileDataLog_c
#define gFsciUseFileDataLog_c     0 /* boolean */
#endif

#ifndef gFsciTimestampSize_c
#define gFsciTimestampSize_c      0 /* bytes */
#endif

#ifndef gFsciLoggingInterface_c
#define gFsciLoggingInterface_c   1 /* [0..gFsciMaxInterfaces_c) */
#endif

#ifndef gFsciHostSupport_c
#define gFsciHostSupport_c        0 /* boolean */
#endif

#ifndef gFsciHostSyncUseEvent_c
#define gFsciHostSyncUseEvent_c   0 /* boolean */
#endif

#ifndef gFsciTxAck_c
#define gFsciTxAck_c              0 /* boolean */
#endif

#ifndef gFsciRxAck_c
#define gFsciRxAck_c              0 /* boolean */
#endif

#ifndef gFsciRxTimeout_c
#define gFsciRxTimeout_c          1 /* boolean */
#endif

#define mFsciInvalidInterface_c   (0xFF)

/* Used for maintaining backward compatibillity */
#define gFSCI_McpsSapId_c  1
#define gFSCI_MlmeSapId_c  2
#define gFSCI_AspSapId_c   3
#define gFSCI_NldeSapId_c  4
#define gFSCI_NlmeSapId_c  5
#define gFSCI_AspdeSapId_c 6
#define gFSCI_AfdeSapId_c  7
#define gFSCI_ApsmeSapId_c 8
#define gFSCI_ZdpSapId_c   9


/* Aditional bytes added by FSCI to a packet */
#if gFsciMaxVirtualInterfaces_c
#define gFsci_TailBytes_c (2)
#else
#define gFsci_TailBytes_c (1)
#endif


#if gFsciHost_802_15_4_c
#ifndef gMaxPHYPacketSize_c
#ifdef gPHY_802_15_4g_d
#define gMaxPHYPacketSize_c 254
#else
#define gMaxPHYPacketSize_c 125
#endif
#endif
#endif


typedef uint8_t logicalChannelId_t;
/*! Logical channel values */
typedef enum {
#ifdef gPHY_802_15_4g_d
  gLogicalChannel0_c = 0,
  gLogicalChannel1_c = 1,
  gLogicalChannel2_c = 2,
  gLogicalChannel3_c = 3,
  gLogicalChannel4_c = 4,
  gLogicalChannel5_c = 5,
  gLogicalChannel6_c = 6,
  gLogicalChannel7_c = 7,
  gLogicalChannel8_c = 8,
  gLogicalChannel9_c = 9,
  gLogicalChannel10_c = 10,
#endif //gPHY_802_15_4g_d  
  gLogicalChannel11_c = 11,
  gLogicalChannel12_c = 12,
  gLogicalChannel13_c = 13,
  gLogicalChannel14_c = 14,
  gLogicalChannel15_c = 15,
  gLogicalChannel16_c = 16,
  gLogicalChannel17_c = 17,
  gLogicalChannel18_c = 18,
  gLogicalChannel19_c = 19,
  gLogicalChannel20_c = 20,
  gLogicalChannel21_c = 21,
  gLogicalChannel22_c = 22,
  gLogicalChannel23_c = 23,
  gLogicalChannel24_c = 24,
  gLogicalChannel25_c = 25,
  gLogicalChannel26_c = 26,
#ifdef gPHY_802_15_4g_d  
  gLogicalChannel27_c = 27,
  gLogicalChannel28_c = 28,
  gLogicalChannel29_c = 29,
  gLogicalChannel30_c = 30,
  gLogicalChannel31_c = 31,
  gLogicalChannel32_c = 32,
  gLogicalChannel33_c = 33,
  gLogicalChannel34_c = 34,
  gLogicalChannel35_c = 35,
  gLogicalChannel36_c = 36,
  gLogicalChannel37_c = 37,
  gLogicalChannel38_c = 38,
  gLogicalChannel39_c = 39,
  gLogicalChannel40_c = 40,
  gLogicalChannel41_c = 41,
  gLogicalChannel42_c = 42,
  gLogicalChannel43_c = 43,
  gLogicalChannel44_c = 44,
  gLogicalChannel45_c = 45,
  gLogicalChannel46_c = 46,
  gLogicalChannel47_c = 47,
  gLogicalChannel48_c = 48,
  gLogicalChannel49_c = 49,
  gLogicalChannel50_c = 50,
  gLogicalChannel51_c = 51,
  gLogicalChannel52_c = 52,
  gLogicalChannel53_c = 53,
  gLogicalChannel54_c = 54,
  gLogicalChannel55_c = 55,
  gLogicalChannel56_c = 56,
  gLogicalChannel57_c = 57,
  gLogicalChannel58_c = 58,
  gLogicalChannel59_c = 59,
  gLogicalChannel60_c = 60,
  gLogicalChannel61_c = 61,
  gLogicalChannel62_c = 62,
  gLogicalChannel63_c = 63,
  gLogicalChannel64_c = 64,
  gLogicalChannel65_c = 65,
  gLogicalChannel66_c = 66,
  gLogicalChannel67_c = 67,
  gLogicalChannel68_c = 68,
  gLogicalChannel69_c = 69,
  gLogicalChannel70_c = 70,
  gLogicalChannel71_c = 71,
  gLogicalChannel72_c = 72,
  gLogicalChannel73_c = 73,
  gLogicalChannel74_c = 74,
  gLogicalChannel75_c = 75,
  gLogicalChannel76_c = 76,
  gLogicalChannel77_c = 77,
  gLogicalChannel78_c = 78,
  gLogicalChannel79_c = 79,
  gLogicalChannel80_c = 80,
  gLogicalChannel81_c = 81,
  gLogicalChannel82_c = 82,
  gLogicalChannel83_c = 83,
  gLogicalChannel84_c = 84,
  gLogicalChannel85_c = 85,
  gLogicalChannel86_c = 86,
  gLogicalChannel87_c = 87,
  gLogicalChannel88_c = 88,
  gLogicalChannel89_c = 89,
  gLogicalChannel90_c = 90,
  gLogicalChannel91_c = 91,
  gLogicalChannel92_c = 92,
  gLogicalChannel93_c = 93,
  gLogicalChannel94_c = 94,
  gLogicalChannel95_c = 95,
  gLogicalChannel96_c = 96,
  gLogicalChannel97_c = 97,
  gLogicalChannel98_c = 98,
  gLogicalChannel99_c = 99,
  gLogicalChannel100_c = 100,
  gLogicalChannel101_c = 101,
  gLogicalChannel102_c = 102,
  gLogicalChannel103_c = 103,
  gLogicalChannel104_c = 104,
  gLogicalChannel105_c = 105,
  gLogicalChannel106_c = 106,
  gLogicalChannel107_c = 107,
  gLogicalChannel108_c = 108,
  gLogicalChannel109_c = 109,
  gLogicalChannel110_c = 110,
  gLogicalChannel111_c = 111,
  gLogicalChannel112_c = 112,
  gLogicalChannel113_c = 113,
  gLogicalChannel114_c = 114,
  gLogicalChannel115_c = 115,
  gLogicalChannel116_c = 116,
  gLogicalChannel117_c = 117,
  gLogicalChannel118_c = 118,
  gLogicalChannel119_c = 119,
  gLogicalChannel120_c = 120,
  gLogicalChannel121_c = 121,
  gLogicalChannel122_c = 122,
  gLogicalChannel123_c = 123,
  gLogicalChannel124_c = 124,
  gLogicalChannel125_c = 125,
  gLogicalChannel126_c = 126,
  gLogicalChannel127_c = 127,
  gLogicalChannel128_c = 128,
#endif /*gPHY_802_15_4g_d  */
} logicalChannelId_tag;

#ifdef gPHY_802_15_4g_d
typedef uint32_t channelMask_t[5];
#else
typedef uint32_t channelMask_t;
#endif /*gPHY_802_15_4g_d*/

/*! \brief These values can be also be used for the other 3 bytes of the channel masks
For instance {0x00, 0x00, gChannelMask00_c, 0x00} represents gChannelMask32_c */
typedef enum
{
#ifdef gPHY_802_15_4g_d
    gChannelMask00_c             = 0x00000001,  /*!< Channel 0 bit-mask */
    gChannelMask01_c             = 0x00000002,  /*!< Channel 1 bit-mask */
    gChannelMask02_c             = 0x00000004,  /*!< Channel 2 bit-mask */
    gChannelMask03_c             = 0x00000008,  /*!< Channel 3 bit-mask */
    gChannelMask04_c             = 0x00000010,  /*!< Channel 4 bit-mask */
    gChannelMask05_c             = 0x00000020,  /*!< Channel 5 bit-mask */
    gChannelMask06_c             = 0x00000040,  /*!< Channel 6 bit-mask */
    gChannelMask07_c             = 0x00000080,  /*!< Channel 7 bit-mask */
    gChannelMask08_c             = 0x00000100,  /*!< Channel 8 bit-mask */
    gChannelMask09_c             = 0x00000200,  /*!< Channel 9 bit-mask */
    gChannelMask10_c             = 0x00000400,  /*!< Channel 10 bit-mask */
#endif   
    gChannelMask11_c             = 0x00000800,  /*!< Channel 11 bit-mask */
    gChannelMask12_c             = 0x00001000,  /*!< Channel 12 bit-mask */
    gChannelMask13_c             = 0x00002000,  /*!< Channel 13 bit-mask */
    gChannelMask14_c             = 0x00004000,  /*!< Channel 14 bit-mask */
    gChannelMask15_c             = 0x00008000,  /*!< Channel 15 bit-mask */
    gChannelMask16_c             = 0x00010000,  /*!< Channel 16 bit-mask */
    gChannelMask17_c             = 0x00020000,  /*!< Channel 17 bit-mask */
    gChannelMask18_c             = 0x00040000,  /*!< Channel 18 bit-mask */
    gChannelMask19_c             = 0x00080000,  /*!< Channel 19 bit-mask */
    gChannelMask20_c             = 0x00100000,  /*!< Channel 20 bit-mask */
    gChannelMask21_c             = 0x00200000,  /*!< Channel 21 bit-mask */
    gChannelMask22_c             = 0x00400000,  /*!< Channel 22 bit-mask */
    gChannelMask23_c             = 0x00800000,  /*!< Channel 23 bit-mask */
    gChannelMask24_c             = 0x01000000,  /*!< Channel 24 bit-mask */
    gChannelMask25_c             = 0x02000000,  /*!< Channel 25 bit-mask */
    gChannelMask26_c             = 0x04000000,  /*!< Channel 26 bit-mask */
#ifdef gPHY_802_15_4g_d
    gChannelMask27_c             = 0x08000000,  /*!< Channel 27 bit-mask */
    gChannelMask28_c             = 0x10000000,  /*!< Channel 28 bit-mask */
    gChannelMask29_c             = 0x20000000,  /*!< Channel 29 bit-mask */
    gChannelMask30_c             = 0x40000000,  /*!< Channel 30 bit-mask */
    gChannelMask31_c             = 0x80000000,  /*!< Channel 31 bit-mask */
#endif
}channelMask_tag;

/*! \brief The MAC Channel Page Ids */
typedef enum{
    gChannelPageId0_c               = 0x00,        /*!< 
                                                        - Channel 0 is in 868 MHz band using BPSK
                                                        - Channels 1 to 10 are in 915 MHz band using BPSK
                                                        - Channels 11 to 26 are in 2.4 GHz band using O-QPSK*/
    gChannelPageId1_c               = 0x01,        /*!< 
                                                       - Channel 0 is in 868 MHz band using ASK
                                                       - Channels 1 to 10 are in 915 MHz band using ASK
                                                       - Channels 11 to 26 are Reserved*/
    gChannelPageId2_c               = 0x02,        /*!< 
                                                       - Channel 0 is in 868 MHz band using O-QPSK
                                                       - Channels 1 to 10 are in 915 MHz band using O-QPSK
                                                       - Channels 11 to 26 are Reserved*/
#ifdef gPHY_802_15_4g_d    
    gChannelPageId9_c               = 0x09,        /*!< 802.15.4g standard-defined SUN Phy Modes */
#endif
} channelPageId_t;


/* Define the message types that Fsci recognizes and/or generates. */
enum {
    mFsciMsgModeSelectReq_c                 = 0x00, /* Fsci-ModeSelect.Request.              */
    mFsciMsgGetModeReq_c                    = 0x02, /* Fsci-GetMode.Request.                 */

    mFsciMsgAFResetReq_c                    = 0x05, /* Fsci-AFReset.Request.                 */
    mFsciMsgAPSResetReq_c                   = 0x06, /* Fsci-APSReset.Request.                */
    mFsciMsgAPSReadyReq_c                   = 0x07, /* Fsci-SetAPSReady.Request.             */
    mFsciMsgResetCPUReq_c                   = 0x08, /* Fsci-CPU_Reset.Request.               */
    mFsciMsgDeregisterEndPoint_c            = 0x0A, /* Fsci-DeregisterEndPoint.Request       */
    mFsciMsgRegisterEndPoint_c              = 0x0B, /* Fsci-RegisterEndPoint.Request         */
    mFsciMsgGetNumberOfEndPoints_c          = 0x0C, /* Fsci-GetNumberOfEndPoints.Request     */
    mFsciMsgGetEndPointDescription_c        = 0x0D, /* Fsci-GetEndPointDescription.Request   */
    mFsciMsgGetEndPointIdList_c             = 0x0E, /* Fsci-GetEndPointIdList.Request        */
    mFsciMsgSetEpMaxWindowSize_c            = 0x0F, /* Fsci-SetEpMaxWindowSize.Request       */
    mFsciMsgGetICanHearYouList_c            = 0x10, /* Fsci-GetICanHearYouList.Request       */
    mFsciMsgSetICanHearYouList_c            = 0x11, /* Fsci-SetICanHearYouList.Request       */
    mFsciMsgGetChannelReq_c                 = 0x12, /* Fsci-GetChannel.Request               */
    mFsciMsgSetChannelReq_c                 = 0x13, /* Fsci-SetChannel.Request               */
    mFsciMsgGetPanIDReq_c                   = 0x14, /* Fsci-GetPanID.Request                 */
    mFsciMsgSetPanIDReq_c                   = 0x15, /* Fsci-SetPanID.Request                 */
    mFsciMsgGetPermissionsTable_c           = 0x16, /* Fsci-GetPermissionsTable.Request      */
    mFsciMsgSetPermissionsTable_c           = 0x17, /* Fsci-SetPermissionsTable.Request      */
    mFsciMsgRemoveFromPermissionsTable_c    = 0x18, /* Fsci-RemoveFromPermissionsTable.Request    */
    mFsciMsgAddDeviceToPermissionsTable_c   = 0x19, /* Fsci-AddDeviceToPermissionsTable.Request   */
    mFsciMsgApsmeGetIBReq_c                 = 0x20, /* Fsci-GetIB.Request, aka APSME-GET.Request  */
    mFsciMsgApsmeSetIBReq_c                 = 0x21, /* Fsci-SetIB.Request, aka APSME-SET.Request  */
    mFsciMsgNlmeGetIBReq_c                  = 0x22, /* Fsci-GetIB.Request, aka NLME-GET.Request   */
    mFsciMsgNlmeSetIBReq_c                  = 0x23, /* Fsci-SetIB.Request, aka NLME-SET.Request   */
    mFsciMsgGetNumOfMsgsReq_c               = 0x24, /* Fsci-Get number of msgs available request  */
    mFsciMsgFreeDiscoveryTablesReq_c        = 0x25, /* Fsci-FreeDiscoveryTables.Request           */
    mFsciMsgSetJoinFilterFlagReq_c          = 0x26, /* Fsci-SetJoinFilterFlag.Request             */
    mFsciMsgGetMaxApplicationPayloadReq_c   = 0x27, /* Fsci-GetMaxApplicationPayload.Request      */

    mFsciOtaSupportSetModeReq_c             = 0x28,
    mFsciOtaSupportStartImageReq_c          = 0x29,
    mFsciOtaSupportPushImageChunkReq_c      = 0x2A,
    mFsciOtaSupportCommitImageReq_c         = 0x2B,
    mFsciOtaSupportCancelImageReq_c         = 0x2C,
    mFsciOtaSupportSetFileVerPoliciesReq_c  = 0x2D,
    mFsciOtaSupportAbortOTAUpgradeReq_c     = 0x2E,
    mFsciOtaSupportImageChunkReq_c          = 0x2F,
    mFsciOtaSupportQueryImageReq_c          = 0xC2,
    mFsciOtaSupportQueryImageRsp_c          = 0xC3,
    mFsciOtaSupportImageNotifyReq_c         = 0xC4,
    mFsciOtaSupportGetClientInfo_c          = 0xC5,            

    mFsciEnableBootloaderReq_c              = 0xCF,
    
    mFsciLowLevelMemoryWriteBlock_c         = 0x30, /* Fsci-WriteRAMMemoryBlock.Request     */
    mFsciLowLevelMemoryReadBlock_c          = 0x31, /* Fsci-ReadMemoryBlock.Request         */
    mFsciLowLevelPing_c                     = 0x38, /* Fsci-Ping.Request                    */

    mFsciMsgGetApsDeviceKeyPairSet_c        = 0x3B,  /* Fsci-GetApsDeviceKeyPairSet         */
    mFsciMsgGetApsDeviceKey_c               = 0x3C,
    mFsciMsgSetApsDeviceKey_c               = 0x3D,
    mFsciMsgSetApsDeviceKeyPairSet_c        = 0x3E,
    mFsciMsgClearApsDeviceKeyPairSet_c      = 0x3F,

    mFsciMsgAllowDeviceToSleepReq_c         = 0x70, /* Fsci-SelectWakeUpPIN.Request         */
    mFsciMsgWakeUpIndication_c              = 0x71, /* Fsci-WakeUp.Indication               */
    mFsciMsgGetWakeUpReasonReq_c            = 0x72, /*                */
#if gBeeStackIncluded_d
    mFsciMsgSetApsDeviceKeyPairSetKeyInfo   = 0x40,
    mFsciMsgSetApsOverrideApsEncryption     = 0x41,
    mFsciMsgSetPollRate                     = 0x43,
    mFsciMsgSetRejoinConfigParams           = 0x44,

    mFsciMsgSetFaMaxIncomingErrorReports_c  = 0x4A,

    mFsciMsgSetHighLowRamConcentrator       = 0x50,
    
    mFsciMsgEZModeComissioningStart                     = 0x51,  /* EZ mode, Touchlink  procedure */
    mFsciMsgZllTouchlinkCommissioningStart_c            = 0x52,
    mFsciMsgZllTouchlinkCommissioningConfigure_c        = 0x53,
    mFsciMsgZllTouchlinkGetListOfCommissionedDevices_c  = 0x54,
    mFsciMsgZllTouchlinkRemoveEntry_c                   = 0x55,
    mFsciMsgClearNeighborTableEntry_c                   = 0x56,
#endif
    mFsciGetUniqueId_c                      = 0xB0,
    mFsciGetMcuId_c                         = 0xB1,
    mFsciGetSwVersions_c                    = 0xB2,

    mFsciMsgAddToAddressMapPermanent_c      = 0xC0,
    mFsciMsgRemoveFromAddressMap_c          = 0xC1,

    mFsciMsgWriteNwkMngAddressReq_c         = 0xAD, /* Fsci-WriteNwkMngAddr.Request         */

    mFsciMsgReadExtendedAdrReq_c            = 0xD2, /* Fsci-ReadExtAddr.Request             */
    mFsciMsgReadNwkMngAddressReq_c          = 0xDA, /* Fsci-ReadNwkMngAddr.Request          */
    mFsciMsgWriteExtendedAdrReq_c           = 0xDB, /* Fsci-WriteExtAddr.Request            */
    mFsciMsgStopNwkReq_c                    = 0xDC, /* Fsci-StopNwk.Request                 */
    mFsciMsgStartNwkReq_c                   = 0xDF, /* Fsci-StartNwk.Request                */
    mFsciMsgStartNwkExReq_c                 = 0xE7, /* Fsci-StartNwkEx.Request              */
    mFsciMsgStopNwkExReq_c                  = 0xE8, /* Fsci-StopNwkEx.Request               */
    mFsciMsgRestartNwkReq_c                 = 0xE0, /* Fsci-RestartNwk.Request              */

    mFsciMsgAck_c                           = 0xFD, /* Fsci acknowledgment.                 */
    mFsciMsgError_c                         = 0xFE, /* Fsci internal error.                 */
    mFsciMsgDebugPrint_c                    = 0xFF, /* printf()-style debug message.        */
};

#if 0

#endif

/*============================================================================
                               MACROS
=============================================================================*/
#if defined(__GNUC__)
#define PACKED_STRUCT struct __attribute__ ((__packed__))
#define PACKED_UNION  union __attribute__ ((__packed__))
#elif defined(__IAR_SYSTEMS_ICC__)
#define PACKED_STRUCT __packed struct
#define PACKED_UNION __packed union
#else
#define PACKED_STRUCT struct
#define PACKED_UNION union
#endif

/*============================================================================
                         STRUCTURES/TYPEDEFS
=============================================================================*/
#if (gMacUsePackedStructs_d)
#define MAC_STRUCT PACKED_STRUCT
#define MAC_UNION PACKED_UNION
#else
#define MAC_STRUCT struct
#define MAC_UNION union
#endif

typedef uint8_t   bool_t;

typedef uint8_t   index_t;

typedef uint8_t clientPacketStatus_t;
typedef uint8_t opGroup_t;
typedef uint8_t opCode_t;
#if gFsciLenHas2Bytes_c
typedef uint16_t fsciLen_t;
#else
typedef uint8_t fsciLen_t;
#endif

/* FSCI status codes */
typedef enum{
    gFsciSuccess_c                 = 0x00,
    gFsciSAPHook_c                 = 0xEF,
    gFsciSAPDisabled_c             = 0xF0,
    gFsciSAPInfoNotFound_c         = 0xF1,
    gFsciUnknownPIB_c              = 0xF2,
    gFsciAppMsgTooBig_c            = 0xF3,
    gFsciOutOfMessages_c           = 0xF4,
    gFsciEndPointTableIsFull_c     = 0xF5,
    gFsciEndPointNotFound_c        = 0xF6,
    gFsciUnknownOpcodeGroup_c      = 0xF7,
    gFsciOpcodeGroupIsDisabled_c   = 0xF8,
    gFsciDebugPrintFailed_c        = 0xF9,
    gFsciReadOnly_c                = 0xFA,
    gFsciUnknownIBIdentifier_c     = 0xFB,
    gFsciRequestIsDisabled_c       = 0xFC,
    gFsciUnknownOpcode_c           = 0xFD,
    gFsciTooBig_c                  = 0xFE,
    gFsciError_c                   = 0xFF    /* General catchall error. */
} gFsciStatus_t;


/*! This type describes the enumerations used as result values by the primitives defined in the MAC layer. */
typedef enum
{
    gSuccess_c                = 0x00,        /*!< The requested operation was completed successfully. For a transmission request, this value indicates a successful transmission.*/
    gPanAtCapacity_c          = 0x01,        /*!< Association Status - PAN at capacity.*/
    gPanAccessDenied_c        = 0x02,        /*!< Association Status - PAN access denied.*/
    gSlotframeNotFound_c      = 0x03,        /*!< MLME-SET-SLOTFRAME Status  requested slotframe not found */
    gMaxSlotframesExceeded_c  = 0x04,        /*!< MLME-SET-SLOTFRAME Status  maximum number of slotframes reached */
    gUnknownLink_c            = 0x05,        /*!< MLME-SET-LINK Status  requested link not found */
    gMaxLinksExceeded_c       = 0x06,        /*!< MLME-SET-LINK Status  maximum number of links  reached */
    gNoSync_c                 = 0x07,        /*!< MLME-TSCH-MODE Status  synchronization could not be performed to the PAN Coordinator */
    gRxEnableDone_c           = 0xd0,        /*!< NOT in Spec: MLME-RX-ENABLE ended */
#ifdef gMAC2011_d
    gDpsNotSupported_d                  = 0xd1, /*!< DPS related */

    gSoundingNotSupported_d             = 0xd2, /*!< Sounding related */

    gComputationNeeded_d                = 0xd3, /*!< Calibration related */

    gRangingNotSupported_d              = 0xd4, /*!< Ranging related */
    gRangingActive_d                    = 0xd5, /*!< Ranging related */
    gRangingRequestedButNotsupported_d  = 0xd6, /*!< Ranging related */
    gNoRangingRequested_d               = 0xd7, /*!< Ranging related */

    gUnavailableDevice_c                = 0xd8,  /*!< The DeviceDescriptor lookup lrocedure has failed for the received frame. No device was found which matches the input addressing mode, address and PAN Id */
    gUnavailableSecurityLevel_c         = 0xd9,  /*!< The SecurityLevelDescriptor lookup procedure has failed for the received frame. No SecurityLevelDescriptor was found which matches the input frmae type and command frame identifier. */
#endif
    gPollReqDecryptionError_c = 0xda,        /*!< NOT in Spec: The decryption of the received MAC Data Request Command frame failed */
    gCounterError_c           = 0xdb,        /*!< The frame counter purportedly applied by the originator of the received frame is invalid.*/
    gImproperKeyType_c        = 0xdc,        /*!< The key purportedly applied by the originator of the received frame is not allowed to be used with that frame type according to the key usage policy of the recipient.*/
    gImproperSecurityLevel_c  = 0xdd,        /*!< The security level purportedly applied by the originator of the received frame does not meet the minimum security level required/expected by the recipient for that frame type.*/
    gUnsupportedLegacy_c      = 0xde,        /*!< The received frame was purportedly secured using security based on IEEE Std 802.15.4-2003, and such security is not supported by this standard.*/
    gUnsupportedSecurity_c    = 0xdf,        /*!< The security purportedly applied by the originator of the received frame is not supported.*/
    gBeaconLoss_c             = 0xe0,        /*!< The beacon was lost following a synchronization request.*/
    gChannelAccessFailure_c   = 0xe1,        /*!< A transmission could not take place due to activity on the channel, i.e., the CSMA-CA mechanism has failed.*/
    gDenied_c                 = 0xe2,        /*!< The GTS request has been denied by the PAN coordinator.*/
    gDisableTrxFailure_c      = 0xe3,        /*!< The attempt to disable the transceiver has failed.*/
    gSecurityError_c          = 0xe4,        /*!< Cryptographic processing of the received secured frame failed.*/
    gFrameTooLong_c           = 0xe5,        /*!< Either a frame resulting from processing has a length that is greater than aMaxPHYPacketSize or a requested transaction is too large to fit in the CAP or GTS.*/
    gInvalidGts_c             = 0xe6,        /*!< The requested GTS transmission failed because the specified GTS either did not have a transmit GTS direction or was not defined.*/
    gInvalidHandle_c          = 0xe7,        /*!< A request to purge an MSDU from the transaction queue was made using an MSDU handle that was not found in the transaction table.*/
    gInvalidParameter_c       = 0xe8,        /*!< A parameter in the primitive is either not supported or is out of the valid range.*/
    gNoAck_c                  = 0xe9,        /*!< No acknowledgment was received after macMaxFrameRetries.*/
    gNoBeacon_c               = 0xea,        /*!< A scan operation failed to find any network beacons.*/
    gNoData_c                 = 0xeb,        /*!< No response data were available following a request.*/
    gNoShortAddress_c         = 0xec,        /*!< The operation failed because a 16-bit short address was not allocated.*/
    gOutOfCap_c               = 0xed,        /*!< A receiver enable request was unsuccessful because it could not be completed within the CAP. It is used only to meet the backwards compatibility requirements for IEEE Std 802.15.4-2003.*/
    gPanIdConflict_c          = 0xee,        /*!< A PAN identifier conflict has been detected and communicated to the PAN coordinator.*/
    gRealignment_c            = 0xef,        /*!< A coordinator realignment command has been received.*/
    gTransactionExpired_c     = 0xf0,        /*!< The transaction has expired and its information was discarded.*/
    gTransactionOverflow_c    = 0xf1,        /*!< There is no capacity to store the transaction.*/
    gTxActive_c               = 0xf2,        /*!< The transceiver was in the transmitter enabled state when the receiver was requested to be enabled.It is used only to meet the backwards compatibility requirements for IEEE Std 802.15.4-2003.*/
    gUnavailableKey_c         = 0xf3,        /*!< The key purportedly used by the originator of the received frame is not available or, if available, the originating device is not known or is blacklisted with that particular key.*/
    gUnsupportedAttribute_c   = 0xf4,        /*!< A SET/GET request was issued with the identifier of a PIB attribute that is not supported.*/
    gInvalidAddress_c         = 0xf5,        /*!< A request to send data was unsuccessful because neither the source address parameters nor the destination address parameters were present.*/
    gOnTimeTooLong_c          = 0xf6,        /*!< A receiver enable request was unsuccessful because it specified a number of symbols that was longer than the beacon interval.*/
    gPastTime_c               = 0xf7,        /*!< A receiver enable request was unsuccessful because it could not be completed within the current superframe and was not permitted to be deferred until the next superframe.*/
    gTrackingOff_c            = 0xf8,        /*!< The device was instructed to start sending beacons based on the timing of the beacon transmissions of its coordinator, but the device is not currently tracking the beacon of its coordinator.*/
    gInvalidIndex_c           = 0xf9,        /*!< An attempt to write to a MAC PIB attribute that is in a table failed because the specified table index was out of range.*/
    gLimitReached_c           = 0xfa,        /*!< A scan operation terminated prematurely because the number of PAN descriptors stored reached an implementation-specified maximum.*/
    gReadOnly_c               = 0xfb,        /*!< A SET/GET request was issued with the identifier of an attribute that is read only.*/
    gScanInProgress_c         = 0xfc,        /*!< A request to perform a scan operation failed because the MLME was in the process of performing a previously initiated scan operation.*/
    gSuperframeOverlap_c      = 0xfd,        /*!< The device was instructed to start sending beacons based on the timing of the beacon transmissions of its coordinator, but the instructed start time overlapped the transmission time of the beacon of its coordinator.*/
} resultType_t;

/*! \brief The 802.15.4 addressing modes */
typedef enum
{
    gAddrModeNoAddress_c            = 0x00,        /*!< No address (addressing fields omitted)*/
    gAddrModeReserved_c             = 0x01,        /*!< Reserved value*/
    gAddrModeShortAddress_c         = 0x02,        /*!< 16-bit short address*/
    gAddrModeExtendedAddress_c      = 0x03,        /*!< 64-bit extended address*/
} addrModeType_t;

/*! Bit type used to indicate the transmission options for the MSDU */
typedef enum
{
    gMacTxOptionsNone_c             = 0x00,        /*!< No Tx options specified. */
    gMacTxOptionsAck_c              = 0x01,        /*!< Acknowledged transmission is required.*/
    gMacTxOptionGts_c               = 0x02,        /*!< GTS transmission is required.*/
    gMacTxOptionIndirect_c          = 0x04,        /*!< Indirect transmission is required.*/
    gMacTxOptionFramePending_c      = 0x40,        /*!< Force set the FP bit of the Frame Control field. */
    gMacTxOptionAltExtAddr_c        = 0x80,        /*!< Use alternate Extended Address for nonce creation (MAC Security).*/
}macTxOptions_t;

/*! \brief MAC Security */
typedef enum
{
    gMacSecurityNone_c              = 0x00, /*!<    Data confidentiality: NO \n
                                                    Data authenticity: NO */

    gMacSecurityMic32_c             = 0x01, /*!<    Data confidentiality: NO \n
                                                    Data authenticity: YES \n
                                                    Data authenticity size: 4 bytes */

    gMacSecurityMic64_c             = 0x02, /*!<    Data confidentiality: NO \n
                                                    Data authenticity: YES \n
                                                    Data authenticity size: 8 bytes */

    gMacSecurityMic128_c            = 0x03, /*!<    Data confidentiality: NO \n
                                                    Data authenticity: YES \n
                                                    Data authenticity size: 16 bytes */

    gMacSecurityEnc_c               = 0x04, /*!<    Data confidentiality: YES \n
                                                    Data authenticity: NO */

    gMacSecurityEncMic32_c          = 0x05, /*!<    Data confidentiality: YES \n
                                                    Data authenticity: YES \n
                                                    Data authenticity size: 4 bytes */

    gMacSecurityEncMic64_c          = 0x06, /*!<    Data confidentiality: YES \n
                                                    Data authenticity: YES \n
                                                    Data authenticity size: 8 bytes */

    gMacSecurityEncMic128_c         = 0x07, /*!<    Data confidentiality: YES \n
                                                    Data authenticity: YES \n
                                                    Data authenticity size: 16 bytes */
} macSecurityLevel_t;

/*! \brief Security Key Identifier */
typedef enum
{
    gKeyIdMode0_c                   = 0x00,        /*!< Key is determined implicitly from the originator and recipient(s) of the frame, as indicated in the frame header.\n
                                                        KeyId length (bytes): 0 */
    gKeyIdMode1_c                   = 0x01,        /*!< Key is determined from the 1-octet Key Index subfield of the Key Identifier field of the auxiliary security header in conjunction with macDefaultKeySource.\n
                                                        KeyId length (bytes): 1 */
    gKeyIdMode2_c                   = 0x02,        /*!< Key is determined explicitly from the 4-octet Key Source subfield and the 1-octet Key Index subfield of the Key Identifier field of the auxiliary security header.\n
                                                        KeyId length (bytes): 5 */
    gKeyIdMode3_c                   = 0x03,        /*!< Key is determined explicitly from the 8-octet Key Source subfield and the 1-octet Key Index subfield of the Key Identifier field of the auxiliary security header.\n
                                                        KeyId length (bytes): 9 */
} keyIdModeType_t;

/*! \brief The 802.15.4 MAC PIB Id \ref macPibId_t */
typedef uint8_t pibId_t;
typedef enum
{
    /* Freescale specific MAC PIBs */
    gMPibRole_c                                 = 0x20,    /*!< The MAC role: Device/Coordinator */
    gMPibLogicalChannel_c                       = 0x21,    /*!< Logical channel number */
    gMPibTreemodeStartTime_c                    = 0x22,    /*!< Unused */
    gMPibPanIdConflictDetection_c               = 0x23,    /*!< Unused */
    gMPibBeaconResponseDenied_c                 = 0x24,    /*!< Ignore MAC Beacon Request commands */
    gMPibNBSuperFrameInterval_c                 = 0x25,    /*!< Unused */
    gMPibBeaconPayloadLengthVendor              = 0x26,    /*!< Unused */
    gMPibBeaconResponseLQIThreshold_c           = 0x27,    /*!< Unused */
    gMPibUseExtendedAddressForBeacon_c          = 0x28,    /*!< Force the use of the extended address for Beacons */
    gMPibCsmaDisableMask_c                      = 0x29,    /*!< Disable first iteration of the CSMA-CA for the specified frame types */
    gMPibAlternateExtendedAddress_c             = 0x2A,    /*!< Alternate Extended Address used for nonce creation (MAC Security) */
    gMPibMac2003Compatibility_c                 = 0x2F,    /*!< Maintain OTA compatibillity with 802.15.4/2003 */
    /* In MAC2011 this is a standard PIB, in MAC2006 it is a costant. It must be modifiable for testing. */
    gMPibExtendedAddress_c                      = 0x30,    /*!< The device's extended address (64-bit) */
    
#if gTschSupport_d
    gMPibDisconnectTime_c                       = 0x31,
    gMPibJoinPriority_c                         = 0x32,
    gMPibASN_c                                  = 0x33,    /*!< Absolute Slot Number*/
    gMPibNoHLBuffers_c                          = 0x34,
    gMPibEBSN_c                                 = 0x35,    /*!< Enhanced Beacon Sequence Number*/
    gMPibTschEnabled_c                          = 0x36,
    gMPibEBIEList_c                             = 0x37,
    gMPibTimeslotTemplate_c                     = 0x38,
    gMPibHoppingSequenceList_c                  = 0x39,
    gMPibHoppingSequenceLength_c                = 0x3A,
    gMPibTschRole_c                             = 0x3B,
#endif

    /* Standard MAC PIBs */
    gMPibAckWaitDuration_c                      = 0x40,    /*!< Number of symbols to wait for an ACK (ReadOnly) */
    gMPibAssociationPermit_c                    = 0x41,    /*!< Allow devices o associate (Coordinator only) */
    gMPibAutoRequest_c                          = 0x42,    /*!< Send automatic MAC Data Requests, and notifications for every Beacon received */
    gMPibBattLifeExt_c                          = 0x43,    /*!< Unused */
    gMPibBattLifeExtPeriods_c                   = 0x44,    /*!< Unused */
    gMPibBeaconPayload_c                        = 0x45,    /*!< Array, containind the Beacon payload */
    gMPibBeaconPayloadLength_c                  = 0x46,    /*!< Number of bytes used for Beacon payload */
    gMPibBeaconOrder_c                          = 0x47,    /*!< How often the beacon is to be transmitted. A value of 15 indicated a non-Beaconned PAN */
    gMPibBeaconTxTime_c                         = 0x48,    /*!< The time of transmission of the most recent Beacon*/
    gMPibBSN_c                                  = 0x49,    /*!< Beacon Sequence Number */
    gMPibCoordExtendedAddress_c                 = 0x4a,    /*!< The extended address of the associated Coordinator */
    gMPibCoordShortAddress_c                    = 0x4b,    /*!< The short address of the associated Coordinator */
    gMPibDSN_c                                  = 0x4c,    /*!< Data Sequence Number */
    gMPibGtsPermit_c                            = 0x4d,    /*!< Coordinator accepts GTS or not */
    gMPibMaxCSMABackoffs_c                      = 0x4e,    /*!< The maximum number of backoffs the CSMA-CA algorithm will attempt before declaring a channel access failure */
    gMPibMinBE_c                                = 0x4f,    /*!< The minimum value of the backoff exponent (BE) in the CSMA-CA algorithm */
    gMPibPanId_c                                = 0x50,    /*!< The 16-bit identifier of the PAN on which the device is operating. If this value is 0xffff, the device is not associated. */
    gMPibPromiscuousMode_c                      = 0x51,    /*!< Remove all MAC filtering. All packets are received (including ACKs) */
    gMPibRxOnWhenIdle_c                         = 0x52,    /*!< Keep the RX on, when MAC is idle */
    gMPibShortAddress_c                         = 0x53,    /*!< The 16-bit address that the device uses to communicate in the PAN. */
    gMPibSuperframeOrder_c                      = 0x54,    /*!< The length of the active portion of the superframe, including the beacon frame */
    gMPibTransactionPersistenceTime_c           = 0x55,    /*!< The storage time of a transaction (in units: aBaseSuperframeDuration for Non-Beaconned) */
    gMPibAssociatedPANCoord_c                   = 0x56,    /*!< Indication of whether the device is associated to the PAN through the PAN coordinator. */
    gMPibMaxBE_c                                = 0x57,    /*!< The maximum value of the backoff exponent, BE, in the CSMA-CA algorithm */
    gMPibMaxFrameTotalWaitTime_c                = 0x58,    /*!< Number of symbols to wait for a frame intended as a response (see 802.15.4 spec for details) */
    gMPibMaxFrameRetries_c                      = 0x59,    /*!< The maximum number of retries allowed after a transmission failure  */
    gMPibResponseWaitTime_c                     = 0x5a,    /*!< The maximum time, in multiples of aBaseSuperframeDuration, a device shall wait for a response command frame to be available following a request command frame */
    gMPibSyncSymbolOffset_c                     = 0x5b,    /*!< Offset between the frame timestamp and the first symbol after SFD */
    gMPibTimestampSupported_c                   = 0x5c,    /*!< Indicates if frame timestamp is supported (ReadOnly) */
    gMPibSecurityEnabled_c                      = 0x5d,    /*!< Indication of whether the MAC sublayer has security enabled */
    gMPibMinLIFSPeriod_c                        = 0x5e,    /*!< Value not in standard - taken from bare metal MAC */
    gMPibMinSIFSPeriod_c                        = 0x5f,    /*!< Value not in standard - taken from bare metal MAC */
#ifdef gMAC2011_d
    gMPibTxControlActiveDuration_c              = 0x60,    /*!< Unused */
    gMPibTxControlPauseDuration_c               = 0x61,    /*!< Unused */
    gMPibTxTotalDuration_c                      = 0x62,    /*!< Unused */
#endif
#if gCslSupport_d
    gMPibCslPeriod_c                            = 0x63,
    gMPibCslMaxPeriod_c                         = 0x64,
    gMPibCslFramePendingWait_c                  = 0x65,
    gMPibEnhAckWaitDuration_c                   = 0x66,
#endif
#if gRitSupport_d
    gMPibRitPeriod_c                            = 0x67,
    gMPibRitDataWaitDuration_c                  = 0x68,
    gMPibRitTxWaitDuration_c                    = 0x69,
    gMPibRitIe_c                                = 0x6A,
#endif
#ifdef gPHY_802_15_4g_d
    gMPibPhyMode_c                              = 0x6B,
    gMPibPhyCCADuration_c                       = 0x6C,
    gMPibPhyFSKScramblePSDU_c                   = 0x6D,
    gMPibPhyFrequencyBand_c                     = 0x6E,
#endif
    
    /* MAC2006 Security Related PIB Attributes */
    gMPibKeyTable_c                             = 0x71,
    gMPibKeyTableEntries_c                      = 0x72,    /*!< Freescale specific MAC 2011 */
    gMPibDeviceTable_c                          = 0x73,
    gMPibDeviceTableEntries_c                   = 0x74,    /*!< Freescale specific MAC 2011 */
    gMPibSecurityLevelTable_c                   = 0x75,
    gMPibSecurityLevelTableEntries_c            = 0x76,    /*!< Freescale specific MAC 2011 */
    gMPibFrameCounter_c                         = 0x77,
    gMPibAutoRequestSecurityLevel_c             = 0x78,    /*!< The security level used for automatic MAC requests (ex: Data Request) */
    gMPibAutoRequestKeyIdMode_c                 = 0x79,    /*!< The key Id mode used for automatic MAC requests*/
    gMPibAutoRequestKeySource_c                 = 0x7a,    /*!< The key source used for automatic MAC requests*/
    gMPibAutoRequestKeyIndex_c                  = 0x7b,    /*!< The key index used for automatic MAC requests*/
#ifndef gMAC2011_d
    gMPibDefaultKeySource_c                     = 0x7c,
    gMPibPANCoordExtendedAddress_c              = 0x7d,
    gMPibPANCoordShortAddress_c                 = 0x7e,
#endif
    /* Freescale specific MAC 2006 and MAC2011 security PIBs */
    /* KeyDescriptor */
    gMPibKeyIdLookupList_c                      = 0x7F,
    gMPibKeyIdLookupListEntries_c               = 0x80,    /*!< Freescale specific MAC 2011 */
#ifndef gMAC2011_d
    gMPibKeyDeviceList_c                        = 0x81,
    gMPibKeyDeviceListEntries_c                 = 0x82,
#else
    gMPibDeviceDescriptorHandleList_c           = 0x9C,
    gMPibDeviceDescriptorHandleListEntries_c    = 0x9D,    /*!< Freescale specific MAC 2011 */
#endif
    gMPibKeyUsageList_c                         = 0x83,
    gMPibKeyUsageListEntries_c                  = 0x84,    /*!< Freescale specific MAC 2011 */
    gMPibKey_c                                  = 0x85,
    /* KeyUsageDescriptor */
    gMPibKeyUsageFrameType_c                    = 0x86,
    gMPibKeyUsageCommnadFrameIdentifier_c       = 0x87,
#ifndef gMAC2011_d
    /* KeyDeviceDescriptor */
    gMPibKeyDeviceDescriptorHandle_c            = 0x88,
    gMPibUniqueDevice_c                         = 0x89,
    gMPibBlackListed_c                          = 0x8A,
#else
    /* DeviceDescriptorHandleList */
    gMPibDeviceDescriptorHandle_c               = 0x9E,    /*!< Freescale specific MAC 2011 */
#endif
    /* SecurityLevelDescriptor */
    gMPibSecLevFrameType_c                      = 0x8B,
    gMPibSecLevCommnadFrameIdentifier_c         = 0x8C,
    gMPibSecLevSecurityMinimum_c                = 0x8D,
    gMPibSecLevDeviceOverrideSecurityMinimum_c  = 0x8E,
#ifdef gMAC2011_d
    gMPibSecLevAllowedSecurityLevels_c          = 0x9F,
#endif
    /* DeviceDescriptor */
    gMPibDeviceDescriptorPanId_c                = 0x8F,
    gMPibDeviceDescriptorShortAddress_c         = 0x90,
    gMPibDeviceDescriptorExtAddress_c           = 0x91,
    gMPibDeviceDescriptorFrameCounter_c         = 0x92,
    gMPibDeviceDescriptorExempt                 = 0x93,
    /* KeyIdLookupDescriptor */
#ifndef gMAC2011_d
    gMPibKeyIdLookupData_c                      = 0x94,
    gMPibKeyIdLookupDataSize_c                  = 0x95,
#else
    gMPibKeyIdLookupKeyIdMode_c                 = 0xA0,
    gMPibKeyIdLookupKeySource_c                 = 0xA1,
    gMPibKeyIdLookupKeyIndex_c                  = 0xA2,
    gMPibKeyIdLookupDeviceAddressMode_c         = 0xA3,
    gMPibKeyIdLookupDevicePANId_c               = 0xA4,
    gMPibKeyIdLookupDeviceAddress_c             = 0xA5,
#endif
    gMPibiKeyTableCrtEntry_c                    = 0x96,
    gMPibiDeviceTableCrtEntry_c                 = 0x97,
    gMPibiSecurityLevelTableCrtEntry_c          = 0x98,
    gMPibiKeyIdLookuplistCrtEntry_c             = 0x99,
#ifndef gMAC2011_d
    gMPibiKeyDeviceListCrtEntry_c               = 0x9B,
#else
    gMPibiDeviceDescriptorHandleListCrtEntry_c  = 0XA6,
#endif
    gMPibiKeyUsageListCrtEntry_c                = 0x9A,
}macPibId_t;


/*! The MCPS-DATA.request primitive is generated by a local entity when a data SPDU (MSDU) is to be transferred to a peer SSCS entity */
/* 
 * Note that this structure has been changed from the definition in the Kinetis SDK. The Kinetis SDK defines this structure with 
 * a pointer to the continuation buffer (last field) which is a driver implementation detail the SDK is doing and not part of the 
 * data actually streamed over the interface. The change here is to reflect that there is continuation data via the data[0].
 */
typedef MAC_STRUCT mcpsDataReq_tag
{
    uint64_t                dstAddr;     /*!< The individual device address of the entity to which the MSDU is being transferred. */
    uint16_t                dstPanId;    /*!< PAN identifier of the entity to which the MSDU is being transferred. */
    addrModeType_t          dstAddrMode; /*!< The destination addressing mode for this primitive. */
    uint64_t                srcAddr;     /*!< The individual device address of the entity from which the MSDU is being sent. (Not in Spec) */
    uint16_t                srcPanId;    /*!< PAN identifier of the entity from which the MSDU is being sent (Not in Spec) */
    addrModeType_t          srcAddrMode; /*!< The source addressing mode for this primitive. */
    uint8_t                 msduLength;  /*!< The number of octets contained in the MSDU to be transmitted by the MAC sublayer entity. */
    uint8_t                 msduHandle;  /*!< The handle associated with the MSDU to be transmitted by the MAC sublayer entity. */
    macTxOptions_t          txOptions;   /*!< Indicates the transmission options for this MSDU. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode;  /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource;  /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;   /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 data[0];    /*!< start of 802.15.4 packet data. */
} mcpsDataReq_t;

/*!  The MCPS-DATA.confirm primitive reports the results of a request to transfer a data SPDU (MSDU) from a local SSCS entity to a single peer SSCS entity. */
typedef MAC_STRUCT mcpsDataCnf_tag
{
    uint8_t                 msduHandle; /*!< The handle associated with the MSDU being confirmed. */
    resultType_t            status;
    uint32_t                timestamp;  /*!< The time (in symbols), at which the data were transmitted. This is a 24-bit value. */
} mcpsDataCnf_t;

/*!  The MCPS-DATA.indication primitive indicates the transfer of data SPDU (MSDU) from the MAC sublayer to the local SSCS entity. */
/* 
 * Note that this structure has been changed from the definition in the Kinetis SDK. The Kinetis SDK defines this structure with 
 * a pointer to the continuation buffer (last field) which is a driver implementation detail the SDK is doing and not part of the 
 * data actually streamed over the interface. The change here is to reflect that there is continuation data via the data[0].
 */
typedef MAC_STRUCT mcpsDataInd_tag
{
    uint64_t                dstAddr;     /*!< The individual device address of the entity to which the MSDU is being transferred. */
    uint16_t                dstPanId;    /*!< PAN identifier of the entity to which the MSDU is being transferred. */
    addrModeType_t          dstAddrMode; /*!< The destination addressing mode for this primitive. */
    uint64_t                srcAddr;     /*!< The individual device address of the entity from which the MSDU was received. */
    uint16_t                srcPanId;    /*!< PAN identifier of the entity from which the MSDU was received. */
    addrModeType_t          srcAddrMode; /*!< The source addressing mode for this primitive. */
    uint8_t                 msduLength;      /*!< The number of octets contained in the MSDU being indicated by the MAC sublayer entity. */
    uint8_t                 mpduLinkQuality; /*!< The LQI value measured during the reception of the MPDU. Lower values represent lower LQI */
    uint8_t                 dsn;             /*!< Data Sequence Number of the packet */
    uint32_t                timestamp;       /*!< The time, in symbols, at which the data were received. This is a 24-bit value */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 data[0];    /*!< start of 802.15.4 packet data. */
} mcpsDataInd_t;


/*
 * DJLZORK: Not used structures... 
 */
#if 0


/*!  The MCPS-PURGE.request primitive allows the next higher layer to purge an MSDU from the transaction queue. */
typedef MAC_STRUCT mcpsPurgeReq_tag
{
    uint8_t                 msduHandle;       /*!< The handle of the MSDU requested to be purged from the transaction queue. */
} mcpsPurgeReq_t;

/*!  The MCPS-PURGE.confirm primitive allows the MAC sublayer to notify the next higher layer of the success of its request to purge an MSDU from the transaction queue. */
typedef MAC_STRUCT mcpsPurgeCnf_tag
{
    uint8_t                 msduHandle; /*!< The handle of the MSDU requested to be purged from the transaction queue. */
    resultType_t            status;     /*!< Status of the Purge Request: gSuccess_c, gInvalidHandle_c */
} mcpsPurgeCnf_t;

/*!  The MCPS-DATA.indication primitive indicates the transfer of a data SPDU (MSDU) from the MAC sublayer to the local SSCS entity. */
/* 
 * Note that this structure has been changed from the definition in the Kinetis SDK. The Kinetis SDK defines this structure with 
 * a pointer to the continuation buffer (last field) which is a driver implementation detail the SDK is doing and not part of the 
 * data actually streamed over the interface. The change here is to reflect that there is continuation data via the data[0].
 */
typedef MAC_STRUCT mcpsPromInd_tag
{
    uint8_t                 mpduLinkQuality; /*!< The LQI value measured during the reception of the MPDU. Lower values represent lower LQI */
    uint32_t                timeStamp;       /*!< The time, in symbols, at which the data were received. This is a 24-bit value */
    uint8_t                 msduLength;      /*!< The number of octets contained in the MSDU being indicated by the MAC sublayer entity. */
    uint8_t                 data[0];         /*!< start of 802.15.4 packet data. */
} mcpsPromInd_t;

/*!  The MLME-ASSOCIATE.request primitive enables a device to request an association with a coordinator. */
typedef MAC_STRUCT mlmeAssociateReq_tag
{
    uint64_t                coordAddress;   /*!< The address of the coordinator with which to associate. */
    uint16_t                coordPanId;     /*!< The identifier of the PAN with which to associate. */
    addrModeType_t          coordAddrMode;  /*!< The coordinator addressing mode for this primitive and subsequent MPDU */
    logicalChannelId_t      logicalChannel; /*!< Indicates the logical channel on which to attempt association */
    macSecurityLevel_t      securityLevel;  /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    macCapabilityInfo_t     capabilityInfo; /*!< The operational capabilities of the device requesting association */
    channelPageId_t         channelPage;
} mlmeAssociateReq_t;

/*!  The MLME-ASSOCIATE.confirm primitive is used to inform the next higher layer of the initiating device whether its request to associate was successful or unsuccessful. */
typedef MAC_STRUCT mlmeAssociateCnf_tag
{
    uint16_t                assocShortAddress; /*!< The short device address allocated by the coordinator on successful association. 
                                                    This parameter will be equal to 0xffff if the association attempt was unsuccessful. */
    resultType_t            status;
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeAssociateCnf_t;

/*!  The MLME-ASSOCIATE.response primitive is used to initiate a response to an MLME-ASSOCIATE.indication primitive. */
typedef MAC_STRUCT mlmeAssociateRes_tag
{
    uint64_t                deviceAddress; /*!< The address of the device requesting association. */
    uint16_t                assocShortAddress; /*!< Contains the short device address allocated by the coordinator on successful association. 
                                                    This parameter is set to 0xfffe if the no short address was allocated. The 64-bit address must be used.
                                                    This parameter is set to 0xffff if the association was unsuccessful */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    resultType_t            status;            /*!< The status of the association attempt: gAssociationSuccessful_c, gPanAtCapacity_c, gPanAccessDenied_c */
} mlmeAssociateRes_t;

/*!  The MLME-ASSOCIATE.indication primitive is used to indicate the reception of an association request command. */
typedef MAC_STRUCT mlmeAssociateInd_tag
{
    uint64_t                deviceAddress; /*!< The 64-bit address of the device requesting association. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    macCapabilityInfo_t     capabilityInfo; /*!< The operational capabilities of the device requesting association */
} mlmeAssociateInd_t;

/*!  The MLME-DISASSOCIATE.request primitive is used by an associated device to notify the coordinator of its intent to leave the PAN. It is also used by the coordinator to instruct an associated device to leave the PAN. */
typedef MAC_STRUCT mlmeDisassociateReq_tag
{
    uint64_t                deviceAddress; /*!< The address of the device to which to send the disassociation notification command. */
    uint16_t                devicePanId;   /*!< The PAN identifier of the device to which to send the disassociation notification command.*/
    addrModeType_t          deviceAddrMode;/*!< The addressing mode of the device to which to send the disassociation notification command */
    macDisassociateReason_t disassociateReason;
    bool_t                  txIndirect;    /*!< Indicates whether the disassociation notification shall be sent indirectly. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeDisassociateReq_t;

/*!  The MLME-DISASSOCIATE.confirm primitive reports the results of an MLME-DISASSOCIATE.request primitive. */
typedef MAC_STRUCT mlmeDisassociateCnf_tag
{
    uint64_t                deviceAddress; /*!< The address of the device that has either requested disassociation or has been instructed to disassociate by its coordinator. */
    uint16_t                devicePanId;   /*!< The PAN identifier of the device that has either requested disassociation or has been instructed to disassociate by its coordinator. */
    addrModeType_t          deviceAddrMode;/*!< The addressing mode of the device that has either requested disassociation or has been instructed to disassociate by its coordinator.  */
    resultType_t            status; /*!< The status of the disassociation attempt: gSuccess_c, gTransactionOverflow_c, gTransactionExpired_c, gNoAck_c, gChannelAccessFailure_c, gCounterError_c, gFrameTooLong_c, gUnavailableKey_c */
} mlmeDisassociateCnf_t;

/*!  The MLME-DISASSOCIATE.indication primitive is used to indicate the reception of a disassociation notification command. */
typedef MAC_STRUCT mlmeDisassociateInd_tag
{
    uint64_t                deviceAddress; /*!< The address of the device requesting disassociation. */
    macDisassociateReason_t disassociateReason;
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeDisassociateInd_t;

/*!  The MLME-GET.request primitive requests information about a given PIB attribute. This request is synchronous */
typedef MAC_STRUCT mlmeGetReq_tag
{
    pibId_t                 pibAttribute;      /*!< The identifier of the PIB attribute. */
    uint8_t                 pibAttributeIndex; /*!< The index within the table of the specified PIB attribute to read.
                                                    This parameter is valid only for the MAC PIB attributes that are tables; it is ignored when accessing PHY PIB attributes. */
    void*                   pibAttributeValue; /*!< Pointer to a location where the PIB value will be stored. (Not in Spec) */
}mlmeGetReq_t;

/* MLME-SET.Confirm structure (Not used - Request is synchronous) */
/*! \cond DOXY_SKIP_TAG */
typedef MAC_STRUCT mlmeGetCnf_tag
{
    resultType_t            status;
    pibId_t                 pibAttribute;
    uint8_t                 pibAttributeIndex;
    void*                   pibAttributeValue;
}mlmeGetCnf_t;
/*! \endcond */

/*!  The MLME-RX-ENABLE.request primitive enables the next higher layer to request that the receiver is either enabled for a finite period of time or disabled. */
typedef MAC_STRUCT mlmeRxEnableReq_tag
{
    bool_t                  deferPermit; /*!< TRUE if the requested operation can be deferred until the next superframe if the requested time has already passed. 
                                              FALSE if the requested operation is only to be attempted in the current superframe. 
                                              This parameter is ignored for non beacon-enabled PANs. */
    uint32_t                rxOnTime;    /*!< Contains the number of symbols measured from the start of the superframe before the receiver is to be enabled or disabled. 
                                              This is a 24-bit value, and it is ignored for non beacon-enabled PANs. */
    uint32_t                rxOnDuration;/*!< Contains the number of symbols for which the receiver is to be enabled. 
                                              If this parameter is equal to 0x000000, the receiver is disabled. 
                                              This is a 24-bit value. */
#ifdef gMAC2011_d
    macRangingRxControl_t   rangingRxControl;
#endif
} mlmeRxEnableReq_t;

/*!  The MLME-RX-ENABLE.confirm primitive reports the results of the attempt to enable or disable the receiver. */
typedef MAC_STRUCT mlmeRxEnableCnf_tag
{
    resultType_t            status; /*!< Possible values: gSuccess_c, gPastTime_c, gOnTimeTooLong_c, gInvalidParameter_c */
} mlmeRxEnableCnf_t;

/*!  The MLME-SAP scan primitives define how a device can determine the energy usage or the presence or absence of PANs in a communications channel.
     The MLME-SCAN.request primitive is used to initiate a channel scan over a given list of channels. 
     A device can use the channel scan to measure the energy on the channel, search for the coordinator with which it associated, or search for all coordinators transmitting beacon frames within the POS of the scanning device. */
typedef MAC_STRUCT mlmeScanReq_tag
{
    macScanType_t           scanType;      /*!< Energy Detect, Active, Passive, Orphan */
    channelMask_t           scanChannels;  /*!< Channel bit-mask (1 = scan, 0 = do not scan) */
    uint8_t                 scanDuration;  /*!< Valid rage: 0-14. Duration in symbols: aBaseSuperframeDuration * ((2^scanDuration) + 1) */
    channelPageId_t         channelPage;
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeScanReq_t;

/*!  PAN Descriptor Block structure */
typedef MAC_STRUCT panDescriptorBlock_tag
{
    panDescriptor_t               panDescriptorList[gScanResultsPerBlock_c]; /*!< Array of PAN descriptors. */
    uint8_t                       panDescriptorCount; /*!< The number of PAD descriptors in this block. */
    struct panDescriptorBlock_tag *pNext; /*!< Pointer to the next PAN descriptor block, or NULL if this is the last block. */
} panDescriptorBlock_t;

/*!  The MLME-SCAN.confirm primitive reports the result of the channel scan request. The panDescriptor_t type provides a link pointer for a chained list. */
typedef MAC_STRUCT mlmeScanCnf_tag
{
    resultType_t            status;                    /*!< The status of the scan attempt: gSuccess_c, gLimitReached_c, gNoBeacon_c, gScanInProgress_c, gCounterError_c, gFrameTooLong_c, gUnavailableKey_c, gUnsupportedSecurity_c, gInvalidParameter_c */
    macScanType_t           scanType;                  /*!< Energy Detect, Active, Passive, Orphan */
    channelPageId_t         channelPage;
    uint8_t                 resultListSize;            /*!< The number of elements returned in the appropriate result lists */
    channelMask_t           unscannedChannels;         /*!< Indicates which channels given in the request were not scanned (1 = not scanned, 0 = scanned or not requested). 
                                                            This parameter is not valid for ED scans */
    MAC_UNION {
        uint8_t*              pEnergyDetectList;       /*!< The list of energy measurements, one for each channel searched during an ED scan. 
                                                            Upper layer must free this message */
        panDescriptorBlock_t* pPanDescriptorBlockList; /*!< Pointer to array of PAN descriptors, one for each beacon found during an active or passive scan. This parameter is null for orphan scans or when macAutoRequest is set to FALSE during an active or passive scan.
                                                            Upper layer must free this message */
    } resList;
} mlmeScanCnf_t;

/*!  The MLME-SAP communication status primitive defines how the MLME communicates to the next higher layer about transmission status, when the transmission was instigated by a response primitive, and about security errors on incoming packets.
     The MLME-COMM-STATUS.indication primitive allows the MLME to indicate a communications status. */
typedef MAC_STRUCT mlmeCommStatusInd_tag
{
    uint64_t                srcAddress;  /*!< Contains the individual device address of the entity, from which the frame causing the error originated. */
    uint16_t                panId;       /*!< PAN identifier of the device from which the frame was received, or to which the frame was being sent. */
    addrModeType_t          srcAddrMode; /*!< The source addressing mode for this primitive. */
    uint64_t                destAddress; /*!< Contains the individual device address of the device, for which the frame was intended. */
    addrModeType_t          destAddrMode;  /*!< The destination addressing mode for this primitive. */
    resultType_t            status;        /*!< The communication status */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeCommStatusInd_t;
#endif

/*!  The MLME-SET.request primitive is generated by the next higher layer, and issued to its MLME to write the indicated PIB attribute. (Request is synchronous) */
typedef MAC_STRUCT mlmeSetReq_tag
{
    pibId_t                 pibAttribute;      /*!< The identifier of the PIB attribute. */
    uint8_t                 pibAttributeIndex; /*!< The index within the table of the specified PIB attribute to read.
                                                    This parameter is valid only for the MAC PIB attributes that are tables; it is ignored when accessing PHY PIB attributes. */
    void*                   pibAttributeValue; /*!< Pointer to the new value of the PIB */
} mlmeSetReq_t;

/* MLME-SET.Confirm structure (Not used - Request is synchronous) */
/*! \cond DOXY_SKIP_TAG */
typedef MAC_STRUCT mlmeSetCnf_tag
{
    resultType_t            status;
    pibId_t                 pibAttribute;
    uint8_t                 pibAttributeIndex;
} mlmeSetCnf_t;
/*! \endcond */

/*!  The MLME-RESET.request primitive is generated by the next higher layer and issued to the MLME to request a reset of the MAC sublayer to its initial conditions. 
     This primitive is issued before using the MLME-START.request or the MLME-ASSOCIATE.request primitives. (Request is synchronous) */
typedef MAC_STRUCT mlmeResetReq_tag
{
    bool_t                  setDefaultPIB; /*!< If TRUE, all MAC PIBs are set to their default values */
} mlmeResetReq_t;

/*!  MLME-RESET.Confirm structure (Not used - Request is synchronous) */
typedef MAC_STRUCT mlmeResetCnf_tag
{
    resultType_t            status;
} mlmeResetCnf_t;

/*!  The MLME-START.request primitive enables the PAN coordinator to initiate a new PAN, or to begin using a new superframe configuration. 
     This primitive may also be used by a device already associated with an existing PAN to begin using a new superframe configuration */
typedef MAC_STRUCT mlmeStartReq_tag
{
    uint16_t                panId;                /*!< The PAN identifier to be used by the device. */
    logicalChannelId_t      logicalChannel;       /*!< The logical channel on which to start using the new superframe configuration. */
    uint32_t                startTime;            /*!< The time at which to begin transmitting beacons. If this parameter is equal to 0x000000, beacon transmissions will begin immediately. This is a 24 bit value. */
    uint8_t                 beaconOrder;          /*!< How often the beacon is to be transmitted (0-15). A value of 15 indicates that the coordinator will not transmit periodic beacons. */
    uint8_t                 superframeOrder;      /*!< The length of the active portion of the superframe, including the beacon frame (0-14). If the beaconOrder parameter (BO) has a value of 15, this parameter is ignored. */
    bool_t                  panCoordinator;       /*!< Indicates if the device will become the PAN coordinator of a new PAN or begin using a new superframe configuration on the PAN with which it is associated. */
    bool_t                  batteryLifeExtension; /*!< If this value is TRUE, the receiver of the beaconing device is disabled macBattLifeExtPeriods full backoff periods after the inter-frame spacing (IFS) period following the beacon frame. 
                                                       If this value is FALSE, the receiver of the beaconing device remains enabled for the entire CAP. */
    bool_t                  coordRealignment;     /*!< Indicates if a coordinator realignment command is to be transmitted before changing the superframe */
    macSecurityLevel_t      coordRealignSecurityLevel; /*!< The security level to be used */
    keyIdModeType_t         coordRealignKeyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                coordRealignKeySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 coordRealignKeyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    macSecurityLevel_t      beaconSecurityLevel; /*!< The security level to be used */
    keyIdModeType_t         beaconKeyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                beaconKeySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 beaconKeyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeStartReq_t;


/*!  The MLME-START.confirm primitive reports the results of the attempt to start using a new superframe configuration.
     The MLME-START.confirm primitive is generated by the MLME, and issued to NWK layer in response to the MLME-START.request primitive. 
     The MLME-START.confirm primitive returns a status of either SUCCESS, indicating that the MAC sublayer has started using the new superframe configuration, or an appropriate error code.
 */
typedef MAC_STRUCT mlmeStartCnf_tag
{
    resultType_t             status; /*!< Possible values: gSuccess_c, gNoShortAddress_c, gSuperframeOverlap_c, gTrackingOff_c, gInvalidParameter_c, gCounterError_c, gFrameTooLong_c, gUnavailableKey_c, gUnsupportedSecurity_c, gChannelAccessFailure_c */
} mlmeStartCnf_t;

#if 0
/*!  The MLME-POLL.request primitive prompts the device to request data from the coordinator */
typedef MAC_STRUCT mlmePollReq_tag
{
    addrModeType_t          coordAddrMode; /*!< The coordinator addressing mode to which the poll request is intended. */
    uint16_t                coordPanId;    /*!< The PAN identifier of the coordinator for which the poll is intended. */
    uint64_t                coordAddress;  /*!< The address of the coordinator for which the poll is intended. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmePollReq_t;

/*!  MLME-POLL.Confirm structure */
typedef MAC_STRUCT mlmePollCnf_tag
{
    resultType_t            status;
} mlmePollCnf_t;

/*!  The MLME-BEACON-NOTIFY.indication primitive is used to send parameters contained within a beacon frame received by the MAC sublayer to the next higher layer. 
     The primitive also sends a measure of the LQI and the time the beacon frame was received. */
typedef MAC_STRUCT mlmeBeaconNotifyInd_tag
{
    uint8_t                 bsn;            /*!< The beacon sequence number */
    uint8_t                 pendAddrSpec;   /*!< The beacon pending address specification.
                                                 Bits 0-2 indicate the number of short addresses contained in the Address List field of the beacon frame.
                                                 Bits 4-6 indicate the number of extended addresses contained in the Address List field of the beacon frame. */
    uint8_t                 sduLength;      /*!< The number of octets contained in the beacon payload of the beacon frame received by the MAC sublayer. */
    uint8_t*                pAddrList;      /*!< The list of addresses of the devices for which the beacon source has data */
    panDescriptor_t*        pPanDescriptor; /*!< The PANDescriptor for the received beacon. */
    uint8_t*                pSdu;           /*!< The set of octets comprising the beacon payload to be transferred from the MAC sublayer entity to the next higher layer. */
#if gTschSupport_d
    uint8_t                 ebsn;
    beaconType_t            beaconType;
#endif
    void*                   pBufferRoot; /*!< Pointer to the start of the message received by air. 
                                              The upper layer must free this buffer before freeing the indication message. */
} mlmeBeaconNotifyInd_t;

/*!  The MLME-GTS.request primitive enables a device to send a request to the PAN coordinator to allocate a new GTS or to deallocate an existing GTS. 
     This primitive is also used by the PAN coordinator to initiate a GTS deallocation. */
typedef MAC_STRUCT mlmeGtsReq_tag
{
    gtsCharacteristics_t    gtsCharacteristics; /*!< The characteristics of the GTS request, including whether the request is for the allocation of a new GTS or the deallocation of an existing GTS. */
    macSecurityLevel_t      securityLevel;      /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeGtsReq_t;

/*!  The MLME-GTS.confirm primitive reports the results of a request to allocate a new GTS or deallocate an existing GTS. */
typedef MAC_STRUCT mlmeGtsCnf_tag
{
    resultType_t            status; /*!< Possible values: gSuccess_c, gDenied_c, gNoShortAddress_c, gChannelAccessFailure_c, gNoAck_c, gNoData_c, gCounterError_c, gFrameTooLong_c, gUnavailableKey_c, gUnsupportedSecurity_c, gInvalidParameter_c */
    gtsCharacteristics_t    gtsCharacteristics;
} mlmeGtsCnf_t;

/*!  The MLME-GTS.indication primitive indicates that a GTS has been allocated or that a previously allocated GTS has been deallocated. */
typedef MAC_STRUCT mlmeGtsInd_tag
{
    uint16_t                deviceAddress;      /*!< The 16-bit short address of the device that has been allocated or deallocated a GTS. */
    gtsCharacteristics_t    gtsCharacteristics; /*!< The characteristics of the GTS. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeGtsInd_t;

/*!  The MLME-ORPHAN.indication primitive allows the MLME of a coordinator to notify the next higher layer of the presence of an orphaned device. */
typedef MAC_STRUCT mlmeOrphanInd_tag
{
    uint64_t                orphanAddress; /*!< The extended address of the orphaned device. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeOrphanInd_t;

/*!  The MLME-ORPHAN.response primitive is generated by the next higher layer and issued to its MLME when it reaches a decision about whether the orphaned device indicated in the MLME-ORPHAN.indication primitive is associated. */
typedef MAC_STRUCT mlmeOrphanRes_tag
{
    uint64_t                orphanAddress;    /*!< The extended address of the orphaned device. */
    uint16_t                shortAddress;     /*!< The 16-bit short address allocated to the orphaned device if it is associated with this coordinator. */
    bool_t                  associatedMember; /*!< TRUE if the orphaned device is associated with this coordinator or FALSE otherwise. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeOrphanRes_t;

/*!  The MLME-SYNC.request primitive is generated by the next higher layer of a device on a beacon-enabled PAN, and issued to its MLME to synchronize with the coordinator. */
typedef MAC_STRUCT mlmeSyncReq_tag
{
    logicalChannelId_t      logicalChannel; /*!< The logical channel on which to attempt coordinator synchronization. */
    channelPageId_t         channelPage;    /*!< The channel page on which to attempt coordinator synchronization. */
    bool_t                  trackBeacon;    /*!< TRUE if the MLME is to synchronize with the next beacon, and to attempt to track all future beacons. 
                                                 FALSE if the MLME is to synchronize with only the next beacon. */
} mlmeSyncReq_t;

/*!  This primitive is generated by the MLME of a device, and issued to its next higher layer in the event of losing synchronization with the coordinator. 
     It is also generated by the MLME of the PAN coordinator and issued to its next higher layer in the event of a PAN ID conflict. */
typedef MAC_STRUCT mlmeSyncLossInd_tag
{
    resultType_t            lossReason; /*!< Possible values: gPanIdConflict_c, gRealignment_c, gBeaconLost_c */
    uint16_t                panId;      /*!< The PAN identifier with which the device lost the synchronization, or to which it was realigned. */
    logicalChannelId_t      logicalChannel; /*!< The logical channel on which the device lost the synchronization, or to which it was realigned. */
    channelPageId_t         channelPage;    /*!< The channel page on which the device lost the synchronization, or to which it was realigned. */
    macSecurityLevel_t      securityLevel; /*!< The security level to be used */
    keyIdModeType_t         keyIdMode; /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */
    uint64_t                keySource; /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
    uint8_t                 keyIndex;  /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */
} mlmeSyncLossInd_t;

/*!  The MLME-POLL-NOTIFY.indication primitive reports a poll indication message to the network (Not in spec) */
typedef MAC_STRUCT mlmePollNotifyInd_tag
{
    addrModeType_t          srcAddrMode; /*!< The source addressing mode for this primitive. */
    uint64_t                srcAddr;     /*!< The individual device address of the entity from which the notification was received. */
    uint16_t                srcPanId;    /*!< PAN identifier of the entity from which the notification was received. */
} mlmePollNotifyInd_t;

/*!  The MLME-SET-SLOTFRAME.request primitive is generated by the next higher layer, and issued to the MLME to request a set of a TSCH slotframe to the MAC sublayer. */
typedef MAC_STRUCT mlmeSetSlotframeReq_tag
{
    uint8_t                 slotframeHandle; /*!< Unique identifier of the slotframe */
    macSetSlotframeOp_t     operation;       /*!< Operation to be performed on the link (add, delete or modify) */
    uint16_t                size;            /*!< Number of timeslots in the new slotframe */
} mlmeSetSlotframeReq_t;

/*!  The MLME-SET-SLOTFRAME.confirm primitive is generated by the MAC layer and issued to the next higher layer to confirm the set of a TSCH slotframe to the MAC sublayer. */
typedef MAC_STRUCT mlmeSetSlotframeCnf_tag
{
    uint8_t                 slotframeHandle; /*!< Unique identifier of the slotframe to be added, deleted, or modified. */
    resultType_t            status; /*!< Possible values: gSuccess_c, gInvalidParameter_c, gSlotframeNotFound_c, gMaxSlotframesExceeded_c */
} mlmeSetSlotframeCnf_t;

/*!  The MLME-SET-LINK.request primitive is generated by the next higher layer, and issued to the MLME to request a set of a TSCH link to the MAC sublayer. */
typedef MAC_STRUCT mlmeSetLinkReq_tag
{
    macSetLinkOp_t          operation;       /*!< Operation to be performed on the link (add, delete or modify) */
    uint16_t                linkHandle;      /*!< Unique identifier (local to specified slotframe) for the link. */
    uint8_t                 slotframeHandle; /*!< The slotframeHandle of the slotframe to which the link is associated. */
    uint16_t                timeslot;        /*!< Timeslot of the link to be added. */
    uint16_t                channelOffset;   /*!< The Channel offset of the link. */
    macLinkOptions_t        linkOptions;     /*!< Bit0: TX
                                                  Bit1: RX
                                                  Bit2: Shared (for TX links)
                                                  Bit3: Timekeeping (for RX links) */
    macLinkType_t           linkType;        /*!< Type of the link; indicates if the link may be used for sending Enhanced Beacons. */
    uint16_t                nodeAddr;        /*!< Address of the neighbour device connected via the link. 0xffff indicates that the link can be used for frames sent with broadcast address. */
} mlmeSetLinkReq_t;

/*!  The MLME-SET-LINK.confirm primitive is generated by the MAC layer, and issued to the next higher layer to confirm the set of a TSCH link to the MAC sublayer */
typedef MAC_STRUCT mlmeSetLinkCnf_tag
{
    resultType_t            status;          /*<! Possible values: gSuccess_c, gInvalidParameter_c, gUnkonwnLink_c, gMaxLinksExceeded_c */
    uint16_t                linkHandle;      /*!< A unique (local to specified slotframe) identifier of the link. */
    uint8_t                 slotframeHandle; /*!< The slotframeHandle of the slotframe to which the link is associated. */
} mlmeSetLinkCnf_t;

/*!  The MLME-TSCH-MODE.request primitive is generated by the next higher layer, and issued to the MLME to request the TSCH enable or disable procedure */
typedef MAC_STRUCT mlmeTschModeReq_tag
{
    macTschMode_t           tschMode;
} mlmeTschModeReq_t;

/*!  The MLME-TSCH-MODE.confirm primitive is generated by the MAC layer, and issued to the next higher layer to confirm the enable or disable of the TSCH module. */
typedef MAC_STRUCT mlmeTschModeCnf_tag
{
    macTschMode_t           tschMode;
    resultType_t            status;   /*!< Possible values: gSuccess_c, gNoSync_c */
} mlmeTschModeCnf_t;

/*!  The MLME-KEEP-ALIVE.request primitive is generated by the next higher layer, and issued to the MLME to request monitor of frames exchanged with a TSCH neighbour used as clock source, and configure the keep-alive period in timeslots for that clock source */
typedef MAC_STRUCT mlmeKeepAliveReq_tag
{
    uint16_t                dstAddr;         /*!< Address of the neighbor device with which to maintain the timing. Keep-alives with dstAddr of 0xffff do not expect to be acknowledged, and cannot be used for timekeeping */
    uint16_t                keepAlivePeriod; /*!< Period in timeslots after which a frame is sent if no frames have been sent to dstAddr. */
} mlmeKeepAliveReq_t;

/*!  The MLME-KEEP-ALIVE.confirm primitive is generated by the MAC layer, and issued to the next higher layer to confirm the request to configure the keep-alive frames for a TSCH clock source. */
typedef MAC_STRUCT mlmeKeepAliveCnf_tag
{
    resultType_t            status; /*!< Possible values: gSuccess_c, gInvalidParameter_c */
} mlmeKeepAliveCnf_t;

/*!  The MLME-BEACON.request primitive is generated by the next higher layer, and issued to the MLME to request the MAC layer to send a beacon or enhanced beacon frame with the specified parameters. 
     It is used for advertising the TSCH parameters to allow new devices to join the network. */
typedef MAC_STRUCT mlmeBeaconReq_tag
{
    beaconType_t            beaconType;          /*!< Indicates whether to send a beacon or an enhanced beacon. */
    uint8_t                 channel;             /*!< The channel number to use. */
    channelPageId_t         channelPage;         /*!< The channel page to use. */
    uint8_t                 superframeOrder;     /*!< The length of the active portion of the superframe, including the beacon frame (0-15). */
    macSecurityLevel_t      beaconSecurityLevel; /*!< The security level to be used */ /*!< Indicates the security level to be used in beacon frame. */
    keyIdModeType_t         beaconKeyIdMode;     /*!< The mode used to identify the key to be used. This parameter is ignored if the SecurityLevel parameter is set to 0x00. */     /*!< The field contains the mode used to identify the key to be used. This parameter is ignored if the beaconSecurityLevel parameter is set to gMacSecurityNone_c.  */
    uint64_t                beaconKeySource;     /*!< The originator of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */     /*!< Indicates the originator of the key to be used. Ignored if the beaconKeyIdMode parameter is ignored or set to gKeyIdMode0_c */
    uint8_t                 beaconKeyIndex;      /*!< The index of the key to be used. This parameter is ignored if the KeyIdMode parameter is ignored or set to 0x00. */      /*!< Indicates the index of the key to be used. Ignored if the beaconKeyIdMode parameter is ignored or set to gKeyIdMode0_c. */
    addrModeType_t          dstAddrMode;         /*!< The destination addressing mode for this primitive. */         /*!< The destination addressing mode for this primitive and subsequent MPDU */
    uint64_t                dstAddr;             /*!< The individual device address of the entity to which the MSDU is being transferred */
    bool_t                  bsnSuppression;      /*!< If BeaconType = 0x01, then if BSNSuppression is TRUE, the EBSN is omitted from the frame and the Sequence Number Suppression field of the Frame Control field is set to 1. */
} mlmeBeaconReq_t;

/*!  The MLME-BEACON.confirm primitive is generated by the MAC layer, and issued to the next higher layer to indicate the result of the MLME-BEACON.request primitive. */
typedef MAC_STRUCT mlmeBeaconCnf_tag
{
    resultType_t            status; /*!< Indicates results of the MLME-BEACON.request: gSuccess_c, gChannelAccessFailure_c, gFrameTooLong_c, gInvalidParameter_c */
} mlmeBeaconCnf_t;

/************************************************************************************/

/*!  Common MAC message header structure */
typedef MAC_STRUCT macMsgHeader_tag{
    macMessageId_t      msgType; /*!< The id of the MAC massage */
} macMsgHeader_t;

/*!  This structure describes the generic MAC message that enables communication from the NWK layer to the MCPS Service Access Point */
typedef MAC_STRUCT nwkToMcpsMessage_tag
{
    macMessageId_t      msgType; /*!< The id of the MAC massage */
    MAC_UNION
    {
        mcpsDataReq_t       dataReq;
        mcpsPurgeReq_t      purgeReq;
    } msgData;
} nwkToMcpsMessage_t;

/*!  This structure describes the generic MAC message that enables communication from the MCPS Service Access Point to the NWK layer */
typedef MAC_STRUCT mcpsToNwkMessage_tag
{
    macMessageId_t      msgType; /*!< The id of the MAC massage */
    MAC_UNION
    {
        mcpsDataCnf_t       dataCnf;
        mcpsDataInd_t       dataInd;
        mcpsPurgeCnf_t      purgeCnf;
        mcpsPromInd_t       promInd;
    } msgData;
} mcpsToNwkMessage_t;

/*!  This structure describes the generic MAC message that enables communication from the NWK layer to the MLME Service Access Point. */
typedef MAC_STRUCT mlmeMessage_tag
{
    macMessageId_t      msgType; /*!< The id of the MAC massage */
    MAC_UNION
    {
        mlmeAssociateReq_t      associateReq;
        mlmeAssociateRes_t      associateRes;
        mlmeDisassociateReq_t   disassociateReq;
        mlmeGetReq_t            getReq;
        mlmeGtsReq_t            gtsReq;
        mlmeOrphanRes_t         orphanRes;
        mlmeResetReq_t          resetReq;
        mlmeRxEnableReq_t       rxEnableReq;
        mlmeScanReq_t           scanReq;
        mlmeSetReq_t            setReq;
        mlmeStartReq_t          startReq;
        mlmeSyncReq_t           syncReq;
        mlmePollReq_t           pollReq;
        mlmeSetSlotframeReq_t   setSlotframeReq;
        mlmeSetLinkReq_t        setLinkReq;
        mlmeTschModeReq_t       tschModeReq;
        mlmeKeepAliveReq_t      keepAliveReq;
        mlmeBeaconReq_t         beaconReq;
    } msgData;
} mlmeMessage_t;

/*!  This structure describes the generic MAC message that enables communication from the MLME Service Access Point to the NWK layer. */
typedef MAC_STRUCT nwkMessage_tag
{
    macMessageId_t      msgType; /*!< The id of the MAC massage */
    MAC_UNION
    {
        mlmeAssociateInd_t      associateInd;
        mlmeAssociateCnf_t      associateCnf;
        mlmeDisassociateInd_t   disassociateInd;
        mlmeDisassociateCnf_t   disassociateCnf;
        mlmeBeaconNotifyInd_t   beaconNotifyInd;
        mlmeGetCnf_t            getCnf;         /*!< Not used. Request is synchronous */
        mlmeGtsInd_t            gtsInd;
        mlmeGtsCnf_t            gtsCnf;
        mlmeOrphanInd_t         orphanInd;
        mlmeResetCnf_t          resetCnf;       /*!< Not used. Request is synchronous */
        mlmeRxEnableCnf_t       rxEnableCnf;
        mlmeScanCnf_t           scanCnf;
        mlmeCommStatusInd_t     commStatusInd;
        mlmeSetCnf_t            setCnf;         /*!< Not used. Request is synchronous */
        mlmeStartCnf_t          startCnf;
        mlmeSyncLossInd_t       syncLossInd;
        mlmePollCnf_t           pollCnf;
        mlmePollNotifyInd_t     pollNotifyInd;
        mlmeSetSlotframeCnf_t   setSlotframeCnf;/*!< Not used. Request is synchronous */
        mlmeSetLinkCnf_t        setLinkCnf;     /*!< Not used. Request is synchronous */
        mlmeTschModeCnf_t       tschModeCnf;    /*!< Not used. Request is synchronous */
        mlmeKeepAliveCnf_t      keepAliveCnf;   /*!< Not used. Request is synchronous */
        mlmeBeaconCnf_t         beaconCnf;        
    } msgData;
} nwkMessage_t;


#endif


/* 
 * The Application Support Package (ASP) contains XCVR-specific functionality. 
 * It can be used for XCVR testing, and for accessing XCVR-specific features (for example FAD).
 */
/*! AspSetPowerLevel.Request      */
typedef PACKED_STRUCT aspSetPowerLevelReq_tag
{
    uint8_t powerLevel; /*!< Platform specific */
} aspSetPowerLevelReq_t;



/* Format of packets exchanged between the external client and Fsci. */
typedef PACKED_STRUCT clientPacketHdr_tag
{
    uint8_t    startMarker;
    opGroup_t  opGroup;
    opCode_t   opCode;
    fsciLen_t  len;      /* Actual length of payload[] */
} clientPacketHdr_t;


/* The terminal checksum is actually stored at payload[len]. The aditional 
   gFsci_TailBytes_c bytes insures that there is always space for it, even 
   if the payload is full. */
typedef PACKED_STRUCT clientPacket_tag
{
    clientPacketHdr_t header;
    PACKED_UNION
    {
		uint8_t payload[gFsciMaxPayloadLen_c + gFsci_TailBytes_c];
		clientPacketStatus_t status;
		
		PACKED_UNION
		{ /* Indication packet types */
			mcpsDataInd_t   data;
		} ind;
		
		PACKED_UNION
		{ /* Request packet types */
			mcpsDataReq_t   data;
			
			mlmeSetReq_t    set;
			mlmeStartReq_t  start;
			mlmeResetReq_t  reset;
			
			aspSetPowerLevelReq_t pwr;
		} req;
		
		PACKED_UNION
		{ /* Confirmation packet types */
			mcpsDataCnf_t   data;

			mlmeSetCnf_t    set;
			mlmeStartCnf_t  start;
		} cnf;
	} ;
} clientPacket_t;




/*============================================================================
                         GLOBAL VARIABLE DECLARATIONS
=============================================================================*/
/*============================================================================
                         FUNCTION PROTOTYPES
=============================================================================*/

#endif  /* __IEEE802154_FSCI_H__ */
