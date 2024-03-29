#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <random>
#include <sstream>

// Given four consecutive little endian bytes, this function interprets them as
// 32 -bit unsigned integer
inline uint32_t
from_le_bytes(const uint8_t* const bytes)
{
#if defined __x86_64__ && !defined __clang__ && defined __GNUG__ &&            \
  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

  uint32_t res;
  std::memcpy(&res, bytes, 4);
  return res;

#else

  return (static_cast<uint32_t>(bytes[3]) << 24) |
         (static_cast<uint32_t>(bytes[2]) << 16) |
         (static_cast<uint32_t>(bytes[1]) << 8) |
         (static_cast<uint32_t>(bytes[0]) << 0);

#endif
}

// Given a 32 -bit unsigned interger, this function interprets it as four
// consecutive little endian bytes
inline void
to_le_bytes(const uint32_t word, uint8_t* const bytes)
{
#if defined __x86_64__ && !defined __clang__ && defined __GNUG__ &&            \
  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

  std::memcpy(bytes, &word, 4);

#else

#if defined __clang__
  // Following
  // https://clang.llvm.org/docs/LanguageExtensions.html#extensions-for-loop-hint-optimizations

#pragma clang loop unroll(enable)
#pragma clang loop vectorize(enable)
#elif defined __GNUG__
  // Following
  // https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#Loop-Specific-Pragmas

#pragma GCC ivdep
#pragma GCC unroll 4
#endif
  for (size_t i = 0; i < 4; i++) {
    bytes[i] = static_cast<uint8_t>(word >> (i << 3));
  }

#endif
}

// Generate N -many random bytes from available random device
inline void
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
inline const std::string
to_hex(const uint8_t* const bytes, const size_t len)
{
  std::stringstream ss;
  ss << std::hex;

  for (size_t i = 0; i < len; i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(bytes[i]);
  }
  return ss.str();
}
