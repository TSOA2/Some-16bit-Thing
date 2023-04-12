[bits 16]
[org 0x7c00]

; idk what to call this thing
; it kind of loads some 16-bit program in disk to memory and calls it
; maybe it's a bootloader but it doesn't really boot a kernel or anything
; anyways I know far too little about this subject to know about the terminology

; thanks to https://ostad.nit.ac.ir/payaidea/ospic/file1615.pdf
_main:
    push dx

    mov ah, byte 0x00
    mov al, byte 0x13
    int 0x10

    pop dx
    mov al, byte 0x1D
    mov bx, word 0x7e00

    call disk_load

    jmp 0x0000:0x7e00

    jmp .exit

.exit:
    jmp $

%include "./disk_load.s"

times 510-($-$$) db 0x00
dw 0xaa55

%macro clr_screen 0
    call load_bg
%endmacro

; 1 - width
; 2 - height
; 3 - x pos
; 4 - y pos
%macro draw_box 4
    mov ax, %1
    mov bx, %2
    mov cx, %3
    mov dx, %4
    call load_box
%endmacro

; 1 - memory location
; 2 - width
; 3 - height
; 4 - x pos
; 5 - y pos
%macro draw_sprite 5
    mov si, %1
    mov ax, %2
    mov bx, %3
    mov cx, %4
    mov dx, %5
    call load_sprite
%endmacro

%macro set_draw_color 1
    mov [DRAW_COLOR], byte %1
%endmacro

_next:
    ; hell yeah we're loaded in a random chunk of memory
    ; we have access to 480.5 kb of memory according to https://wiki.osdev.org/Memory_Map_(x86)

    set_draw_color 0x00
    clr_screen
    draw_sprite SPRITE_TWO, 39, 39, 200, 50

    set_draw_color 0x02
    draw_box 10, 10, 50, 50

    set_draw_color 0x05
    draw_box 50, 50, 90, 90
    jmp $

%include "./graphics.s"
%include "./sprites.s"