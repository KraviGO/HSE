#include <bits/stdc++.h>
using namespace std;

const int D = 1e6;
const int N = 2e6 + 1;
int cnt[N] = {0};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr); cout.tie(nullptr);
  
  int n; cin >> n;

  for (int i = 0; i < n; ++i) {
    int x; cin >> x;
    ++cnt[x + D];
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < cnt[i]; ++j) {
      cout << i - D << " ";
    }
  }

  return 0;
}