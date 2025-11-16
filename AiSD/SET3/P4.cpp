#include "bits/stdc++.h"

using namespace std;

bool cmp(pair<int, int> l, pair<int, int> r) {
  return l.first * r.second < r.first * l.second;
}

int main() {
  int n; cin >> n;

  vector<pair<int, int>> a(n);
  for (int i = 0; i < n; ++i) {
    char c;
    cin >> a[i].first >> c >> a[i].second;
  }

  stable_sort(a.begin(), a.end(), cmp);

  for (auto [x, y] : a) {
    cout << x << '/' << y << " ";
  }

  return 0;
}