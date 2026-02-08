#include "bits/stdc++.h"
using namespace std;

struct Treap {
  struct Node {
    int val;
    int prio;
    int sz;
    long long s;
    Node *l, *r;
    Node(int v, int p) : val(v), prio(p), sz(1), s(v), l(nullptr), r(nullptr) {}
  };

  Node* root = nullptr;
  std::mt19937 rnd;

  Treap() {
    rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  }

  int sz(Node* t) const {
    return t ? t->sz : 0;
  }

  void upd(Node* t) {
    if (!t) return;
    t->sz = 1;
    t->s = t->val;
    if (t->l) {
      t->sz += t->l->sz;
      t->s += t->l->s;
    }
    if (t->r) {
      t->sz += t->r->sz;
      t->s += t->r->s;
    }
  }

  void split(Node* t, int k, Node*& a, Node*& b) {
    if (!t) {
      a = b = nullptr;
      return;
    }

    int left_sz = sz(t->l);
    if (k <= left_sz) {
      split(t->l, k, a, t->l);
      b = t;
    } else {
      split(t->r, k - left_sz - 1, t->r, b);
      a = t;
    }

    upd(t);
  }

  Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;
    if (a->prio > b->prio) {
      a->r = merge(a->r, b);
      upd(a);
      return a;
    } else {
      b->l = merge(a, b->l);
      upd(b);
      return b;
    }
  }

  int size() const { return sz(root); }

  void build(const std::vector<int>& v) {
    root = nullptr;
    for (int x : v) {
      root = merge(root, new Node(x, (int)rnd()));
    }
  }

  void insert(int pos, int v) {
    Node *a, *b;
    split(root, pos, a, b);
    Node* n = new Node(v, (int)rnd());
    root = merge(merge(a, n), b);
  }

  void erase(int pos) {
    Node *a, *b, *c;
    split(root, pos, a, b);
    split(b, 1, b, c);
    root = merge(a, c);
    delete b;
  }

  long long sum(int l, int r) {
    Node *a, *b, *c;
    split(root, l, a, b);
    split(b, r - l + 1, b, c);
    long long ans = b ? b->s : 0;
    root = merge(merge(a, b), c);
    return ans;
  }
};

void solve(int n, int m) {
  vector<int> a0(n / 2 + n % 2), a1(n / 2);
  for (int i = 0; i < n; ++i) {
    if (i % 2) cin >> a1[i / 2];
    else cin >> a0[i / 2];
  }

  Treap t0, t1;
  t0.build(a0);
  t1.build(a1);

  for (int i = 0; i < m; ++i) {
    int cmd, l, r;
    cin >> cmd >> l >> r;
    --l, --r;

    int l0 = l / 2 + l % 2, l1 = l / 2;
    int r0 = r / 2, r1 = r / 2 - ((r+1) % 2);

    if (cmd == 1) {
      Treap::Node *a0, *b0, *c0;
      Treap::Node *a1, *b1, *c1;

      t0.split(t0.root, l0, a0, b0);
      t0.split(b0, r0 - l0 + 1, b0, c0);

      t1.split(t1.root, l1, a1, b1);
      t1.split(b1, r1 - l1 + 1, b1, c1);

      t0.root = t0.merge(t0.merge(a0, b1), c0);
      t1.root = t1.merge(t1.merge(a1, b0), c1);
    } else {
      cout << t0.sum(l0, r0) + t1.sum(l1, r1) << '\n';
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr); cout.tie(nullptr);

  int n, m;
  cin >> n >> m;

  int cnt = 0;
  while (n != 0 && m != 0) {
    cout << "Swapper " << ++cnt << ":\n";
    solve(n, m);
    cout << '\n';
    cin >> n >> m;
  }
}

