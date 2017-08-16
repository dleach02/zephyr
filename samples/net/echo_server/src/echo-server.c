/* echo.c - Networking echo server */

/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if 1
#define SYS_LOG_DOMAIN "echo-server"
#define NET_SYS_LOG_LEVEL SYS_LOG_LEVEL_DEBUG
#define NET_LOG_ENABLED 1
#endif

#include <zephyr.h>
#include <linker/sections.h>
#include <errno.h>

#include <net/net_pkt.h>
#include <net/net_core.h>
#include <net/net_context.h>

#include <net/net_app.h>

#if defined(CONFIG_NET_L2_BLUETOOTH)
#include <bluetooth/bluetooth.h>
#include <gatt/ipss.h>
#endif

#if defined(CONFIG_NET_L2_IEEE802154)
#include <ieee802154_settings.h>
#endif

#include "common.h"

static void flicker_led_handler(struct k_timer *timer);
K_TIMER_DEFINE(led_timer, &flicker_led_handler, NULL);

static int led_flicker = 0;
static struct device *led_dev;


/* The startup time needs to be longish if DHCP is enabled as setting
 * DHCP up takes some time.
 */
#define APP_STARTUP_TIME K_SECONDS(20)

#define APP_BANNER "Run echo server"

static struct k_sem quit_lock;

void panic(const char *msg)
{
	if (msg) {
		NET_ERR("%s", msg);
	}

	for (;;) {
		k_sleep(K_FOREVER);
	}
}

void quit(void)
{
	k_sem_give(&quit_lock);
}

struct net_pkt *build_reply_pkt(const char *name,
				struct net_app_ctx *ctx,
				struct net_pkt *pkt)
{
	struct net_pkt *reply_pkt;
	struct net_buf *frag, *tmp;
	int header_len = 0, recv_len, reply_len;

	NET_INFO("%s received %d bytes", name, net_pkt_appdatalen(pkt));
    
    led_flicker = 1;

	if (net_pkt_appdatalen(pkt) == 0) {
		return NULL;
	}

	reply_pkt = net_app_get_net_pkt(ctx, net_pkt_family(pkt), K_FOREVER);

	NET_ASSERT(reply_pkt);
	NET_ASSERT(net_pkt_family(reply_pkt) == net_pkt_family(pkt));

	recv_len = net_pkt_get_len(pkt);

	tmp = pkt->frags;

	/* If we have link layer headers, then get rid of them here. */
	if (recv_len != net_pkt_appdatalen(pkt)) {
		/* First fragment will contain IP header so move the data
		 * down in order to get rid of it.
		 */
		header_len = net_pkt_appdata(pkt) - tmp->data;

		NET_ASSERT(header_len < CONFIG_NET_BUF_DATA_SIZE);

		/* After this pull, the tmp->data points directly to application
		 * data.
		 */
		net_buf_pull(tmp, header_len);
	}

	net_pkt_set_appdatalen(reply_pkt, net_pkt_appdatalen(pkt));

	while (tmp) {
		frag = net_app_get_net_buf(ctx, reply_pkt, K_FOREVER);

		if (net_buf_headroom(tmp) == 0) {
			/* If there is no link layer headers in the
			 * received fragment, then get rid of that also
			 * in the sending fragment. We end up here
			 * if MTU is larger than fragment size, this
			 * is typical for ethernet.
			 */
			net_buf_push(frag, net_buf_headroom(frag));

			frag->len = 0; /* to make fragment empty */

			/* Make sure to set the reserve so that
			 * in sending side we add the link layer
			 * header if needed.
			 */
			net_pkt_set_ll_reserve(reply_pkt, 0);
		}

		NET_ASSERT_INFO(net_buf_tailroom(frag) >= tmp->len,
				"tail %zd longer than len %d",
				net_buf_tailroom(frag), tmp->len);

		memcpy(net_buf_add(frag, tmp->len), tmp->data, tmp->len);

		net_pkt_frag_add(reply_pkt, frag);

		tmp = net_pkt_frag_del(pkt, NULL, tmp);
	}

	reply_len = net_pkt_get_len(reply_pkt);

	NET_ASSERT_INFO((recv_len - header_len) == reply_len,
			"Received %d bytes, sending %d bytes",
			recv_len - header_len, reply_len);

	return reply_pkt;
}

void pkt_sent(struct net_app_ctx *ctx,
	     int status,
	     void *user_data_send,
	     void *user_data)
{
	if (!status) {
		NET_INFO("Sent %d bytes", POINTER_TO_UINT(user_data_send));
	}
}

static inline int init_app(void)
{
	k_sem_init(&quit_lock, 0, UINT_MAX);

#if defined(CONFIG_NET_L2_BLUETOOTH)
	if (bt_enable(NULL)) {
		NET_ERR("Bluetooth init failed");
		return -EFAULT;
	}
	ipss_init();
	ipss_advertise();
#endif

#if defined(CONFIG_NET_L2_IEEE802154)
	if (ieee802154_sample_setup()) {
		NET_ERR("IEEE 802.15.4 setup failed");
		return -EFAULT;
	}
#endif

	return 0;
}

#include <adc.h>
#include <board.h>
#include <device.h>
#include <gpio.h>

#define BUFFER_SIZE 5

#if defined(CONFIG_BOARD_FRDM_K64F)
#define ADC_DEV_NAME    CONFIG_ADC_1_NAME
#define ADC_CHANNEL     14
#elif defined(CONFIG_BOARD_FRDM_KL25Z)
#define ADC_DEV_NAME    CONFIG_ADC_0_NAME
#define ADC_CHANNEL     12
#elif defined(CONFIG_BOARD_FRDM_KW41Z)
#define ADC_DEV_NAME    CONFIG_ADC_0_NAME
#define ADC_CHANNEL     3
#elif defined(CONFIG_BOARD_HEXIWEAR_K64)
#define ADC_DEV_NAME    CONFIG_ADC_0_NAME
#define ADC_CHANNEL     16
#elif defined(CONFIG_BOARD_HEXIWEAR_KW40Z)
#define ADC_DEV_NAME    CONFIG_ADC_0_NAME
#define ADC_CHANNEL     1
#else
#define ADC_DEV_NAME    CONFIG_ADC_0_NAME
#define ADC_CHANNEL     10
#endif
static u16_t seq_buffer[BUFFER_SIZE];

/* Change this if you have an LED connected to a custom port */
#define PORT    LED0_GPIO_PORT
/* Change this if you have an LED connected to a custom pin */
#define LED     LED0_GPIO_PIN

/* 1000 msec = 1 sec */
#define SLEEP_TIME      1000


static struct adc_seq_entry entry = {
	.sampling_delay = 30,
	.channel_id = ADC_CHANNEL,
	.buffer = (void *)seq_buffer,
	.buffer_length = BUFFER_SIZE * sizeof(seq_buffer[0])
};

static struct adc_seq_table table = {
	.entries = &entry,
	.num_entries = 1,
};



static void flicker_led_handler(struct k_timer *timer)
{
    if (led_flicker)
    {
        gpio_pin_write(led_dev, LED, 0);
        led_flicker = 0;
    }
    else
    {
        gpio_pin_write(led_dev, LED, 1);
    }
}


void djlzork(void)
{
#if 1    
    int i;
	int ret;
	struct device *adc_dev = device_get_binding(ADC_DEV_NAME);

	led_dev = device_get_binding(PORT);
	/* Set LED pin as output */
	gpio_pin_configure(led_dev, LED, GPIO_DIR_OUT);
    gpio_pin_write(led_dev, LED, 1);
    k_sleep(SLEEP_TIME);
    gpio_pin_write(led_dev, LED, 0);
    k_sleep(SLEEP_TIME);
    gpio_pin_write(led_dev, LED, 1);
    k_sleep(SLEEP_TIME);
    gpio_pin_write(led_dev, LED, 0);


	if (!adc_dev) {
		SYS_LOG_INF("Cannot get ADC device\n");
		return;
	}

	/* 1. Verify adc_enable() */
	adc_enable(adc_dev);

	k_sleep(500);

	/* 2. Verify adc_read() */
	ret = adc_read(adc_dev, &table);
	if (ret != 0) {
		SYS_LOG_INF("Failed to fetch sample data from ADC controller\n");
		return;
	}

	SYS_LOG_INF("Channel %d ADC Sample: ", ADC_CHANNEL);
	for (i = 0; i < BUFFER_SIZE; i++) {
		SYS_LOG_INF("%d ", seq_buffer[i]);
	}
	SYS_LOG_INF("\n");

	/* 3. Verify adc_disable() */
	adc_disable(adc_dev);
    
    //k_timer_start(&led_timer,10,10);
#endif
}

void main(void)
{
	init_app();
    
    djlzork();

	if (IS_ENABLED(CONFIG_NET_TCP)) {
		start_tcp();
	}

	if (IS_ENABLED(CONFIG_NET_UDP)) {
		start_udp();
	}

	k_sem_take(&quit_lock, K_FOREVER);

	NET_INFO("Stopping...");

	if (IS_ENABLED(CONFIG_NET_TCP)) {
		stop_tcp();
	}

	if (IS_ENABLED(CONFIG_NET_UDP)) {
		stop_udp();
	}
}
