# Copyright (c) 2018 Foundries.io Ltd
#
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_SOC_OPENISA_RV32M1_RI5CY)
  if(CONFIG_RISCV_GENERIC_TOOLCHAIN)
    # The RI5CY core has some ISA extensions that are best supported
    # using the Pulpino toolchain. Use of a generic toolchain is
    # untested and left to future work.
    message(FATAL_ERROR "generic toolchain is not supported")
    zephyr_compile_options(-march=rv32imcxpulpv2)
  endif()
elseif(CONFIG_SOC_OPENISA_RV32M1_ZERO_RISCY)
  zephyr_compile_options(-march=rv32imc)
endif()

zephyr_sources(
  vector.S
  soc_irq.S
  wdog.S
  soc.c
)
