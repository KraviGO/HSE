echo "Сколько раз вывести 'Hello World!'?"
read count

# Цикл while от 1 до count
i=1
while [ $i -le $count ]; do
    echo "$i: Hello World!"
    i=$((i + 1))
done

echo "Скрипт выполнен!"