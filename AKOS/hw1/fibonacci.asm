# ФИО: 	   	Кравченко Игорь Александрович.
# Группа:  	БПИ249.

# Вычисление N-го числа Фибоначчи.

# t3 		- номер числа Фибоначчи, он же счетчик "цикла".
# t0, t1 	- поддерживаем последние 2 числа Фибоначчи.
# t2 		- вспомогательный регистр для вычисления вычисления следующего числа Фибоначчи.

main:
  mv    t0, zero       # R-type		ПСЕВДО: mv t0, zero -> add t0, x0, x0			(t0 = 0)
  li    t1, 1          # I-type		ПСЕВДО: li t1, 1    -> addi t1, x0, 1			(t1 = 1)

  li    a7, 5          # I-type		ПСЕВДО: li a7, 5    -> addi a7, x0, 5			(сис. вызов: считать число)
  ecall                # I-type		РЕАЛ:   ecall       -> выполнить системный вызов 	(считать число)
  mv    t3, a0         # R-type		ПСЕВДО: mv t3, a0   -> add t3, a0, x0     		(t3 = <введенное число>)

fib:
  beqz 	t3, finish     # B-type		ПСЕВДО: beqz t3, finish  -> beq t3, x0, finish		(усл: если t3 = 0 -> goto finish)
  add  	t2, t1, t0     # R-type		РЕАЛ:   add t2, t1, t0					(t2 = t1 + t0)
  mv   	t0, t1         # R-type		ПСЕВДО: mv t0, t1    	 -> add t0, t1, x0		(t0 = t1)
  mv   	t1, t2         # R-type		ПСЕВДО: mv t1, t2    	 -> add t1, t2, x0		(t1 = t2)
  addi 	t3, t3, -1     # I-type		РЕАЛ:   addi t3, t3, -1					(t3 = t3 - 1)
  j    	fib            # J-type		ПСЕВДО: j fib        	 -> jal x0, fib			(goto fib)

finish:
  li   	a7, 1          # I-type		ПСЕВДО: li a7, 1  	 -> addi a7, x0, 1     		(сис. вызов: вывести число)
  mv   	a0, t0         # R-type		ПСЕВДО: mv a0, t0 	 -> add a0, t0, x0     		(a0 = t0, число для вывода)
  ecall                # I-type		РЕАЛ:   ecall     	 -> выполнить системный вызов 	(вывод числа)