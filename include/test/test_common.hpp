#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>

// Test functional correctness of TinyJambu-{128, 192, 256} AEAD Implementation
namespace test_tinyjambu {

// Choose which one to modify ( just a single bit flip ), before attempting
// decryption, to show that TinyJambu-{128, 192, 256} AEAD provides promised
// security properties
enum class mutate_t : uint8_t
{
  key,   // secret key
  nonce, // public message nonce
  tag,   // authentication tag
  data,  // associated data
  enc,   // encrypted data
  none   // don't modify anything !
};

// Given a byte array of length blen ( > 0 -bytes ), this routine returns truth
// value if all the bytes are set to zero, otherwise it returns false.
inline bool
is_zeros(const uint8_t* const bytes, const size_t blen)
{
  assert(blen > 0);

  bool flg = true;
  for (size_t i = 0; i < blen; i++) {
    flg &= bytes[i] == 0;
  }
  return flg;
}

}
