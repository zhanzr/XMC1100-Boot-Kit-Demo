set(CMAKE_C_COMPILER "armcc")
set(CMAKE_CXX_COMPILER "armcc")
set(CMAKE_EXECUTABLE_SUFFIX "axf")

set(VERBOSE "1")

cmake_minimum_required(VERSION 3.14)

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

set(CMAKE_EXE_LINKER_FLAGS "--cpu ${CORE_NAME} --strict --scatter ${SCATTER} --summary_stderr --info summarysizes --map --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers" 
)
 
#set(CMAKE_TOOLCHAIN_FILE "D:/CMake/share/cmake-3.15/Modules/Compiler/ARMCC.cmake") 
#set(CMAKE_ARMCC-C_COMPILER_ENV_VAR "")

#ENABLE_LANGUAGE(ARMCC-C)
#ENABLE_LANGUAGE(C)
#ENABLE_LANGUAGE(ASM)

#SET(CMAKE_ASM_FLAGS "${CFLAGS} -x assembler-with-cpp")

#set(LINKER_SCRIPT "mylinker.ld")

set(CPU_OPTIONS "-mthumb -mcpu=cortex-m0")

#SET(CMAKE_ASM_FLAGS_DEBUG "-g")
#set(CMAKE_C_FLAGS_DEBUG "-Og -g -DDEBUG")
#set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")

add_executable(flash_debug led.c
main.c
ring_buffer.c
shell.c
stdio_impl.c
xmc1100_it.c
)

target_include_directories(flash_debug PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(flash_debug PRIVATE ${XMCLIB})
target_include_directories(flash_debug PRIVATE ${DEVICE_INC})
target_include_directories(flash_debug PRIVATE ${CMSIS_CORE_INC})
target_include_directories(flash_debug PRIVATE ${RTE_INC})
target_include_directories(flash_debug PRIVATE ${RTE_DEVICE_INC})


