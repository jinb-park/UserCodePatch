#include <stdio.h>
#include <unistd.h>

void funcB(void) {
	printf("funcB\n");
}

void funcA(void) {
	printf("funcA\n");
}

void CodePatch(unsigned int *func, unsigned int *from, unsigned int *to);
unsigned int *GetDestAddr(unsigned int pc, int inst);
int GetBlOffset(unsigned int pc, unsigned int *dest);

int main() {
	unsigned int cnt = 6;
	unsigned int i;

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

unsigned int *GetDestAddr(unsigned int pc, int inst) {
	unsigned int *dest = 0x00;
	int mask = 0xff000000;

	inst = (inst | mask);
	inst = (inst << 2);
	dest = (pc + inst);
	
	return dest;
}

int GetBlOffset(unsigned int pc, unsigned int *dest) {
	int off = 0xffffffff;

	off = (int)((unsigned int)dest - pc);
	off = (off >> 2);

	return off;
}

unsigned int GetFinalBlInst(int offset) {
	unsigned int inst = 0x00;
	char *ptr = &inst;

	inst = (unsigned int)offset;
	ptr[3] = 0xeb;
	return inst;
}

/*
 * Replace (bl from) with (bl to) in func
*/
void CodePatch(unsigned int *func, unsigned int *from, unsigned int *to) {
	unsigned int i = 0;
	unsigned int inst = 0x00;
	unsigned int *dest = 0x00;
	int offset = 0x00;
	unsigned int pc = 0x00;
	char *ptr = &inst;

	unsigned int finalInst = 0x00;	// final instruction to be patched

	while(1) {
		inst = *(func + i);

		/* If pop {fp, pc}, It's last of func */
		if( inst == 0xe8bd8800 ) {
			break;
		}

		if( ptr[3] == 0xeb) {
			pc = (unsigned int)(func + i + 2);
			dest = GetDestAddr( pc, (int)inst );

			if(dest == from) {
				offset = GetBlOffset(pc, to);
				finalInst = GetFinalBlInst(offset);
				printf("dest : [%08x], orig-inst : [%08x], patched-inst : [%08x]\n", (unsigned int)dest, inst, finalInst);

				/* Patch */
				*(func + i) = finalInst;
			}
		}
		i++;
	}
}
