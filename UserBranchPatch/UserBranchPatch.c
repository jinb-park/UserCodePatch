#include <stdio.h>
#include <unistd.h>
#include "UserBranchPatch.h"

#define FUNC_MODE_ARM 0
#define FUNC_MODE_THUMB 1

#if defined(__arm__) && defined(__thumb2__)
	#pragma message("thumb2");
#endif

void funcB(void) {
	printf("funcB\n");
}

void funcA(void) {
	printf("funcA\n");
}

void CodePatch(unsigned long *func, unsigned long *from, unsigned long *to);

int main() {
	unsigned long cnt = 6;
	unsigned long i;

	for(i=0; i<cnt; i++) {
		sleep(1);

		if(i == 3) {
			CodePatch(main, funcA, funcB);
		}
		funcA();
		funcB();
	}

	return 0;
}

/*
 * Replace (bl from) with (bl to) in func
*/
void CodePatch(unsigned long *func, unsigned long *from, unsigned long *to) {

#if defined(__arm__) && defined(__thumb2__)
	Thumb_CodePatch(func, from, to);
#else
	ARM_CodePatch(func, from, to);
#endif

}
