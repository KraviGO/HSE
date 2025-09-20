//
// Created by Igor Kravchenko on 16.09.2025.
//

#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

int main() {
  std::string s;
  std::getline(std::cin, s);

  std::string res;
  std::stack<std::string> st;
  std::stack<int> st_range;

  for (char c : s) {
    if (c == '[') {
      st.push(res);
      res.clear();
    } else if (c == ']') {
      std::string tmp = st.top();
      st.pop();
      for (int i = 0; i < st_range.top(); ++i) {
        tmp += res;
      }
      st_range.pop();
      std::swap(tmp, res);
    } else if (isdigit(c)) {
      st_range.push(c - '0');
    } else {
      res += c;
    }
  }

  std::cout << res;
}
