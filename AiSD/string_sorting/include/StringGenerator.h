#pragma once
#include <vector>
#include <string>
#include <random>

// Класс для генерации массивов строк под разные тестовые сценарии
class StringGenerator {
private:
    // Алфавит из 74 уникальных символов по заданию
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";
    std::mt19937 rng; // Генератор случайных чисел MT19937

public:
    // Инициализация генератора конкретным сидом для воспроизводимости тестов
    StringGenerator(uint32_t seed = 42);

    // Генерация одной случайной строки заданной длины
    std::string generateRandomString(size_t min_len = 10, size_t max_len = 200);
    
    // Сценарий 1: Полностью случайные (неупорядоченные) строки
    std::vector<std::string> generateRandomArray(size_t size);
    
    // Сценарий 2: Строки в обратном лексикографическом порядке (обратно отсортированные)
    std::vector<std::string> generateReverseSortedArray(size_t size);
    
    // Сценарий 3: Почти отсортированные строки (5% случайных перестановок элементов)
    std::vector<std::string> generateNearlySortedArray(size_t size, double swap_fraction = 0.05);
    
    // Сценарий 4 (дополнительный): Строки со специальным длинным общим префиксом (длина 50)
    std::vector<std::string> generateSharedPrefixArray(size_t size, size_t prefix_len = 50);
};
