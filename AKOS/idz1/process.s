# Подпрограмма обработки массива
.globl process_array

.text
# Обработка массива - замена нулей перед первым отрицательным единицами
# a0 - адрес исходного массива A
# a1 - адрес результирующего массива B  
# a2 - размер массивов
process_array:
	addi sp, sp, -24
	sw ra, 20(sp)
	sw s0, 16(sp)
	sw s1, 12(sp)
	sw s2, 8(sp)
	sw s3, 4(sp)
	sw s4, 0(sp)

	mv s0, a0	# адрес массива A
	mv s1, a1	# адрес массива B
	mv s2, a2	# размер
    
	# Находим первый отрицательный элемент
	li s3, -1	# индекс первого отрицательного
	li t0, 0	# текущий индекс
	
find_negative_loop:
	bge t0, s2, find_done
	slli t1, t0, 2
	add t1, s0, t1
	lw t2, 0(t1)
	blt t2, zero, found_negative
	addi t0, t0, 1
	j find_negative_loop

found_negative:
	mv s3, t0	# сохраняем индекс первого отрицательного

find_done:
	# Формируем массив B
	li t0, 0	# текущий индекс
process_loop:
	bge t0, s2, process_done

	slli t1, t0, 2
	add t1, s0, t1
	lw t2, 0(t1)	# A[i]

	# Проверяем условие замены: 
	# если элемент нулевой И находится до первого отрицательного
	blt s3, zero, copy_element	# если отрицательных нет
	bge t0, s3, copy_element	# если после первого отрицательного
	beqz t2, replace_with_one	# если ноль до первого отрицательного
	j copy_element

replace_with_one:
	li t2, 1	# заменяем на 1

copy_element:
	slli t1, t0, 2
	add t1, s1, t1
	sw t2, 0(t1)	# B[i] = значение

	addi t0, t0, 1
	j process_loop

process_done:
	lw s4, 0(sp)
	lw s3, 4(sp)
	lw s2, 8(sp)
	lw s1, 12(sp)
	lw s0, 16(sp)
	lw ra, 20(sp)
	addi sp, sp, 24
	ret