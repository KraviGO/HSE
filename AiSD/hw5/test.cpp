#include <iostream>
#include "tree.h"

class Tests {
public:

  static void runAll() {
    std::cout << "Запуск всех тестов...\n\n";

    testForwardIteration();
    testReverseIteration();
    testInterval();
    testPredecessorSuccessor();

    std::cout << "Тестирование завершено.\n";
  }

private:

  // Проверка условия.
  static bool check(bool cond, const std::string& msg) {
    std::cout << (cond ? "| OK | " : "| ERROR | ") << msg << "\n";
    return cond;
  }

  // Дерево для тестирования.
  static Tree<int> prepareTree() {
    Tree<int> t;
    int arr[] = {5,900,30,45,222,40,100,1,0,22,440};
    for (int x : arr) {
      t.insert(x);
    }
    return t;
  }

  // Обход по возрастанию.
  static void testForwardIteration() {
    std::cout << "Тест 1: Обход по возрастанию.\n";

    auto t = prepareTree();

    int expected[] = {0,1,5,22,30,40,45,100,222,440,900};
    int i = 0;
    bool ok = true;

    for (auto & it : t) {
      if (it.data != expected[i]) ok = false;
      i++;
    }

    check(ok, "Порядок значений при обходе соответствует ожидаемому.");
    std::cout << "\n";
  }

  // Обход по убыванию.
  static void testReverseIteration() {
    std::cout << "Тест 2: Обход по убыванию.\n";

    auto t = prepareTree();

    int expected[] = {900,440,222,100,45,40,30,22,5,1,0};
    int i = 0;
    bool ok = true;

    for (auto it = t.rbegin(); ; --it) {
      if (it->data != expected[i]) ok = false;
      i++;
      if (it == t.begin()) break;
    }

    check(ok, "Обход даёт значения по убыванию.");
    std::cout << "\n";
  }

  // Обход диапазона.
  static void testInterval() {
    std::cout << "Тест 3: Вывод значений от 30 до 100.\n";

    auto t = prepareTree();

    auto it1 = t.begin();
    while (it1 != t.end() && it1->data != 30) ++it1;

    auto it2 = t.begin();
    while (it2 != t.end() && it2->data != 100) ++it2;

    int expected[] = {30,40,45,100};
    int i = 0;
    bool ok = true;

    for (auto it = it1; it != it2; ++it) {
      if (it->data != expected[i]) {
        ok = false;
      }
      i++;
    }

    if (it2->data != expected[i]) {
      ok = false;
    }

    check(ok, "Диапазон значений от 30 до 100 выбран верно.");
    std::cout << "\n";
  }

  // Проверка поиска предыдущего и следующего элемент.
  static void testPredecessorSuccessor() {
    std::cout << "Тест 4: Поиск предыдущего и следующего элемента.\n";

    auto t = prepareTree();

    auto it = t.begin();
    while (it != t.end() && it->data != 40) ++it;

    auto before = it;
    auto after = it;

    --before;
    ++after;

    bool ok = (before->data == 30) && (after->data == 45);

    check(ok, "Предыдущий и следующий элементы относительно 40 найдены правильно.");
    std::cout << "\n";
  }
};

int main() {
  Tests::runAll();
}