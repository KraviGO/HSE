#include <bits/stdc++.h>
using namespace std;

// int = 32 бита
// основание 256 -> 32 / 8 = 4 прохода

const int B = 256;
const int K = 4;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr); cout.tie(nullptr);

  int n; cin >> n;

  vector<int> a(n), res(n);
  for (auto &x : a) cin >> x;

  for (int &x : a) x ^= INT_MIN;

  for (int i = 0; i < K; ++i) {
    int d = i * 8;
    int cnt[B] = {0};

    for (int x : a) {
      int byte = (x >> d) & 255;
      ++cnt[byte];
    }

    for (int j = 1; j < B; ++j) {
      cnt[j] += cnt[j - 1];
    }

    for (int j = n - 1; j >= 0; --j) {
      int byte = (a[j] >> d) & 255;
      res[--cnt[byte]] = a[j];
    }

    a.swap(res);
  }

  for (int &x : a) x ^= INT_MIN;

  for (int x : a) cout << x << ' ';

  return 0;
}