# Copyright (c) 2018 Linaro Limited
#
# SPDX-License-Identifier: Apache-2.0
#

# N.B.: Temporary. See the README but currently need to manually copy
# the BLE Link Layer controller library from the NXP KW41Z SDK. NXP is
# working on a more seamless solution using an NXP managed git
# repository.

if(CONFIG_BT_LL_RV32M1)
  add_library(rv32m1_ble STATIC IMPORTED GLOBAL)
  set_target_properties(rv32m1_ble PROPERTIES IMPORTED_LOCATION
    ${ZEPHYR_BASE}/ext/lib/ble/rv32m1/lib_ble_k32w042_controller_zero_riscy.a
    )
  zephyr_append_cmake_library(rv32m1_ble)
endif()
