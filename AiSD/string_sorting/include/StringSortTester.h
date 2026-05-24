#pragma once
#include <vector>
#include <string>
#include <functional>

// Класс для тестирования производительности различных алгоритмов сортировки строк
class StringSortTester {
public:
    // Структура для сохранения результатов одного замера
    struct TestResult {
        std::string algorithmName;    // Название алгоритма
        std::string arrayType;        // Тип массива строк (Random, Reverse и т.д.)
        size_t arraySize;             // Размер тестируемого массива (N)
        double averageTimeMs;         // Среднее время выполнения (в миллисекундах)
        size_t averageComparisons;    // Среднее число посимвольных сравнений
    };

    // Запускает многократное тестирование алгоритма на конкретных данных и возвращает средние метрики
    TestResult runTest(
        const std::string& algoName, 
        const std::string& arrayType,
        const std::vector<std::string>& data, 
        std::function<void(std::vector<std::string>&)> sortFunc,
        size_t numRuns = 5
    );

    // Экспортирует собранную таблицу результатов в CSV-файл
    void exportToCSV(const std::vector<TestResult>& results, const std::string& filename);
};
