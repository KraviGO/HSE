.text
.globl sqrt

sqrt:
	fmv.s ft0, fa0
	li t0, 2
	fcvt.s.w ft1, t0
	fdiv.s ft0, fa0, ft1

	fmv.s ft3, fa1
    
    
sqrt_loop:
	fdiv.s ft2, fa0, ft0
	fadd.s ft2, ft0, ft2

	li t0, 2
	fcvt.s.w ft1, t0
	fdiv.s ft1, ft2, ft1

	fsub.s ft2, ft1, ft0
	fabs.s ft2, ft2

	flt.s t0, ft2, ft3
	bnez t0, sqrt_done

	fmv.s ft0, ft1
	j sqrt_loop


sqrt_done:
	fmv.s fa0, ft1
	ret