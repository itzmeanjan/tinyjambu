#pragma once
#include "tinyjambu_128.hpp"
#include <cassert>

// Test functional correctness of TinyJambu-{128, 192, 256} AEAD Implementation
namespace test_tinyjambu {

// Test TinyJambu-128 AEAD Implementation by executing encrypt -> decrypt ->
// compare, on randomly generated input bytes
static inline void
key_128(const size_t dt_len, const size_t ct_len)
{
  uint8_t* key = static_cast<uint8_t*>(std::malloc(16u));
  uint8_t* nonce = static_cast<uint8_t*>(std::malloc(12u));
  uint8_t* tag = static_cast<uint8_t*>(std::malloc(8u));
  uint8_t* data = static_cast<uint8_t*>(std::malloc(dt_len));
  uint8_t* text = static_cast<uint8_t*>(std::malloc(ct_len));
  uint8_t* enc = static_cast<uint8_t*>(std::malloc(ct_len));
  uint8_t* dec = static_cast<uint8_t*>(std::malloc(ct_len));

  random_data(key, 16);
  random_data(nonce, 12);
  random_data(data, dt_len);
  random_data(text, ct_len);

  using namespace tinyjambu_128;

  encrypt(key, nonce, data, dt_len, text, enc, ct_len, tag);
  const bool f = decrypt(key, nonce, tag, data, dt_len, enc, dec, ct_len);

  // authentication
  assert(f);

  // byte-by-byte comparison to be sure that original plain text & decrypted
  // plain text bytes are actually same !
  for (size_t i = 0; i < ct_len; i++) {
    assert(text[i] == dec[i]);
  }

  std::free(key);
  std::free(nonce);
  std::free(tag);
  std::free(data);
  std::free(text);
  std::free(enc);
  std::free(dec);
}

}
