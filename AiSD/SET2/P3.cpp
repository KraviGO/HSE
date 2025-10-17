#include <bits/stdc++.h>
using namespace std;

const int k_POROG = 64;

typedef long long ll;
using Matrix = vector<vector<ll>>;

Matrix sum(const Matrix& a, const Matrix& b) {
    int n = a.size();
    Matrix res(n, vector<ll> (n));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            res[i][j] = a[i][j] + b[i][j];
        }
    }
    
    return res;
}

Matrix sub(const Matrix& a, const Matrix& b) {
    int n = a.size();
    Matrix res(n, vector<ll> (n));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            res[i][j] = a[i][j] - b[i][j];
        }
    }
    
    return res;
}

Matrix mult(const Matrix& a, const Matrix& b) {
    int n = a.size();
    Matrix res(n, vector<ll> (n));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    
    return res;
}

void split(const Matrix& a, Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22) {
    int n = a.size() / 2;
    
    for (int i = 0; i < n; ++i) {
        copy(a[i].begin(), a[i].begin() + n, a11[i].begin());
        copy(a[i].begin() + n, a[i].end(), a12[i].begin());
        copy(a[i + n].begin(), a[i + n].begin() + n, a21[i].begin());
        copy(a[i + n].begin() + n, a[i + n].end(), a22[i].begin());
    }
}

Matrix join(const Matrix& a11, const Matrix& a12, const Matrix& a21, const Matrix& a22) {
    int n = a11.size();
    int sz = n * 2;
    Matrix a(sz, vector<ll> (sz));
    
    for (int i = 0; i < n; ++i) {
        copy(a11[i].begin(), a11[i].end(), a[i].begin());
        copy(a12[i].begin(), a12[i].end(), a[i].begin() + n);
        copy(a21[i].begin(), a21[i].end(), a[i + n].begin());
        copy(a22[i].begin(), a22[i].end(), a[i + n].begin() + n);
    }
    
    return a;
}

Matrix fastMult(const Matrix& a, const Matrix& b, int n) {
    if (n <= k_POROG) {
        return mult(a, b);
    }
    
    int sz = n / 2;
    
    Matrix a11(sz, vector<ll> (sz));
    Matrix a12(sz, vector<ll> (sz));
    Matrix a21(sz, vector<ll> (sz));
    Matrix a22(sz, vector<ll> (sz));
    
    Matrix b11(sz, vector<ll> (sz));
    Matrix b12(sz, vector<ll> (sz));
    Matrix b21(sz, vector<ll> (sz));
    Matrix b22(sz, vector<ll> (sz));
    
    split(a, a11, a12, a21, a22);
    split(b, b11, b12, b21, b22);
    
    Matrix p1 = fastMult(sum(a11, a22), sum(b11, b22), sz);
    Matrix p2 = fastMult(sum(a21, a22), b11, sz);
    Matrix p3 = fastMult(a11, sub(b12, b22), sz);
    Matrix p4 = fastMult(a22, sub(b21, b11), sz);
    Matrix p5 = fastMult(sum(a11, a12), b22, sz);
    Matrix p6 = fastMult(sub(a21, a11), sum(b11, b12), sz);
    Matrix p7 = fastMult(sub(a12, a22), sum(b21, b22), sz);
    
    Matrix c11 = sum(sum(p1, p4), sub(p7, p5));
    Matrix c12 = sum(p3, p5);
    Matrix c21 = sum(p2, p4);
    Matrix c22 = sum(sub(p1, p2), sum(p3, p6));
    
    return join(c11, c12, c21, c22);
}

void readMatrix(Matrix& a) {
    for (auto& row : a) {
        for (ll& val : row) {
            cin >> val;
        }
    }
}

void printMatrix(const Matrix& a) {
    for (auto& row : a) {
        for (ll val : row) {
            cout << val << " ";
        }
        cout << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    int n; cin >> n;
    
    Matrix a(n, vector<ll> (n));
    Matrix b(n, vector<ll> (n));
    
    readMatrix(a);
    readMatrix(b);
    
    Matrix c = fastMult(a, b, n);
    
    printMatrix(c);
    
    return 0;
}