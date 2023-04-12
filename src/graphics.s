[bits 16]

DRAW_COLOR db 0x00
SCREEN_WIDTH equ 320
SCREEN_HEIGHT equ 200

; look at bios color codes here: https://en.wikipedia.org/wiki/Video_Graphics_Array#/media/File:VGA_palette_with_black_borders.svg
load_bg:
    pusha

    mov dx, word 0x00

.loop:
    cmp dx, word SCREEN_HEIGHT
    je .done

    jmp .fill_line

.loop2:
    inc dx
    jmp .loop

.fill_line:
    mov cx, word 0x00
    jmp .floop

.floop:
    cmp cx, word SCREEN_WIDTH
    je .loop2

    mov ah, byte 0x0c
    mov al, byte [DRAW_COLOR]

    int 0x10
    inc cx
    jmp .floop

.done:
    popa
    ret

;
; bx  = height
; ax  = width
; cx  = x position
; dx  = y position

load_box:
    pusha

    add ax, cx
    add bx, dx

.loop:
    cmp dx, bx
    je .done

    push cx
    jmp .write_line

.loop_end:
    pop cx
    inc dx
    jmp .loop

.write_line:
    cmp cx, ax
    je .loop_end

    push ax
    push bx
    mov ah, 0x0c
    mov al, byte [DRAW_COLOR]
    mov bh, 0x00
    int 0x10

    pop bx
    pop ax

    inc cx

    jmp .write_line

.done:
    popa
    ret

; si = image data
; ax = image width
; bx = image height
; cx = x pos
; dx = y pos
load_sprite:
    pusha

    add ax, cx
    add bx, dx

.loop:
    cmp dx, bx
    je .done

    push cx
    jmp .write_line

.loop_end:
    pop cx
    inc dx
    jmp .loop

.write_line:
    cmp cx, ax
    je .loop_end

    push ax
    push bx
    push si

    mov ah, 0x0c
    mov al, byte [si]
    mov bh, 0x00
    int 0x10

    pop si
    pop bx
    pop ax

    inc cx
    inc si
    jmp .write_line

.done:
    popa
    ret

; ; si = font memory location
; ; bx = text

; load_font:
;     pusha

; .loop:
;     cmp [bx], byte 0x00
;     je .done

;     push bx
;     push si
    
;     mov ax, bx
;     mov bx, 8
;     mul bx
;     add si, ax
;     jmp $
;     jmp .load_char

; .loop_cont:
;     pop si
;     pop bx
;     inc bx
;     jmp .loop

; .load_char:
;     mov dx, 0x00

; .loop1:
;     cmp dx, 8
;     je .loop_cont
;     add si, cx
;     mov ax, [si]
;     sub si, cx
;     jmp .loop2

; .loop1_cont:
;     mov cx, 0x00
;     inc dx
;     jmp .loop1

; .loop2: 
;     cmp cx, 8
;     jmp .loop1_cont

;     mov bx, 1
;     shl bx, cl
;     and bx, ax
;     cmp bx, byte 0x00
;     jne .render_pixel

;     inc cx
;     jmp .loop2

; .render_pixel:
;     push ax
;     push bx
    
;     mov ah, 0x0c
;     mov al, [DRAW_COLOR]
;     mov bh, 0x00
    
;     pop bx
;     pop ax

;     inc cx
;     jmp .loop2

; .done:
;     popa
;     ret