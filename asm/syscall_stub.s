; syscall_stub.s — Assembly trampoline for INT 0x80
;
; Saves caller registers, builds C-calling-convention arguments from
; eax/ebx/ecx/edx, calls syscall_handler(), restores registers.

section .text
global syscall_stub
extern syscall_handler

syscall_stub:
    pusha               ; save all general-purpose registers

    push edx            ; arg3
    push ecx            ; arg2
    push ebx            ; arg1
    push eax            ; syscall number

    call syscall_handler

    add esp, 16         ; clean up four pushed args

    popa                ; restore registers
    iret                ; return from interrupt (restores eip, cs, eflags)
