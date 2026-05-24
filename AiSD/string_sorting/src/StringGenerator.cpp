#include "StringGenerator.h"
#include <algorithm>

StringGenerator::StringGenerator(uint32_t seed) : rng(seed) {}

// Генерирует одну случайную строку со случайной длиной от min_len до max_len
std::string StringGenerator::generateRandomString(size_t min_len, size_t max_len) {
    std::uniform_int_distribution<size_t> len_dist(min_len, max_len);
    std::uniform_int_distribution<size_t> char_dist(0, alphabet.size() - 1);
    
    size_t len = len_dist(rng);
    std::string res = "";
    res.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        res.push_back(alphabet[char_dist(rng)]);
    }
    return res;
}

// Создает массив из случайных строк
std::vector<std::string> StringGenerator::generateRandomArray(size_t size) {
    std::vector<std::string> arr(size);
    for (size_t i = 0; i < size; ++i) {
        arr[i] = generateRandomString(10, 200);
    }
    return arr;
}

// Создает обратно отсортированный массив строк
std::vector<std::string> StringGenerator::generateReverseSortedArray(size_t size) {
    auto arr = generateRandomArray(size);
    // Сортируем по убыванию (лексикографически обратно)
    std::sort(arr.begin(), arr.end(), std::greater<std::string>());
    return arr;
}

// Создает почти отсортированный массив строк на основе отсортированного
std::vector<std::string> StringGenerator::generateNearlySortedArray(size_t size, double swap_fraction) {
    auto arr = generateRandomArray(size);
    // Сначала полностью сортируем по возрастанию
    std::sort(arr.begin(), arr.end());
    
    // Вычисляем количество случайных перестановок
    size_t num_swaps = static_cast<size_t>(size * swap_fraction);
    if (num_swaps == 0 && swap_fraction > 0 && size > 1) {
        num_swaps = 1;
    }
    
    // Делаем случайные обмены пар строк для нарушения идеального порядка
    std::uniform_int_distribution<size_t> idx_dist(0, size - 1);
    for (size_t i = 0; i < num_swaps; ++i) {
        std::swap(arr[idx_dist(rng)], arr[idx_dist(rng)]);
    }
    return arr;
}

// Создает строки, которые делят общий префикс заданной длины (SharedPrefix)
std::vector<std::string> StringGenerator::generateSharedPrefixArray(size_t size, size_t prefix_len) {
    // 1. Сначала генерируем случайный префикс общей длины prefix_len
    std::uniform_int_distribution<size_t> char_dist(0, alphabet.size() - 1);
    std::string common_prefix = "";
    common_prefix.reserve(prefix_len);
    for (size_t i = 0; i < prefix_len; ++i) {
        common_prefix.push_back(alphabet[char_dist(rng)]);
    }
    
    // 2. Добавляем к этому общему префиксу уникальные случайные хвосты
    std::vector<std::string> arr(size);
    for (size_t i = 0; i < size; ++i) {
        // Чтобы общая длина строки укладывалась в рамки 10..200
        size_t min_suffix = (prefix_len >= 10) ? 0 : (10 - prefix_len);
        size_t max_suffix = (prefix_len >= 200) ? 0 : (200 - prefix_len);
        
        arr[i] = common_prefix + generateRandomString(min_suffix, max_suffix);
    }
    return arr;
}
