.model tiny
.code

org 100h

locals @@

;---------------------------------- CONSTANTS ---------------------------------

CR						equ 0dh			; \r
CRLF					equ 0dh, 0ah	; \r \n
INPUT_BUFFER_SIZE		equ	256d
MAX_READ_FILE_LENGTH 	equ 0FFFFh

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
	; get CX = string length
	call StrLen

	; DS:[SI] --> password
	mov si, offset InputBuffer
	; get AL = access status
	call VerifyPassword

	; AL = verification result
	; tail call --> ret in inside function will return from our function
	jmp PrintAccessStatus

	ret

Main endp

;--------------------------------------------------------------------------------
; Short:   Reads input from console until the '\r'
; Exp:	   DS = CS
; Out:     CS:[InputBuffer] --> input
;		   CX = input length
; Destroy: AX, BX, CX, DX
; NOTE:	   This function IS NOT checking input length!!!
;		   InputBufferOverflow is not checked!!!
;--------------------------------------------------------------------------------

GetConsoleInput proc

; call DOS function Read from file
	; AH = DOS function Read from file code
	mov ah, 03fh
	; BX = standard console input
	xor bx, bx
	; CX = maximum value to read --> buffer length
	mov cx, MAX_READ_FILE_LENGTH
	; DS:[DX] --> input buffer
	mov dx, offset InputBuffer
	; DOS function interrupt
	mov si, sp
	mov sp, offset EnterPasswordMsg
	sub sp, 2
	int 21h
	mov sp, si

	ret

GetConsoleInput endp

;--------------------------------------------------------------------------------
; Short:   Counts length of the string until the '\r'
; In:      DS:[SI] --> string
; Out:     CX = string length
; Destroy: AL, SI
;--------------------------------------------------------------------------------

StrLen proc

	; DF = 0
	cld

	; CX = FFFFh --> max value
	xor cx, cx
	dec cx

@@Next:
	; AL = DS[SI++]
	lodsb
	; if AL = '\r' --> end
	cmp al, CR
	loopne @@Next

	; CX = FFFF - len - 1 (-1 for '\r') ==> len = FFFF - CX - 1 = - 2 - CX
	; - CX
	neg cx
	; - 2
	dec cx
	dec cx

	ret

StrLen endp

;--------------------------------------------------------------------------------
; Short:   Verifies input password and returns the result
; Exp:     CS:[PasswordHash] --> actual password hash
; In:      DS:[SI] --> input password
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
	mov di, offset PasswordHash

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
	shl bx, 5
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

StackAreaForInt		dw 90 DUP(256)

EnterPasswordMsg	db 'Enter password: $'
AccessGrantedMsg	db 'Access granted', CRLF, '$'
AccessDeniedMsg		db 'Access denied',  CRLF, '$'

; 1ая уязвимость --> перезапись переменной Password с хэшом пароля
; хэш лежит сразу после буфера ввода
; и буфер не проверяется на переполнение --> правильным вводом
; можно переписать значение хэша правильного пароля на свое

InputBuffer			db INPUT_BUFFER_SIZE DUP(01h)

; password hash == 7846h ("KusKus")
PasswordHash		dw 7846h

;--------------------------------------------------------------------------------

end	Start
