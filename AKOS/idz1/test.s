.include "array_io.s"

# Модуль автоматизированного тестирования
.globl test_handler, run_single_test

.data
# Тестовые данные
test_A1: .word 0, 2, -3, 0, 5
test_B1: .word 0:10
test_A2: .word 1, 2, 3, 4, 5
test_B2: .word 0:10  
test_A3: .word 0, 0, 0, -1, 2
test_B3: .word 0:10
test_A4: .word 0, 0, 0, 0, 0
test_B4: .word 0:10
test_size: .word 5

test_header_msg: .asciz "\n=== Running Automated Tests ===\n"
test1_msg: .asciz "Test 1: [0, 2, -3, 0, 5] -> [1, 2, -3, 0, 5] (zeros before negative replaced)\n"
test2_msg: .asciz "Test 2: [1, 2, 3, 4, 5] -> [1, 2, 3, 4, 5] (no negatives, no changes)\n"  
test3_msg: .asciz "Test 3: [0, 0, 0, -1, 2] -> [1, 1, 1, -1, 2] (all zeros before negative replaced)\n"
test4_msg: .asciz "Test 4: [0, 0, 0, 0, 0] -> [0, 0, 0, 0, 0] (no negatives, zeros remain)\n"
test_footer_msg: .asciz "=== Tests Complete ===\n\n"

.text
# Обработчик тестов
test_handler:
	addi sp, sp, -4
	sw ra, 0(sp)

	la a0, test_header_msg
	li a7, 4
	ecall

	# Тест 1: нули до отрицательного
	la a0, test1_msg
	li a7, 4
	ecall
	la a0, test_A1
	la a1, test_B1
	lw a2, test_size
	jal run_single_test
    
	# Тест 2: нет отрицательных
	la a0, test2_msg
	li a7, 4
	ecall
	la a0, test_A2
	la a1, test_B2
	lw a2, test_size
	jal run_single_test

	# Тест 3: отрицательный в конце
	la a0, test3_msg
	li a7, 4
	ecall
	la a0, test_A3
	la a1, test_B3
	lw a2, test_size
	jal run_single_test

	# Тест 4: все нули, нет отрицательных
	la a0, test4_msg
	li a7, 4
	ecall
	la a0, test_A4
	la a1, test_B4
	lw a2, test_size
	jal run_single_test

	la a0, test_footer_msg
	li a7, 4
	ecall

	lw ra, 0(sp)
	addi sp, sp, 4
	ret

# Запуск одного теста
# a0 - адрес A, a1 - адрес B, a2 - размер
run_single_test:
	addi sp, sp, -20
	sw ra, 16(sp)
	sw s0, 12(sp)
	sw s1, 8(sp)
	sw s2, 4(sp)
	sw s3, 0(sp)
 
	mv s0, a0	# A
	mv s1, a1	# B
	mv s2, a2	# размер

	# Вывод исходного массива
	la a0, array_A_msg
	li a7, 4
	ecall
	mv a0, s0
	mv a1, s2
	jal output_array

	# Обработка
	mv a0, s0
	mv a1, s1
	mv a2, s2
	jal process_array

	# Вывод результата
	la a0, array_B_msg
	li a7, 4
	ecall
	mv a0, s1
	mv a1, s2
	jal output_array

	li a7, 11
	li a0, '\n'
	ecall

	lw s3, 0(sp)
	lw s2, 4(sp)
	lw s1, 8(sp)
	lw s0, 12(sp)
	lw ra, 16(sp)
	addi sp, sp, 20
	ret