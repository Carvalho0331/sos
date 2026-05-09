global outb
global inb


outb:
    mov al, [esp + 8]    ; data
    mov dx, [esp + 4]    ; port
    out dx, al
    ret

; unsigned char inb(unsigned short port)
inb:
    mov dx, [esp + 4]
    in  al, dx
    ret
