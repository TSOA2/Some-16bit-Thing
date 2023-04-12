[bits 16]

; es:bx = the destination buffer
; al = num of sectors to read
; dl = hard disk number
disk_load:
    pusha

    mov ah, byte 0x02
    mov ch, byte 0x00
    mov cl, byte 0x02
    int 0x13

    popa
    ret
