//
// Created by Igor Kravchenko on 16.09.2025.
//

#include <vector>
#include <iostream>

int main() {
  int n = 0;
  std::cin >> n;

  std::vector<int> a(n, 0);
  for (int& x : a) {
    std::cin >> x;
  }

  int repeatCnt = 1;
  int best = 0;
  int cntBest = 1;
  for (int d = 1; d < n; ++d) {
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
      if (a[i] != a[(i + d) % n]) {
        ++cnt;
      }
    }
    if (cnt == 0) {
      ++repeatCnt;
    }
    if (best == cnt) {
      ++cntBest;
    } else if (cnt > best) {
      best = cnt;
      cntBest = 1;
    }
  }

  std::cout << cntBest / repeatCnt;

  return 0;
}