global gdt_reload_segments

gdt_reload_segments:
    ; Far jump para atualizar cs com o seletor 0x08 (código kernel)
    jmp 0x08:.flush
.flush:
    ; Carregar os registos de dados com o seletor 0x10 (dados kernel)
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret
