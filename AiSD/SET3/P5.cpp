#include <bits/stdc++.h>
using namespace std;

auto now = chrono::steady_clock::now();
mt19937 rnd(now.time_since_epoch().count());

void read(vector<vector<int>>& a) {
  int n = a.size();
  int m = (n + 3) >> 2;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      char c; cin >> c;
      int mask = isdigit(c) ? c - '0' : c - 'A' + 10;
      for (int k = 0; k < (j == m-1 ? n - 4 * (m-1) : 4); ++k) {
        a[i][j * 4 + k] = (mask >> (3 - k)) & 1;
      }
    }
  }
}

vector<int> mult(vector<vector<int>>& a, vector<int> v) {
  int n = v.size();
  vector<int> res(n, 0);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      res[i] ^= a[i][j] * v[j];
    }
  }

  return res;
}

bool oneCheck(vector<vector<int>>& a, vector<vector<int>>& b, vector<vector<int>>& c) {
  int n = a.size();

  vector<int> v(n);
  for (auto &x : v) x = rnd() % 2;

  vector<int> X = mult(a, mult(b, v));
  vector<int> Y = mult(c, v);

  for (int i = 0; i < n; ++i) {
    if (X[i] != Y[i]) return false;
  }

  return true;
}

bool check(vector<vector<int>>& a, vector<vector<int>>& b, vector<vector<int>>& c) {
  for (int i = 0; i < 10; ++i) {
    if (!oneCheck(a, b, c)) return false;
  }
  return true;
}

int main() {
  int n; cin >> n;

  vector<vector<int>> a(n, vector<int> (n));
  vector<vector<int>> b(n, vector<int> (n));
  vector<vector<int>> c(n, vector<int> (n));

  read(a);
  read(b);
  read(c);

  cout << (check(a, b, c) ? "YES" : "NO");

  return 0;
}

