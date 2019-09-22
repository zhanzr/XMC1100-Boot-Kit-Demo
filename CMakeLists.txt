cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER "armcc")
set(CMAKE_ASM_COMPILER "armasm")
set(CMAKE_CXX_COMPILER "armcc")
set(FROMELF "fromelf")

set(OUTPUT_SUFFIX ".axf")
                  
ENABLE_LANGUAGE(C)
ENABLE_LANGUAGE(ASM)

set(VERBOSE "1")

project (flash_debug)
                  
set(SCATTER "${CMAKE_CURRENT_SOURCE_DIR}/flash_debug.sct")

set(CORE_NAME "Cortex-M0")
set(DERIVATE "XMC1100_T038x0064")

set(XMCLIB = "G:/Keil_v5/ARM/PACK/Infineon/XMC1000_DFP/2.11.0/Device/XMClib/inc")
set(DEVICE_INC "G:/Keil_v5/ARM/PACK/Infineon/XMC1000_DFP/2.11.0/Device/XMC1100_series/Include")
set(CMSIS_CORE_INC "G:/Keil_v5/ARM/PACK/ARM/CMSIS/5.6.0/CMSIS/Core/Include")
set(RTE_INC "RTE/_flash_debug/")
set(RTE_DEVICE_INC "RTE/Device/XMC1100-T038x0064/")

set(OPT_LEVEL 0)

set(CMAKE_C_FLAGS "-D${DERIVATE} --c99 --cpu ${CORE_NAME} -g -O${OPT_LEVEL} --apcs=interwork --split_sections -D_RTE_"
)

set(AFLAGS "--cpu ${CORE_NAME}")
set(CMAKE_ASM_FLAGS "${AFLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "--cpu ${CORE_NAME} --strict --scatter ${SCATTER} --summary_stderr --info summarysizes --map --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers" 
)

add_executable(${CMAKE_PROJECT_NAME} 
"RTE/Device/XMC1100-T038x0064/startup_xmc1100.s"
"RTE/Device/XMC1100-T038x0064/system_XMC1100.c"
retarget_io.c
led.c
main.c
ring_buffer.c
shell.c
stdio_impl.c
xmc1100_it.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc_common.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc1_flash.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc1_gpio.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc_gpio.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc_usic.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc_uart.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc1_scu.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc_rtc.c
${CMAKE_CURRENT_SOURCE_DIR}/XMCLib/src/xmc1_rtc.c
)

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES 
                  SUFFIX ${OUTPUT_SUFFIX})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${XMCLIB}
 ${DEVICE_INC}
 ${CMSIS_CORE_INC}
 ${RTE_INC}
 ${RTE_DEVICE_INC})

add_custom_command(
  OUTPUT "${CMAKE_PROJECT_NAME}.hex"
  COMMAND ${FROMELF} "${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}" -v --i32combined --output "${CMAKE_PROJECT_NAME}.hex"
  DEPENDS "${CMAKE_PROJECT_NAME}${OUTPUT_SUFFIX}"
  ARGS    ""
  COMMENT "Converting binary file from elf"
)

add_custom_target(hex_file ALL echo
      DEPENDS   "${CMAKE_PROJECT_NAME}.hex"
    )
    