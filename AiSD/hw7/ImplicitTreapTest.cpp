#include <iostream>
#include "ImplicitTreap.h"

int main() {
  ImplicitTreap tr;

  std::vector<int> a = {5, 1, 3, 7, 9};
  tr.build(a);

  std::cout << "После билда:\n";
  for (int x : tr.to_vector()) std::cout << x << " ";
  std::cout << "\n";

  tr.insert(0, 100);
  tr.insert(3, 200);
  tr.insert(tr.size(), 300);

  std::cout << "После вставки:\n";
  for (int x : tr.to_vector()) std::cout << x << " ";
  std::cout << "\n";

  tr.erase(0);
  tr.erase(2);

  std::cout << "После удаления:\n";
  for (int x : tr.to_vector()) std::cout << x << " ";
  std::cout << "\n";

  tr.range_add(1, 3, 10);

  std::cout << "После прибавления 10 к элементам 1-3:\n";
  for (int x : tr.to_vector()) std::cout << x << " ";
  std::cout << "\n";

  std::cout << "Минимум и максимум на 0-2:\n";
  std::cout << tr.range_min(0, 2) << "\n";
  std::cout << tr.range_max(0, 2) << "\n";

  tr.insert(2, -50);
  tr.range_add(0, tr.size()-1, 5);

  std::cout << "После добавления -50 и добавления 5 на всем массиве:\n";
  for (int x : tr.to_vector()) std::cout << x << " ";
  std::cout << "\n";

  return 0;
}