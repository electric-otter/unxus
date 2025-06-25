BITS 16
org 0x7c00

start:
    mov ah, 0x0e         ; teletype output
    mov al, '!'          
    int 0x10

    ; Load sector 2 (LBA 1) from disk into 0x7e00
    mov ah, 0x02         ; BIOS read sectors
    mov al, 1            ; number of sectors
    mov ch, 0            ; cylinder
    mov cl, 2            ; sector (starts at 1)
    mov dh, 0            ; head
    mov dl, 0x80         ; first hard disk
    mov bx, 0x7e00       ; ES:BX buffer (ES=0x0000)
    int 0x13             ; BIOS disk service

    jc disk_error        ; if carry set, error

    jmp 0x0000:0x7e00    ; jump to loaded code

disk_error:
    mov al, 'E'
    int 0x10
    jmp $

hang:
    jmp hang

times 510-($-$$) db 0
dw 0xAA55
