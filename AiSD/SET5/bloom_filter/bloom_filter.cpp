#include "bloom_filter.h"

BloomFilter::BloomFilter(size_t hashes, size_t bits)
    : _hashes(hashes), _bits(bits) {
  _filter.resize(_bits, false);
}

void BloomFilter::add(const std::string &value) {
  _raw_set.insert(value);

  for (size_t i = 0; i < _hashes; ++i) {
    size_t hash = _get_hash(i, value);
    _filter[hash % _bits] = true;
  }
}

bool BloomFilter::verify(const std::string &value) {
  cnt++;

  bool exists_in_filter = true;

  for (size_t i = 0; i < _hashes; ++i) {
    size_t hash = _get_hash(i, value);
    if (!_filter[hash % _bits]) {
      exists_in_filter = false;
      break;
    }
  }

  if (exists_in_filter) {
    if (_raw_set.find(value) == _raw_set.end()) {
      false_positive_cnt++;
    }
  }

  return exists_in_filter;
}

double BloomFilter::getFPRate() const {
  if (cnt == 0) {
    return 0.0;
  }
  return static_cast<double>(false_positive_cnt) / static_cast<double>(cnt);
}

size_t BloomFilter::numberOfHashFunctions() const {
  return _hashes;
}

size_t BloomFilter::numberOfBits() const {
  return _bits;
}

inline size_t BloomFilter::_get_hash(int n, const std::string &value) const {
  if (n == 0) {
    return std::hash<std::string>{}(value);
  }
  return std::hash<std::string>{}(value + std::to_string(n));
}