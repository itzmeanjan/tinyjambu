#pragma once
#include "permute.hpp"
#include "utils.hpp"
#include <cstring>
#include <type_traits>

// Commonly used routines in TinyJambu-{128, 192, 256} Authenticated Encryption
// with Associated Data ( AEAD ) cipher suite
namespace tinyjambu {

// Three TinyJambu variants based on different secret key sizes
enum class variant : uint8_t
{
  key_128, // 128 -bit secret key
  key_192, // 192 -bit secret key
  key_256, // 256 -bit secret key
};

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

// Initializes 128 -bit permutation state using {128, 192, 256} -bit secret key
// & 96 -bit public message nonce
//
// See section 3.3.1 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline constexpr void
initialize(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict nonce // 96 -bit public message nonce
)
{
  // Initialize state array with 128 zero bits - just to be safe !
  std::memset(state, 0, 16);

  // key setup
  if constexpr (v == variant::key_128) {
    tinyjambu_128::state_update<1024ul>(state, key);
  } else if constexpr (v == variant::key_192) {
    tinyjambu_192::state_update<1152ul>(state, key);
  } else if constexpr (v == variant::key_256) {
    tinyjambu_256::state_update<1280ul>(state, key);
  }

  // nonce setup
  for (size_t i = 0; i < 3; i++) {
    state[1] ^= FRAMEBITS_NONCE;

    if constexpr (v == variant::key_128) {
      tinyjambu_128::state_update<640ul>(state, key);
    } else if constexpr (v == variant::key_192) {
      tinyjambu_192::state_update<640ul>(state, key);
    } else if constexpr (v == variant::key_256) {
      tinyjambu_256::state_update<640ul>(state, key);
    }

    state[3] ^= from_le_bytes(nonce + i * 4);
  }
}

// Processing associated data such that first all full blocks ( each of size 32
// -bits ) are mixed into state, then remaining partial data block ( bit length
// of partial data block should be >= 8 && <= 24 | evenly divisible by 8 ) will
// be mixed into permutation state
//
// See section 3.3.2 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline constexpr void
process_associated_data(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict data, // N -bytes of associated data
  const size_t data_len                 // # -of associated data bytes
)
{
  const size_t part_byte_cnt = data_len & 3ul;

  size_t b_off = 0ul;
  while (b_off < data_len) {
    state[1] ^= FRAMEBITS_AD;

    if constexpr (v == variant::key_128) {
      tinyjambu_128::state_update<640ul>(state, key);
    } else if constexpr (v == variant::key_192) {
      tinyjambu_192::state_update<640ul>(state, key);
    } else if constexpr (v == variant::key_256) {
      tinyjambu_256::state_update<640ul>(state, key);
    }

    const size_t take = std::min(4ul, data_len - b_off);

    uint32_t word = 0u;
    for (size_t i = 0; i < take; i++) {
      word |= static_cast<uint32_t>(data[b_off + i]) << (i << 3);
    }

    state[3] ^= word;
    b_off += take;
  }

  state[1] ^= static_cast<uint32_t>(part_byte_cnt);
}

// Process N -many plain text bytes and computes equal number of cipher text
// bytes, using TinyJambu-{128, 192, 256} AEAD
//
// See section 3.3.3 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline constexpr void
process_plain_text(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict text, // N -bytes plain text
  uint8_t* const __restrict cipher,     // N -bytes cipher text ( output )
  const size_t ct_len                   // # -of plain/ cipher text bytes
)
{
  const size_t part_byte_cnt = ct_len & 3ul;

  size_t b_off = 0ul;
  while (b_off < ct_len) {
    state[1] ^= FRAMEBITS_CT;

    if constexpr (v == variant::key_128) {
      tinyjambu_128::state_update<1024ul>(state, key);
    } else if constexpr (v == variant::key_192) {
      tinyjambu_192::state_update<1152ul>(state, key);
    } else if constexpr (v == variant::key_256) {
      tinyjambu_256::state_update<1280ul>(state, key);
    }

    const size_t take = std::min(4ul, ct_len - b_off);

    uint32_t word = 0u;
    for (size_t i = 0; i < take; i++) {
      word |= static_cast<uint32_t>(text[b_off + i]) << (i << 3);
    }

    state[3] ^= word;

    const uint32_t enc = state[2] ^ word;
    for (size_t i = 0; i < take; i++) {
      cipher[b_off + i] = static_cast<uint8_t>(enc >> (i << 3));
    }

    b_off += take;
  }

  state[1] ^= static_cast<uint32_t>(part_byte_cnt);
}

// Process N -many cipher text bytes and computes equal number of plain text
// bytes, using TinyJambu-128 AEAD
//
// See section 3.3.5 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline constexpr void
process_cipher_text(
  uint32_t* const __restrict state,       // 128 -bit state
  const uint32_t* const __restrict key,   // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict cipher, // N -bytes cipher text
  uint8_t* const __restrict text,         // N -bytes plain text ( output )
  const size_t ct_len                     // # -of cipher/ plain text bytes
)
{
  const size_t part_byte_cnt = ct_len & 3ul;

  size_t b_off = 0ul;
  while (b_off < ct_len) {
    state[1] ^= FRAMEBITS_CT;

    if constexpr (v == variant::key_128) {
      tinyjambu_128::state_update<1024ul>(state, key);
    } else if constexpr (v == variant::key_192) {
      tinyjambu_192::state_update<1152ul>(state, key);
    } else if constexpr (v == variant::key_256) {
      tinyjambu_256::state_update<1280ul>(state, key);
    }

    const size_t take = std::min(4ul, ct_len - b_off);

    uint32_t word = 0u;
    for (size_t i = 0; i < take; i++) {
      word |= static_cast<uint32_t>(cipher[b_off + i]) << (i << 3);
    }

    const uint32_t dec = state[2] ^ word;

    const uint32_t mask = 0xffffffffu >> ((4ul - take) << 3);
    state[3] ^= (dec & mask);

    for (size_t i = 0; i < take; i++) {
      text[b_off + i] = static_cast<uint8_t>(dec >> (i << 3));
    }

    b_off += take;
  }

  state[1] ^= static_cast<uint32_t>(part_byte_cnt);
}

// Finalization step, computing 64 -bit authentication tag for AEAD scheme
//
// See section 3.3.4 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline constexpr void
finalize(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  uint8_t* const __restrict tag         // 64 -bit authentication tag
)
{
  state[1] ^= FRAMEBITS_TAG;

  if constexpr (v == variant::key_128) {
    tinyjambu_128::state_update<1024ul>(state, key);
  } else if constexpr (v == variant::key_192) {
    tinyjambu_192::state_update<1152ul>(state, key);
  } else if constexpr (v == variant::key_256) {
    tinyjambu_256::state_update<1280ul>(state, key);
  }

  to_le_bytes(state[2], tag);

  state[1] ^= FRAMEBITS_TAG;

  if constexpr (v == variant::key_128) {
    tinyjambu_128::state_update<640ul>(state, key);
  } else if constexpr (v == variant::key_192) {
    tinyjambu_192::state_update<640ul>(state, key);
  } else if constexpr (v == variant::key_256) {
    tinyjambu_256::state_update<640ul>(state, key);
  }

  to_le_bytes(state[2], tag + 4);
}

}
