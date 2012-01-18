C:\Dev-Cpp\bin\gcc -c main.c -o main.o
nasm -f win32 mult.asm -o mult.o
C:\Dev-Cpp\bin\gcc main.o mult.o -o main.txt