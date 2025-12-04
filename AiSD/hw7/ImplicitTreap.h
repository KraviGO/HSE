#pragma once

#include <random>
#include <chrono>
#include <vector>
#include <cmath>

struct ImplicitTreap {
  struct Node {
    int val;
    int prio;
    int sz;
    long long mn, mx;
    long long lazy;
    Node *l, *r;
    Node(int v, int p) : val(v), prio(p), sz(1), mn(v), mx(v), lazy(0), l(nullptr), r(nullptr) {}
  };

  const long long INF = 1e18;
  Node* root = nullptr;
  std::mt19937 rnd;

  ImplicitTreap() {
    rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  }

  int sz(Node* t) const {
    return t ? t->sz : 0;
  }

  void apply_add(Node* t, long long delta) {
    if (!t) return;
    t->val += delta;
    t->mn += delta;
    t->mx += delta;
    t->lazy += delta;
  }

  void push(Node* t) {
    if (!t || t->lazy == 0) return;
    apply_add(t->l, t->lazy);
    apply_add(t->r, t->lazy);
    t->lazy = 0;
  }

  void upd(Node* t) {
    if (!t) return;
    t->sz = 1;
    t->mn = t->mx = t->val;
    if (t->l) {
      t->sz += t->l->sz;
      t->mn = std::min(t->mn, t->l->mn);
      t->mx = std::max(t->mx, t->l->mx);
    }
    if (t->r) {
      t->sz += t->r->sz;
      t->mn = std::min(t->mn, t->r->mn);
      t->mx = std::max(t->mx, t->r->mx);
    }
  }

  void split(Node* t, int k, Node*& a, Node*& b) {
    if (!t) {
      a = b = nullptr;
      return;
    }

    push(t);
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
      push(a);
      a->r = merge(a->r, b);
      upd(a);
      return a;
    } else {
      push(b);
      b->l = merge(a, b->l);
      upd(b);
      return b;
    }
  }

  Node* build(const std::vector<int>& v, int l, int r) {
    if (l > r) return nullptr;
    int m = (l + r) >> 1;
    Node* t = new Node(v[m], (int)rnd());
    t->l = build(v, l, m - 1);
    t->r = build(v, m + 1, r);
    upd(t);
    return t;
  }

  int size() const { return sz(root); }

  void build(const std::vector<int>& v) {
    root = build(v, 0, (int)v.size() - 1);
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

  void range_add(int l, int r, long long delta) {
    Node *a, *b, *c;
    split(root, l, a, b);
    split(b, r - l + 1, b, c);
    apply_add(b, delta);
    root = merge(merge(a, b), c);
  }

  long long range_min(int l, int r) {
    Node *a, *b, *c;
    split(root, l, a, b);
    split(b, r - l + 1, b, c);
    long long ans = b ? b->mn : INF;
    root = merge(merge(a, b), c);
    return ans;
  }

  long long range_max(int l, int r) {
    Node *a, *b, *c;
    split(root, l, a, b);
    split(b, r - l + 1, b, c);
    long long ans = b ? b->mx : -INF;
    root = merge(merge(a, b), c);
    return ans;
  }

  int get(int pos) {
    Node *a, *b, *c;
    split(root, pos, a, b);
    split(b, 1, b, c);
    int ans = b->val;
    root = merge(merge(a, b), c);
    return ans;
  }

  void dfs(Node* t, std::vector<int>& res) {
    if (!t) return;
    push(t);
    dfs(t->l, res);
    res.push_back(t->val);
    dfs(t->r, res);
  };

  std::vector<int> to_vector() {
    std::vector<int> res;
    dfs(root, res);
    return res;
  }
};
