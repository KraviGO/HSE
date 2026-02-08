#include "btree.h"
#include <algorithm>

Node::Node(int t) : parent(nullptr), isLeaf(true), t(t) {}

void clear(Node* node) {
  if (!node) return;
  for (Node* ch : node->children) clear(ch);
  delete node;
}

int64_t sumLeaves(const Node* node) {
  if (!node) return 0;
  if (node->isLeaf) {
    int64_t s = 0;
    for (int k : node->key) s += k;
    return s;
  }
  int64_t s = 0;
  for (const Node* ch : node->children) s += sumLeaves(ch);
  return s;
}

void splitNode(Node* node, int t, Node*& root, int& nodeCounter) {
  Node* right = new Node(t);
  ++nodeCounter;

  right->isLeaf = node->isLeaf;
  right->parent = node->parent;

  int median = node->key[t];

  right->key.assign(node->key.begin() + (t + 1), node->key.end());

  node->key.resize(t);

  if (!node->isLeaf) {
    right->children.assign(node->children.begin() + (t + 1), node->children.end());
    node->children.resize(t + 1);
    for (Node* ch : right->children) ch->parent = right;
  }

  Node* parent = node->parent;
  if (!parent) {
    Node* newRoot = new Node(t);
    ++nodeCounter;

    newRoot->isLeaf = false;
    newRoot->key.push_back(median);
    newRoot->children.push_back(node);
    newRoot->children.push_back(right);

    node->parent = newRoot;
    right->parent = newRoot;
    root = newRoot;
    return;
  }

  auto it = std::lower_bound(parent->key.begin(), parent->key.end(), median);
  int idx = (int)std::distance(parent->key.begin(), it);

  parent->key.insert(it, median);
  parent->children.insert(parent->children.begin() + idx + 1, right);
  right->parent = parent;

  if ((int)parent->key.size() == 2 * t) {
    splitNode(parent, t, root, nodeCounter);
  }
}

BTree::BTree(int t) : root(nullptr), t_(t), size_(0) {}

BTree::~BTree() {
  clear(root);
}

void BTree::insert(int k) {
  if (!root) {
    root = new Node(t_);
    size_ = 1;
    root->key.push_back(k);
    return;
  }

  Node* cur = root;
  while (!cur->isLeaf) {
    auto it = std::lower_bound(cur->key.begin(), cur->key.end(), k);
    if (it != cur->key.end() && *it == k) return;

    int idx = (int)std::distance(cur->key.begin(), it);
    cur = cur->children[idx];
  }

  auto it = std::lower_bound(cur->key.begin(), cur->key.end(), k);
  if (it != cur->key.end() && *it == k) return;

  cur->key.insert(it, k);

  if ((int)cur->key.size() == 2 * t_) {
    splitNode(cur, t_, root, size_);
  }
}

size_t BTree::size() const {
  return (size_t)size_;
}

int64_t BTree::sum() const {
  return sumLeaves(root);
}