#include "rbtree.h"
#include <stack>

Node::Node(int key)
    : key(key),
      height(0),
      size(1),
      left(nullptr),
      right(nullptr),
      parent(nullptr),
      color(Color::RED) {}

RBTree::RBTree() : root(nullptr) {}

RBTree::RBTree(std::initializer_list<int> list) : root(nullptr) {
  for (auto x : list) {
    insert(x);
  }
}

RBTree::~RBTree() {
  if (!root) {
    return;
  }

  std::stack<Node*> st;
  st.push(root);
  while (!st.empty()) {
    Node* node = st.top();
    st.pop();

    if (node->left) {
      st.push(node->left);
    }
    if (node->right) {
      st.push(node->right);
    }

    delete node;
  }

  root = nullptr;
}

int sizeNode(Node* node) {
  return (node ? node->size : 0);
}

int heightNode(Node* node) {
  return (node ? node->height : 0);
}

bool isRed(Node* node) {
  return node && node->color == Color::RED;
}

void upd(Node* node) {
  if (!node) {
    return;
  }

  node->size = sizeNode(node->left) + sizeNode(node->right) + 1;
  int lh = heightNode(node->left);
  int rh = heightNode(node->right);
  node->height = std::max(lh, rh) + (isRed(node) ? 0 : 1);
}

Node* rotateLeft(Node* node) {
  Node* tmp = node->right;
  node->right = tmp->left;
  if (tmp->left) {
    tmp->left->parent = node;
  }

  tmp->parent = node->parent;
  tmp->left = node;
  node->parent = tmp;

  tmp->color = node->color;
  node->color = Color::RED;

  upd(node);
  upd(tmp);

  return tmp;
}

Node* rotateRight(Node* node) {
  Node* tmp = node->left;
  node->left = tmp->right;
  if (tmp->right) {
    tmp->right->parent = node;
  }

  tmp->parent = node->parent;
  tmp->right = node;
  node->parent = tmp;

  tmp->color = node->color;
  node->color = Color::RED;

  upd(node);
  upd(tmp);

  return tmp;
}

void flipColors(Node* node) {
  node->color = Color::RED;
  node->left->color = Color::BLACK;
  node->right->color = Color::BLACK;

  upd(node->left);
  upd(node->right);
  upd(node);
}

Node* insertRec(Node* node, int key, Node* parent) {
  if (!node) {
    Node* tmp = new Node(key);
    tmp->parent = parent;
    return tmp;
  }
  if (key < node->key) {
    node->left = insertRec(node->left, key, node);
    node->left->parent = node;
  }
  else if (key > node->key) {
    node->right = insertRec(node->right, key, node);
    node->right->parent = node;
  }
  else {
    return node;
  }

  if (isRed(node->right) && !isRed(node->left)) {
    node = rotateLeft(node);
  }
  if (isRed(node->left) && isRed(node->left->left)) {
    node = rotateRight(node);
  }
  if (isRed(node->left) && isRed(node->right)) {
    flipColors(node);
  }

  upd(node);

  return node;
}

void RBTree::insert(int key) {
  root = insertRec(root, key, nullptr);

  if (root) {
    root->color = Color::BLACK;
    root->parent = nullptr;
    upd(root);
  }
}

int *RBTree::find(int key) {
  Node* cur = root;
  while (cur) {
    if (cur->key == key) {
      return &cur->key;
    }
    else if (key < cur->key) {
      cur = cur->left;
    }
    else {
      cur = cur->right;
    }
  }
  return nullptr;
}

int RBTree::size() const {
  return sizeNode(root);
}

int *RBTree::lowerBound(int key) {
  Node* cur = root;
  Node* ans = nullptr;

  while (cur) {
    if (cur->key >= key) {
      ans = cur;
      cur = cur->left;
    }
    else {
      cur = cur->right;
    }
  }

  return (ans ? &ans->key : nullptr);
}

bool RBTree::empty() const {
  return !size();
}

int RBTree::height() const {
  return heightNode(root);
}