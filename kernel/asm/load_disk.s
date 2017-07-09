[BITS 16]

disk_addr_packet:
    db 0x10
    db 0
block_count:
    dw 16
buff_dest_addr:
    dw 0x7c00 ; memory buffer destination address (0x1000:7c00)
    dw 0x1000 ;
lba:
    dd 1
    dd 0

[GLOBAL load_disk]

idt_real:
    dw 0x3ff
    dd 0

savcr0:
    dd 0

entry16:
    cli
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax

    mov eax, cr0
    mov [savcr0], eax
    and eax, 0x7FFFFFFe
    mov cr0, eax

    jmp 0:real_mode

real_mode:
    mov sp, 0x8000
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    lidt [idt_real]
    sti
    ret

load_disk:
    call entry16
    mov si, disk_addr_packet ; addres of "disk address packet"
    mov ah, 0x42             ; AL is unused
    mov dl, 0x80             ; drive 0
    int 0x13
    ret