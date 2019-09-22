Dhrystone for XMC1100 Bootkit board.

========================================
#define RUN_NUMBER    300000
ARMCC 5.06 -O3 -Otime microlib
Microseconds for one run through Dhrystone[56-15923]:    52.9
Dhrystones per Second:                      18907.2
========================================
#define RUN_NUMBER    300000
ARMCC 5.06 -O3 -Otime stdlib
Microseconds for one run through Dhrystone[22-7289]:    24.2
Dhrystones per Second:                      41282.5
=======================================
clang-format -style=llvm -dump-config > .clang-format
