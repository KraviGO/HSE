#include "StringSortTester.h"
#include "SortAlgorithms.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdexcept>
#include <algorithm>

// Инициализируем внешнюю переменную счетчика сравнений из пространства имен Algorithms
size_t Algorithms::char_comp_count = 0;

// Функция запускает алгоритм несколько раз для надежности, проверяет корректность сортировки и усредняет замеры
StringSortTester::TestResult StringSortTester::runTest(
    const std::string& algoName, 
    const std::string& arrayType,
    const std::vector<std::string>& data, 
    std::function<void(std::vector<std::string>&)> sortFunc,
    size_t numRuns) 
{
    double totalTimeMs = 0.0;
    size_t totalComparisons = 0;

    for (size_t i = 0; i < numRuns; ++i) {
        std::vector<std::string> copyData = data; // Сортируем отдельную копию, чтобы не портить исходник
        Algorithms::resetComparisons();           // Сбрасываем глобальный счетчик сравнений

        // Фиксируем время старта с высокой точностью
        auto start = std::chrono::high_resolution_clock::now();
        sortFunc(copyData);
        auto end = std::chrono::high_resolution_clock::now();

        // Проверяем корректность сортировки. Если алгоритм отработал с багом - выбрасываем ошибку
        if (!std::is_sorted(copyData.begin(), copyData.end())) {
            throw std::runtime_error("Алгоритм '" + algoName + "' не смог отсортировать массив типа '" + arrayType + "'!");
        }

        // Подсчитываем прошедшее время в миллисекундах
        std::chrono::duration<double, std::milli> elapsed = end - start;
        totalTimeMs += elapsed.count();
        totalComparisons += Algorithms::char_comp_count;
    }

    // Возвращаем усредненный результат по всем прогонам
    return {
        algoName, 
        arrayType, 
        data.size(), 
        totalTimeMs / numRuns, 
        totalComparisons / numRuns
    };
}

// Записывает накопленные результаты замеров в файл в формате CSV
void StringSortTester::exportToCSV(const std::vector<TestResult>& results, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << " для записи результатов.\n";
        return;
    }

    // Шапка CSV файла
    file << "Algorithm,ArrayType,Size,TimeMs,Comparisons\n";
    for (const auto& res : results) {
        file << res.algorithmName << ","
             << res.arrayType << ","
             << res.arraySize << ","
             << res.averageTimeMs << ","
             << res.averageComparisons << "\n";
    }
    file.close();
    std::cout << "Результаты замеров успешно записаны в: " << filename << "\n";
}
