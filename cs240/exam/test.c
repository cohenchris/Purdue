#include <stdio.h>

#define C 1
#define A B
#define B C

int main() {
	int x = 0;
	#if (A == 1)
		return;
	#else
		return 0;
	#endif
}

