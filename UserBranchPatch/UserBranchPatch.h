#ifndef _USER_BRANCH_PATCH_H
#define _USER_BRANCH_PATCH_H

void ARM_CodePatch(unsigned long *func, unsigned long *from, unsigned long *to);
void Thumb_CodePatch(unsigned long *func, unsigned long *from, unsigned long *to);

#endif