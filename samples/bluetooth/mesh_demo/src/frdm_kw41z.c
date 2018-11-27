/* frdm_kw41z.c - FRDM KW41Z specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gpio.h>
#include <soc.h>
#include <misc/printk.h>
#include <ctype.h>
#include <pwm.h>

#include <display/mb_display.h>

#include <bluetooth/mesh.h>

#include "board.h"

#define SCROLL_SPEED   K_MSEC(300)

#define BEEP_DURATION  K_MSEC(60)

#define SEQ_PER_BIT  976
#define SEQ_PAGE     (NRF_FICR->CODEPAGESIZE * (NRF_FICR->CODESIZE - 1))
#define SEQ_MAX      (NRF_FICR->CODEPAGESIZE * 8 * SEQ_PER_BIT)

static struct device *gpio;
static struct device *gpio_red;
static struct device *gpio_green;
static struct device *gpio_blue;

static struct device *nvm;

static struct k_work button_work;

void configure_color(u16_t addr);
void color_reset(void);


static void button_send_pressed(struct k_work *work)
{
	configure_color(0);
	printk("%s()\n", __func__);
	board_button_1_pressed();
	color_reset();
}

static void button_pressed(struct device *dev,
			   struct gpio_callback *cb,
			   u32_t pins)
{
	printk("Button pressed at %u, %x\n", k_cycle_get_32(), pins);

	if (pins & BIT(SW1_GPIO_PIN)) {
		k_work_submit(&button_work);
	} else {
		u16_t target = board_set_target();

		if (target > 0x0009) {
			printk("Target out of bound\n");
		} else {
			configure_color(target);
			printk(" Target Value = %u\n", (target & 0xf));
		}
	}
}

static const struct {
	char note;
	u32_t period;
	u32_t sharp;
} period_map[] = {
	{ 'C',  3822,  3608 },
	{ 'D',  3405,  3214 },
	{ 'E',  3034,  3034 },
	{ 'F',  2863,  2703 },
	{ 'G',  2551,  2407 },
	{ 'A',  2273,  2145 },
	{ 'B',  2025,  2025 },
};

static u32_t get_period(char note, bool sharp)
{
	int i;

	if (note == ' ') {
		return 0;
	}

	for (i = 0; i < ARRAY_SIZE(period_map); i++) {
		if (period_map[i].note != note) {
			continue;
		}

		if (sharp) {
			return period_map[i].sharp;
		} else {
			return period_map[i].period;
		}
	}

	return 1500;
}

void board_play_tune(const char *str)
{
	printk("Entered Board Play Tune Function\n");
	while (*str) {
		u32_t period, duration = 0;

		while (*str && !isdigit((unsigned char)*str)) {
			str++;
		}

		while (isdigit((unsigned char)*str)) {
			duration *= 10;
			duration += *str - '0';
			str++;
		}

		if (!*str) {
			break;
		}

		if (str[1] == '#') {
			period = get_period(*str, true);
			str += 2;
		} else {
			period = get_period(*str, false);
			str++;
		}

		k_sleep(duration);
	}
	color_reset();
}

void board_heartbeat(u8_t hops, u16_t feat)
{
	printk("%u hops\n", hops);

	if (hops) {
		printk("If Hops Section\n");
	}
}

void board_other_dev_pressed(u16_t addr)
{
	configure_color(addr);
	printk("%s(0x%04x)\n", __func__, addr);
}

void board_attention(bool attention)
{
	if (attention) {
		printk("Board Attention Function\n");
	} else {
		printk("Board Attention stop disp Function\n");
	}
}

void color_reset(void)
{
	gpio_pin_write(gpio_red, LED2_GPIO_PIN, 1);
	gpio_pin_write(gpio_green, LED0_GPIO_PIN, 1);
	gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 1);
}


void configure_color(u16_t addr)
{
	switch (addr) {
	/* Color White */
	case 0:
		gpio_pin_write(gpio_red, LED2_GPIO_PIN, 0);
		gpio_pin_write(gpio_green, LED0_GPIO_PIN, 0);
		gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 0);
		break;
	/* Color Red */
	case 1:
		gpio_pin_write(gpio_red, LED2_GPIO_PIN, 0);
		gpio_pin_write(gpio_green, LED0_GPIO_PIN, 1);
		gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 1);
		break;
	/* Color Green */
	case 2:
		gpio_pin_write(gpio_red, LED2_GPIO_PIN, 1);
		gpio_pin_write(gpio_green, LED0_GPIO_PIN, 0);
		gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 1);
		break;
	/* Color Blue */
	case 3:
		gpio_pin_write(gpio_red, LED2_GPIO_PIN, 1);
		gpio_pin_write(gpio_green, LED0_GPIO_PIN, 1);
		gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 0);
		break;
	/* Color Yellow */
	case 4:
		gpio_pin_write(gpio_red, LED2_GPIO_PIN, 0);
		gpio_pin_write(gpio_green, LED0_GPIO_PIN, 0);
		gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 1);
		break;
	/* Color Magenta */
	case 5:
		gpio_pin_write(gpio_red, LED2_GPIO_PIN, 0);
		gpio_pin_write(gpio_green, LED0_GPIO_PIN, 1);
		gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 0);
		break;
	/* Color Cyan */
	case 6:
		gpio_pin_write(gpio_red, LED2_GPIO_PIN, 1);
		gpio_pin_write(gpio_green, LED0_GPIO_PIN, 0);
		gpio_pin_write(gpio_blue, LED1_GPIO_PIN, 0);
		break;
	/* No Color */
	default:
		color_reset();
		break;
	}
}

static void configure_button(void)
{
	static struct gpio_callback button_cb;

	k_work_init(&button_work, button_send_pressed);

	gpio = device_get_binding(SW0_GPIO_CONTROLLER);

	gpio_pin_configure(gpio, SW0_GPIO_PIN,
			   (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE |
			    GPIO_INT_ACTIVE_LOW | SW0_GPIO_FLAGS));
	gpio_pin_configure(gpio, SW1_GPIO_PIN,
			   (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE |
			    GPIO_INT_ACTIVE_LOW | SW1_GPIO_FLAGS));

	gpio_init_callback(&button_cb, button_pressed,
			   BIT(SW0_GPIO_PIN) | BIT(SW1_GPIO_PIN));
	gpio_add_callback(gpio, &button_cb);

	gpio_pin_enable_callback(gpio, SW0_GPIO_PIN);
	gpio_pin_enable_callback(gpio, SW1_GPIO_PIN);
}


void configure_led(void)
{
	gpio_red = device_get_binding(LED2_GPIO_CONTROLLER);
	gpio_green = device_get_binding(LED0_GPIO_CONTROLLER);
	gpio_blue = device_get_binding(LED1_GPIO_CONTROLLER);

	gpio_pin_configure(gpio_red, LED2_GPIO_PIN, GPIO_DIR_OUT);
	gpio_pin_configure(gpio_green, LED0_GPIO_PIN, GPIO_DIR_OUT);
	gpio_pin_configure(gpio_blue, LED1_GPIO_PIN, GPIO_DIR_OUT);
}


void board_init(u16_t *addr)
{
	nvm = device_get_binding(DT_FLASH_DEV_NAME);
	printk("KW41Z %s entry\n", __func__);
	*addr = 0x0000;
	if (!*addr || *addr == 0xffff) {
#if defined(NODE_ADDR)
		*addr = NODE_ADDR;
#else
		*addr = 0x0b0c;
#endif
	}
	configure_button();
	configure_led();
	color_reset();
}
