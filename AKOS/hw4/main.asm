# Итерационная формула: x_{n+1} = 0.5 × (x_n + a / x_n)


.include "macros.asm"

.data
number_prompt:		.asciz "Введите положительное число: "
precision_prompt:	.asciz "Введите точность вычисления: "
result_msg:		.asciz "Квадратный корень: "
error_msg:		.asciz "Ошибка: число должно быть положительным!\n"
newline:		.asciz	"\n"
zero_float:		.float 0.0

.text
.globl main

main:
	la t0, zero_float
	flw ft0, 0(t0)

	read_float(number_prompt, fs0)
	check_positive(fs0, input_error)

	read_float(precision_prompt, fs1)
	check_positive(fs1, input_error)

	call_sqrt(fs0, fs1, fs2)

	print_string(result_msg)
	print_float(fs2, newline)
    
	j exit_program

input_error:
	print_string(error_msg)
    
exit_program:
	li a7, 10
	ecall
	
.include "sqrt.asm"