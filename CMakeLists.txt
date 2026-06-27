CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

# PROJECT FILE NAME
project(gpio_toggle_xmc11 C CXX ASM)

# Device selection
set(DEVICE XMC1100_T038x0064)

FIND_PACKAGE(CMSIS)
FIND_PACKAGE(XMCLib)
FIND_PACKAGE(Newlib)

include_directories(
    ${ProjDirPath}
    ${CMSIS_INCLUDE_DIRS}
    ${XMCLIB_INCLUDE_DIRS}
)

# Order of files is important for LTO to work properly
# https://github.com/ObKo/stm32-cmake/issues/78
# https://bugs.launchpad.net/gcc-arm-embedded/+bug/1747966
set(PROJECT_SOURCES 
    ${CMSIS_SOURCES} 
    ${XMCLIB_SOURCES} 
    ${NEWLIB_SOURCES}

    "main.c"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

XMC_SET_TARGET_PROPERTIES(${PROJECT_NAME})
XMC_ADD_DUMP_TARGET(${PROJECT_NAME})
XMC_ADD_HEX_TARGET(${PROJECT_NAME})
XMC_ADD_BIN_TARGET(${PROJECT_NAME})
XMC_PRINT_SIZE_OF_TARGETS(${PROJECT_NAME})

