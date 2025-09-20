//
// Created by Igor Kravchenko on 20.09.2025.
//

#include <iostream>
#include <deque>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::deque <int> d1, d2;

  while (n--) {
    char cmd;
    std::cin >> cmd;

    if (cmd == '+') {
      int x = 0;
      std::cin >> x;
      d2.push_back(x);

    } else if (cmd == '*') {
      int x = 0;
      std::cin >> x;
      d2.push_front(x);

    } else {
      std::cout << d1.front() << "\n";
      d1.pop_front();
    }
    while (d2.size() > d1.size()) {
      d1.push_back(d2.front());
      d2.pop_front();
    }
  }
}