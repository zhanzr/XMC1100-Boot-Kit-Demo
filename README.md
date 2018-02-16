Sieve for XMC1100 Bootkit board.

========================================
#define ITERATION_NUM    200
#define SIEVE_TEST_SIZE    8190
#define    HZ    1000
ARMCC 5.06 -O3 -Otime microlib
200 iterations
1899 primes.
Time: 11.042 sec.
========================================
#define ITERATION_NUM    200
#define SIEVE_TEST_SIZE    8190
#define    HZ    1000
ARMCC 5.06 -O3 -Otime stdlib
200 iterations
1899 primes.
Time: 2.478 sec.
