#include <stdio.h>

unsigned long ARM_GenBranch(unsigned long pc, unsigned long addr, int link)
{
	unsigned long opcode = 0xea000000;
	long offset;

	if (link)
		opcode |= 1 << 24;

	offset = (long)addr - (long)(pc + 8);
	if (offset < -33554432 || offset > 33554428) {
		return 0;
	}

	offset = (offset >> 2) & 0x00ffffff;

	return opcode | offset;
}

/*
 * Replace (bl from) with (bl to) in func
*/
void ARM_CodePatch(unsigned long *func, unsigned long *from, unsigned long *to) {
	unsigned long i = 0;
	unsigned long inst = 0x00;
	unsigned long fromInst = 0x00;
	unsigned long *dest = 0x00;
	unsigned long pc = 0x00;
	char *ptr = &inst;

	unsigned long finalInst = 0x00;	// final instruction to be patched

	while(1) {
		inst = *(func + i);

		/* If pop {fp, pc}, It's last of func */
		if( inst == 0xe8bd8800 ) {
			break;
		}

		if( ptr[3] == 0xeb) {
			pc = (unsigned long)(func + i);
			fromInst = ARM_GenBranch(pc, from, 1);

			if(inst == fromInst) {
				finalInst = ARM_GenBranch(pc, to, 1);

				printf("dest : [%08x], orig-inst : [%08x], patched-inst : [%08x]\n", (unsigned long)to, inst, finalInst);

				/* Patch */
				*(func + i) = finalInst;
			}
		}
		i++;
	}
}
