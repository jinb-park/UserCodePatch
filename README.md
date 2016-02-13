
	UserCodePatch

 patch user code


 	~~ UserBranchPatch ~~

 Patch bl-instruction of user code
 Refer to UserBranchPatch/UserBranchPatch.c. 
 Support both ARM & Thumb function patch. (32 bit)

 	Patch from (bl from) to (bl to) in func

 One thing that I have to do before patch is to add Write-Permission to text segment of ELF-Binary.
 I did it manually by HEX-Editor

 * UserBranchPatch.ARM - ELF Binary that patch ARM-Function. It have tested on qemu.
 * UserBranchPatch.Thumb - ELF Binary that patch Thumb-Function. It have tested on Rasberrypi-2. (UbuntuMate 15.10)

