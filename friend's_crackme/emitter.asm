.model tiny
.code

org 100h

;================================================================================

JZ_BYTECODE         equ 74h
NOP_BYTECODE        equ 90h
TARGET_FUNC         equ 0ah
INT_ADDR_SIZE       equ 4d
PATCH_ADDR          equ 011Ah

;================================================================================

Start:
    ; 21h = DOS func interrupt
    ; 0000:0000 - int table start
    ; ES:[BX] = 0000:(21h * 4) --> int 21h address
    mov ax, 0000h
    mov es, ax
    mov bx, 21h * INT_ADDR_SIZE

    ; save old 09h interrupt address in memory
    ; Little-endian
    mov ax, es:[bx]
    mov cs:[Old21IntOffset], ax
    mov ax, es:[bx + 2]
    mov cs:[Old21IntSegment], ax

    ; set new int
    cli
    ; ES:[BX] = CS:[New21Int]
    ; set int offset
    mov ax, offset New21Int
    mov es:[bx], ax
    ; set int segment
    mov ax, cs
    mov es:[bx + 2], ax

    sti

    ; TSR(0)
    mov dx, offset EndOfProgram
    shr dx, 4
    inc dx

    mov ax, 3100h
    int 21h

;--------------------------------------------------------------------------------
; Short:   My resident-emitter (patcher) for simple.com that will change
;          JZ ... --> nop nop, disabling test for access status
;--------------------------------------------------------------------------------

New21Int proc

    cmp ah, TARGET_FUNC
    jne @@Done

    ; save everything
    push ax di bp es

    mov bp, sp

    ; AX = CS from iret frame
    ; when an int called in stack pushed:
    ; [FLAGS]
    ; [CS] <--- bp + 10
    ; [IP] <--- bp + 8
    ; i pushed
    ; [old AX, old DI, old BP, old ES] <--- bp
    mov ax, ss:[bp + 10]
    ; ES = old CS
    mov es, ax

    ; DI = abs address of command JZ checking access status for 0
    mov di, PATCH_ADDR

    ; verify that int call was in program we want to crack:
    ; at PATCH_ADDR should be jz
    cmp byte ptr es:[di], JZ_BYTECODE
    jne @@CleanUp
    ; AL = nop
    mov al, NOP_BYTECODE
    ; store NOPs instead of JZ <jump addr>
    ; es[di++] = al (JZ --> NOP)
    stosb
    ; es[di++] = al (<jump addr> --> NOP)
    stosb

@@CleanUp:

    pop es bp di ax

@@Done:

; jmp far to an old interrupt 21h
    db 0EAh

Old21IntOffset     dw 0
Old21IntSegment    dw 0

New21Int endp

;================================================================================

EndOfProgram:

end Start
