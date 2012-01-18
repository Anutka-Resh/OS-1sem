extern _print

section .text
global _multnum

_multnum:
	push ebp
	mov ebp, esp
	sub esp, 0x8

	finit
	fld qword [ebp+8]
	fimul dword [ebp+16]
	fstp qword [ebp-8]
	call _print
	mov esp, ebp
	pop ebp
	ret