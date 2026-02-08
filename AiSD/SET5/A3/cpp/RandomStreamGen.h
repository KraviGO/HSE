#pragma once
#include <random>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

class RandomStreamGen {
public:
  explicit RandomStreamGen(uint32_t seed = 123456u, int maxLen = 30)
      : rng_(seed), maxLen_(std::max(1, std::min(maxLen, 30))) {}

  std::string next() {
    static const char alphabet[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "-";
    static constexpr int A = (int)(sizeof(alphabet) - 1);

    std::uniform_int_distribution<int> lenDist(1, maxLen_);
    std::uniform_int_distribution<int> chDist(0, A - 1);

    int len = lenDist(rng_);
    std::string s;
    s.reserve((size_t)len);
    for (int i = 0; i < len; i++) s.push_back(alphabet[chDist(rng_)]);
    return s;
  }

  std::vector<std::string> generate(size_t n) {
    std::vector<std::string> out;
    out.reserve(n);
    for (size_t i = 0; i < n; i++) out.push_back(next());
    return out;
  }

  static std::vector<size_t> checkpoints(size_t n, const std::vector<double>& fractions) {
    std::vector<size_t> pts;
    pts.reserve(fractions.size());
    for (double f : fractions) {
      double ff = std::max(0.0, std::min(1.0, f));
      size_t p = (size_t)std::max<size_t>(1, (size_t)std::llround(ff * (double)n));
      p = std::min(p, n);
      pts.push_back(p);
    }
    std::sort(pts.begin(), pts.end());
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
    return pts;
  }

private:
  std::mt19937 rng_;
  int maxLen_;
};