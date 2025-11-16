#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void heapify(vector<ll> &a, int n, int i) {
  int max = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < n && a[left] > a[max]) {
    max = left;
  }
  if (right < n && a[right] > a[max]) {
    max = right;
  }

  if (max != i) {
    swap(a[i], a[max]);
    heapify(a, n, max);
  }
}

void buildMaxHeap(vector<ll> &a, int n) {
  for (int i = n / 2 - 1; i >= 0; --i) {
    heapify(a, n, i);
  }
}

void heapSort(vector<ll> &a, int n) {
  buildMaxHeap(a, n);

  for (int i = n - 1; i > 0; i--) {
    swap(a[0], a[i]);
    heapify(a, i, 0);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr); cout.tie(nullptr);

  int n; cin >> n;

  vector<ll> a(n);
  for (auto &x : a) cin >> x;

  heapSort(a, n);

  for (auto x : a) cout << x << " ";

  return 0;
}