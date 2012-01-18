C:\Dev-Cpp\bin\gcc -c print.c
ld --script lnk.ld -o 1 print.o
objcopy -O binary 1 2
nasm protmod.asm
cat protmod 2 > res
rm print.o
rm 1
rm 2
rm protmod