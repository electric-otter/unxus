; Bootloader: Loads sector after itself, switches to long mode, jumps to loaded kernel

BITS 16
org 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Print '!'
    mov ah, 0x0E
    mov al, '!'
    int 0x10

    ; Load sector 2 (next sector) to 0x10000 (arbitrary, safe from bootloader area)
    mov ah, 0x02
    mov al, 4            ; 4 sectors (2KB, adjust as needed for your kernel size)
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x1000       ; offset 0x10000 = ES:BX (ES=0), so BX=0x1000
    mov es, ax           ; ES=0
    int 0x13
    jc disk_error

    ; Setup GDT (see your example above)
    lgdt [gdt_desc]

    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

[BITS 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set up page tables for identity mapping first 2 MB
    mov dword [ptl4], ptpd
    mov dword [ptl4+4], 0x0
    mov dword [ptpd], ptpd2mb | 0x83
    mov dword [ptpd+4], 0x0

    mov eax, ptl4
    mov cr3, eax

    ; Enable long mode in EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x00000100
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    jmp 0x18:long_mode

align 8
gdt:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
    dq 0x00AF9A000000FFFF

gdt_desc:
    dw 8*4-1
    dd gdt

align 4096
ptl4:
    dq 0
ptpd:
    dq 0
ptpd2mb equ 0x0000000000000083

disk_error:
    mov al, 'E'
    mov ah, 0x0E
    int 0x10
    jmp $

[BITS 64]
long_mode:
    ; Now in 64-bit mode! Jump to loaded kernel
    mov rsi, 0x10000           ; Address where kernel was loaded (linear address)
    jmp rsi

times 510-($-$$) db 0
dw 0xAA55
