
	UserCodePatch

 patch user code


 	~~ UserBranchPatch ~~

 Patch bl-instruction of user code
 Refer to UserBranchPatch/UserBranchPatch.c

 	Patch from (bl from) to (bl to) in func

 One thing that I have to do before patch is to add Write-Permission to text segment of ELF-Binary.
 I did it manually by HEX-Editor