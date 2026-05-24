#include "StringGenerator.h"
#include "StringSortTester.h"
#include "SortAlgorithms.h"
#include <iostream>
#include <vector>

int main() {
    StringGenerator generator(1337); 
    StringSortTester tester;
    std::vector<StringSortTester::TestResult> allResults;

    // Задаем шаг изменения размеров массивов: от 100 до 3000 элементов с шагом 100
    std::vector<size_t> sizes;
    for (size_t s = 100; s <= 3000; s += 100) {
        sizes.push_back(s);
    }

    // Четыре сценария тестовых данных
    std::vector<std::string> arrayTypes = {"Random", "ReverseSorted", "NearlySorted", "SharedPrefix"};

    // 1. Для максимальной стабильности генерируем 4 мастер-массива максимальной длины (3000)
    std::cout << "Генерация базовых массивов строк (размер = 3000)...\n";
    std::vector<std::string> masterRandom = generator.generateRandomArray(3000);
    std::vector<std::string> masterReverse = generator.generateReverseSortedArray(3000);
    std::vector<std::string> masterNearly = generator.generateNearlySortedArray(3000);
    std::vector<std::string> masterSharedPrefix = generator.generateSharedPrefixArray(3000, 50);

    // 2. Последовательно прогоняем замеры для каждого типа данных
    for (const auto& type : arrayTypes) {
        std::cout << "Тестирование на типе данных: " << type << "...\n";
        
        // Выбираем соответствующий мастер-массив
        const std::vector<std::string>& masterArray = 
            (type == "Random") ? masterRandom :
            (type == "ReverseSorted") ? masterReverse :
            (type == "NearlySorted") ? masterNearly : masterSharedPrefix;

        // Идем по шагам размера массива N = 100, 200, ..., 3000
        for (size_t size : sizes) {
            // Выбираем подмассив (срез) длины N из мастер-массива
            std::vector<std::string> baseData(masterArray.begin(), masterArray.begin() + size);

            // 1. Стандартный QuickSort (std::sort)
            allResults.push_back(tester.runTest("Std QuickSort", type, baseData, Algorithms::stdQuickSort));

            // 2. Стандартный MergeSort (std::stable_sort)
            allResults.push_back(tester.runTest("Std MergeSort", type, baseData, Algorithms::stdMergeSort));

            // 3. Собственный 3-Way QuickSort на полных сравнениях строк
            allResults.push_back(tester.runTest("Custom QuickSort", type, baseData, Algorithms::customQuickSort));

            // 4. Собственный MergeSort на полных сравнениях строк
            allResults.push_back(tester.runTest("Custom MergeSort", type, baseData, Algorithms::customMergeSort));

            // 5. Тернарный String QuickSort
            allResults.push_back(tester.runTest("Ternary QuickSort", type, baseData, Algorithms::ternaryStringQuickSort));

            // 6. Сортировка слиянием с LCP-кэшированием префиксов
            allResults.push_back(tester.runTest("String MergeSort", type, baseData, Algorithms::stringMergeSort));

            // 7. Чистый MSD Radix Sort по 257 корзинам
            allResults.push_back(tester.runTest("MSD Radix Sort", type, baseData, Algorithms::msdRadixSort));

            // 8. Гибридный MSD Radix Sort с переключением на Ternary QuickSort при N < 74
            allResults.push_back(tester.runTest("MSD Radix Sort + Fallback", type, baseData, Algorithms::msdRadixSortFallback));
        }
        std::cout << "  [OK] Тип " << type << " успешно протестирован!\n";
    }

    // 3. Экспортируем собранную базу данных результатов в файл результатов
    tester.exportToCSV(allResults, "results/benchmark_results.csv");

    return 0;
}
