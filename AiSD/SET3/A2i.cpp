#include <bits/stdc++.h>
using namespace std;

void insertionSort(vector<int>& a, int l, int r) {
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

void merge(vector<int>& a, int l, int mid, int r) {
  vector<int> tmp;

  int i = l;
  int j = mid + 1;

  while (i <= mid && j <= r) {
    if (a[i] <= a[j]) {
      tmp.push_back(a[i++]);
    }
    else {
      tmp.push_back(a[j++]);
    }
  }

  while (i <= mid) {
    tmp.push_back(a[i++]);
  }

  while (j <= r) {
    tmp.push_back(a[j++]);
  }

  for (int k = 0; k < tmp.size(); ++k) {
    a[l + k] = tmp[k];
  }
}

void mergeSortHybrid(vector<int>& a, int l, int r, int limit) {
  if (r - l + 1 <= limit) {
    insertionSort(a, l, r);
    return;
  }
  
  int mid = l + (r - l) / 2;
  mergeSortHybrid(a, l, mid, limit);
  mergeSortHybrid(a, mid + 1, r, limit);
  merge(a, l, mid, r);
}

void mergeSortHybrid(vector<int>& a, int limit) {
  if (!a.empty()) {
    mergeSortHybrid(a, 0, (int)a.size() - 1, limit);
  }
}

int main() {
  int n; cin >> n;

  vector<int> a(n);
  for (auto &x : a) cin >> x;

  mergeSortHybrid(a, 15);

  for (auto x : a) cout << x << " ";

  return 0;
}