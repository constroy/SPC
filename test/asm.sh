rm test.o test
nasm -f elf64 -F stabs test.asm && \
gcc -o test test.o