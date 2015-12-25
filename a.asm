BITS 64
extern exit
extern scanf
extern printf
global main
SECTION .data
fmt0	db '%hd', 0h
fmt1	db '%hd', 0h
fmt2	db 'the result is : %hd', 0Ah, 0h
fmt3	db ' %hd', 0Ah, 0h
SECTION .text
main:
label_1:    enter 220, 0
label_2:    xchg rbp, r8
label_3:    mov [r8-2], bx
label_4:    mov [r8-4], si
label_5:    mov [r8-212], di
label_6:    push r8
label_7:    lea rsi, [r8-212]
label_8:    mov rdi, fmt0
label_9:    xor rax, rax
label_10:   call scanf
label_11:   pop r8
label_12:   mov bx, [r8-2]
label_13:   mov si, [r8-4]
label_14:   mov di, [r8-212]
label_15:   mov [r8-2], bx
label_16:   mov [r8-4], si
label_17:   mov [r8-212], di
label_18:   push r8
label_19:   lea rsi, [r8-220]
label_20:   mov rdi, fmt1
label_21:   xor rax, rax
label_22:   call scanf
label_23:   pop r8
label_24:   mov bx, [r8-2]
label_25:   mov si, [r8-4]
label_26:   mov di, [r8-212]
label_27:   mov cx, 0
label_28:   mov word [r8-214], cx
label_29:   mov cx, 1
label_30:   mov word [r8-218], cx
label_31:   mov cx, word [r8-218]
label_32:   imul cx, word [r8-220]
label_33:   mov word [r8-218], cx
label_34:   mov cx, word [r8-218]
label_35:   add word [r8-214], cx
label_36:   push word [r8-214]
label_37:   mov cx, 0
label_38:   mov word [r8-210], cx
label_39:   mov cx, 1
label_40:   mov word [r8-216], cx
label_41:   mov cx, word [r8-216]
label_42:   imul cx, di
label_43:   mov word [r8-216], cx
label_44:   mov cx, word [r8-216]
label_45:   add word [r8-210], cx
label_46:   push word [r8-210]
label_47:   mov [r8-2], bx
label_48:   mov [r8-4], si
label_49:   mov [r8-212], di
label_50:   call main@swap@
label_51:   mov bx, [r8-2]
label_52:   mov si, [r8-4]
label_53:   mov di, [r8-212]
label_54:   add rsp, 2
label_55:   add rsp, 2
label_56:   mov cx, 0
label_57:   mov word [r8-6], cx
label_58:   mov si, 1
label_59:   imul si, di
label_60:   add word [r8-6], si
label_61:   mov [r8-2], bx
label_62:   mov [r8-4], si
label_63:   mov [r8-212], di
label_64:   push r8
label_65:   movsx rsi, di
label_66:   mov rdi, fmt2
label_67:   xor rax, rax
label_68:   call printf
label_69:   pop r8
label_70:   mov bx, [r8-2]
label_71:   mov si, [r8-4]
label_72:   mov di, [r8-212]
label_73:   mov cx, 0
label_74:   mov word [r8-8], cx
label_75:   mov bx, 1
label_76:   imul bx, word [r8-220]
label_77:   add word [r8-8], bx
label_78:   mov [r8-2], bx
label_79:   mov [r8-4], si
label_80:   mov [r8-212], di
label_81:   push r8
label_82:   movsx rsi, word [r8-220]
label_83:   mov rdi, fmt3
label_84:   xor rax, rax
label_85:   call printf
label_86:   pop r8
label_87:   mov bx, [r8-2]
label_88:   mov si, [r8-4]
label_89:   mov di, [r8-212]
label_90:   mov ax, 0
label_91:   xchg rbp, r8
label_92:   leave
label_93:   call exit
main@swap@:
label_95:   enter 14, 0
label_96:   xchg rbp, r9
label_97:   mov si, 0
label_98:   mov cx, 1
label_99:   mov word [r9-12], cx
label_100:  mov cx, word [r9-12]
label_101:  imul cx, word [r9+16]
label_102:  mov word [r9-12], cx
label_103:  add si, word [r9-12]
label_104:  mov word [r9-8], si
label_105:  mov di, 0
label_106:  mov cx, 1
label_107:  mov word [r9-14], cx
label_108:  mov cx, word [r9-14]
label_109:  imul cx, word [r9+18]
label_110:  mov word [r9-14], cx
label_111:  add di, word [r9-14]
label_112:  mov word [r9+16], di
label_113:  mov cx, 0
label_114:  mov word [r9-10], cx
label_115:  mov bx, 1
label_116:  imul bx, word [r9-8]
label_117:  add word [r9-10], bx
label_118:  mov cx, word [r9-10]
label_119:  mov word [r9+18], cx
label_120:  xchg rbp, r9
label_121:  leave
label_122:  ret
