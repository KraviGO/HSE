#pragma once

#include <random>
#include <chrono>
#include <vector>

struct RandTree {
  struct Node {
    int key;
    int sz;
    Node *l, *r;

    explicit Node(int k) : key(k), sz(1), l(nullptr), r(nullptr) {}
  };

  Node* root = nullptr;
  std::mt19937 rnd;

  RandTree() {
    rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  }

  RandTree(Node* v) : root(v) {
    RandTree();
  }

  static int sz(Node* t) {
    return t ? t->sz : 0;
  }

  static void upd(Node* t) {
    if (!t) return;
    t->sz = 1 + sz(t->l) + sz(t->r);
  }

  Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;
    int rrnd = std::uniform_int_distribution<int>(1, sz(a) + sz(b))(rnd);
    if (rrnd <= sz(a)) {
      a->r = merge(a->r, b);
      upd(a);
      return a;
    } else {
      b->l = merge(a, b->l);
      upd(b);
      return b;
    }
  }

  void split(Node* t, int key, Node*& a, Node*& b) {
    if (!t) {
      a = b = nullptr;
      return;
    }

    if (t->key < key) {
      split(t->r, key, t->r, b);
      a = t;
    } else {
      split(t->l, key, a, t->l);
      b = t;
    }
    upd(t);
  }

  Node* insert(Node* t, int key) {
    if (!t) return new Node(key);
    int s = sz(t);
    if (std::uniform_int_distribution<int>(0, s)(rnd) == 0) {
      Node *a, *b;
      split(t, key, a, b);
      Node* n = new Node(key);
      n->l = a;
      n->r = b;
      upd(n);
      return n;
    }
    if (key < t->key) {
      t->l = insert(t->l, key);
    }
    else if (key > t->key) {
      t->r = insert(t->r, key);
    }
    upd(t);
    return t;
  }

  Node* erase(Node* t, int key) {
    if (!t) return nullptr;
    if (key == t->key) {
      Node* res = merge(t->l, t->r);
      delete t;
      return res;
    }
    if (key < t->key) {
      t->l = erase(t->l, key);
    }
    else {
      t->r = erase(t->r, key);
    }
    upd(t);
    return t;
  }

  void inorder(Node* t, std::vector<int>& out) const {
    if (!t) return;
    inorder(t->l, out);
    out.push_back(t->key);
    inorder(t->r, out);
  }

  void insert(int key) {
    root = insert(root, key);
  }

  void erase(int key) {
    root = erase(root, key);
  }

  std::vector<int> ordered() const {
    std::vector<int> v;
    inorder(root, v);
    return v;
  }
};