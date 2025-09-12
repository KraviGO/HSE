//
// Created by Igor Kravchenko on 11.09.2025.
//

/*
  Проведите сравнительный анализ количества сравнений и перестановок,
  которые требуются для сортировки массивов с помощью сортировки Шелла
  [длина интервала на ваше усмотрение] и стандартной сортировки вставками.
  Массив заполняется случайными значениями.
    • Выполните несколько тестов для оценки алгоритмов.
    • Сформулируйте выводы на основании тестов.
      Какой алгоритм показывает себя лучше?
*/

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <string>

size_t insertionSortSwap(std::vector<int>& arr) {
  size_t cnt = 0;
  for (size_t i = 1; i < arr.size(); ++i) {
    for (size_t j = i; j > 0 && arr[j - 1] > arr[j]; --j) {
      ++cnt;
      std::swap(arr[j-1], arr[j]);
    }
  }
  return cnt;
}

size_t insertionSort(std::vector<int>& arr) {
  size_t cnt = 0;
  for (size_t i = 1, j = 0; i < arr.size(); ++i) {
    int tmp = arr[i];
    for (j = i; j > 0 && arr[j - 1] > tmp; --j) {
      ++cnt;
      arr[j] = arr[j - 1];
    }
    ++cnt;
    arr[j] = tmp;
  }
  return cnt;
}

size_t shellSortSwap(std::vector<int>& arr) {
  size_t cnt = 0;
  size_t n = arr.size();
  for (size_t interval = n / 2; interval > 0; interval /= 2) {
    for (size_t i = interval; i < n; ++i) {
      for (size_t j = i; j >= interval && arr[j - interval] > arr[j]; j -= interval) {
        ++cnt;
        std::swap(arr[j], arr[j - interval]);
      }
    }
  }
  return cnt;
}

size_t shellSort(std::vector<int>& arr) {
  size_t cnt = 0;
  size_t n = arr.size();
  for (size_t interval = n / 2, j = 0; interval > 0; interval /= 2) {
    for (size_t i = interval; i < n; ++i) {
      int tmp = arr[i];
      for (j = i; j >= interval && arr[j - interval] > tmp; j -= interval) {
        ++cnt;
        arr[j] = arr[j - interval];
      }
      ++cnt;
      arr[j] = tmp;
    }
  }
  return cnt;
}

// Генерация массива размера sz заполненного случайными значениями от a до b.
std::vector<int> genArr(size_t sz=0, int a=0, int b=1000) {
  std::random_device rd;
  std::mt19937 gen(rd());

  if (sz == 0) {
    sz = gen() % 100;
  }

  std::vector<int> arr(sz);
  for (size_t i = 0; i < sz; ++i) {
    arr[i] = (static_cast<int>(gen()) % (a + b)) - a;
  }

  return arr;
}

// Проверка корректности алгоритма сортировки.
template <class F>
bool checker(F& func, size_t cnt=1) {
  // cnt запусков сортировок для разных массивов.
  while (cnt-- > 0) {
    std::vector<int> arr1 = genArr();
    std::vector<int> arr2 = arr1;

    func(arr1);
    std::sort(arr2.begin(), arr2.end());

    // Сравнение значений отсортированных массивов самописным алгоритмом и sort.
    for (int i = 0; i < arr1.size(); ++i) {
      if (arr1[i] != arr2[i]) {
        return false;
      }
    }
  }

  return true;
}

// Вывод количества итераций и время исполнения алгоритма сортировки.
template <class F>
void printTimeCnt(std::vector<int> arr, F& func) {
  auto start = std::chrono::high_resolution_clock::now();
  size_t cnt = func(arr);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time = end - start;

  std::cout << "Количество итераций: " << cnt << " | Время: " << time.count() << "\n";
}

int main() {
  // Проверим алгоритмы сортировок на корректность.
  std::cout << "insertionSortSwap: " << checker(insertionSortSwap, 10) << '\n';
  std::cout << "insertionSort: " << checker(insertionSort, 10) << '\n';
  std::cout << "shellSortSwap: " << checker(shellSortSwap, 10) << '\n';
  std::cout << "shellSort: " << checker(shellSort, 10) << '\n';

  // Проверим для массивов разной длины.
  std::vector<size_t> sizes = {10, 100, 1000, 10000, 100000};

  for (size_t sz : sizes) {
    std::cout << std::string(15, '-') << '\n';
    std::cout << "Размер генерируемого массива: " << sz << "\n";

    std::vector<int> arr = genArr(sz, 0, sz);

    std::cout << "\ninsertionSortSwap\n";
    printTimeCnt(arr, insertionSortSwap);

    std::cout << "\ninsertionSort\n";
    printTimeCnt(arr, insertionSort);

    std::cout << "\nshellSortSwap\n";
    printTimeCnt(arr, shellSortSwap);

    std::cout << "\nshellSort\n";
    printTimeCnt(arr, shellSort);
  }

  /*
   * Вывод:
   * Практика показала, что алгоритмы написанные со swap медленнее тех, которые не используют его.
   * Так же выяснилось, что shell sort на практике работает значительно быстрее insertion sort.
   * Если посмотреть на количество итераций, то для insertion sort оно растет пропорционально с n^2,
   * а для shell sort пропорционально с n^(3/2).
   */

  return 0;
}