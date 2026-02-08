#include "avl_tree.h"

Node::Node(int value)
    : height(1), left(nullptr), right(nullptr), value(value) {}

Node::~Node() {
  delete left;
  delete right;
}

AVLTree::AVLTree() : root_(nullptr), size_(0) {}

AVLTree::AVLTree(int value) {
  root_ = new Node(value);
  size_ = 1;
}

int AVLTree::getHeight() {
  return (root_ ? root_->height : 0);
}

void AVLTree::insert(int value) {
  root_ = insertNode(root_, value);
}

void AVLTree::erase(int value) {
  root_ = removeNode(root_, value);
}

int *AVLTree::find(int value) {
  Node* tmp = findNode(root_, value);
  return tmp ? &tmp->value : nullptr;
}

int *AVLTree::traversal() {
  if (empty()) return nullptr;
  int* array = new int[size_];
  int index = 0;
  traversalInternal(root_, array, &index);
  return array;
}

int *AVLTree::lowerBound(int value) {
  Node* tmp = lowerBoundInternal(root_, value);
  return (tmp ? &tmp->value : nullptr);
}

bool AVLTree::empty() {
  return size_ == 0;
}

Node *AVLTree::getRoot() {
  return root_;
}

int AVLTree::getSize() {
  return size_;
}

AVLTree::~AVLTree() {
  delete root_;
}

int AVLTree::getNodeHeight(Node *node) {
  return (node ? node->height : 0);
}

int AVLTree::balanceFactor(Node *node) {
  return getNodeHeight(node->right) - getNodeHeight(node->left);
}

void AVLTree::balanceHeight(Node *node) {
  int lh = (node->left ? node->left->height : 0);
  int rh = (node->right ? node->right->height : 0);
  node->height = 1 + (lh < rh ? rh : lh);
}

Node *AVLTree::rotateRight(Node *node) {
  Node* tmp = node->left;
  node->left = tmp->right;
  tmp->right = node;

  balanceHeight(tmp->right);
  balanceHeight(tmp);

  return tmp;
}

Node *AVLTree::rotateLeft(Node *node) {
  Node* tmp = node->right;
  node->right = tmp->left;
  tmp->left = node;

  balanceHeight(tmp->left);
  balanceHeight(tmp);

  return tmp;
}

Node *AVLTree::balanceNode(Node *node) {
  balanceHeight(node);

  if (balanceFactor(node) == 2) {
    if (balanceFactor(node->right) < 0) {
      node->right = rotateRight(node->right);
    }
    return rotateLeft(node);
  }

  if (balanceFactor(node) == -2) {
    if (balanceFactor(node->left) > 0) {
      node->left = rotateLeft(node->left);
    }
    return rotateRight(node);
  }

  return node;
}

Node *AVLTree::insertNode(Node *node, int value) {
  if (!node) {
    ++size_;
    return new Node(value);
  }

  if (value == node->value) {
    return node;
  } else if (value < node->value) {
    node->left = insertNode(node->left, value);
  } else {
    node->right = insertNode(node->right, value);
  }

  return balanceNode(node);
}

Node *AVLTree::findMinNode(Node *node) {
  return (node->left ? findMinNode(node->left) : node);
}

Node *AVLTree::removeMinNode(Node *node) {
  if (!node->left) {
    Node* tmp = node->right;
    return tmp;
  }
  node->left = removeMinNode(node->left);
  return balanceNode(node);
}

Node *AVLTree::removeNode(Node *node, int value) {
  if (!node) return nullptr;

  if (value < node->value) {
    node->left = removeNode(node->left, value);
  } else if (value > node->value) {
    node->right = removeNode(node->right, value);
  } else {
    size_--;
    Node *l = node->left;
    Node *r = node->right;
    node->left = node->right = nullptr;
    delete node;

    if (!r) return l;
    Node *min = findMinNode(r);
    min->right = removeMinNode(r);
    min->left = l;
    return balanceNode(min);
  }

  return balanceNode(node);
}

Node *AVLTree::findNode(Node *node, int value) {
  if (!node) return nullptr;

  if (node->value == value) {
    return node;
  } else if (value < node->value) {
    return findNode(node->left, value);
  }
  return findNode(node->right, value);
}

void AVLTree::traversalInternal(Node *node, int *array, int *index) {
  if (!node) return;
  traversalInternal(node->left, array, index);
  array[(*index)++] = node->value;
  traversalInternal(node->right, array, index);
}

Node *AVLTree::lowerBoundInternal(Node *current, int value) const {
  Node *res = nullptr;
  while (current) {
    if (current->value >= value) {
      res = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }
  return res;
}
