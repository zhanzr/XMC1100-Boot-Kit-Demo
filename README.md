Dhrystone for XMC1100 Bootkit board.

========================================
#define RUN_NUMBER    200000
ARMCC 5.06 -O3 -Otime microlib
MicroSecond for one run through Dhrystone[27-10618]:     52.955 
Dhrystones per Second:  18883.959 
DMIPS/MHz:      0.336
=======================================
#define RUN_NUMBER    200000
MicroSecond for one run through Dhrystone[9-4866]:       24.285 
Dhrystones per Second:  41177.684 
DMIPS/MHz:      0.732
=======================================
using dynamic wait latency
USE_DYNAMIC_FLASH_WS

MicroSecond for one run through Dhrystone[10-5406]:      26.980 
Dhrystones per Second:  37064.492 
DMIPS/MHz:      0.659
=======================================
