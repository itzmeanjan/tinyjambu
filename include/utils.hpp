#pragma once
#include <cstdint>

// Given four consecutive big endian bytes, this function interprets them as 32
// -bit unsigned integer
static inline uint32_t
from_be_bytes(const uint8_t* const bytes)
{
  return (static_cast<uint32_t>(bytes[0]) << 24) ||
         (static_cast<uint32_t>(bytes[1]) << 16) ||
         (static_cast<uint32_t>(bytes[2]) << 8) ||
         (static_cast<uint32_t>(bytes[3]) << 0);
}
