.model	small
.stack	100h

; коды перенсоа строки
crlf	equ	13, 10
	
.data

	; массив
	arr		db 	128, 200, 130, 4, 15, 55, 128, 25
	; длина массива
	len		equ $-arr
	; буфер под строку
	msg		db	10 dup (0)
	; хранение суммы
	sum		dw	0
	; строки 
	str1	db	crlf, "massiv : $"
	str2	db  crlf, "summa  : $"
	
.code
start:
	; иницализация сегмента данных
	mov ax, @data
	mov ds, ax
	mov es, ax
	
	; вывод строки массива
	lea dx, str1
	call print_str	
	call print_arr
	
	; поиск суммы
	lea dx, str2
	call print_str	
	call get_summ
	
	; преобразовать число сцммы в строку
	mov ax, sum
	lea di, msg
	call to_string
	; вывод сусммы
	lea dx, msg
	call print_str
	
	; делаем преобразование согласно задания
	call convert
	
	
	; вывод получившегося массива
	lea dx, str1
	call print_str	
	call print_arr
	
	lea dx, str2
	call print_str	
	call get_summ
	
	; найти новую сумму
	mov ax, sum
	lea di, msg
	call to_string
	lea dx, msg
	call print_str
	
	;ожидать нажати клавиши
	call press_key
	; завершить программу
    mov ax, 4c00h
	int 21h

; нажатие клавиши
press_key	proc
	mov ah, 08h
	int 21h
	ret
endp
	
; преобразование массива
convert		proc
	mov cx, len
	lea si, arr
@convert:
	; беерм значение массива si+=1
	lodsb
	; проверить 7 бит
	test al, 10000000b
	; дальше если не установлен
	jz @next
	; иначе делаем сдвиг вправо на 1
	shr byte ptr [si-1], 1
@next:
loop @convert
	ret
endp

; подсчет суммы
get_summ	proc
	mov cx, len
	lea si, arr
	; начальная 0
	mov sum, 0
@summ:
	; взять элемент массива si += 1
	lodsb
	; оставить младшую часть регистра
	and ax, 0FFh
	; добавить к сумме
	add sum, ax
loop @summ
	ret
endp

; вывод массива
print_arr	proc
	mov si, offset arr
	mov cx, len
	; 
@print:
	; взять очередной элемент массва
	lodsb
	; оставить млашую часть
	and ax, 0FFh
	mov di, offset msg
	; преобразовать число в строку цифр
	call to_string
	; вывод числа
	lea dx, msg
	call print_str
loop @print
	ret
	
endp

; печать заданной строки
; dx - адрес строки
print_str proc
	push cx
	mov ah, 09h
	int 21h
	pop cx
	ret
endp

; преобразование числа в строку
; di - адрес буфера строки
to_string proc
	push cx
	mov cx, 0
	; делитель
	mov bx, 10
@do:
	; обнулить под остаток
	xor dx, dx
	; ax /= bx
	div bx
	; увеличить счетчик цифр
	inc cx
	; преобразовать в символ цифры
	add dl,'0'
	; положить в стек
	push dx
	; выход только если 0
	cmp ax, 0
jne @do

	; запись в строку цифр
@while:
	; достать из стека
	pop ax
	; записать в массив по адресу di; di += 1
	stosb
loop @while
	; дополнить строку пробелом и концом строки
	mov al, ' '
	stosb
	mov byte ptr [di], '$'
	pop cx
	ret
endp

end start



