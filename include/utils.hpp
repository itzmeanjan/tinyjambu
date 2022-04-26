#pragma once
#include <cstdint>
#include <iomanip>
#include <random>
#include <sstream>

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

// Given byte array of length N, this function converts that into hex
// representation
//
// Taken from
// https://github.com/itzmeanjan/ascon/blob/6050ca9/include/utils.hpp#L325-L336
static inline const std::string
to_hex(const uint8_t* const bytes, const size_t len)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < len; i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }
  return ss.str();
}
