/*
 * Copyright (c) 2017 Linaro Limited.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <entropy.h>
#include <random/rand32.h>
#include <init.h>

#include "fsl_trng.h"

static int entropy_rv32m1_trng_get_entropy(struct device *dev, u8_t *buffer,
					 u16_t length)
{
	status_t status;

	ARG_UNUSED(dev);

	status = TRNG_GetRandomData(TRNG0, buffer, length);
	__ASSERT_NO_MSG(!status);

	return 0;
}

static const struct entropy_driver_api entropy_rv32m1_trng_api_funcs = {
	.get_entropy = entropy_rv32m1_trng_get_entropy
};

static int entropy_rv32m1_trng_init(struct device *);

DEVICE_AND_API_INIT(entropy_rv32m1_trng, CONFIG_ENTROPY_NAME,
		    entropy_rv32m1_trng_init, NULL, NULL,
		    PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
		    &entropy_rv32m1_trng_api_funcs);

static int entropy_rv32m1_trng_init(struct device *dev)
{
	trng_config_t conf;
	status_t status;

	ARG_UNUSED(dev);

	status = TRNG_GetDefaultConfig(&conf);
	__ASSERT_NO_MSG(!status);

	status = TRNG_Init(TRNG0, &conf);
	__ASSERT_NO_MSG(!status);

	return 0;
}
