#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const ll k_INF = 5e18;

struct Point {
    ll x, y;
};

bool comp_x(const Point& p1, const Point& p2) {
    if (p1.x == p2.x) {
        return p1.y < p2.y;
    }
    return p1.x < p2.x;
}

bool comp_y(const Point& p1, const Point& p2) {
    return p1.y < p2.y;
}

ll dist(const Point& p1, const Point& p2) {
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}

ll easyCalc(vector<Point>& a, int l, int r) {
    
    ll res = k_INF;
    for (int i = l; i < r; ++i) {
        for (int j = i + 1; j < r; ++j) {
            res = min(res, dist(a[i], a[j]));
        }
    }
    
    sort(a.begin() + l, a.begin() + r, comp_y);
    
    return res;
}

ll calc(vector<Point>& a, int l, int r) {
    int n = a.size();
    
    if (r - l <= 3) {
        return easyCalc(a, l, r);
    }
    
    int m = (l + r) / 2;
    ll mx = a[m].x;
    
    ll d1 = calc(a, l, m);
    ll d2 = calc(a, m, r);
    ll d = min(d1, d2);
    
    inplace_merge(a.begin() + l, a.begin() + m, a.begin() + r, comp_y);
    
    vector<Point> strip;
    for (int i = l; i < r; ++i) {
        if ((a[i].x - mx) * (a[i].x - mx) < d) {
            strip.push_back(a[i]);
        }
    }

    for (int i = 0; i < strip.size(); ++i) {
        for (int j = i + 1; j < strip.size(); ++j) {
            if ((strip[j].y - strip[i].y) * (strip[j].y - strip[i].y) >= d) {
                break;
            }
            d = min(d, dist(strip[i], strip[j]));
        }
    }
    
    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    vector<Point> a;
    Point p;
    
    while (cin >> p.x >> p.y) {
        a.push_back(p);
    }
    
    sort(a.begin(), a.end(), comp_x);
    
    cout << static_cast<int>(sqrt(calc(a, 0, a.size())));
    
    return 0;
}