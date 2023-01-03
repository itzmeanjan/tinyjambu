#pragma once
#include "test_common.hpp"
#include "tinyjambu_128.hpp"
#include <cassert>

namespace test_tinyjambu {

// Test TinyJambu-128 AEAD Implementation by executing encrypt -> decrypt ->
// compare, on randomly generated input bytes, while also mutating ( a single
// bit flip ) decrypt routine input set to show that AEAD scheme works as
// expected
void
key_128(const size_t dt_len, const size_t ct_len, const mutate_t m)
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

  // Mutate ( single bit flip ), if possible !
  switch (m) {
    case mutate_t::key:
      key[0] ^= static_cast<uint8_t>(1);
      break;
    case mutate_t::nonce:
      nonce[0] ^= static_cast<uint8_t>(1);
      break;
    case mutate_t::tag:
      tag[0] ^= static_cast<uint8_t>(1);
      break;
    case mutate_t::data:
      if (dt_len > 0) {
        data[0] ^= static_cast<uint8_t>(1);
      }
      break;
    case mutate_t::enc:
      if (ct_len > 0) {
        enc[0] ^= static_cast<uint8_t>(1);
      }
      break;
    case mutate_t::none:
      // don't mutate anything --- ideal world !
      break;
  }

  const bool f = decrypt(key, nonce, tag, data, dt_len, enc, dec, ct_len);

  // authentication
  switch (m) {
    case mutate_t::key:
      assert(!f);
      break;
    case mutate_t::nonce:
      assert(!f);
      break;
    case mutate_t::tag:
      assert(!f);
      break;
    case mutate_t::data:
      if (dt_len > 0) {
        assert(!f);
      } else {
        assert(f);

        // byte-by-byte comparison to be sure that original plain text &
        // decrypted
        // plain text bytes are actually same !
        for (size_t i = 0; i < ct_len; i++) {
          assert(text[i] == dec[i]);
        }
      }
      break;
    case mutate_t::enc:
      if (ct_len > 0) {
        assert(!f);
      } else {
        assert(f);

        // byte-by-byte comparison to be sure that original plain text &
        // decrypted
        // plain text bytes are actually same !
        for (size_t i = 0; i < ct_len; i++) {
          assert(text[i] == dec[i]);
        }
      }
      break;
    case mutate_t::none:
      assert(f);

      // byte-by-byte comparison to be sure that original plain text & decrypted
      // plain text bytes are actually same !
      for (size_t i = 0; i < ct_len; i++) {
        assert(text[i] == dec[i]);
      }
      break;
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
