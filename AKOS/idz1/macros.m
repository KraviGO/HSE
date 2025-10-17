# Макросы для системных вызовов

# Сохранение регистра на стеке
.macro push(%reg)
	addi sp, sp, -4
	sw %reg, 0(sp)
.end_macro

# Восстановление регистра со стека  
.macro pop(%reg)
	lw %reg, 0(sp)
	addi sp, sp, 4
.end_macro

# Вывод строки
.macro print_str(%str)
	.data
	macro_str: .asciz %str

	.text
	push(a0)
	push(a7)
	li a7, 4
	la a0, macro_str
	ecall
	pop(a7)
	pop(a0)
.end_macro

# Вывод целого числа
.macro print_int(%reg)
	push(a0)
	push(a7)
	mv a0, %reg
	li a7, 1
	ecall
	pop(a7)
	pop(a0)
.end_macro

# Ввод целого числа в a0
.macro read_int_a0
	li a7, 5
	ecall
.end_macro

# Ввод целого числа в указанный регистр
.macro read_int(%reg)
	push(a0)
	li a7, 5
	ecall
	mv %reg, a0
	pop(a0)
.end_macro

# Завершение программы
.macro exit
	li a7, 10
	ecall
.end_macro