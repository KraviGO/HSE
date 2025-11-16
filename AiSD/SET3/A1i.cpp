#include <bits/stdc++.h>
using namespace std;
typedef long double ld;

auto now = chrono::steady_clock::now();
mt19937 rnd(now.time_since_epoch().count());

bool inCircle(ld x, ld y, ld x0, ld y0, ld r) {
  return sqrt(pow(x - x0, 2) + pow(y - y0, 2)) <= r;
}

int main() {
  ld x1, y1, r1;
  cin >> x1 >> y1 >> r1;

  ld x2, y2, r2;
  cin >> x2 >> y2 >> r2;

  ld x3, y3, r3;
  cin >> x3 >> y3 >> r3;

  ld xmin = min({x1, x2, x3});
  ld xmax = max({x1, x2, x3});
  ld ymin = min({y1, y2, y3});
  ld ymax = max({y1, y2, y3});
  ld rmax = max({r1, r2, r3});

  int xl = int(xmin - rmax - 1);
  int xr = int(xmax + rmax + 1);
  int yd = int(ymin - rmax - 1);
  int yu = int(ymax + rmax + 1);

  ld s = (xr - xl) * (yu - yd);

  uniform_real_distribution<ld> rndX(xl, xr);
  uniform_real_distribution<ld> rndY(yd, yu);

  const int N = 1e6;
  int cnt = 0;

  for (int i = 0; i < N; ++i) {
    ld x = rndX(rnd);
    ld y = rndY(rnd);

    if (inCircle(x, y, x1, y1, r1) &&
        inCircle(x, y, x2, y2, r2) &&
        inCircle(x, y, x3, y3, r3))
    {
      ++cnt;
    }
  }

  cout << fixed << setprecision(5);
  cout << s * cnt / N;

  return 0;
}