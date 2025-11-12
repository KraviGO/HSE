echo "Текущая папка: $(pwd)"
echo "Файлы в папке:"
ls

# Проверка существует ли файл
echo "Введите имя файла для проверки:"
read filename

if [ -f "$filename" ]; then
    echo "Файл '$filename' существует"
    
    # Показываем размер файла
    size=$(wc -c < "$filename")
    echo "Размер файла: $size байт"
else
    echo "Файл '$filename' не существует"
    
    # Предлагаем создать файл
    echo "Хотите создать этот файл? (y/n)"
    read answer
    if [ "$answer" = "y" ]; then
        touch "$filename"
        echo "Файл '$filename' создан"
    fi
fi