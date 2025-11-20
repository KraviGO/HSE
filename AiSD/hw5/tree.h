#pragma once

template<typename T>
struct Node {
  T data{};
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;

  explicit Node(const T& value) : data(value) {}
};

template<typename T>
class Tree {
public:
  Tree() : root(nullptr) {}

  void insert(const T& value) {
    root = insert(root, value);
  }

private:
  Node<T>* insert(Node<T>* node, const T& value) {
    if (!node) {
      return new Node<T>(value);
    }

    if (value < node->data) {
      node->left = insert(node->left, value);
      node->left->parent = node;
    }
    else if (value > node->data) {
      node->right = insert(node->right, value);
      node->right->parent = node;
    }

    return node;
  }

public:
  struct Iterator {
    using pointer = Node<T>*;
    using reference = Node<T>&;

    explicit Iterator(pointer p = nullptr) : ptr(p) {}

    reference operator*() { return *ptr; }
    pointer operator->() { return ptr; }

    Iterator& operator++() {
      if (!ptr) {
        return *this;
      }

      if (ptr->right) {
        ptr = ptr->right;
        while (ptr->left) {
          ptr = ptr->left;
        }
      }
      else {
        auto p = ptr->parent;
        while (p && ptr == p->right) {
          ptr = p;
          p = p->parent;
        }
        ptr = p;
      }

      return *this;
    }

    Iterator& operator--() {
      if (!ptr) {
        return *this;
      }

      if (ptr->left) {
        ptr = ptr->left;
        while (ptr->right) {
          ptr = ptr->right;
        }
      } else {
        auto p = ptr->parent;
        while (p && ptr == p->left) {
          ptr = p;
          p = p->parent;
        }
        ptr = p;
      }
      return *this;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.ptr == b.ptr;
    }

    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.ptr != b.ptr;
    }

    pointer ptr;
  };

  Iterator begin() const {
    Node<T>* cur = root;
    if (!cur) {
      return Iterator();
    }
    while (cur->left) {
      cur = cur->left;
    }
    return Iterator(cur);
  }

  Iterator end() const {
    return Iterator(nullptr);
  }

  Iterator rbegin() const {
    Node<T>* cur = root;
    if (!cur) {
      return Iterator();
    }
    while (cur->right) {
      cur = cur->right;
    }
    return Iterator(cur);
  }

private:
  Node<T>* root;
};