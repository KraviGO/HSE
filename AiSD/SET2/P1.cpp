//
// Created by Igor Kravchenko on 08.10.2025.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct Interval {
  int left;
  int right;

  Interval() : left(1), right(0) {};
  Interval(int left, int right) : left(left), right(right) {}

  size_t length() {
    return right - left + 1;
  };

  Interval overlap(const Interval& other) {
    int r = min(right, other.right);
    int l = max(left, other.left);

    if (l > r) {
      return {1, 0};
    }

    return {l, r};
  };

  bool operator<(const Interval& other) const {
    if (left != other.left) {
      return left < other.left;
    }
    return right < other.right;
  }
};

template <class T>
void MergeSort(vector<T>& a, int l, int r) {
  if (r-l <= 1) {
    return;
  }

  int mid = (l + r) / 2;
  MergeSort(a, l, mid);
  MergeSort(a, mid, r);
  
  vector<T> tmp(r-l);
  for (int t = 0, i = l, j = mid; t < r-l; ++t) {
      if (i < mid && j < r) {
          tmp[t] = a[i] < a[j] ? a[i++] : a[j++];
      }
      else if (i < mid) {
          tmp[t] = a[i++];
      } else {
          tmp[t] = a[j++];
      }
  }
  
  for (int i = l; i < r; ++i) {
      a[i] = tmp[i - l];
  }
}


int main() {
  int n; cin >> n;

  vector<Interval> a(n);
  for (auto& [l, r] : a) {
    cin >> l >> r;
  }

  MergeSort(a, 0, n);
  // sort(a.begin(), a.end());

  Interval best;
  int cur = 0;
  for (int i = 1; i < n; ++i) {
    Interval tmp = a[i].overlap(a[cur]);
    if (tmp.length() > best.length()) {
      best = tmp;
    }

    if (a[i].right > a[cur].right) {
      cur = i;
    }
  }

  cout << best.length() << '\n';

  if (best.length()) {
    cout << best.left << " " << best.right;
  }
}