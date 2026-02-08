#include "splay-tree.h"
#include <algorithm>

Node::Node(int key, Node *parent)
    : key(key),
      left(nullptr),
      right(nullptr),
      parent(parent) {}

SplayTree::SplayTree() : root(nullptr) {}

SplayTree::~SplayTree() {
  clear(root);
}

void SplayTree::clear(Node *node) {
  if (!node) {
    return;
  }

  clear(node->left);
  clear(node->right);

  delete node;
}

void SplayTree::insert(int key) {
  if (!root) {
    root = new Node(key, nullptr);
    return;
  }

  Node *curr = root;
  while (curr) {
    if (key == curr->key) {
      return;
    }

    if (key < curr->key) {
      if (curr->left) {
        curr = curr->left;
      }
      else {
        curr->left = new Node(key, curr);
        return;
      }
    }
    else {
      if (curr->right) {
        curr = curr->right;
      }
      else {
        curr->right = new Node(key, curr);
        return;
      }
    }
  }
}

Node *SplayTree::find(int key) const {
  Node *curr = root;

  while (curr) {
    if (key == curr->key) {
      return curr;
    }
    if (key < curr->key) {
      curr = curr->left;
    }
    else {
      curr = curr->right;
    }
  }

  return nullptr;
}

void SplayTree::rotateLeft(Node *node) {
  Node *rightChild = node->right;

  if (!rightChild) {
    return;
  }

  node->right = rightChild->left;
  if (rightChild->left) {
    rightChild->left->parent = node;
  }

  rightChild->parent = node->parent;
  if (!node->parent) {
    root = rightChild;
  }
  else if (node == node->parent->left) {
    node->parent->left = rightChild;
  }
  else {
    node->parent->right = rightChild;
  }

  rightChild->left = node;
  node->parent = rightChild;
}

void SplayTree::rotateRight(Node *node) {
  Node *leftChild = node->left;

  if (!leftChild) {
    return;
  }

  node->left = leftChild->right;
  if (leftChild->right) {
    leftChild->right->parent = node;
  }

  leftChild->parent = node->parent;
  if (!node->parent) {
    root = leftChild;
  }
  else if (node == node->parent->left) {
    node->parent->left = leftChild;
  }
  else {
    node->parent->right = leftChild;
  }

  leftChild->right = node;
  node->parent = leftChild;
}

int SplayTree::splay(Node *node) {
  int rotations = 0;

  while (node->parent) {
    Node *p = node->parent;
    Node *g = p->parent;

    if (!g) {
      if (p->left == node) {
        rotateRight(p);
      }
      else {
        rotateLeft(p);
      }
      rotations += 1;
    }
    else {
      bool p_is_left = (g->left == p);
      bool node_is_left = (p->left == node);

      if (p_is_left == node_is_left) {
        if (p_is_left) {
          rotateRight(g);
          rotateRight(p);
        }
        else {
          rotateLeft(g);
          rotateLeft(p);
        }
        rotations += 2;
      }
      else {
        if (p_is_left) {
          rotateLeft(p);
          rotateRight(g);
        } else {
          rotateRight(p);
          rotateLeft(g);
        }
        rotations += 1;
      }
    }
  }

  return rotations;
}

int SplayTree::getHeight() const {
  return (root ? getHeight(root) : -1);
}

int SplayTree::getHeight(Node *node) const {
  if (!node) {
    return -1;
  }
  return 1 + std::max(getHeight(node->left), getHeight(node->right));
}