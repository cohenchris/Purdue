# Framework structure
The framework consists of the following files and functions.
### Makefile
Do not modify the Makefile in your submission. The Makefile has several targets you should find useful.

- library : this will compile your testlib.so shared object library
- test_build : this will compile your testlib.so and the *_test.c files in the tests directory in a way which should be compatible with gcov.
- test this will make test_build and then call the test.py script you must implement

### utils.h/utils.c
Do not modify these helper functions. They are here to simplify your work. Please use the INFO(...) function defined here instead of printf in your work. This makes it easy to toggle printing when grading.

### testlib.h/testlib.c
This is a skeleton for your testing library implementation. The given functions should get intercepted by your library with the help of LD_PRELOAD. A simple example has already been added to help you.
The last function in testlib.c is a constructor function which will get called at the start of a target programs main function. Don't modify its priority of 200.

### framework.py
Do not modify this file. Wrapper which should greatly simplify setting environment variables when calling a target program with LD_PRELOAD. It is strongly recommended to use this in your work.

### tests/ directory
This is the directory where you should add programs you can use to test your library implementation.

### coverage.py
Do not modify this file. Runs all the test files in tests/ with environment variables mathcing what you provide. Finally, it will call gcov.

### test.py
This is where you should implement your testing framework to test your library. The skeleton simply calls coverage.py

### libunwind/ directory
This contains a compilation of the libunwind library. Don't touch it.
