#include <stdio.h>
#include <stdint.h>

void test_ram_code(uint32_t lp_cnt) {	

for(uint32_t i=0; i<lp_cnt; ++i)
	{
		//Test unsigned integer division
		{
			volatile uint32_t au = 101;
			volatile uint32_t bu = 10;
			volatile uint32_t cu = au/bu;
			volatile uint32_t du = au%bu;

			//		printf("%u %u -> %u %u\n",
			//				au, bu, cu, du);
		}

		//Test signed integer division
		{
			volatile int32_t ai = 101;
			volatile int32_t bi = 10;
			volatile int32_t ci = ai/bi;
			volatile int32_t di = ai%bi;

			//		printf("%i %i -> %i %i\n",
			//				ai, bi, ci, di);
		}

		//Test float division
		{
			volatile float af = 101.0;
			volatile float bf = 10.0;
			volatile float cf = af/bf;
			//		float df = af%bf;

			//		printf("%f %f -> %f\n",
			//				af, bf, cf);
		}
	}
	
	return;
}

//Blank function for size measure, the size could also be got in the map file.
void test_ram_code_func2(void) {
}

