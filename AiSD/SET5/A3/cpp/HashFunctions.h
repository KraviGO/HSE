#pragma once
#include <cstdint>
#include <cstring>
#include <string>

namespace hashfn {

inline uint32_t rotl32(uint32_t x, int8_t r) {
  return (x << r) | (x >> (32 - r));
}

inline uint32_t fmix32(uint32_t h) {
  h ^= h >> 16;
  h *= 0x85ebca6bU;
  h ^= h >> 13;
  h *= 0xc2b2ae35U;
  h ^= h >> 16;
  return h;
}

inline uint32_t murmur3_32(const void *key, size_t len,
                           uint32_t seed = 0x9747b28cU) {
  const uint8_t *data = static_cast<const uint8_t *>(key);
  const size_t nblocks = len / 4;

  uint32_t h1 = seed;
  const uint32_t c1 = 0xcc9e2d51U;
  const uint32_t c2 = 0x1b873593U;

  const uint32_t *blocks = reinterpret_cast<const uint32_t *>(data);
  for (size_t i = 0; i < nblocks; i++) {
    uint32_t k1;
    std::memcpy(&k1, blocks + i, sizeof(uint32_t));

    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = rotl32(h1, 13);
    h1 = h1 * 5U + 0xe6546b64U;
  }

  const uint8_t *tail = data + nblocks * 4;
  uint32_t k1 = 0;

  switch (len & 3) {
  case 3:
    k1 ^= uint32_t(tail[2]) << 16;
    [[fallthrough]];
  case 2:
    k1 ^= uint32_t(tail[1]) << 8;
    [[fallthrough]];
  case 1:
    k1 ^= uint32_t(tail[0]);
    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
    break;
  default:
    break;
  }

  h1 ^= static_cast<uint32_t>(len);
  h1 = fmix32(h1);
  return h1;
}

inline uint32_t murmur3_32(const std::string &s, uint32_t seed = 0x9747b28cU) {
  return murmur3_32(s.data(), s.size(), seed);
}

inline uint64_t rotl64(uint64_t x, int8_t r) {
  return (x << r) | (x >> (64 - r));
}

inline uint64_t fmix64(uint64_t k) {
  k ^= k >> 33;
  k *= 0xff51afd7ed558ccdULL;
  k ^= k >> 33;
  k *= 0xc4ceb9fe1a85ec53ULL;
  k ^= k >> 33;
  return k;
}

inline uint64_t murmur3_64(const void *key, size_t len,
                           uint32_t seed = 0x9747b28cU) {
  const uint8_t *data = (const uint8_t *)key;
  const size_t nblocks = len / 16;

  uint64_t h1 = seed;
  uint64_t h2 = seed;

  const uint64_t c1 = 0x87c37b91114253d5ULL;
  const uint64_t c2 = 0x4cf5ad432745937fULL;

  for (size_t i = 0; i < nblocks; i++) {
    uint64_t k1, k2;
    std::memcpy(&k1, data + i * 16 + 0, 8);
    std::memcpy(&k2, data + i * 16 + 8, 8);

    k1 *= c1;
    k1 = rotl64(k1, 31);
    k1 *= c2;
    h1 ^= k1;
    h1 = rotl64(h1, 27);
    h1 += h2;
    h1 = h1 * 5 + 0x52dce729;

    k2 *= c2;
    k2 = rotl64(k2, 33);
    k2 *= c1;
    h2 ^= k2;
    h2 = rotl64(h2, 31);
    h2 += h1;
    h2 = h2 * 5 + 0x38495ab5;
  }

  const uint8_t *tail = data + nblocks * 16;
  uint64_t k1 = 0, k2 = 0;

  switch (len & 15) {
  case 15:
    k2 ^= uint64_t(tail[14]) << 48;
    [[fallthrough]];
  case 14:
    k2 ^= uint64_t(tail[13]) << 40;
    [[fallthrough]];
  case 13:
    k2 ^= uint64_t(tail[12]) << 32;
    [[fallthrough]];
  case 12:
    k2 ^= uint64_t(tail[11]) << 24;
    [[fallthrough]];
  case 11:
    k2 ^= uint64_t(tail[10]) << 16;
    [[fallthrough]];
  case 10:
    k2 ^= uint64_t(tail[9]) << 8;
    [[fallthrough]];
  case 9:
    k2 ^= uint64_t(tail[8]) << 0;
    k2 *= c2;
    k2 = rotl64(k2, 33);
    k2 *= c1;
    h2 ^= k2;
    [[fallthrough]];
  case 8:
    k1 ^= uint64_t(tail[7]) << 56;
    [[fallthrough]];
  case 7:
    k1 ^= uint64_t(tail[6]) << 48;
    [[fallthrough]];
  case 6:
    k1 ^= uint64_t(tail[5]) << 40;
    [[fallthrough]];
  case 5:
    k1 ^= uint64_t(tail[4]) << 32;
    [[fallthrough]];
  case 4:
    k1 ^= uint64_t(tail[3]) << 24;
    [[fallthrough]];
  case 3:
    k1 ^= uint64_t(tail[2]) << 16;
    [[fallthrough]];
  case 2:
    k1 ^= uint64_t(tail[1]) << 8;
    [[fallthrough]];
  case 1:
    k1 ^= uint64_t(tail[0]) << 0;
    k1 *= c1;
    k1 = rotl64(k1, 31);
    k1 *= c2;
    h1 ^= k1;
    break;
  default:
    break;
  }

  h1 ^= (uint64_t)len;
  h2 ^= (uint64_t)len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  return h1;
}

inline uint64_t murmur3_64(const std::string &s, uint32_t seed = 0x9747b28cU) {
  return murmur3_64(s.data(), s.size(), seed);
}
}  // namespace hashfn