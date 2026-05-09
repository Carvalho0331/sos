section .text

; Macro for interrupts without error code
%macro no_error_code_interrupt 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0          ; push fake error code
    push dword %1         ; push interrupt number
    jmp common_handler
%endmacro

; Macro for interrupts with error code (CPU already pushed error code)
%macro error_code_interrupt 1
global interrupt_handler_%1
interrupt_handler_%1:
    ; CPU already pushed error code, but we want uniform stack:
    ; We need [number, error]. Currently stack: [error, eip, cs, eflags]
    ; So we push number, then swap so number is above error
    push dword %1         ; push interrupt number
    ; Now stack: [number, error, eip, cs, eflags]
    ; We want [number, error] at top for common handler to pop easily.
    ; Actually, we can just leave as is and common handler will read them.
    ; But the common handler expects number at [esp+?]. Let's keep simple:
    ; The common handler will pop number and error in order.
    ; So we need to ensure that the first value on stack is number, second is error.
    ; Currently, after the CPU's push, the stack has error at top.
    ; After our push dword %1, stack has number at top, then error below.
    ; That's perfect: number at [esp], error at [esp+4].
    jmp common_handler
%endmacro

; Generate all 256 handlers
%assign i 0
%rep 256
    %if i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17
        error_code_interrupt i
    %else
        no_error_code_interrupt i
    %endif
%assign i i+1
%endrep

; External C function
extern interrupt_handler_c

; Common handler: saves registers, calls C, restores, returns
common_handler:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10          ; kernel data segment selector
    mov ds, ax
    mov es, ax

    ; At this point, stack layout (from top):
    ; [interrupt number, error code, gs, fs, es, ds, edi, esi, ebp, esp, ebx, edx, ecx, eax]
    ; We need to call interrupt_handler_c(number, error)
    ; The number is at [esp + 40] (since pusha = 32 bytes + 5 segment pushes = 40 bytes? Let's calculate manually:
    ; After pusha: esp points to eax (32 bytes below original). Then push ds (4), es (4), fs (4), gs (4) = +16.
    ; So total saved registers = 32+16 = 48 bytes. Then above that are number (4) and error (4).
    ; So number is at [esp + 48], error at [esp + 52].
    ; We'll just use a simpler approach: pop the values before pushing registers? No.
    ; Let's just pass the stack pointer to C and let C extract values.
    ; But easier: We can push number and error again? No.
    ; Let's use the stack pointer as argument to a C function that reads them.
    ; We'll call interrupt_handler_c with one argument: pointer to the interrupt number on stack.
    ; That function can then read number and error.
    ; Simpler: We'll just load the values into registers and push them as arguments.
    mov eax, [esp + 48]   ; interrupt number
    mov ebx, [esp + 52]   ; error code
    push ebx
    push eax
    call interrupt_handler_c
    add esp, 8

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8            ; remove interrupt number and error code from stack
    iret

section .data
global interrupt_handlers
interrupt_handlers:
%assign j 0
%rep 256
    dd interrupt_handler_%+j
%assign j j+1
%endrep
