BITS 16
org 0x7c00

start:
    mov ah, 0x0e         ; teletype output
    mov al, '!'          
    int 0x10

    ; Load fs.asm (sector 2) into 0x7c00 (overwriting self)
    mov ah, 0x02         ; BIOS read sectors
    mov al, 1            ; number of sectors (adjust if fs.asm > 1 sector)
    mov ch, 0            ; cylinder
    mov cl, 2            ; sector (starts at 1)
    mov dh, 0            ; head
    mov dl, 0x80         ; first hard disk
    mov bx, 0x7c00       ; ES:BX buffer (ES=0x0000:0x7c00)
    int 0x13             ; BIOS disk service

    jc disk_error        ; if carry set, error

    jmp 0x0000:0x7c00    ; jump to loaded fs.asm code

disk_error:
    mov al, 'E'
    int 0x10
    jmp $

times 510-($-$$) db 0
dw 0xAA55
dw 0xAA55
