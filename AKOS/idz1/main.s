.include "macros.m"
.include "array_io.s"

.data
array_A: .word 0:10
array_B: .word 0:10

menu_text: .asciz "Choose input method:\n1 - Keyboard\n2 - File\n3 - Run automated tests\nChoice: "
error_choice: .asciz "Invalid choice!\n"

.text
.globl main

main:
	# Вывод меню
	print_str("Choose input method:\n1 - Keyboard\n2 - File\n3 - Run automated tests\nChoice: ")
	read_int_a0

	li t0, 1
	beq a0, t0, keyboard_input
	li t0, 2
	beq a0, t0, file_input
	li t0, 3
	beq a0, t0, run_tests

	print_str("Invalid choice!\n")
	j main

keyboard_input:
	# Ввод с клавиатуры
	la a0, array_A
	li a1, 10
	jal input_array

	blez a0, main	# если ошибка ввода
	mv s0, a0	# сохраняем размер
	j process_and_output

file_input:
	# Ввод из файла
	la a0, array_A
	li a1, 10
	jal input_array_from_file

	blez a0, main	# если ошибка ввода
	mv s0, a0	# сохраняем размер

process_and_output:
	# Вывод исходного массива
	print_str("Array A: ")
	la a0, array_A
	mv a1, s0
	jal output_array

	# Обработка массива
	la a0, array_A
	la a1, array_B
	mv a2, s0
	jal process_array

	# Вывод результата
	print_str("Array B: ")
	la a0, array_B
	mv a1, s0
	jal output_array

	j main

run_tests:
	jal test_handler
	j main