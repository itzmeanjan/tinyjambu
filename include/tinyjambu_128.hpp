#pragma once
#include "tinyjambu.hpp"

// TinyJambu-128 Authenticated Encryption with Associated Data Implementation
namespace tinyjambu_128 {

// TinyJambu-128 Authenticated Encryption, which takes 128 -bit secret key, 96
// -bit public message nonce, N -bytes of associated data ( which is never
// encrypted ) & M -bytes of plain text ( which is encryptred ), producing M
// -bytes of cipher text & 64 -bit of authentication tag --- providing
// confidentiality, integrity & authentication
static inline void
encrypt(const uint8_t* const __restrict key,   // 128 -bit secret key
        const uint8_t* const __restrict nonce, // 96 -bit public message nonce
        const uint8_t* const __restrict data,  // associated data
        const size_t data_len,                 // associated data byte length
        const uint8_t* const __restrict text,  // plain text
        uint8_t* const __restrict cipher,      // cipher text
        const size_t ct_len,                   // plain/ cipher text byte length
        uint8_t* const __restrict tag          // 64 -bit authentication tag
)
{
  using namespace tinyjambu;

  // note permutation state must be zero initialized !
  uint32_t state[4] = { 0u };
  uint32_t key_[4] = { 0u };

#if defined __clang__
#pragma unroll 4
#endif
  for (size_t i = 0; i < 4; i++) {
    key_[i] = from_be_bytes(key + (i << 2));
  }

  initialize<key_128>(state, key_, nonce);
  process_associated_data<key_128>(state, key_, data, data_len);
  process_plain_text<key_128>(state, key_, text, cipher, ct_len);
  finalize<key_128>(state, key_, tag);
}

// TinyJambu-128 Verified Decryption, which takes 128 -bit secret key, 96
// -bit public message nonce, 64 -bit authentication tag, N -bytes of associated
// data ( which was never encrypted ) & M -bytes of cipher text, producing M
// -bytes of plain text & boolean verification flag denoting authentication
// status --- providing confidentiality, integrity & authentication
//
// Note, if returned boolean verification status is not true value, don't
// consume decrypted bytes !
static inline bool
decrypt(const uint8_t* const __restrict key,    // 128 -bit secret key
        const uint8_t* const __restrict nonce,  // 96 -bit public message nonce
        const uint8_t* const __restrict tag,    // 64 -bit authentication tag
        const uint8_t* const __restrict data,   // associated data
        const size_t data_len,                  // associated data byte length
        const uint8_t* const __restrict cipher, // cipher text
        uint8_t* const __restrict text,         // plain text
        const size_t ct_len // cipher/ plain text byte length
)
{
  using namespace tinyjambu;

  // note permutation state must be zero initialized !
  uint32_t state[4] = { 0u };
  uint32_t key_[4] = { 0u };
  uint8_t tag_[8] = { 0u };

#if defined __clang__
#pragma unroll 4
#endif
  for (size_t i = 0; i < 4; i++) {
    key_[i] = from_be_bytes(key + (i << 2));
  }

  initialize<key_128>(state, key_, nonce);
  process_associated_data<key_128>(state, key_, data, data_len);
  process_cipher_text<key_128>(state, key_, cipher, text, ct_len);
  finalize<key_128>(state, key_, tag_);

  bool flag = false;
  for (size_t i = 0; i < 8; i++) {
    flag |= static_cast<bool>(tag[i] ^ tag_[i]);
  }
  return !flag;
}

}
