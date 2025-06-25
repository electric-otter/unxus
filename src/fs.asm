; fs.asm - Simple Bootloader File System Loader (example for unxus)
; Loads a file by name from a fake file table in the first sector(s)
; Assumes: 512-byte sectors, BIOS disk access (INT 13h), real mode
; Place file table after boot sector. Each entry: [name:12][start:2][sectors:2]

BITS 16
ORG 0x7c00

; BIOS Disk Params
BOOT_DRIVE  equ 0x80   ; First HDD (change to 0 if floppy)

; File Table Params
FILETABLE_LBA   equ 2  ; Sector where file table starts
FILETABLE_SIZE  equ 16 ; Number of entries in file table
FILENAME_LEN    equ 12 ; Filename max length

; Buffer for file table (must not overwrite boot code)
filetable_buf   equ 0x0600
filename_buf    equ 0x0500

; ========== Main Entrypoint ==========
start:
    mov si, filename      ; File to load
    mov di, filename_buf
    mov cx, FILENAME_LEN
    rep movsb            ; Copy filename to buffer
    
    call load_file_by_name

    ; Add your logic here (jump to loaded file, etc.)
    cli
    hlt

; ========== Load File By Name ==========
; IN: DS:SI = ptr to filename (max 12 bytes)
; OUT: BX = loaded address, CX = file size (sectors)
load_file_by_name:
    ; Load file table into filetable_buf
    mov bx, filetable_buf
    mov dh, 1            ; Only 1 sector for file table
    mov dl, BOOT_DRIVE
    mov ch, 0
    mov cl, FILETABLE_LBA
    call read_sectors

    ; Search for filename
    mov si, filename_buf
    mov di, filetable_buf
    mov cx, FILETABLE_SIZE
find_file_entry:
    push cx
    push si
    push di
    mov cx, FILENAME_LEN
    repe cmpsb
    pop di
    pop si
    pop cx
    je found_file_entry
    add di, (FILENAME_LEN+4) ; skip rest of entry
    loop find_file_entry
    jmp file_not_found

found_file_entry:
    ; di = start of entry
    add di, FILENAME_LEN     ; skip name
    mov ax, [di]             ; starting sector
    add di, 2
    mov cx, [di]             ; sector count

    mov bx, 0x1000           ; Load address (adjust as needed)
    mov dl, BOOT_DRIVE
    mov dh, cl               ; sectors to read
    mov ch, 0
    mov cl, al               ; starting sector (LBA->CHS for real FS)
    call read_sectors

    ret

file_not_found:
    mov si, msg_not_found
    call print_string
    cli
    hlt

; ========== BIOS Disk Read =============
; IN: BX=buffer, DH=sectors, DL=drive, CH=cylinder, CL=sector
read_sectors:
    pusha
    mov ah, 0x02
    mov al, dh
    mov es, bx
    xor bx, bx
    int 0x13
    jc disk_error
    popa
    ret

disk_error:
    mov si, msg_disk_error
    call print_string
    cli
    hlt

; ========== Print String ===============
; IN: DS:SI -> string, terminated by 0
print_string:
    mov ah, 0x0e
.print_char:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .print_char
.done:
    ret

; ========== Data =======================
filename:       db 'UNXUSCMD   ' ; pad to 12 bytes
msg_not_found:  db 'File not found!',0
msg_disk_error: db 'Disk error!',0

times 510-($-$$) db 0
dw 0xAA55
