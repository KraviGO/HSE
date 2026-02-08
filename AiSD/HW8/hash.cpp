#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class HashCuckooTable {
private:
  struct Entry {
    int key;
    bool is_occupied = false;
  };

  std::vector<Entry> table1;
  std::vector<Entry> table2;
  size_t capacity;
  int max_displacements;

  // h1(key) = key mod 11
  size_t h1(int key) const {
    return static_cast<size_t>(key) % capacity;
  }

  // h2(key) = (key / 11) mod 11
  size_t h2(int key) const {
    return (static_cast<size_t>(key) / capacity) % capacity;
  }

  void rehash() {
    std::vector<Entry> old_t1 = table1;
    std::vector<Entry> old_t2 = table2;

    capacity = capacity * 2 + 1;
    table1.assign(capacity, {0, false});
    table2.assign(capacity, {0, false});
    max_displacements = static_cast<int>(log2(capacity) * 2) + 5;

    for (const auto& entry : old_t1) {
      if (entry.is_occupied) insert(entry.key);
    }
    for (const auto& entry : old_t2) {
      if (entry.is_occupied) insert(entry.key);
    }
  }

public:
  explicit HashCuckooTable(size_t size = 11) : capacity(size) {
    table1.resize(capacity);
    table2.resize(capacity);
    max_displacements = static_cast<int>(capacity * 1.5);
  }

  bool find(int key) const {
    if (table1[h1(key)].is_occupied && table1[h1(key)].key == key) return true;
    if (table2[h2(key)].is_occupied && table2[h2(key)].key == key) return true;
    return false;
  }

  void insert(int key) {
    if (find(key)) return;

    int current_key = key;
    for (int i = 0; i < max_displacements; ++i) {
      size_t pos1 = h1(current_key);
      if (!table1[pos1].is_occupied) {
        table1[pos1] = {current_key, true};
        return;
      }

      std::swap(current_key, table1[pos1].key);

      size_t pos2 = h2(current_key);
      if (!table2[pos2].is_occupied) {
        table2[pos2] = {current_key, true};
        return;
      }

      std::swap(current_key, table2[pos2].key);
    }

    rehash();
    insert(current_key);
  }

  void print() const {
    std::cout << "T1: ";
    for (const auto& e : table1) {
      if (e.is_occupied) {
        std::cout << e.key << " ";
      } else {
        std::cout << "- ";
      }
    }

    std::cout << "\nT2: ";
    for (const auto& e : table2) {
      if (e.is_occupied) {
        std::cout << e.key << " ";
      } else {
        std::cout << "- ";
      }
    }
    std::cout << "\n\n";
  }
};

int main() {
  HashCuckooTable ht(11);

  int keys[] = {20, 50, 53, 75, 100, 67, 105, 3, 36, 39, 6};

  for (int k : keys) {
    std::cout << "Вставка " << k << ": " << '\n';
    ht.insert(k);
    ht.print();
  }

  return 0;
}