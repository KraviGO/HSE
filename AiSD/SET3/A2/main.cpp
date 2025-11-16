#include <iostream>
#include <vector>

#include "ArrayGenerator.h"
#include "SortTester.h"

int main() {
  const int MAX_N = 100000;
  const int MIN_N = 500;
  const int STEP = 100;
  const int MAX_VALUE = 6000;

  ArrayGenerator generator(MAX_N, MAX_VALUE, 100);
  SortTester tester(generator,5);

  std::vector<int> sizes;
  for (int n = MIN_N; n <= MAX_N; n += STEP) {
    sizes.push_back(n);
  }

  // Стандартный MergeSort.
  tester.runMergeSort(sizes, "../SET3/A2/MergeSort.csv");
  std::cout << "MergeSort done.\n";

  // MergeInsertionSort.
  std::vector<int> limits = {5, 10, 20, 30, 50};
  tester.runMergeInsertionSort(sizes, limits, "../SET3/A2/MergeInsertionSort.csv");
  std::cout << "MergeInsertionSort done.\n";

  return 0;
}