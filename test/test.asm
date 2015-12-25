;
;  This program runs in 32-bit protected mode.
;  gcc links the standard-C library by default

;  build: nasm -f elf -F stabs name.asm
;  link:  gcc -o name name.o
;
; In 64-bit long mode you can use 64-bit registers (e.g. rax instead of eax, rbx instead of ebx, etc..)
; Also change "-f elf " for "-f elf64" in build command.
;
BITS 64

        global  main                                ;main must be defined as it being compiled against the C-Standard Library
        extern  printf                               ;declares use of external symbol as printf is declared in a different object-module. 
                                                           ;Linker resolves this symbol later.

SECTION .data                                       ;section for initialized data
	string db 'Hello world!', 0Ah, 0h           ;message string with new-line char (10 decimal) and the NULL terminator
                                                    ;string now refers to the starting address at which 'Hello, World' is stored.

SECTION .text
main:
		
		cmp ax, 234556
       	mov rdi, string
   		mov rax, 0
   		push  ax,
   		cmp  ax,[rsp]                        ;advances stack-pointer by 4 flushing out the pushed string argument
		call fun
		pop ax,
        mov rax,60
        mov rdi,0
        syscall

fun:
		enter 0,0
		leave
		ret
