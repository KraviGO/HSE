//
// Created by Igor Kravchenko on 20.09.2025.
//

#include <iostream>
#include <stack>
#include <queue>
#include <string>

int main() {
  int n = 0;
  std::cin >> n;

  std::queue<int> q;
  std::stack<int> st;

  for(int i = 0; i < n; ++i) {
    int x = 0;
    std::cin >> x;
    q.push(x);
  }

  int cur = 1;
  std::string ans;

  int cnt1 = 0;
  for (int i = 0; i < n; ++i) {
    st.push(q.front());
    q.pop();
    ++cnt1;

    if (st.top() == cur) {
      ans += "1 " + std::to_string(cnt1) + "\n";
      cnt1 = 0;
    }

    int cnt2 = 0;
    while (!st.empty() && st.top() == cur) {
      ++cur;
      ++cnt2;
      st.pop();
    }
    if (cnt2 > 0) {
      ans += "2 " + std::to_string(cnt2) + '\n';
    }
  }

  std::cout << (st.empty() ? ans : "0");
}