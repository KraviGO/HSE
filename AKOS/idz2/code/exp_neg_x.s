.data
.align 3
epsilon: .double 0.0005
one: .double 1.0
minus_one: .double -1.0

.text
.globl exp_neg_x

# Подпрограмма вычисления e^(-x)
# Вход: fa0 = x
# Выход: fa0 = e^(-x)
exp_neg_x:
	# Пролог
	addi sp, sp, -48
	sw ra, 44(sp)
	fsw fs0, 40(sp)
	fsw fs1, 36(sp)
	fsw fs2, 32(sp)
	fsw fs3, 28(sp)
	sw s0, 24(sp)

	# Сохраняем входное значение
	fmv.d fs0, fa0

	# Инициализация переменных
	la t0, one
	fld ft0, 0(t0)
	fmv.d fs1, ft0
	fmv.d fs2, ft0
	li s0, 1

	# Загружаем epsilon
	la t0, epsilon
	fld fs3, 0(t0)

calculation_loop:
	# current_term = current_term * (-x) / n
	la t0, minus_one
	fld ft0, 0(t0)
	fmul.d fs2, fs2, ft0	# current_term *= -1
	fmul.d fs2, fs2, fs0	# current_term *= x

	# Конвертируем n в double и делим
	fcvt.d.w ft1, s0	# ft1 = n как double
	fdiv.d fs2, fs2, ft1	# current_term /= n

	# current_sum += current_term
	fadd.d fs1, fs1, fs2

	# Проверка точности: |current_term| < epsilon * |current_sum|
	fabs.d ft2, fs2          # ft2 = |current_term|
	fabs.d ft3, fs1          # ft3 = |current_sum|
	fmul.d ft3, ft3, fs3     # ft3 = epsilon * |current_sum|

	# Если |current_term| < epsilon * |current_sum|, выходим
	flt.d t0, ft2, ft3
	bnez t0, calculation_end

	# Увеличиваем n и продолжаем
	addi s0, s0, 1
	j calculation_loop

calculation_end:
	# Возвращаем результат
	fmv.d fa0, fs1

	# Эпилог
	lw ra, 44(sp)
	flw fs0, 40(sp)
	flw fs1, 36(sp)
	flw fs2, 32(sp)
	flw fs3, 28(sp)
	lw s0, 24(sp)
	addi sp, sp, 48
	ret