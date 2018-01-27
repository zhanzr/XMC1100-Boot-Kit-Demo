RAM Test Demo.

March C and March X algorithm.

The memeory test function needs to be located before the CMSIS startup function.

So the reset handler would be customized handler, which will be executed just after
the power up. During the memory test, the stack will be destroyed, so any function need
stack to function normally such as STDIO interface would be unavailable.

After the memory test, depending on the result of the test, two possibilities:
1. Fail Safe if the memory test failed.
2. Compiler supplied reset handler if the memory test pass.
