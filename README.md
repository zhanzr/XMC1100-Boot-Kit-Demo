Will use the hardware ECC support to test the on chip FLASH.

The Flash ECC test uses error injection to write corrupted data into the flash page to ensure that the single-bit
and multi-bit errors are correctly detected. If the page is not programmed then it is allocated, programmed and
corrupted by the POST function. The test checks the detection of single and multi-bit errors on the flash ECC.
Interrupts are disabled during this test to ensure that the test does not raise an SCU interrupt. The result of the
test is returned to the caller.
