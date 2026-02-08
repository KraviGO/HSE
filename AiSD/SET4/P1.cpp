#include <bits/stdc++.h>

using namespace std;

struct BinaryTree {
  struct Node {
    Node* left;
    Node* right;
    int value;

    explicit Node() : left(nullptr), right(nullptr), value(0) {}

    explicit Node(int value) : left(nullptr), right(nullptr), value(value) {}

    Node(Node* left, Node* right, int value)
        : left(left), right(right), value(value) {}
  };

  explicit BinaryTree() : root(nullptr) {}
  explicit BinaryTree(Node* root) : root(root) {}

  void insert(int value) {
    root = insert(root, value);
  }

  int height(Node* node) {
    if (!node) {
      return 0;
    }

    int lh = height(node->left);
    int rh = height(node->right);

    if (lh == INF || rh == INF || abs(rh - lh) > 1) {
      return INF;
    }

    return max(lh, rh) + 1;
  }

  bool isAVL() {
    return height(root) != INF;
  }

private:
  Node* insert(Node* node, int value) {
    if (!node) {
      return root = new Node(value);
    }

    if (value <= node->value) {
      node->left = insert(node->left, value);
    } else {
      node->right = insert(node->right, value);
    }

    return node;
  }

private:
  const int INF = 2e9;
  Node* root;
};

int main() {
  BinaryTree tree = BinaryTree();

  int x; cin >> x;
  while (x != 0) {
    tree.insert(x);
    cin >> x;
  }

  cout << (tree.isAVL() ? "YES" : "NO");
}