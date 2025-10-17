#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const ll k_INF = 5e18;

struct Point {
    ll x, y, z;
    int id;
};

bool comp_x(const Point& p1, const Point& p2) {
    if (p1.x == p2.x) {
        if (p1.y == p2.y) {
            return p1.z < p2.z;
        }
        return p1.y < p2.y;
    }
    return p1.x < p2.x;
}

bool comp_y(const Point& p1, const Point& p2) {
    if (p1.y == p2.y) {
        return p1.z < p2.z;
    }
    return p1.y < p2.y;
}

bool comp_z(const Point& p1, const Point& p2) {
    return p1.z < p2.z;
}

ll dist(const Point& p1, const Point& p2) {
    return (p2.x - p1.x) * (p2.x - p1.x) + 
           (p2.y - p1.y) * (p2.y - p1.y) + 
           (p2.z - p1.z) * (p2.z - p1.z);
}

pair<ll, pair<int, int>> easyCalc(vector<Point>& a, int l, int r) {
    ll res = k_INF;
    int idx1 = -1, idx2 = -1;
    
    for (int i = l; i < r; ++i) {
        for (int j = i + 1; j < r; ++j) {
            ll d = dist(a[i], a[j]);
            if (d < res) {
                res = d;
                idx1 = a[i].id;
                idx2 = a[j].id;
            }
        }
    }
    
    sort(a.begin() + l, a.begin() + r, comp_y);
    
    return {res, {min(idx1, idx2), max(idx1, idx2)}};
}

pair<ll, pair<int, int>> calc(vector<Point>& a, int l, int r) {
    int n = a.size();
    
    if (r - l <= 3) {
        return easyCalc(a, l, r);
    }
    
    int m = (l + r) / 2;
    ll mx = a[m].x;
    
    auto lr = calc(a, l, m);
    auto rr = calc(a, m, r);
    
    ll d = min(lr.first, rr.first);
    auto best = (lr.first < rr.first) ? lr.second : rr.second;
    
    inplace_merge(a.begin() + l, a.begin() + m, a.begin() + r, comp_y);
    
    vector<Point> strip;
    for (int i = l; i < r; ++i) {
        if ((a[i].x - mx) * (a[i].x - mx) < d) {
            strip.push_back(a[i]);
        }
    }

    sort(strip.begin(), strip.end(), comp_y);
    
    for (int i = 0; i < strip.size(); ++i) {
        for (int j = i + 1; j < strip.size(); ++j) {
            if ((strip[j].y - strip[i].y) * (strip[j].y - strip[i].y) >= d) {
                break;
            }
            if ((strip[j].z - strip[i].z) * (strip[j].z - strip[i].z) >= d) {
                break;
            }
            
            ll tmp = dist(strip[i], strip[j]);
            if (tmp < d) {
                d = tmp;
                best = {min(strip[i].id, strip[j].id), max(strip[i].id, strip[j].id)};
            }
        }
    }
    
    return {d, best};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    int n; cin >> n;
    
    vector<Point> a(n);
    Point p;
    int cnt = 0;
    
    for (auto& [x, y, z, id] : a) {
        cin >> x >> y >> z;
        id = ++cnt;
    }
    
    sort(a.begin(), a.end(), comp_x);
    
    auto res = calc(a, 0, a.size());
    
    cout << fixed << setprecision(10) << sqrt((ld)res.first) << "\n";
    cout << res.second.first << " " << res.second.second;
    
    return 0;
}