#pragma once

#include <chrono>
#include <fstream>
#include <string>
#include <vector>

#include "SortAlgorithms.h"
#include "ArrayGenerator.h"

enum class ArrayType {
  Random,
  Reverse,
  AlmostSorted
};

class SortTester {
public:
  SortTester(ArrayGenerator& generator, int runs = 5) : gen_(generator), runs_(runs) {}

  // Функция измерения времени сортировки.
  template <typename SortFunction>
  long double measure(ArrayType type, int n, SortFunction sorter) {
    long long allTime = 0;

    for (int i = 0; i < runs_; ++i) {
      std::vector<int> arr = generateArray(type, n);

      auto start= std::chrono::high_resolution_clock::now();
      sorter(arr);
      auto end = std::chrono::high_resolution_clock::now();

      long long time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
      allTime += time;
    }

    return (long double)(allTime) / runs_;
  }

  // Тестирование обычного merge sort.
  void runMergeSort(std::vector<int>& sizes, const std::string& csvFile) {
    std::ofstream out(csvFile);
    out << "type,n,limit,time\n";

    for (ArrayType t : {ArrayType::Random, ArrayType::Reverse, ArrayType::AlmostSorted}) {
      for (int n : sizes) {
        long double avg = measure(t, n,[](std::vector<int>& a) {mergeSort(a, 0, int(a.size()) - 1);});
        out << arrayTypeToString(t) << "," << n << ",0," << avg << "\n";
      }
    }
  }

  // Тестирование merge + insertion.
  void runMergeInsertionSort(std::vector<int>& sizes, std::vector<int>& limit, const std::string& csvFile) {
    std::ofstream out(csvFile);
    out << "type,n,limit,time\n";

    for (ArrayType t : {ArrayType::Random, ArrayType::Reverse, ArrayType::AlmostSorted}) {
      for (int lm : limit) {
        for (int n : sizes) {
          long double avg = measure(t, n,[lm](std::vector<int>& a) {mergeInsertionSort(a, 0, int(a.size()) - 1, lm);});
          out << arrayTypeToString(t) << "," << n << "," << lm << "," << avg << "\n";
        }
      }
    }
  }

private:
  ArrayGenerator& gen_;
  int runs_;

  std::vector<int> generateArray(ArrayType type, int n) {
    switch (type) {
    case ArrayType::Random:
      return gen_.getRandomPrefix(n);
    case ArrayType::Reverse:
      return gen_.getReversePrefix(n);
    case ArrayType::AlmostSorted:
      return gen_.getAlmostSortedPrefix(n);
    }
    return {};
  }

  std::string arrayTypeToString(ArrayType t) const {
    switch (t) {
    case ArrayType::Random:
      return "random";
    case ArrayType::Reverse:
      return "reverse";
    case ArrayType::AlmostSorted:
      return "almost";
    }
    return "unknown";
  }
};