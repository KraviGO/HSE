#include <stdexcept>
#include "list.h"

List::List() {
  head = tail = nullptr;
  _size = 0;
}

List::List(const List& other) {
  head = tail = nullptr;
  _size = 0;
  for (Node* cur = other.head; cur != nullptr; cur = cur->next) {
     if (_size == 0) {
       head = tail = new Node(cur->value);
     } else {
       tail = new Node(tail, nullptr, cur->value);
       tail->prev->next = tail;
     }
     ++_size;
   }
}

List::List(std::vector<int> array) {
  head = tail = nullptr;
  _size = 0;
  for (int val : array) {
    if (_size == 0) {
      head = tail = new Node(val);
    } else {
      tail = new Node(tail, nullptr, val);
      tail->prev->next = tail;
    }
    ++_size;
  }
}

List::~List() {
  Node* cur = head;
  while (cur != nullptr) {
    Node* next = cur->next;
    delete cur;
    cur = next;
  }
  head = tail = nullptr;
  _size = 0;
}

int List::front() {
  return head->value;
}


int List::back() {
  return tail->value;
}

void List::push_back(int value) {
  tail = new Node(tail, nullptr, value);
  if (tail->prev) {
    tail->prev->next = tail;
  } else {
    head = tail;
  }
  ++_size;
}

void List::push_front(int value) {
  head = new Node(nullptr, head, value);
  if (head->next) {
    head->next->prev = head;
  } else {
    tail = head;
  }
  ++_size;
}

void List::insert(Node* pos, int value) {
    if (pos == nullptr) {
      throw std::runtime_error("Incorrect position!");
    }

    bool flag = true;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
      if (cur == pos) {
        flag = false;
        break;
      }
    }

    if (flag) {
      throw std::runtime_error("Incorrect position!");
    }

    auto tmp = new Node(pos, pos->next, value);
    if (tmp->prev) {
      tmp->prev->next = tmp;
    } else {
      head = tmp;
    }
    if (tmp->next) {
      tmp->next->prev = tmp;
    } else {
      tail = tmp;
    }
    ++_size;
}

void List::pop_front() {
    if (_size == 0) {
      throw std::runtime_error("Deleting in empty list");
    }
    if (_size == 1) {
      delete head;
      head = tail = nullptr;
    } else {
      auto tmp = head;
      head = tmp->next;
      head->prev = nullptr;
      delete tmp;
    }
    --_size;
}

void List::pop_back() {
  if (_size == 0) {
    throw std::runtime_error("Deleting in empty list");
  }
  if (_size == 1) {
    delete tail;
    head = tail = nullptr;
  } else {
    auto tmp = tail;
    tail = tmp->prev;
    tail->next = nullptr;
    delete tmp;
  }
  --_size;
}

void List::erase(Node* pos) {
  if (pos == nullptr) {
    throw std::runtime_error("Incorrect position!");
  }

  bool flag = true;
  for (Node* cur = head; cur != nullptr; cur = cur->next) {
    if (cur == pos) {
      flag = false;
      break;
    }
  }

  if (flag) {
    throw std::runtime_error("Incorrect position!");
  }

  if (pos->prev == nullptr) {
    pop_front();
  } else if (pos->next == nullptr) {
    pop_back();
  } else {
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    delete pos;
    --_size;
  }
}

void List::clear() {
    throw std::runtime_error("Not implemented!");
}

void List::reverse() {
    throw std::runtime_error("Not implemented!");
}

void List::remove_duplicates() {
    throw std::runtime_error("Not implemented!");
}

void List::replace(int old_value, int new_value) {
    throw std::runtime_error("Not implemented!");
}

void List::merge(const List& other) {
    throw std::runtime_error("Not implemented!");
}

bool List::check_cycle() const {
//    throw std::runtime_error("Not implemented!");
    return false;
}

size_t List::size() const {
    return _size;
}

bool List::empty() const {
    return _size == 0;
}

void List::copy(const List& other) {
    throw std::runtime_error("Not implemented!");
}
