#pragma once
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>

class HyperLogLog {
public:
  explicit HyperLogLog(uint8_t b = 10) : b_(b) {
    if (b_ < 4) b_ = 4;
    if (b_ > 16) b_ = 16;
    m_ = 1u << b_;
    regs_.assign(m_, 0);
  }

  void reset() { std::fill(regs_.begin(), regs_.end(), 0); }

  uint8_t b() const { return b_; }
  uint32_t m() const { return m_; }

  void addHash(uint32_t x) {
    uint32_t idx = x >> (32 - b_);

    uint32_t w = x << b_;

    uint8_t r = rho(w);
    if (r > regs_[idx]) regs_[idx] = r;
  }

  double estimate() const {
    const double alpha = alpha_m(m_);
    double sum = 0.0;
    uint32_t V = 0;

    for (uint8_t r : regs_) {
      sum += std::ldexp(1.0, -int(r));
      if (r == 0) V++;
    }

    double E = alpha * double(m_) * double(m_) / sum;

    if (E <= 2.5 * double(m_) && V > 0) {
      E = double(m_) * std::log(double(m_) / double(V));
    }

    const double two32 = 4294967296.0;
    if (E > (two32 / 30.0)) {
      E = -two32 * std::log(1.0 - E / two32);
    }

    return E;
  }

private:
  uint8_t b_;
  uint32_t m_;
  std::vector<uint8_t> regs_;

  static double alpha_m(uint32_t m) {
    if (m == 16) return 0.673;
    if (m == 32) return 0.697;
    if (m == 64) return 0.709;
    return 0.7213 / (1.0 + 1.079 / double(m));
  }

  uint8_t rho(uint32_t w) const {
    if (w == 0) {
      return uint8_t((32 - b_) + 1);
    }

    int lz = 0;
    uint32_t mask = 0x80000000u;
    while ((w & mask) == 0) {
      lz++;
      mask >>= 1;
    }

    int r = lz + 1;
    int rmax = (32 - b_) + 1;
    if (r > rmax) r = rmax;
    return (uint8_t)r;
  }
};