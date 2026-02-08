#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <unordered_set>
#include <vector>

#include "HyperLogLogPlusTables.h"

namespace hll {

class HyperLogLogPlus {
public:
  explicit HyperLogLogPlus(int p, int p0 = 25)
      : p_(p),
        p0_(p0),
        m_(1u << p_),
        m0_(1u << p0_),
        dense_(false),
        regs_(m_, 0) {
    if (p_ < 4) p_ = 4;
    if (p_ > 18) p_ = 18;
    if (p0_ <= p_) p0_ = p_ + 1;
    if (p0_ > 25) p0_ = 25;
    m_ = 1u << p_;
    m0_ = 1u << p0_;
    regs_.assign(m_, 0);
  }

  int precision() const { return p_; }
  int sparse_precision() const { return p0_; }
  bool is_dense() const { return dense_; }

  void clear() {
    dense_ = false;
    std::fill(regs_.begin(), regs_.end(), 0);
    sparse_list_.clear();
    tmp_set_.clear();
  }

  // Добавляет 64-битный хеш.
  void add(uint64_t x) {
    if (!dense_) {
      uint32_t k = encode_sparse(x);
      tmp_set_.insert(k);

      if (tmp_set_.size() >= kTmpSetFlush) {
        flush_tmp_set();
        maybe_to_dense();
      }
    } else {
      update_dense(x);
    }
  }

  double estimate() {
    if (!dense_) {
      // В разреженном режиме считаем через `linear_counting`: m0 и число
      // уникальных idx0.
      flush_tmp_set();
      const uint32_t distinct_idx0 = (uint32_t)sparse_list_.size();
      const double V0 = double(m0_ - distinct_idx0);
      if (V0 <= 0.0) return double(distinct_idx0);
      return linear_counting(double(m0_), V0);
    }

    const double E = raw_estimate_dense();

    // Поправка смещения для p=4..18 и E <= 5m.
    double Eb = E;
    if (E <= 5.0 * double(m_) && p_ >= 4 && p_ <= 18) {
      Eb = E - estimate_bias_knn(E);
    }

    // Выбираем между `linear_counting` и оценкой с поправкой по табличному
    // порогу.
    const uint32_t thr = threshold_for_p(p_);
    const uint32_t V = count_zero_registers();

    if (V > 0) {
      const double EL = linear_counting(double(m_), double(V));
      if (thr > 0 && EL <= double(thr)) return EL;
    }

    return Eb;
  }

private:
  int p_;
  int p0_;
  uint32_t m_;
  uint32_t m0_;
  bool dense_;

  std::vector<uint8_t> regs_;

  // Разреженная часть: отсортированный список + временное хеш-множество для
  // быстрых вставок.
  std::vector<uint32_t> sparse_list_;
  std::unordered_set<uint32_t> tmp_set_;

  static constexpr size_t kTmpSetFlush = 4096;

private:
  static inline double alpha_m(uint32_t m) {
    if (m == 16) return 0.673;
    if (m == 32) return 0.697;
    if (m == 64) return 0.709;
    return 0.7213 / (1.0 + 1.079 / double(m));
  }

  static inline double linear_counting(double m, double V) {
    return m * std::log(m / V);
  }

  static inline uint32_t idx_p(uint64_t x, int p) {
    return uint32_t(x >> (64 - p));
  }

  static inline uint8_t clz64_nonzero(uint64_t w) {
#if __cplusplus >= 202002L
    return (uint8_t)std::countl_zero(w);
#else
    uint8_t lz = 0;
    uint64_t mask = (1ULL << 63);
    while ((w & mask) == 0) {
      lz++;
      mask >>= 1;
    }
    return lz;
#endif
  }

  // Считаем rho по хвосту после p бит индекса: rho(w) = число нулей в начале
  // + 1.
  static inline uint8_t rho64(uint64_t x, int p) {
    const int q = 64 - p;
    uint64_t w = (x << p);
    if (w == 0) return (uint8_t)(q + 1);
    return (uint8_t)(clz64_nonzero(w) + 1);
  }

  // Число ведущих нулей для фиксированной длины битовой строки (параметр
  // `width` <= 32), для средних битов.
  static inline uint8_t lz_fixed(uint32_t v, int width) {
    // v не ноль; смотрим только `width` бит, начиная со старшего.
    int lz = 0;
    uint32_t mask = 1u << 31;
    while ((v & mask) == 0) {
      lz++;
      mask >>= 1;
    }
    // v лежит в младших битах, поэтому сдвигаем к старшему биту 32-битного
    // числа.
    int shift = 32 - width;
    uint32_t vv = v << shift;

    lz = 0;
    uint32_t mask2 = 1u << 31;
    while ((vv & mask2) == 0) {
      lz++;
      mask2 >>= 1;
    }

    return (uint8_t)lz;
  }

  // Кодирование разреженного режима HLL++
  uint32_t encode_sparse(uint64_t x) const {
    const uint32_t idx0 = idx_p(x, p0_);

    const int mid_len = p0_ - p_;
    const uint32_t mid_mask =
        (mid_len >= 32) ? 0xFFFFFFFFu : ((1u << mid_len) - 1u);
    const uint32_t mid =
        idx0 & mid_mask;  // это биты x63-p .. x64-p0 внутри idx0

    if (mid == 0) {
      uint8_t r0 = rho64(x, p0_);
      if (r0 > 63) r0 = 63;
      // [idx0:25][r0:6][1]
      return (idx0 << 7) | (uint32_t(r0) << 1) | 1u;
    } else {
      // [idx0:25][0]
      return (idx0 << 1);
    }
  }

  static inline uint32_t decode_idx0(uint32_t k) {
    if ((k & 1u) == 0u) return (k >> 1);
    return (k >> 7);
  }

  static inline uint8_t decode_r0(uint32_t k) {
    return (uint8_t)((k >> 1) & 0x3Fu);
  }

  // По idx0 и, если есть, r0 восстанавливаем rho(w) для точности p.
  uint8_t decode_rho_for_p(uint32_t k) const {
    const uint32_t idx0 = decode_idx0(k);
    const int mid_len = p0_ - p_;
    const uint32_t mid_mask =
        (mid_len >= 32) ? 0xFFFFFFFFu : ((1u << mid_len) - 1u);
    const uint32_t mid = idx0 & mid_mask;

    if (mid != 0) {
      // Первая единица уже в средних битах, поэтому rho(w) берём только из них.
      // rho = число нулей в начале `mid` при длине `mid_len` + 1
      uint8_t lz = lz_fixed(mid, mid_len);
      return (uint8_t)(lz + 1);
    } else {
      // Если mid весь нулевой, берём сохранённый rho(w') и добавляем (p0 - p).
      const uint8_t r0 = decode_r0(k);
      uint32_t r = uint32_t(r0) + uint32_t(mid_len);
      if (r > 255) r = 255;
      return (uint8_t)r;
    }
  }

  void flush_tmp_set() {
    if (tmp_set_.empty()) return;

    std::vector<uint32_t> batch;
    batch.reserve(tmp_set_.size());
    for (uint32_t v : tmp_set_) batch.push_back(v);
    tmp_set_.clear();

    std::sort(batch.begin(), batch.end());
    // Сливаем в `sparse_list_`, потом объединяем по idx0 (оставляем
    // максимальный r0).
    sparse_list_ = merge_sorted(sparse_list_, batch);
    sparse_list_ = reduce_by_idx0_keep_max_r0(sparse_list_);
  }

  static std::vector<uint32_t> merge_sorted(const std::vector<uint32_t> &a,
                                            const std::vector<uint32_t> &b) {
    std::vector<uint32_t> out;
    out.reserve(a.size() + b.size());
    size_t i = 0, j = 0;
    while (i < a.size() || j < b.size()) {
      uint32_t va = (i < a.size()) ? a[i] : 0xFFFFFFFFu;
      uint32_t vb = (j < b.size()) ? b[j] : 0xFFFFFFFFu;
      if (va <= vb) {
        out.push_back(va);
        i++;
      } else {
        out.push_back(vb);
        j++;
      }
    }
    return out;
  }

  static std::vector<uint32_t> reduce_by_idx0_keep_max_r0(
      const std::vector<uint32_t> &sorted) {
    if (sorted.empty()) return {};

    std::vector<uint32_t> out;
    out.reserve(sorted.size());

    uint32_t cur_idx0 = decode_idx0(sorted[0]);
    bool cur_has_r0 = (sorted[0] & 1u) != 0u;
    uint8_t cur_r0 = cur_has_r0 ? decode_r0(sorted[0]) : 0;

    auto emit = [&](uint32_t idx0, bool has_r0, uint8_t r0) {
      if (!has_r0)
        out.push_back(idx0 << 1);
      else
        out.push_back((idx0 << 7) | (uint32_t(r0) << 1) | 1u);
    };

    for (size_t i = 1; i < sorted.size(); ++i) {
      uint32_t k = sorted[i];
      uint32_t idx0 = decode_idx0(k);

      if (idx0 != cur_idx0) {
        emit(cur_idx0, cur_has_r0, cur_r0);
        cur_idx0 = idx0;
        cur_has_r0 = (k & 1u) != 0u;
        cur_r0 = cur_has_r0 ? decode_r0(k) : 0;
      } else {
        if ((k & 1u) != 0u) {
          uint8_t r0 = decode_r0(k);
          if (!cur_has_r0 || r0 > cur_r0) {
            cur_has_r0 = true;
            cur_r0 = r0;
          }
        }
        // Если у k нет r0, новой информации для этого же idx0 он не даёт.
      }
    }
    emit(cur_idx0, cur_has_r0, cur_r0);

    return out;
  }

  void maybe_to_dense() {
    // Сначала сбрасываем буфер, чтобы память считалась корректно.
    flush_tmp_set();

    // Память в плотном режиме (регистры uint8): примерно m байт.
    const size_t dense_bytes = size_t(m_);

    // Память в разреженном режиме: 4 байта на запись (`std::vector<uint32_t>`),
    // без учёта накладных расходов.
    const size_t sparse_bytes = 4ull * sparse_list_.size();

    if (sparse_bytes >= dense_bytes) {
      to_dense();
    }
  }

  void to_dense() {
    std::fill(regs_.begin(), regs_.end(), 0);

    // Переносим каждую разреженную запись в обновление плотного регистра.
    for (uint32_t k : sparse_list_) {
      uint32_t idx0 = decode_idx0(k);
      uint32_t idx = idx0 >> (p0_ - p_);
      uint8_t r = decode_rho_for_p(k);
      if (r > regs_[idx]) regs_[idx] = r;
    }

    sparse_list_.clear();
    tmp_set_.clear();
    dense_ = true;
  }

  // ---------- обновление и оценка в плотном режиме ----------
  void update_dense(uint64_t x) {
    const uint32_t idx = idx_p(x, p_);
    const uint8_t r = rho64(x, p_);
    if (r > regs_[idx]) regs_[idx] = r;
  }

  uint32_t count_zero_registers() const {
    uint32_t V = 0;
    for (uint8_t r : regs_)
      if (r == 0) V++;
    return V;
  }

  double raw_estimate_dense() const {
    const double a = alpha_m(m_);
    double z = 0.0;
    for (uint8_t r : regs_) {
      z += std::ldexp(1.0, -int(r));
    }
    return a * double(m_) * double(m_) / z;
  }

  // Поправка смещения: берём среднее по 6 ближайшим точкам из таблиц.
  double estimate_bias_knn(double E) const {
    if (p_ < 4 || p_ > 18) return 0.0;
    const BiasTable &T = kBiasTables[p_ - 4];
    const int n = T.n;
    if (n <= 0) return 0.0;

    // Ищем 6 ближайших точек по модулю расстояния.
    constexpr int k = 6;
    struct Pair {
      double d;
      double b;
    };
    Pair best[k];
    for (int i = 0; i < k; ++i) best[i] = {1e300, 0.0};

    for (int i = 0; i < n; ++i) {
      double d = std::abs(T.raw[i] - E);
      // Ставим в список ближайших, если нашли точку ближе.
      int worst = 0;
      for (int j = 1; j < k; ++j)
        if (best[j].d > best[worst].d) worst = j;
      if (d < best[worst].d) best[worst] = {d, T.bias[i]};
    }

    double sum = 0.0;
    int cnt = 0;
    for (int i = 0; i < k; ++i) {
      if (best[i].d < 1e299) {
        sum += best[i].b;
        cnt++;
      }
    }
    return (cnt > 0) ? (sum / double(cnt)) : 0.0;
  }
};

}  // namespace hll