.data
filename: .space 64
input_size_msg: .asciz "Input array size (1-10): "
input_element_msg: .asciz "Element "
input_colon_msg: .asciz ": "
input_size_error_msg: .asciz "Error: size must be 1-10\n"
input_filename_msg: .asciz "Input filename: "
file_size_error_msg: .asciz "Error: invalid size in file\n"
file_error_msg: .asciz "Error: cannot open file\n"
array_A_msg: .asciz "Array A: "
array_B_msg: .asciz "Array B: "

.text

# Ввод массива с клавиатуры
# a0 - адрес массива, a1 - максимальный размер
# Возвращает: фактический размер в a0
input_array:
	addi sp, sp, -20
	sw ra, 16(sp)
	sw s0, 12(sp)
	sw s1, 8(sp)
	sw s2, 4(sp)
	sw s3, 0(sp)

	mv s0, a0	# адрес массива
	mv s1, a1	# максимальный размер
    
	# Ввод размера
	la a0, input_size_msg
	li a7, 4
	ecall
    
	li a7, 5
	ecall
	mv s2, a0	# сохраняем размер

	# Проверка корректности размера
	li t0, 1
	blt s2, t0, input_error
	li t0, 10
	bgt s2, t0, input_error
    
	# Ввод элементов
	li s3, 0
	
input_loop:
	bge s3, s2, input_success

	la a0, input_element_msg
	li a7, 4
	ecall

	li a7, 1
	mv a0, s3
	ecall

	la a0, input_colon_msg
	li a7, 4
	ecall

	li a7, 5
	ecall

	slli t0, s3, 2
	add t0, s0, t0
	sw a0, 0(t0)

	addi s3, s3, 1
	j input_loop

input_error:
	la a0, input_size_error_msg
	li a7, 4
	ecall
	li a0, -1
	j input_end

input_success:
	mv a0, s2

input_end:
	lw s3, 0(sp)
	lw s2, 4(sp)
	lw s1, 8(sp)
	lw s0, 12(sp)
	lw ra, 16(sp)
	addi sp, sp, 20
	ret

# Вывод массива
# a0 - адрес массива, a1 - размер
output_array:
	addi sp, sp, -16
	sw ra, 12(sp)
	sw s0, 8(sp)
	sw s1, 4(sp)
	sw s2, 0(sp)

	mv s0, a0	# адрес массива
	mv s1, a1	# размер
    
	li a7, 11
	li a0, '['
	ecall

	li s2, 0

output_loop:
	bge s2, s1, output_end

	slli t0, s2, 2
	add t0, s0, t0
	lw a0, 0(t0)
	li a7, 1
	ecall

	addi s2, s2, 1
	bge s2, s1, output_end

	li a7, 11
	li a0, ','
	ecall
	li a0, ' '
	ecall

	j output_loop

output_end:
	li a7, 11
	li a0, ']'
	ecall
	li a0, '\n'
	ecall

	lw s2, 0(sp)
	lw s1, 4(sp)
	lw s0, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	ret

# Ввод массива из файла
# a0 - адрес массива, a1 - максимальный размер
# Возвращает: фактический размер в a0
input_array_from_file:
	addi sp, sp, -24
	sw ra, 20(sp)
	sw s0, 16(sp)
	sw s1, 12(sp)
	sw s2, 8(sp)
	sw s3, 4(sp)
	sw s4, 0(sp)

	mv s0, a0	# адрес массива
	mv s1, a1	# максимальный размер

	# Ввод имени файла
	la a0, input_filename_msg
	li a7, 4
	ecall

	la a0, filename
	li a1, 64
	li a7, 8
	ecall

	# Удаляем символ новой строки
	la t0, filename

remove_newline:
	lb t1, 0(t0)
	beqz t1, open_file
	li t2, '\n'
	beq t1, t2, found_newline
	addi t0, t0, 1
	j remove_newline

found_newline:
	sb zero, 0(t0)

open_file:
	# Открываем файл
	la a0, filename
	li a1, 0	# read-only
	li a7, 1024
	ecall
	blt a0, zero, file_error
	mv s2, a0	# file descriptor

	# Читаем размер
	mv a0, s2
	addi sp, sp, -4
	mv a1, sp
	li a2, 4
	li a7, 63
	ecall
	lw s3, 0(sp)	# размер из файла
	addi sp, sp, 4

	# Проверка размера
	blez s3, file_size_error
	bgt s3, s1, file_size_error
	
	# Читаем элементы
	li s4, 0
	
file_read_loop:
	bge s4, s3, file_success

	mv a0, s2
	addi sp, sp, -4
	mv a1, sp
	li a2, 4
	li a7, 63
	ecall

	lw t0, 0(sp)
	addi sp, sp, 4

	slli t1, s4, 2
	add t1, s0, t1
	sw t0, 0(t1)

	addi s4, s4, 1
	j file_read_loop

file_size_error:
	la a0, file_size_error_msg
	li a7, 4
	ecall
	li a0, -1
	j close_file

file_error:
	la a0, file_error_msg
	li a7, 4
	ecall
	li a0, -1
	j file_end

file_success:
	mv a0, s3

close_file:
	# Закрываем файл
	mv a0, s2
	li a7, 57
	ecall

file_end:
	lw s4, 0(sp)
	lw s3, 4(sp)
	lw s2, 8(sp)
	lw s1, 12(sp)
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	ret