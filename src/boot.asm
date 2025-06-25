BITS 16
org 0x7c00

start:
    ; Print a character (optional)
    mov ah, 0x0e
    mov al, '!'
    int 0x10

    ; Infinite loop to halt
hang:
    jmp hang

times 510-($-$$) db 0
dw 0xAA55
