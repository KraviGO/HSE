#include <bits/stdc++.h>
using namespace std;
typedef long double ld;
typedef long long ll;

auto now = chrono::steady_clock::now();
mt19937 rnd(now.time_since_epoch().count());

int f(vector<int>& a) {
  int n = a.size();

  int cnt = 0;
  for (int i = 0; i < n-1; ++i) {
    for (int j = i+1; j < n; ++j) {
      if (abs(a[i] - a[j]) == abs(i - j)) {
        ++cnt;
      }
    }
  }

  return cnt;
}

int calc(vector<int>& a) {
  int n = a.size();
  int ans = f(a);
  ld t = 1;
  for (int i = 0; i < 5000; ++i) {
    t *= 0.8;
    t *= t;
    vector<int> tmp1(a.begin(), a.end());
    int x = rnd() % n;
    int y = rnd() % n;
    swap(tmp1[x], tmp1[y]);

    int val = f(tmp1);

    if (val == 0) {
      a = tmp1;
      return val;
    }

    if (val < ans || rnd() % int(1e6) < int(1e6) * exp(-(val - ans) / t)) {
      a = tmp1;
      ans = val;
    }
  }

  return ans;
}

signed main() {
  int n; cin >> n;

  vector<int> a(n);
  iota(a.begin(), a.end(), 0);

  for (int i = 0; i < n; ++i) {
    int x = rnd() % n;
    int y = rnd() % n;
    swap(a[x], a[y]);
  }

  while (calc(a)) {}

  for (auto x : a) cout << x + 1 << " ";

  return 0;
}
