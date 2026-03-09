.model tiny
.code

org 100h

locals @@

;---------------------------------- CONSTANTS ---------------------------------

CR					equ 0dh			; \r
CRLF				equ 0dh, 0ah	; \r \n
INPUT_BUFFER_SIZE	equ	256

;--------------------------------------------------------------------------------

Start:
	call Main
	jmp  Exit

;--------------------------------------------------------------------------------
; Short:   Reads and verifies password from console input
;--------------------------------------------------------------------------------

Main proc

	; DS = CS
	mov ax, cs
	mov ds, ax

	; DS:[DX] --> EnterPassword message
	mov dx, offset EnterPasswordMsg
	; call DOS func DisplayString
	; AH = 09h = function code
	mov ah, 09h
	; DOS func interrupt
	int 21h

	; get password from console
	call GetConsoleInput

	; DS:[SI] --> password
	mov si, offset InputBuffer
	; CX = password len
	call VerifyPassword

	; AL = verification result
	; tail call --> ret in inside function will return from our function
	jmp PrintAccessStatus

	ret

Main endp

;--------------------------------------------------------------------------------
; Short:   Reads input from console until the '\r'
; Out:     CS:[InputBuffer] --> input
;		   CX = input length
; Destroy: AX, DI, ES
; NOTE:	   This function IS NOT checking input length!!!
;		   InputBufferOverflow is not checked!!!
;--------------------------------------------------------------------------------

GetConsoleInput proc

	; ES = CS
	mov ax, cs
	mov es, ax
	; ES:[DI] --> input buffer
	mov di, offset InputBuffer

@@Next:
	; call DOS function KeyboardInput
	; AH = function code (01h)
	mov ah, 01h
	; DOS function interrupt
	int 21h

	; AL = character from Standard Input
	cmp al, CR
	; if reached \r --> input ended
	je @@InputEnded
	; else --> store in buffer
	; ES:[DI++] = AL
	stosb

	jmp @@Next

@@InputEnded:
	; CX = input length
	; CX = DI - InputBufferStart = -(InputBufferStart - DI)
	mov cx, offset InputBuffer
	sub cx, di
	neg cx

	ret

GetConsoleInput endp

;--------------------------------------------------------------------------------
; Short:   Verifies input password and returns the result
; Exp:     CS:[Password] --> actual password
; In:      DS:[SI] 	     --> input password
; Out:     AL = 00h if password is correct
;		   AL = 01h if password was denied
; Destroy: AH, BX, CX, SI, DI
;--------------------------------------------------------------------------------

VerifyPassword proc

	; count hash for input password
	call CountHash
	; BX = hash value

	; AX = 0
	xor ax, ax

	; CS:[DX] = actual password hash
	mov di, offset Password

	; cmp password hash and input hash
	cmp word ptr bx, cs:[di]

	; if not equal --> AX = 0
	jne @@Done
	; if equal --> AX = 01h
	inc ax

@@Done:

	ret

VerifyPassword endp

;--------------------------------------------------------------------------------
; Short:   Counts hash for string (algorithm = djb2)
; In:      DS:[SI] --> string
;		   CX = string len
; Out:     BX = hash value
; Destroy: AX, CX, SI
;--------------------------------------------------------------------------------

CountHash proc

	; AH = 00h
	xor ax, ax

	; BX = 0
	xor bx, bx

	; if length == 0 --> end
	cmp cx, 00h
	je @@Done

	; count BX = sum of symbol codes
@@Next:
	; remember hash to than add once
	mov ax, bx
	; hash = hash * 33
	shr bx, 5
	add bx, ax
	; hash += character
	; restore AX = 00h
	xor ax, ax
	; AL = DS[SI++]
	lodsb
	; hash += AL (symbol ascii code)
	add bx, ax

	loop @@Next

@@Done:

	ret

CountHash endp

;--------------------------------------------------------------------------------
; Short:   Prints access status in console output
; Exp:	   DS = CS
;		   CS:[AccessGrantedMsg]
;		   CS:[AccessDeniedMsg]
; In:      AL = access status (0 --> denied; 1 --> granted)
; Destroy: AH, DX
;--------------------------------------------------------------------------------

PrintAccessStatus proc

	; check AL for access status
	cmp al, 00h

	je @@AccessDenied
	; AL = 01 --> put access granted message
	mov dx, offset AccessGrantedMsg
	jmp @@AccessGranted

; AL = 00 --> put access denied message
@@AccessDenied:
	mov dx, offset AccessDeniedMsg

@@AccessGranted:

	; call DOS func DisplayString
	; AH = 09h = function code
	mov ah, 09h
	; DOS func interrupt
	int 21h

	ret

PrintAccessStatus endp

;--------------------------------------------------------------------------------
; Short:   Exits program with return code 0 (success)
; Destroy: AX
;--------------------------------------------------------------------------------

Exit proc

	; AH = DOS function code 4ch = Exit
	; AL = return code 00 = success
	mov ax, 4c00h
	; call DOS func interrupt
	int 21h

	; no return --> program ends

Exit endp

; ---------------------------------- DATA ----------------------------------

.data

EnterPasswordMsg	db 'Enter password: $'
AccessGrantedMsg	db 'Access granted', CRLF, '$'
AccessDeniedMsg		db 'Access denied',  CRLF, '$'

; 1ая уязвимость --> хэш лежит сразу после буфера ввода
; и буфер не проверяется на переполнение --> правильным вводом
; можно переписать значение хэша правильного пароля на свое
InputBuffer			db INPUT_BUFFER_SIZE DUP('$')

; password hash == 0293h (KusKus)
Password			dw 0293h

;--------------------------------------------------------------------------------

end	Start
