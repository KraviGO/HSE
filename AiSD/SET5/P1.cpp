#include <vector>
#include <stdexcept>
#include <cstddef>

template <typename KeyType, typename ValueType, typename Func = std::hash<KeyType>>
class HashTable {
public:
  struct Node {
    KeyType key;
    ValueType value;
    Node* next;

    Node(KeyType k, ValueType v) : key(k), value(v), next(nullptr) {}
  };

  HashTable() : HashTable(100, 0.5) {}
  explicit HashTable(Func hasher) : HashTable(100, 0.5, hasher) {}

  HashTable(size_t capacity, double loadFactor, Func hasher = Func())
      : capacity_(capacity), loadFactor_(loadFactor), hasher_(hasher), size_(0)
  {
    if (loadFactor_ <= 0 || loadFactor_ > 1) {
      loadFactor_ = 0.5;
    }
    if (capacity_ == 0) {
      capacity_ = 100;
    }

    table_ = new Node*[capacity_]();
  }

  ~HashTable() {
    clear();
    delete[] table_;
  }

  size_t size() const {
    return size_;
  }

  size_t capacity() const {
    return capacity_;
  }

  void insert(const KeyType& key, const ValueType& value) {
    if (static_cast<double>(size_ + 1) / capacity_ > loadFactor_) {
      rehash();
    }

    size_t idx = hasher_(key) % capacity_;
    Node* current = table_[idx];

    if (!current) {
      table_[idx] = new Node(key, value);
      size_++;
      return;
    }

    Node* prev = nullptr;
    while (current) {
      if (current->key == key) {
        current->value = value;
        return;
      }
      prev = current;
      current = current->next;
    }

    prev->next = new Node(key, value);
    size_++;
  }

  void erase(const KeyType& key) {
    size_t idx = hasher_(key) % capacity_;
    Node* current = table_[idx];
    Node* prev = nullptr;

    while (current) {
      if (current->key == key) {
        if (prev) {
          prev->next = current->next;
        } else {
          table_[idx] = current->next;
        }
        delete current;
        size_--;
        return;
      }
      prev = current;
      current = current->next;
    }
  }

  ValueType* find(const KeyType& key) {
    size_t idx = hasher_(key) % capacity_;
    Node* current = table_[idx];
    while (current) {
      if (current->key == key) {
        return &(current->value);
      }
      current = current->next;
    }
    return nullptr;
  }

  Node& operator[](size_t index) {
    if (index >= capacity_) {
      throw std::out_of_range("Index out of range");
    }
    if (table_[index] == nullptr) {
      throw std::runtime_error("Bucket is empty");
    }
    return *table_[index];
  }

  Node at(size_t index) {
    if (index >= capacity_) {
      throw std::out_of_range("Index out of range");
    }
    if (table_[index] == nullptr) {
      throw std::runtime_error("Bucket is empty");
    }
    return *table_[index];
  }

private:
  size_t capacity_;
  double loadFactor_;
  Func hasher_;
  size_t size_;
  Node** table_;

  void clear() {
    for (size_t i = 0; i < capacity_; ++i) {
      Node* current = table_[i];
      while (current) {
        Node* temp = current;
        current = current->next;
        delete temp;
      }
      table_[i] = nullptr;
    }
    size_ = 0;
  }

  void rehash() {
    size_t oldCapacity = capacity_;
    Node** oldTable = table_;

    capacity_ *= 2;
    table_ = new Node*[capacity_]();
    size_ = 0;

    for (size_t i = 0; i < oldCapacity; ++i) {
      Node* current = oldTable[i];
      while (current) {
        Node* nextNode = current->next;
        current->next = nullptr;

        size_t newIdx = hasher_(current->key) % capacity_;

        if (table_[newIdx] == nullptr) {
          table_[newIdx] = current;
        } else {
          Node* tail = table_[newIdx];
          while (tail->next) {
            tail = tail->next;
          }
          tail->next = current;
        }

        size_++;
        current = nextNode;
      }
    }

    delete[] oldTable;
  }
};