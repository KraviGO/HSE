#include <bits/stdc++.h>
using namespace std;

struct Node {
  int left, right;
  int key;
  char color;
  int bh = 0;
};

bool isRB(int idNode, vector<Node>& nodes, int l = INT_MIN, int r = INT_MAX) {
  if (idNode == -1) return true;

  Node* node = &nodes[idNode];

  if (node->key >= r || node->key <= l) {
    return false;
  }

  if (node->color == 'R'
      && (
             (node->left != -1 && nodes[node->left].color == 'R') ||
             (node->right != -1 && nodes[node->right].color == 'R')
                 )
  )
  {
    return false;
  }

  if (!isRB(node->left, nodes, l, node->key) ||
      !isRB(node->right, nodes, node->key, r))
  {
    return false;
  }

  int lbh = (node->left != -1 ? nodes[node->left].bh : 1);
  int rbh = (node->right != -1 ? nodes[node->right].bh : 1);

  if (lbh != rbh) {
    return false;
  }

  node->bh = lbh + (node->color == 'B' ? 1 : 0);

  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr); cout.tie(nullptr);

  int n; cin >> n;

  if (n == 0) {
    cout << "NO\n";
    return 0;
  }

  int root;
  cin >> root;
  --root;

  vector<Node> nodes(n);

  for (int i = 0; i < n; i++) {
    int id, key;
    string left, right;
    char c;

    cin >> id >> key >> left >> right >> c;
    Node* node = &nodes[--id];

    node->left = (left == "null" ? -1 : stoi(left) - 1);
    node->right = (right == "null" ? -1 : stoi(right) - 1);
    node->key = key;
    node->color = c;
  }

  if (nodes[root].color == 'R') {
    cout << "NO\n";
    return 0;
  }

  cout << (isRB(root, nodes) ? "YES" : "NO");

  return 0;
}