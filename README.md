Coremark for XMC1100 Bootkit board.

========================================
#define ITERATIONS    2000
#define    HZ    1000
ARMCC 5.06 -O3 -Otime microlib
CoreMark 1.0 : 40.287654 / ARMCC 5060422 --c99 -c --cpu Cortex-M0 -g -O3 -Otime --apcs=interwork --split_sections / STACK
=======================================
#define ITERATIONS    2000
#define    HZ    1000
ARMCC 5.06 -O3 -Otime stdlib
CoreMark 1.0 : 40.420372 / ARMCC 5060422 --c99 -c --cpu Cortex-M0 -g -O3 -Otime --apcs=interwork --split_sections / STACK
========================================
#define ITERATIONS    2000
#define    HZ    1000
ARMCLANG 6.12 stdlib
CoreMark 1.0 : 39.385585 / 4.2.1 Compatible Clang 9.0.0  xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m0 -gdwarf-3 -Ofast -ffunction-sections  / STACK
========================================
#define ITERATIONS    2000
#define    HZ    1000
ARMCLANG 6.12  microlib
CoreMark 1.0 : 39.384034 / 4.2.1 Compatible Clang 9.0.0  xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m0 -gdwarf-3 -Ofast -ffunction-sections  / STACK