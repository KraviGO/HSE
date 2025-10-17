.macro read_float(%prompt, %result_reg)
	li a7, 4
	la a0, %prompt
	ecall

	li a7, 6
	ecall
	fmv.s %result_reg, fa0
.end_macro


.macro print_float(%value_reg, %format)
	li a7, 2
	fmv.s fa0, %value_reg
	ecall

	li a7, 4
	la a0, %format
	ecall
.end_macro


.macro print_string(%str)
	li a7, 4
	la a0, %str
	ecall
.end_macro


.macro check_positive(%value_reg, %error_label)
	flt.s t0, %value_reg, ft0
	bnez t0, %error_label
.end_macro


.macro call_sqrt(%number_reg, %precision_reg, %result_reg)
	fmv.s fa0, %number_reg
	fmv.s fa1, %precision_reg
	jal ra, sqrt
	fmv.s %result_reg, fa0
.end_macro