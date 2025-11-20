.data
.align 3
test_cases:
    .double 0.0, 0.5, 1.0, 1.5, 2.0
expected_results:
    .double 1.0, 0.6065306597126334, 0.36787944117144233, 0.22313016014842987, 0.1353352832366127
test_count: .word 5

test_header: .asciz "=== Automated Tests ===\n"
test_case_msg: .asciz "Test case x = "
computed_msg: .asciz " Computed: "
expected_msg: .asciz " Expected: "
error_msg: .asciz " Error: "
passed_msg: .asciz " PASSED\n"
failed_msg: .asciz " FAILED\n"
threshold_msg: .asciz "Threshold: 0.05%\n"
epsilon_const: .double 0.0005

.text
.globl test_main

test_main:
    # Вывод заголовка тестов
    la a0, test_header
    li a7, 4
    ecall
    
    la a0, threshold_msg
    li a7, 4
    ecall
    
    # Инициализация указателей
    la s0, test_cases
    la s1, expected_results
    lw s2, test_count
    
test_loop:
    beqz s2, tests_complete
    
    # Вывод информации о тесте
    la a0, test_case_msg
    li a7, 4
    ecall
    
    fld fa0, 0(s0)
    li a7, 3
    ecall
    
    li a0, '\n'
    li a7, 11
    ecall
    
    # Вычисление e^(-x)
    fld fa0, 0(s0)
    jal exp_neg_x
    fmv.d fs0, fa0
    
    # Загрузка ожидаемого значения
    fld fs1, 0(s1)
    
    # Вывод вычисленного значения
    la a0, computed_msg
    li a7, 4
    ecall
    
    fmv.d fa0, fs0
    li a7, 3
    ecall
    
    li a0, '\n'
    li a7, 11
    ecall
    
    # Вывод ожидаемого значения
    la a0, expected_msg
    li a7, 4
    ecall
    
    fmv.d fa0, fs1
    li a7, 3
    ecall
    
    li a0, '\n'
    li a7, 11
    ecall
    
    # Вычисление ошибки
    fsub.d ft0, fs0, fs1
    fabs.d ft0, ft0
    fdiv.d ft0, ft0, fs1
    
    # Вывод ошибки
    la a0, error_msg
    li a7, 4
    ecall
    
    fmv.d fa0, ft0
    li a7, 3
    ecall
    
    li a0, '\n'
    li a7, 11
    ecall
    
    # Проверка порога 0.05%
    la t0, epsilon_const
    fld ft1, 0(t0)           # Загружаем epsilon = 0.0005
    flt.d t1, ft0, ft1       # Сравниваем ошибку с epsilon
    
    bnez t1, test_passed
    
test_failed:
    la a0, failed_msg
    li a7, 4
    ecall
    j test_next
    
test_passed:
    la a0, passed_msg
    li a7, 4
    ecall
    
test_next:
    # Следующий тест
    addi s0, s0, 8
    addi s1, s1, 8
    addi s2, s2, -1
    j test_loop

tests_complete:
    li a7, 10
    ecall