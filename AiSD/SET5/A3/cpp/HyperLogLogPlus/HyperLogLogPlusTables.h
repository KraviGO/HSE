#pragma once
#include <cstdint>

namespace hll {

struct BiasTable {
  const double *raw;
  const double *bias;
  int n;
};

// Таблицы поправки смещения для p = 4..18 (индекс 0 соответствует p=4).
extern const BiasTable kBiasTables[15];

// Эмпирические пороги из приложения (p=4..18). Если p вне диапазона, вернётся 0.
uint32_t threshold_for_p(int p);

}