#pragma once
#include "utils.hpp"

// TinyJambu-128 Authenticated Encryption with Associated Data Implementation
namespace tinyjambu_128 {

// Note, following three bit framebits are defined such that they can be easily
// xor-ed into state[36..38] i.e. from bit 36 to bit 38 of permutation state.
//
// Here, 128 -bit permutation state is defined as
//
// ( MSB ) state[3] || state[2] || state[1] || state[0] ( LSB )
//
// where each limb is of 32 -bits. Just to skip ( no need to touch those bits )
// from bit 32 to bit 35, framebits defined in section 3.2.2 of TinyJambu
// specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
// are left shifted by 4 bit positions.

// Three bit framebits, when mixing public message nonce into permutation state
//
// == hex(1 << 4)
constexpr uint32_t FRAMEBITS_NONCE = 0x10u;

// Three bit framebits, when mixing associated data into permutation state
//
// == hex(3 << 4)
constexpr uint32_t FRAMEBITS_AD = 0x30u;

// Three bit framebits, when mixing plain/ cipher text into permutation state
//
// == hex(5 << 4)
constexpr uint32_t FRAMEBITS_CT = 0x50u;

// Three bit framebits, when generating authentication tag
//
// == hex(7 << 4)
constexpr uint32_t FRAMEBITS_TAG = 0x70u;

// TinyJambu `StateUpdate` function, specifically written for TinyJambu-128
// variant, computing 128 feedback bits during each iteration ( see inside body
// of for loop )
//
// Note, this function will update state of 128 -bit Non-Linear Feedback Shift
// Register `rounds` -many time; so ensure that
//
// assert rounds % 128 == 0
//
// See section 3.2.3 in TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const size_t rounds>
static inline void
state_update(uint32_t* const __restrict state,    // 128 -bit permutation state
             const uint32_t* const __restrict key // 128 -bit secret key
)
{
  const size_t itr_cnt = rounds >> 7;

  for (size_t i = 0; i < itr_cnt; i++) {
    {
      const uint32_t s47 = (state[2] << 17) || (state[1] >> 15);
      const uint32_t s70 = (state[3] << 26) || (state[2] >> 6);
      const uint32_t s85 = (state[3] << 11) || (state[2] >> 21);
      const uint32_t s91 = (state[3] << 5) || (state[2] >> 27);

      state[0] = state[0] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[0];
    }
    {
      const uint32_t s47 = (state[3] << 17) || (state[2] >> 15);
      const uint32_t s70 = (state[0] << 26) || (state[3] >> 6);
      const uint32_t s85 = (state[0] << 11) || (state[3] >> 21);
      const uint32_t s91 = (state[0] << 5) || (state[3] >> 27);

      state[1] = state[1] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[1];
    }
    {
      const uint32_t s47 = (state[0] << 17) || (state[3] >> 15);
      const uint32_t s70 = (state[1] << 26) || (state[0] >> 6);
      const uint32_t s85 = (state[1] << 11) || (state[0] >> 21);
      const uint32_t s91 = (state[1] << 5) || (state[0] >> 27);

      state[2] = state[2] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[2];
    }
    {
      const uint32_t s47 = (state[1] << 17) || (state[0] >> 15);
      const uint32_t s70 = (state[2] << 26) || (state[1] >> 6);
      const uint32_t s85 = (state[2] << 11) || (state[1] >> 21);
      const uint32_t s91 = (state[2] << 5) || (state[1] >> 27);

      state[3] = state[3] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[3];
    }
  }
}

// Initializes 128 -bit permutation state of TinyJambu-128 variant, using 128
// -bit secret key & 96 -bit public message nonce
//
// Note, this function expects that state is already zero-initialized !
//
// See section 3.3.1 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
static inline void
initialize(uint32_t* const __restrict state,     // 128 -bit state
           const uint32_t* const __restrict key, // 128 -bit secret key
           const uint8_t* const __restrict nonce // 96 -bit public message nonce
)
{
  // key setup
  state_update<1024ul>(state, key);

  // nonce setup
  for (size_t i = 0; i < 3; i++) {
    state[1] = state[1] ^ FRAMEBITS_NONCE;
    state_update<640ul>(state, key);
    state[3] = state[3] ^ from_be_bytes(nonce + (i << 2));
  }
}

// Processing associated data such that first all full blocks ( each of size 32
// -bits ) are mixed into state, then remaining partial data block ( bit length
// of partial data block should be >= 8 && <= 24 | evenly divisible by 8 ) will
// be mixed into permutation state
//
// See section 3.3.2 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
static inline void
process_associated_data(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // 128 -bit secret key
  const uint8_t* const __restrict data, // N -bytes of associated data
  const size_t data_len                 // # -of associated data bytes
)
{
  const size_t full_blk_cnt = data_len >> 2;

  for (size_t i = 0; i < full_blk_cnt; i++) {
    state[1] = state[1] ^ FRAMEBITS_AD;
    state_update<640ul>(state, key);
    state[3] = state[3] ^ from_be_bytes(data + (i << 2));
  }

  // > 0 && < 4
  const size_t partial_byte_cnt = data_len & 3ul;

  if (partial_byte_cnt > 0ul) {
    const size_t partial_byte_off = data_len - partial_byte_cnt;

    state[1] = state[1] ^ FRAMEBITS_AD;
    state_update<640ul>(state, key);

    switch (partial_byte_cnt) {
      case 1:
        state[3] ^= (static_cast<uint32_t>(data[partial_byte_off + 0]) << 24);
        break;
      case 2:
        state[3] ^= (static_cast<uint32_t>(data[partial_byte_off + 0]) << 24) ||
                    (static_cast<uint32_t>(data[partial_byte_off + 1]) << 16);
        break;
      case 3:
        state[3] ^= (static_cast<uint32_t>(data[partial_byte_off + 0]) << 24) ||
                    (static_cast<uint32_t>(data[partial_byte_off + 1]) << 16) ||
                    (static_cast<uint32_t>(data[partial_byte_off + 2]) << 8);
        break;
    }

    state[1] = state[1] ^ static_cast<uint32_t>(partial_byte_cnt);
  }
}

// Process N -many plain text bytes and computes equal number of cipher text
// bytes, using TinyJambu-128 AEAD
//
// See section 3.3.3 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
static inline void
process_plain_text(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // 128 -bit secret key
  const uint8_t* const __restrict text, // N -bytes plain text
  uint8_t* const __restrict cipher,     // N -bytes cipher text ( output )
  const size_t ct_len                   // # -of plain/ cipher text bytes
)
{
  const size_t full_blk_cnt = ct_len >> 2;

  for (size_t i = 0; i < full_blk_cnt; i++) {
    state[1] = state[1] ^ FRAMEBITS_CT;
    state_update<1024ul>(state, key);

    const uint32_t msg = from_be_bytes(text + (i << 2));
    state[3] = state[3] ^ msg;

    const uint32_t enc = state[2] ^ msg;
    to_be_bytes(enc, cipher + (i << 2));
  }

  // > 0 && < 4
  const size_t partial_byte_cnt = ct_len & 3ul;

  if (partial_byte_cnt > 0ul) {
    const size_t partial_byte_off = ct_len - partial_byte_cnt;

    state[1] = state[1] ^ FRAMEBITS_CT;
    state_update<1024ul>(state, key);

    uint32_t msg = 0u;
    switch (partial_byte_cnt) {
      case 1:
        msg = (static_cast<uint32_t>(text[partial_byte_off + 0]) << 24);
        break;
      case 2:
        msg = (static_cast<uint32_t>(text[partial_byte_off + 0]) << 24) ||
              (static_cast<uint32_t>(text[partial_byte_off + 1]) << 16);
        break;
      case 3:
        msg = (static_cast<uint32_t>(text[partial_byte_off + 0]) << 24) ||
              (static_cast<uint32_t>(text[partial_byte_off + 1]) << 16) ||
              (static_cast<uint32_t>(text[partial_byte_off + 2]) << 8);
        break;
    }
    state[3] = state[3] ^ msg;

    const uint32_t enc = state[2] ^ msg;

    switch (partial_byte_cnt) {
      case 1:
        cipher[partial_byte_off + 0] = static_cast<uint8_t>(enc >> 24);
        break;
      case 2:
        cipher[partial_byte_off + 0] = static_cast<uint8_t>(enc >> 24);
        cipher[partial_byte_off + 1] = static_cast<uint8_t>(enc >> 16);
        break;
      case 3:
        cipher[partial_byte_off + 0] = static_cast<uint8_t>(enc >> 24);
        cipher[partial_byte_off + 1] = static_cast<uint8_t>(enc >> 16);
        cipher[partial_byte_off + 2] = static_cast<uint8_t>(enc >> 8);
        break;
    }

    state[1] = state[1] ^ static_cast<uint32_t>(partial_byte_cnt);
  }
}

}
