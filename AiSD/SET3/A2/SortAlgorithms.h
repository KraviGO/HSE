#pragma once

#include <vector>

// Сортировка вставками.
void insertionSort(std::vector<int>& a, int l, int r) {
  for (int i = l + 1; i <= r; ++i) {
    int tmp = a[i];
    int j = i - 1;
    while (j >= l && a[j] > tmp) {
      a[j + 1] = a[j];
      --j;
    }
    a[j + 1] = tmp;
  }
}


// Слияние.
void merge(std::vector<int>& a, int l, int m, int r) {
  std::vector<int> tmp;
  
  int i = l;
  int j = m + 1;

  while (i <= m && j <= r) {
    if (a[i] <= a[j]) {
      tmp.push_back(a[i++]);
    }
    else {
      tmp.push_back(a[j++]);
    }
  }

  while (i <= m) {
    tmp.push_back(a[i++]);
  }

  while (j <= r) {
    tmp.push_back(a[j++]);
  }

  for (int k = 0; k < tmp.size(); ++k) {
    a[l + k] = tmp[k];
  }
}


// Классическая сортировка слиянием.
void mergeSort(std::vector<int>& a, int l, int r) {
  if (l >= r) return;
  
  int m = l + (r - l) / 2;
  mergeSort(a, l, m);
  mergeSort(a, m + 1, r);
  merge(a, l, m, r);
}

void mergeSort(std::vector<int>& a) {
  if (!a.empty()) {
    mergeSort(a, 0, (int)a.size() - 1);
  }
}


// Сортировка слиянием + сортировка вставками.
void mergeInsertionSort(std::vector<int>& a, int l, int r, int limit) {
  if (r - l + 1 <= limit) {
    insertionSort(a, l, r);
    return;
  }
  
  int m = l + (r - l) / 2;
  mergeInsertionSort(a, l, m, limit);
  mergeInsertionSort(a, m + 1, r, limit);
  merge(a, l, m, r);
}

void mergeInsertionSort(std::vector<int>& a, int limit) {
  if (!a.empty()) {
    mergeInsertionSort(a, 0, (int)a.size() - 1, limit);
  }
}
