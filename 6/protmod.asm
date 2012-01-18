org 0x7c00
jmp 0x0:start
start:
cli
xor ax,ax
mov ss,ax
mov ds,ax
mov es,ax
mov sp,0xffff
sti
mov ah, 2
mov al,1
mov cx, 2
mov bx, buf
int 0x13
cli
lgdt [seg0ptr]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp 8:continue
seg0ptr dw 24
dd segdes0
align 16
segdes0 dq 0
segdes1 db 0xff
	db 0xff
	db 0
	db 0
	db 0
	db 0x9a
	db 0x4f
	db 0
segdes2 db 0xff
	db 0xff
	db 0
	db 0
	db 0
	db 0x92
	db 0x4f
	db 0
BITS 32
continue:
mov ax, 16
mov ds, ax
mov ss, ax
mov esp, 0x10000
sti
call buf
loop:
cli
hlt
jmp loop
times 510-($-$$) db 0
db 0x55,0xaa
buf: