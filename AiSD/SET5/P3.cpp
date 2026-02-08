#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
  int n; cin >> n;
  vector<int> a(n);
  for (auto &x : a) cin >> x;

  int m; cin >> m;
  vector<int> b(m);
  for (auto &x : b) cin >> x;

  for (int l = min(n, m); l >= 1; --l) {
    unordered_map<int, vector<int>> mp;
    int tmp = 0;
    for (int i = 0; i < l; ++i) {
      tmp += b[i];
    }
    mp[tmp].push_back(0);

    for (int i = 1; i + l <= m; ++i) {
      tmp -= b[i-1];
      tmp += b[i+l-1];
      mp[tmp].push_back(i);
    }

    tmp = 0;
    for (int i = 0; i < n; ++i) {
      tmp += a[i];
      if (i >= l) tmp -= a[i-l];

      if (i >= l-1 && mp.count(tmp)) {
        for (int j : mp[tmp]) {
          int cnt = 0;
          unordered_map<int, int> mpCnt;
          for (int z = 0; z < l; ++z) {
            mpCnt[a[i-z]]++;
            if (mpCnt[a[i-z]] == 1) ++cnt;
            if (mpCnt[a[i-z]] == 0) --cnt;
            mpCnt[b[j + z]]--;
            if (mpCnt[b[j + z]] == -1) ++cnt;
            if (mpCnt[b[j + z]] == 0) --cnt;
          }
          if (cnt == 0) {
            cout << l << " " << i-l+2 << " " << j + 1;
            return 0;
          }
        }
      }
    }
  }

  cout << "0 -1 -1";

  return 0;
}