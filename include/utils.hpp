#pragma once
#include <cstdint>
#include <random>

using size_t = std::size_t;

// Given four consecutive little endian bytes, this function interprets them as
// 32 -bit unsigned integer
static inline uint32_t
from_le_bytes(const uint8_t* const bytes)
{
  return (static_cast<uint32_t>(bytes[3]) << 24) |
         (static_cast<uint32_t>(bytes[2]) << 16) |
         (static_cast<uint32_t>(bytes[1]) << 8) |
         (static_cast<uint32_t>(bytes[0]) << 0);
}

// Given a 32 -bit unsigned interger, this function interprets it as four
// consecutive little endian bytes
static inline void
to_le_bytes(const uint32_t word, uint8_t* const bytes)
{
#if defined __clang__
#pragma unroll 4
#endif
  for (size_t i = 0; i < 4; i++) {
    bytes[i] = static_cast<uint8_t>(word >> (i << 3));
  }
}

// Generate N -many random bytes from available random device
static inline void
random_data(uint8_t* const data, const size_t dt_len)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  for (size_t i = 0; i < dt_len; i++) {
    data[i] = dis(gen);
  }
}
