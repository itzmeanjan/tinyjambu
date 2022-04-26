#pragma once
#include "tinyjambu.hpp"

// TinyJambu-192 Authenticated Encryption with Associated Data Implementation
namespace tinyjambu_192 {

// TinyJambu-192 Authenticated Encryption, which takes 192 -bit secret key, 96
// -bit public message nonce, N -bytes of associated data ( which is never
// encrypted ) & M -bytes of plain text ( which is encrypted ), producing M
// -bytes of cipher text & 64 -bit of authentication tag --- providing
// confidentiality, integrity & authentication
static inline void
encrypt(const uint8_t* const __restrict key,   // 192 -bit secret key
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
  uint32_t key_[6] = { 0u };

#if defined __clang__
#pragma unroll 6
#endif
  for (size_t i = 0; i < 6; i++) {
    key_[i] = from_le_bytes(key + (i << 2));
  }

  initialize<key_192>(state, key_, nonce);
  process_associated_data<key_192>(state, key_, data, data_len);
  process_plain_text<key_192>(state, key_, text, cipher, ct_len);
  finalize<key_192>(state, key_, tag);
}

// TinyJambu-192 Verified Decryption, which takes 192 -bit secret key, 96
// -bit public message nonce, 64 -bit authentication tag, N -bytes of associated
// data ( which was never encrypted ) & M -bytes of cipher text, producing M
// -bytes of plain text & boolean verification flag denoting authentication
// status --- providing confidentiality, integrity & authentication
//
// Note, if returned boolean verification status is not truth value, don't
// consume decrypted bytes !
static inline bool
decrypt(const uint8_t* const __restrict key,    // 192 -bit secret key
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
  uint32_t key_[6] = { 0u };
  uint8_t tag_[8] = { 0u };

#if defined __clang__
#pragma unroll 6
#endif
  for (size_t i = 0; i < 6; i++) {
    key_[i] = from_le_bytes(key + (i << 2));
  }

  initialize<key_192>(state, key_, nonce);
  process_associated_data<key_192>(state, key_, data, data_len);
  process_cipher_text<key_192>(state, key_, cipher, text, ct_len);
  finalize<key_192>(state, key_, tag_);

  bool flag = false;
  for (size_t i = 0; i < 8; i++) {
    flag |= static_cast<bool>(tag[i] ^ tag_[i]);
  }
  return !flag;
}

}