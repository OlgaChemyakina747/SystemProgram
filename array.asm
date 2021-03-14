.model	small
.stack	100h

; ���� ��७ᮠ ��ப�
crlf	equ	13, 10
	
.data

	; ���ᨢ
	arr		db 	128, 200, 130, 4, 15, 55, 128, 25
	; ����� ���ᨢ�
	len		equ $-arr
	; ���� ��� ��ப�
	msg		db	10 dup (0)
	; �࠭���� �㬬�
	sum		dw	0
	; ��ப� 
	str1	db	crlf, "massiv : $"
	str2	db  crlf, "summa  : $"
	
.code
start:
	; ���栫����� ᥣ���� ������
	mov ax, @data
	mov ds, ax
	mov es, ax
	
	; �뢮� ��ப� ���ᨢ�
	lea dx, str1
	call print_str	
	call print_arr
	
	; ���� �㬬�
	lea dx, str2
	call print_str	
	call get_summ
	
	; �८�ࠧ����� �᫮ �欬� � ��ப�
	mov ax, sum
	lea di, msg
	call to_string
	; �뢮� ��ᬬ�
	lea dx, msg
	call print_str
	
	; ������ �८�ࠧ������ ᮣ��᭮ �������
	call convert
	
	
	; �뢮� ����稢襣��� ���ᨢ�
	lea dx, str1
	call print_str	
	call print_arr
	
	lea dx, str2
	call print_str	
	call get_summ
	
	; ���� ����� �㬬�
	mov ax, sum
	lea di, msg
	call to_string
	lea dx, msg
	call print_str
	
	;������� ����� ������
	call press_key
	; �������� �ணࠬ��
    mov ax, 4c00h
	int 21h

; ����⨥ ������
press_key	proc
	mov ah, 08h
	int 21h
	ret
endp
	
; �८�ࠧ������ ���ᨢ�
convert		proc
	mov cx, len
	lea si, arr
@convert:
	; ���� ���祭�� ���ᨢ� si+=1
	lodsb
	; �஢���� 7 ���
	test al, 10000000b
	; ����� �᫨ �� ��⠭�����
	jz @next
	; ���� ������ ᤢ�� ��ࠢ� �� 1
	shr byte ptr [si-1], 1
@next:
loop @convert
	ret
endp

; ������ �㬬�
get_summ	proc
	mov cx, len
	lea si, arr
	; ��砫쭠� 0
	mov sum, 0
@summ:
	; ����� ����� ���ᨢ� si += 1
	lodsb
	; ��⠢��� ������� ���� ॣ����
	and ax, 0FFh
	; �������� � �㬬�
	add sum, ax
loop @summ
	ret
endp

; �뢮� ���ᨢ�
print_arr	proc
	mov si, offset arr
	mov cx, len
	; 
@print:
	; ����� ��।��� ����� ���ᢠ
	lodsb
	; ��⠢��� ������ ����
	and ax, 0FFh
	mov di, offset msg
	; �८�ࠧ����� �᫮ � ��ப� ���
	call to_string
	; �뢮� �᫠
	lea dx, msg
	call print_str
loop @print
	ret
	
endp

; ����� �������� ��ப�
; dx - ���� ��ப�
print_str proc
	push cx
	mov ah, 09h
	int 21h
	pop cx
	ret
endp

; �८�ࠧ������ �᫠ � ��ப�
; di - ���� ���� ��ப�
to_string proc
	push cx
	mov cx, 0
	; ����⥫�
	mov bx, 10
@do:
	; ���㫨�� ��� ���⮪
	xor dx, dx
	; ax /= bx
	div bx
	; 㢥����� ���稪 ���
	inc cx
	; �८�ࠧ����� � ᨬ��� ����
	add dl,'0'
	; �������� � �⥪
	push dx
	; ��室 ⮫쪮 �᫨ 0
	cmp ax, 0
jne @do

	; ������ � ��ப� ���
@while:
	; ������ �� �⥪�
	pop ax
	; ������� � ���ᨢ �� ����� di; di += 1
	stosb
loop @while
	; ��������� ��ப� �஡���� � ���殬 ��ப�
	mov al, ' '
	stosb
	mov byte ptr [di], '$'
	pop cx
	ret
endp

end start



