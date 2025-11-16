#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>

auto now = std::chrono::steady_clock::now();
std::mt19937 rnd(now.time_since_epoch().count());

const int kPOINTS = 1e6;


// =========== Структуры, Circle, Rectangle ===========
struct Point {
  long double x, y;
  Point() : x(0), y(0) {}
  Point(long double x, long double y) : x(x), y(y) {}
};

struct Circle {
  Point p;
  long double r;
  Circle() : p(), r(0) {}
  Circle(Point p, long double r) : p(p), r(r) {}
};

struct Rectangle {
  Point leftDown, rightUp;
  Rectangle() : leftDown(), rightUp() {}
};

Point readPoint() {
  Point p;
  std::cin >> p.x >> p.y;
  return p;
}

Circle readCircle() {
  Circle c;
  c.p = readPoint();
  std::cin >> c.r;
  return c;
}


// =========== Генерация прямоугольных областей ===========
Rectangle getBigRec(Circle c1, Circle c2, Circle c3) {
  Rectangle rec;

  long double rmax = std::max({c1.r, c2.r, c3.r});

  rec.leftDown = Point(
      std::min({c1.p.x, c2.p.x, c3.p.x}) - rmax,
      std::min({c1.p.y, c2.p.y, c3.p.y}) - rmax
      );

  rec.rightUp = Point(
      std::max({c1.p.x, c2.p.x, c3.p.x}) + rmax,
      std::max({c1.p.y, c2.p.y, c3.p.y}) + rmax
      );

  return rec;
}

Rectangle getSmolRec(Circle c1, Circle c2, Circle c3) {
  Rectangle rec;

  rec.leftDown = Point(
      std::max({c1.p.x - c1.r, c2.p.x - c2.r, c3.p.x - c3.r}),
      std::max({c1.p.y - c1.r, c2.p.y - c2.r, c3.p.y - c3.r})
      );

  rec.rightUp = Point(
      std::min({c1.p.x + c1.r, c2.p.x + c2.r, c3.p.x + c3.r}),
      std::min({c1.p.y + c1.r, c2.p.y + c2.r, c3.p.y + c3.r})
  );

  return rec;
}


// =========== Алгоритм вычисления площади ===========
bool inCircle(Point p, Circle c) {
  return sqrt(pow(p.x - c.p.x, 2) + pow(p.y - c.p.y, 2)) <= c.r;
}

long double calc(Circle c1, Circle c2, Circle c3, Rectangle rec, int points = kPOINTS) {
  Point p1 = rec.leftDown;
  Point p2 = rec.rightUp;
  long double s = (p2.x - p1.x) * (p2.y - p1.y);

  std::uniform_real_distribution<long double> rndX(p1.x, p2.x);
  std::uniform_real_distribution<long double> rndY(p1.y, p2.y);

  int cnt = 0;

  for (int i = 0; i < points; ++i) {
    Point p;
    p.x = rndX(rnd);
    p.y = rndY(rnd);

    if (inCircle(p, c1) && inCircle(p, c2) && inCircle(p, c3)) {
      ++cnt;
    }
  }

  return s * cnt / points;
}


// =========== Основная программа ===========
int main() {
  Circle c1 = {Point(1, 1), 1};
  Circle c2 = {Point(1.5, 2), std::sqrt(5) / 2};
  Circle c3 = {Point(2, 1.5), std::sqrt(5) / 2};

  // Точная формула площади из условия.
  long double exactS = 0.25 * M_PI + 1.25 * std::asin(0.8) - 1;

  Rectangle bigRec = getBigRec(c1, c2, c3);
  Rectangle smolRec = getSmolRec(c1, c2, c3);

  // Потоки вывода в файлы.
  std::ofstream fout_big("../SET3/A1/big_area_results.csv");
  std::ofstream fout_small("../SET3/A1/small_area_results.csv");

  // Заголовки CSV.
  fout_big << "N,S_approx,RelError\n";
  fout_small << "N,S_approx,RelError\n";

  // Широкая области.
  for (int points = 100; points < 1e5; points += 500) {
    long double s = calc(c1, c2, c3, bigRec, points);
    long double rel_err = std::fabsl(s - exactS) / exactS;
    fout_big << points << "," << std::setprecision(12) << s << "," << rel_err << "\n";
  }

  // Узкая области.
  for (int points = 100; points < 1e5; points += 500) {
    long double s = calc(c1, c2, c3, smolRec, points);
    long double rel_err = std::fabsl(s - exactS) / exactS;
    fout_small << points << "," << std::setprecision(12) << s << "," << rel_err << "\n";
  }

  return 0;
}