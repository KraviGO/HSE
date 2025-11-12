echo "Введите число:"
read number

# Проверка, что число положительное
if [ $number -gt 0 ]; then
    echo "Это положительное число"
elif [ $number -lt 0 ]; then
    echo "Это отрицательное число"
else
    echo "Это ноль"
fi

# Проверка, что число четное
if [ $((number % 2)) -eq 0 ]; then
    echo "И оно четное"
else
    echo "И оно нечетное"
fi