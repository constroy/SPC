rm a.asm a.o a
./spc a.pas a.asm && \
nasm -f elf64 -F stabs a.asm && \
gcc -g -o a a.o && \
./a
