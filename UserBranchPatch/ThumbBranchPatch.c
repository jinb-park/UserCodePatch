#include <stdio.h>

#define ___asm_opcode_identity32(x) ((x) & 0xFFFFFFFF)
#define ___asm_opcode_identity16(x) ((x) & 0xFFFF)

#define ___opcode_identity32(x) ___asm_opcode_identity32(x)
#define ___opcode_identity16(x) ___asm_opcode_identity16(x)

/* Operations to construct or split 32-bit Thumb instructions: */
#define __opcode_thumb32_first(x) (___opcode_identity16((x) >> 16))
#define __opcode_thumb32_second(x) (___opcode_identity16(x))
#define __opcode_thumb32_compose(first, second) ( (___opcode_identity32(___opcode_identity16(first)) << 16) | ___opcode_identity32(___opcode_identity16(second)) )		

unsigned long Thumb2_GenBranch(unsigned long pc, unsigned long addr, int link)
{
	unsigned long s, j1, j2, i1, i2, imm10, imm11;
	unsigned long first, second;
	long offset;
	unsigned long branch = 0x00;

	offset = (long)addr - (long)(pc + 4);
	if (offset < -16777216 || offset > 16777214) {
		return 0;
	}

	s	= (offset >> 24) & 0x1;
	i1	= (offset >> 23) & 0x1;
	i2	= (offset >> 22) & 0x1;
	imm10	= (offset >> 12) & 0x3ff;
	imm11	= (offset >>  1) & 0x7ff;

	j1 = (!i1) ^ s;
	j2 = (!i2) ^ s;

	first = 0xf000 | (s << 10) | imm10;
	second = 0x9000 | (j1 << 13) | (j2 << 11) | imm11;
	if (link)
		second |= 1 << 14;

	branch = __opcode_thumb32_compose(first, second);
	return (branch << 16) | (branch >> 16);
}

/*
 * Replace (bl from) with (bl to) in func
*/
void Thumb_CodePatch(unsigned long *func, unsigned long *from, unsigned long *to) {
	unsigned long i = 0;

	unsigned short thumb1Inst = 0x00;
	unsigned long thumb2Inst = 0x00;

	unsigned long fromInst = 0x00;

	unsigned long *dest = 0x00;
	unsigned long pc = 0x00;
	char *ptr = &thumb1Inst;

	unsigned long finalInst = 0x00;	// final instruction to be patched

	/* func is Thumb-Function */
	/* [ToDo] How to know func is Thumb?? */
	func = (unsigned long)func + 1;

	while(1) {
		thumb1Inst = *((unsigned short*)func + i);

		/* If pop {rN, pc}, It's last of func */
		if( (thumb1Inst & 0xff00) == 0xbd00 ) {
			break;
		}

		/* Thumb2 - bl */
		if( thumb1Inst == 0xf7ff ) {
			pc = (unsigned long)((unsigned short*)func + i);
			thumb2Inst = *((unsigned long*)pc);

			fromInst = Thumb2_GenBranch(pc, from, 1);
			if(thumb2Inst == fromInst) {
				finalInst = Thumb2_GenBranch(pc, to, 1);

				printf("dest : [%08x], orig-inst : [%08x], patched-inst : [%08x]\n", (unsigned long)to, thumb2Inst, finalInst);

				/* Patch */
				*((unsigned long*)pc) = finalInst;
			}
		}
		i++;
	}
}
