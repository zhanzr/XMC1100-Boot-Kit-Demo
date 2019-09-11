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
#define RUN_NUMBER    300000
ARMCLANG 6.12 -Ofast stdlib
Microseconds for one run through Dhrystone[14-6737]:    22.4 
Dhrystones per Second:                      44622.9
========================================
#define RUN_NUMBER    300000
ARMCLANG 6.12 -Oz stdlib
Microseconds for one run through Dhrystone[16-10925]:    36.4 
Dhrystones per Second:                      27500.2 
========================================
