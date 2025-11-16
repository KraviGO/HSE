#pragma once

#include <vector>
#include <random>
#include <algorithm>

class ArrayGenerator {
public:
  ArrayGenerator(
      int maxSize,
      int maxValue,
      int swapsForAlmost = 100,
      unsigned seed = std::random_device{}())
      : maxSize_(maxSize),
        maxValue_(maxValue),
        swapsForAlmost_(swapsForAlmost),
        rng_(seed)
  {
    // Заполнение случайными числами.
    baseRandom_.resize(maxSize_);

    std::uniform_int_distribution<int> rndInt(0, maxValue_);
    for (int i = 0; i < maxSize_; ++i) {
      baseRandom_[i] = rndInt(rng_);
    }

    // Отсортированный.
    baseSorted_ = baseRandom_;
    std::sort(baseSorted_.begin(), baseSorted_.end());

    // Отсортированный в обратном порядке.
    baseReverse_ = baseSorted_;
    std::reverse(baseReverse_.begin(), baseReverse_.end());
  }

  std::vector<int> getRandomPrefix(int n) {
    return std::vector<int>(baseRandom_.begin(), baseRandom_.begin() + n);
  }

  std::vector<int> getReversePrefix(int n) const {
    return std::vector<int>(baseReverse_.begin(), baseReverse_.begin() + n);
  }

  std::vector<int> getAlmostSortedPrefix(int n) {
    std::vector<int> v(baseSorted_.begin(), baseSorted_.begin() + n);

    if (swapsForAlmost_ <= 0) return v;

    std::uniform_int_distribution<int> rndId(0, n - 1);
    for (int i = 0; i < swapsForAlmost_; ++i) {
      int i1 = rndId(rng_);
      int i2 = rndId(rng_);
      std::swap(v[i1], v[i2]);
    }

    return v;
  }

private:
  int maxSize_;
  int maxValue_;
  int swapsForAlmost_;
  std::vector<int> baseRandom_;
  std::vector<int> baseSorted_;
  std::vector<int> baseReverse_;
  mutable std::mt19937 rng_;
};