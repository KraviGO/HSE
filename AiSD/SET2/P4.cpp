#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const int k_EPS = 1000;

bool check(const vector<pair<int, int>>& a, const ld& L, ld d) {
    int n = a.size();
    
    ld rt = 0;
    ld rs = 0;
    int rcur = 0;
    
    for (int i = 0; i < n; ++i) {
        if (d >= a[i].first) {
            ++rcur;
            d -= a[i].first;
            rs += a[i].first * a[i].second;
        } else {
            rt = d;
            rs += a[i].second * rt;
            break;
        }
    }
    
    if (rcur >= n) {
        return 1;
    }
    
    if (rs < L) {
        return 0;
    }
    
    ld lt = 0;
    ld ls = 0;
    int lcur = 0;
    
    while (rcur < n) {
        ld ldelta = a[lcur].first - lt;
        ld rdelta = a[rcur].first - rt;
        
        if (ldelta < rdelta) {
            rt += ldelta;
            rs += a[rcur].second * ldelta;
            
            lt = 0;
            ls += a[lcur].second * ldelta;
            ++lcur;
        }
        else if (ldelta > rdelta) {
            lt += rdelta;
            ls += a[lcur].second * rdelta;
            
            rt = 0;
            rs += a[rcur].second * rdelta;
            ++rcur;
        }
        else {
            lt = 0;
            ls += a[lcur].second * ldelta;
            ++lcur;
            
            rt = 0;
            rs += a[rcur].second * rdelta;
            ++rcur;
        }
        
        if (rs - ls < L) {
            return 0;
        }
    }
    
    return 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    ld L; cin >> L;
    int n; cin >> n;
    
    vector<pair<int, int>> a(n);
    for (auto& [t, v] : a) {
        cin >> t >> v;
    }
    
    ld l = 0, r = 1e7;
    for (int i = 0; i < k_EPS; ++i) {
        ld m = (l + r) / 2;
        bool c = check(a, L, m);
        (c ? r : l) = m;
    }
    
    cout << r;
    
    return 0;
}