#include <bits/stdc++.h>
using namespace std;

const int k_POROG = 50;

void clearZero(vector<int>& a) {
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
}

vector<int> nativeProd(vector<int> a, vector<int> b) {
    vector<int> res(a.size() + b.size(), 0);
    
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            res[i + j] += a[i] * b[j];
            if (res[i + j] > 9) {
                res[i + j + 1] += res[i + j] / 10;
                res[i + j] %= 10;
            }
        }
    }
    
    clearZero(res);
    
    return res;
}

vector<int> nativeSub(vector<int> a, vector<int> b) {
    int sz = a.size();
    b.resize(sz, 0);
    for (int i = 0; i < sz; ++i) {
        a[i] -= b[i];
        if (a[i] < 0) {
            if (i + 1 < sz) {
                a[i + 1]--;
            }
            a[i] += 10;
        }
    }
    
    clearZero(a);
    
    return a;
}

vector<int> nativeSum(vector<int> a, vector<int> b) {
    int sz = max(a.size(), b.size()) + 1;
    
    vector<int> res(sz, 0);
    
    a.resize(sz, 0);
    b.resize(sz, 0);
    
    for (int i = 0; i < sz - 1; ++i) {
        res[i] += a[i] + b[i];
        
        if (res[i] > 9) {
            res[i + 1] += res[i] / 10;
            res[i] %= 10;
        }
    }
    
    clearZero(res);
    
    return res;
}

void cutNumber(vector<int>& a, vector<int>& a1, vector<int>& a2) {
    a1.clear();
    a2.clear();
    for (int i = 0; i < a.size(); ++i) {
        if (i < a.size() / 2) {
            a1.push_back(a[i]);
        } else {
            a2.push_back(a[i]);
        }
    }
}

vector<int> karatsubaProd(vector<int> a, vector<int> b) {
    int sz = max(a.size(), b.size());
    
    a.resize(sz, 0);
    b.resize(sz, 0);
    
    if (sz < k_POROG) {
        return nativeProd(a, b);
    }
    
    vector<int> a1, a2, b1, b2;
    cutNumber(a, a1, a2);
    cutNumber(b, b1, b2);
    
    vector<int> c0 = karatsubaProd(a2, b2);
    vector<int> c2 = karatsubaProd(a1, b1);
    vector<int> c1 = karatsubaProd(nativeSum(a1, a2), nativeSum(b1, b2));
    c1 = nativeSub(c1, c0);
    c1 = nativeSub(c1, c2);
    
    int m = sz / 2;
    
    reverse(c1.begin(), c1.end());
    c1.resize(c1.size() + m);
    reverse(c1.begin(), c1.end());
    
    reverse(c0.begin(), c0.end());
    c0.resize(c0.size() + 2 * m);
    reverse(c0.begin(), c0.end());
    
    vector<int> res = nativeSum(c0, c1);
    res = nativeSum(res, c2);
    
    return res;
}

vector<int> strToVec(string s) {
    vector<int> res(s.size());
    for (int i = 0; i < s.size(); ++i) {
        res[i] = s[i] - '0';
    }
    reverse(res.begin(), res.end());
    
    return res;
}

int main() {
    string s1, s2;
    cin >> s1 >> s2;
    
    vector<int> n1 = strToVec(s1);
    vector<int> n2 = strToVec(s2);
    
    vector<int> c = karatsubaProd(n1, n2);
    
    reverse(c.begin(), c.end());
    
    for (auto x : c) {
        cout << x;
    }
}