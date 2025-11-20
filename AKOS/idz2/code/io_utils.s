.data
prompt_x: .asciz "Enter x: "
result_msg: .asciz "e^(-x) = "

.text
.globl input_x, print_result

# Подпрограмма ввода x
# Выход: fa0 = x
input_x:
	addi sp, sp, -8
	sw ra, 4(sp)
    
	# Вывод приглашения
	la a0, prompt_x
	li a7, 4
	ecall

	# Ввод числа
	li a7, 7
	ecall
	# fa0 уже содержит введенное значение

	lw ra, 4(sp)
	addi sp, sp, 8
	ret

# Подпрограмма вывода результата
# Вход: fa0 = результат
print_result:
	addi sp, sp, -8
	sw ra, 4(sp)

	# Вывод сообщения
	la a0, result_msg
	li a7, 4
	ecall

	# Вывод результата
	li a7, 3
	ecall

	# Перевод строки
	li a0, '\n'
	li a7, 11
	ecall

	lw ra, 4(sp)
	addi sp, sp, 8
	ret