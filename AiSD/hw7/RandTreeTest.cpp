#include "RandTree.h"
#include <iostream>

int main() {
  RandTree tree;

  std::vector<int> a = {10, 2, 1, 8, 5, 15};
  for (auto x : a) {
    tree.insert(x);
  }

  for (auto x : tree.ordered()) std::cout << x << " ";
  std::cout << "\n";

  tree.erase(2);
  tree.erase(10);

  for (auto x : tree.ordered()) std::cout << x << " ";
  std::cout << "\n";

  tree.insert(-1);
  tree.insert(25);
  tree.insert(9);

  for (auto x : tree.ordered()) std::cout << x << " ";
  std::cout << "\n";
}