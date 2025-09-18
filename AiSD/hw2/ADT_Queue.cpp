#include <vector>
#include <stdexcept>
#include <algorithm>

template <class T = int>
class Stack {
public:
  Stack() : sz(0) {}

  bool empty() const {
    return sz == 0;
  }

  size_t size() const {
    return sz;
  }

  void push(const T& value) {
    data.push_back(value);
    if (minData.empty()) {
      minData.push_back(value);
    } else {
      minData.push_back(std::min(value, minData.back()));
    }
    ++sz;
  }

  void pop() {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    data.pop_back();
    minData.pop_back();
    --sz;
  }

  T& top() {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    return data.back();
  }

  const T& top() const {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    return data.back();
  }

  T& bottom() {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    return data.front();
  }

  const T& bottom() const {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    return data.front();
  }

  T currentMin() const {
    if (empty()) {
      throw std::out_of_range("Stack is empty");
    }
    return minData.back();
  }

private:
  std::vector<T> data;
  std::vector<T> minData;
  size_t sz;
};


template <class T = int>
class Queue {
public:
  Queue() : sz(0) {}

  bool empty() const {
    return sz == 0;
  }

  size_t size() const {
    return sz;
  }

  void push(T elem) {
    pushStack_.push(elem);
    ++sz;
  }

  T pop() {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    moveIfNeeded();
    T val = popStack_.top();
    popStack_.pop();
    --sz;
    return val;
  }

  T& front() {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    moveIfNeeded();
    return popStack_.top();
  }

  int& back() {
    if (!pushStack_.empty()) {
      return pushStack_.top();
    } else if (!popStack_.empty()) {
      return popStack_.bottom();
    } else {
      throw std::out_of_range("Queue is empty");
    }
  }

  int curentMin() {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    if (pushStack_.empty()) {
      return popStack_.currentMin();
    }
    if (popStack_.empty()) {
      return pushStack_.currentMin();
    }

    return std::min(pushStack_.currentMin(), popStack_.currentMin());
  }

private:
  void moveIfNeeded() {
    if (popStack_.empty()) {
      while (!pushStack_.empty()) {
        int val = pushStack_.top();
        pushStack_.pop();
        popStack_.push(val);
      }
    }
  }

  Stack<int> pushStack_;
  Stack<int> popStack_;
  size_t sz;
};

#include <iostream>

int main() {
  Queue<int> q;

  std::cout << "=== Test 1: базовые операции ===\n";
  q.push(5);
  q.push(3);
  q.push(7);
  std::cout << q.curentMin() << '\n';   // ожидаем 3
  std::cout << q.front() << '\n';       // ожидаем 5
  std::cout << q.back() << '\n';        // ожидаем 7
  std::cout << "size=" << q.size() << "\n\n";

  std::cout << "=== Test 2: pop + проверка front/back/min ===\n";
  q.pop();        // снимаем 5
  std::cout << q.front() << '\n';       // ожидаем 3
  std::cout << q.back() << '\n';        // ожидаем 7
  std::cout << q.curentMin() << '\n';   // ожидаем 3
  std::cout << "size=" << q.size() << "\n\n";

  std::cout << "=== Test 3: добиваем до переноса и дальше ===\n";
  q.pop();         // снимаем 3
  std::cout << q.front() << '\n';       // должна сработать перегонка и показать 7
  std::cout << q.back() << '\n';        // когда pushStack пуст, back = bottom(popStack)
  std::cout << q.curentMin() << '\n';   // единственный элемент 7
  std::cout << "size=" << q.size() << "\n\n";

  std::cout << "=== Test 4: смешанные вставки (включая отрицательные) ===\n";
  q.push(10);
  q.push(-4);
  q.push(2);
  std::cout << q.curentMin() << '\n';   // ожидаем -4
  std::cout << q.front() << '\n';       // 7
  std::cout << q.back() << '\n';        // 2
  std::cout << "size=" << q.size() << "\n\n";

  std::cout << "=== Test 5: поэтапные pops до пустоты ===\n";
  while (!q.empty()) {
    q.pop();
    if (!q.empty()) {
      try { std::cout << q.curentMin() << "\n"; } catch(...) {}
      try { std::cout << q.front() << "\n"; } catch(...) {}
      try { std::cout << q.back() << "\n"; } catch(...) {}
    }
    std::cout << "size=" << q.size() << "\n";
  }
  std::cout << "\n";

  std::cout << "=== Test 6: вызовы на пустой очереди (ожидаем исключения) ===\n";
  try {
    q.front();
  } catch (const std::exception& e) {
    std::cout << "front() EXC: " << e.what() << "\n";
  }

  try {
    q.back();
  } catch (const std::exception& e) {
    std::cout << "back()  EXC: " << e.what() << "\n";
  }

  try {
    q.pop();
  }   catch (const std::exception& e) {
    std::cout << "pop()   EXC: " << e.what() << "\n";
  }

  try {
    q.curentMin();
  } catch (const std::exception& e) {
    std::cout << "min()   EXC: " << e.what() << "\n";
  }

  std::cout << "\n=== Test 7: повторное наполнение и проверка back после переноса ===\n";
  q.push(100);
  q.push(50);
  q.push(200);
  std::cout << q.back() << '\n';        // 200
  std::cout << q.front() << '\n';       // 100
  q.pop();                              // убрали 100
  q.pop();                              // убрали 50 -> теперь в popStack только 200
  std::cout << q.back() << '\n';        // pushStack пуст -> back должен показать 200 через bottom(popStack)
  std::cout << q.curentMin() << '\n';   // 200
  std::cout << "size=" << q.size() << "\n";

  std::cout << "\n=== Done ===\n";

  return 0;
}