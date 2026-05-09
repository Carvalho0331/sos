global outb

outb:
    mov al, [esp + 8]    ; data
    mov dx, [esp + 4]    ; port
    out dx, al
    ret
