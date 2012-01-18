#!/usr/bin/perl
unless($ARGV[0])
{
	print "Please, use: 1.pl <filename> \n";
	exit;
}

$\='';
eval("use String::CRC32;");
die ("Module String::CRC32 was not found!") if $@;

open FILE, "<", $ARGV[0] or die("Cannot open file $!");
binmode FILE;
$/=\0;
$len=0;
$cont = <FILE>;
$len=length($cont);
close FILE or die("cannot close file $!");

$crc32 = crc32($cont);
print "CRC32: 0x";
printf("%02x\n", $crc32);

if($len > 512 * 32)
{
	print "ERROR!File is too big! \n";
	exit;
}

$outasm = '

        org     0x7C00

	cli
        jmp     0x0:start
start:
        push    cs
        pop     es
        push    cs
        pop     ds
        push    cs
        pop     ss

        mov     sp, 0xffff
        sti

        mov     ah, 2
        mov     al, ' . ($len%512==0?(int $len/512 ):(int ($len/512) + 1)) . '
        mov     cx, 4
        mov     bx, buf
        int     0x13

        jc      @err

;--------i8086,i286------------
	pushf
	pop	ax
	and	ax, 0F000h
	cmp	ax, 0
	je	ok321
	cmp	ax, 0F000h
	je	ok321
;------i386---------------------
        mov     cx, sp
        sub     cx, 4
        pushfd
        cmp     cx,sp
        je      ok32
        popfd
n32:    mov     bp,not32str
        mov     bx,4
        mov     ah,3
        int     0x10
        mov     cx,18
        mov     ax,0x1301
        int     0x10
        jmp     @a
ok32:
        popfd
ok321:
        mov     ecx, 0
        xor     di, di
_tab:
        mov     eax, ecx
        push    cx
        mov     cx, 8
_mktab:
        shr     eax, 1
        jnc     mktabend
        xor     eax, 0xEDB88320
mktabend:
        loop    _mktab
        mov     [crctab+di], eax
        add     di, 4
        pop     cx

        inc     cx
        cmp     cx,0xff
        jbe     _tab

        mov     eax, 0xFFFFFFFF
        mov     ecx, [len]
        xor     esi, esi
calc:
        mov     edx, eax
        shr     edx, 8
        mov     ebx, eax
        xor     bl, [buf+esi]
        inc     esi
        and     ebx, 0xff
        shl     ebx, 2
        mov     ebx, [ebx+crctab]
        xor     ebx, edx
        mov     eax, ebx
        loop    calc
        xor     eax, 0xFFFFFFFF


        cmp     eax, [crc]
        je      crcok
        mov     bp, failstr
        mov     bx, 4
        mov     ah, 3
        int     0x10
        mov     cx, 15
        mov     ax, 0x1301
        int     0x10
@err:
        mov     bp, errstr
        mov     bx, 4
        mov     ah, 3
        int     0x10
        mov     cx,24
        mov     ax,0x1301

@a:
        cli
        hlt
        jmp     @a
crcok:
	jmp	buf
        jmp     @a

failstr:        db      \'CRC not equal! \'
errstr: db      \'Error. Execution stopped.\'
not32str:       db      \'Error. Not 32-bit.\'
len:    dd      ' . $len . '
crc:    dd      ' . $crc32 . '
        times   510-($-$$) db 0
        db      0x55
        db      0xaa
crctab: times   256 dd 0
buf:
';

open FILE, ">tmp.asm" or die("cannot open file $!");
print FILE $outasm or die("cannot write in file $!");
close FILE or die("can't close file $!");

system("nasm tmp.asm") == 0 or die("cannot execute nasm: $!");
system("cat tmp $ARGV[0] > load") == 0 or die("cannot execute cat: $!");

$draft = $len %512;
$draft = 512 if $draft==0;

$count0 = chr(0) x (512-$draft);
open FILE, ">>load" or die("cannot open file $!");
binmode FILE;
print FILE $count0 or die("cannot write in file $!");
close FILE or die("cannot close file $!");
