.code

RunProc64 proc       
; rcx: proc
; rdx: remaining
;  r8: argv
;  r9: x64cc
    mov rdi, rcx
    mov r10, rdx
    mov rsi, r8
    mov r8, r9
    shl r9, 2
    std
_more_args:
    cmp r10, r9
    jle _register_args
  lodsq
    dec r10
   push rax
    jmp _more_args
_register_args:
   test r8, r8
     jz _pp0
   test r10, r10
     jz _populate_registers
    mov r8, r10
    dec r10
    shl r10, 3
    sub rsi, r10
    mov r10, r8
    xor r9, r9
    cld
    jmp _more_args
_populate_registers:
    cmp r8, 1
    jle _pp1
    cmp r8, 2
    jle _pp2
    cmp r8, 3
    jle _pp3
    pop r9
_pp3:
    pop r8
_pp2:
    pop rdx
_pp1:
    pop rcx
_pp0:
    sub rsp, 40
   call rdi
    add rsp, 40
    ret
RunProc64 endp

end