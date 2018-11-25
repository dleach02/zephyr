/*
 * Copyright (c) 2018 Foundries.io Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SOC_RISCV32_OPENISA_RV32M1_SOC_H_
#define SOC_RISCV32_OPENISA_RV32M1_SOC_H_

#ifndef _ASMLANGUAGE

#include "fsl_device_registers.h"

void soc_interrupt_init(void);

#endif	/* !_ASMLANGUAGE */

#if defined(CONFIG_SOC_OPENISA_RV32M1_RI5CY)
#include "soc_ri5cy.h"
#elif defined(CONFIG_SOC_OPENISA_RV32M1_ZERO_RISCY)
#include "soc_zero_riscy.h"
#endif

#endif /* SOC_RISCV32_OPENISA_RV32M1_SOC_H_ */
