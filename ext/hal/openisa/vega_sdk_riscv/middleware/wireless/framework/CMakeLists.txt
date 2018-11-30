zephyr_include_directories(Common)
zephyr_include_directories(OSAbstraction/Interface)
zephyr_include_directories(XCVR/RV32M1)

add_subdirectory(XCVR/RV32M1)

zephyr_sources(OSAbstraction/Source/fsl_os_abstraction_zephyr.c)
