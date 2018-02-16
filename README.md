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
