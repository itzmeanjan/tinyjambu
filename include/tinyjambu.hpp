#pragma once
#include "permute.hpp"
#include "utils.hpp"

// Commonly used routines in TinyJambu-{128, 192, 256} Authenticated Encryption
// with Associated Data ( AEAD ) cipher suite
namespace tinyjambu {

// Three TinyJambu variants based on different secret key sizes
enum variant
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
// Note, this function expects that state is already zero-initialized !
//
// See section 3.3.1 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline void
initialize(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict nonce // 96 -bit public message nonce
)
{
  // key setup
  switch (v) {
    case key_128:
      tinyjambu_128::state_update<1024ul>(state, key);
      break;
    case key_192:
      tinyjambu_192::state_update<1152ul>(state, key);
      break;
    case key_256:
      tinyjambu_256::state_update<1280ul>(state, key);
      break;
  }

  // nonce setup
  for (size_t i = 0; i < 3; i++) {
    state[1] = state[1] ^ FRAMEBITS_NONCE;
    switch (v) {
      case key_128:
        tinyjambu_128::state_update<640ul>(state, key);
        break;
      case key_192:
        tinyjambu_192::state_update<640ul>(state, key);
        break;
      case key_256:
        tinyjambu_256::state_update<640ul>(state, key);
        break;
    }
    state[3] = state[3] ^ from_le_bytes(nonce + (i << 2));
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
static inline void
process_associated_data(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict data, // N -bytes of associated data
  const size_t data_len                 // # -of associated data bytes
)
{
  const size_t full_blk_cnt = data_len >> 2;

  for (size_t i = 0; i < full_blk_cnt; i++) {
    state[1] = state[1] ^ FRAMEBITS_AD;
    switch (v) {
      case key_128:
        tinyjambu_128::state_update<640ul>(state, key);
        break;
      case key_192:
        tinyjambu_192::state_update<640ul>(state, key);
        break;
      case key_256:
        tinyjambu_256::state_update<640ul>(state, key);
        break;
    }
    state[3] = state[3] ^ from_le_bytes(data + (i << 2));
  }

  // > 0 && < 4
  const size_t partial_byte_cnt = data_len & 3ul;

  if (partial_byte_cnt > 0ul) {
    const size_t partial_byte_off = data_len - partial_byte_cnt;

    state[1] = state[1] ^ FRAMEBITS_AD;
    switch (v) {
      case key_128:
        tinyjambu_128::state_update<640ul>(state, key);
        break;
      case key_192:
        tinyjambu_192::state_update<640ul>(state, key);
        break;
      case key_256:
        tinyjambu_256::state_update<640ul>(state, key);
        break;
    }

    switch (partial_byte_cnt) {
      case 1:
        state[3] ^= static_cast<uint32_t>(data[partial_byte_off + 0]);
        break;
      case 2:
        state[3] ^= (static_cast<uint32_t>(data[partial_byte_off + 1]) << 8) |
                    (static_cast<uint32_t>(data[partial_byte_off + 0]) << 0);
        break;
      case 3:
        state[3] ^= (static_cast<uint32_t>(data[partial_byte_off + 2]) << 16) |
                    (static_cast<uint32_t>(data[partial_byte_off + 1]) << 8) |
                    (static_cast<uint32_t>(data[partial_byte_off + 0]) << 0);
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
template<const variant v>
static inline void
process_plain_text(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict text, // N -bytes plain text
  uint8_t* const __restrict cipher,     // N -bytes cipher text ( output )
  const size_t ct_len                   // # -of plain/ cipher text bytes
)
{
  const size_t full_blk_cnt = ct_len >> 2;

  for (size_t i = 0; i < full_blk_cnt; i++) {
    state[1] = state[1] ^ FRAMEBITS_CT;
    switch (v) {
      case key_128:
        tinyjambu_128::state_update<1024ul>(state, key);
        break;
      case key_192:
        tinyjambu_192::state_update<1152ul>(state, key);
        break;
      case key_256:
        tinyjambu_256::state_update<1280ul>(state, key);
        break;
    }

    const uint32_t msg = from_le_bytes(text + (i << 2));
    state[3] = state[3] ^ msg;

    const uint32_t enc = state[2] ^ msg;
    to_le_bytes(enc, cipher + (i << 2));
  }

  // > 0 && < 4
  const size_t partial_byte_cnt = ct_len & 3ul;

  if (partial_byte_cnt > 0ul) {
    const size_t partial_byte_off = ct_len - partial_byte_cnt;

    state[1] = state[1] ^ FRAMEBITS_CT;
    switch (v) {
      case key_128:
        tinyjambu_128::state_update<1024ul>(state, key);
        break;
      case key_192:
        tinyjambu_192::state_update<1152ul>(state, key);
        break;
      case key_256:
        tinyjambu_256::state_update<1280ul>(state, key);
        break;
    }

    uint32_t msg = 0u;
    switch (partial_byte_cnt) {
      case 1:
        msg = static_cast<uint32_t>(text[partial_byte_off + 0]);
        break;
      case 2:
        msg = (static_cast<uint32_t>(text[partial_byte_off + 1]) << 8) |
              (static_cast<uint32_t>(text[partial_byte_off + 0]) << 0);
        break;
      case 3:
        msg = (static_cast<uint32_t>(text[partial_byte_off + 2]) << 16) |
              (static_cast<uint32_t>(text[partial_byte_off + 1]) << 8) |
              (static_cast<uint32_t>(text[partial_byte_off + 0]) << 0);
        break;
    }
    state[3] = state[3] ^ msg;

    const uint32_t enc = state[2] ^ msg;

    switch (partial_byte_cnt) {
      case 1:
        cipher[partial_byte_off + 0] = static_cast<uint8_t>(enc);
        break;
      case 2:
#if defined __clang__
#pragma unroll 2
#endif
        for (size_t i = 0; i < 2; i++) {
          const uint8_t b = static_cast<uint8_t>(enc >> (i << 3));
          cipher[partial_byte_off + i] = b;
        }
        break;
      case 3:
#if defined __clang__
#pragma unroll 3
#endif
        for (size_t i = 0; i < 3; i++) {
          const uint8_t b = static_cast<uint8_t>(enc >> (i << 3));
          cipher[partial_byte_off + i] = b;
        }
        break;
    }

    state[1] = state[1] ^ static_cast<uint32_t>(partial_byte_cnt);
  }
}

// Process N -many cipher text bytes and computes equal number of plain text
// bytes, using TinyJambu-128 AEAD
//
// See section 3.3.5 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline void
process_cipher_text(
  uint32_t* const __restrict state,       // 128 -bit state
  const uint32_t* const __restrict key,   // {128, 192, 256} -bit secret key
  const uint8_t* const __restrict cipher, // N -bytes cipher text
  uint8_t* const __restrict text,         // N -bytes plain text ( output )
  const size_t ct_len                     // # -of cipher/ plain text bytes
)
{
  const size_t full_blk_cnt = ct_len >> 2;

  for (size_t i = 0; i < full_blk_cnt; i++) {
    state[1] = state[1] ^ FRAMEBITS_CT;
    switch (v) {
      case key_128:
        tinyjambu_128::state_update<1024ul>(state, key);
        break;
      case key_192:
        tinyjambu_192::state_update<1152ul>(state, key);
        break;
      case key_256:
        tinyjambu_256::state_update<1280ul>(state, key);
        break;
    }

    const uint32_t enc = from_le_bytes(cipher + (i << 2));
    const uint32_t dec = state[2] ^ enc;

    state[3] = state[3] ^ dec;
    to_le_bytes(dec, text + (i << 2));
  }

  // > 0 && < 4
  const size_t partial_byte_cnt = ct_len & 3ul;

  if (partial_byte_cnt > 0ul) {
    const size_t partial_byte_off = ct_len - partial_byte_cnt;

    state[1] = state[1] ^ FRAMEBITS_CT;
    switch (v) {
      case key_128:
        tinyjambu_128::state_update<1024ul>(state, key);
        break;
      case key_192:
        tinyjambu_192::state_update<1152ul>(state, key);
        break;
      case key_256:
        tinyjambu_256::state_update<1280ul>(state, key);
        break;
    }

    uint32_t enc = 0u;
    switch (partial_byte_cnt) {
      case 1:
        enc = static_cast<uint32_t>(cipher[partial_byte_off + 0]);
        break;
      case 2:
        enc = (static_cast<uint32_t>(cipher[partial_byte_off + 1]) << 8) |
              (static_cast<uint32_t>(cipher[partial_byte_off + 0]) << 0);
        break;
      case 3:
        enc = (static_cast<uint32_t>(cipher[partial_byte_off + 2]) << 16) |
              (static_cast<uint32_t>(cipher[partial_byte_off + 1]) << 8) |
              (static_cast<uint32_t>(cipher[partial_byte_off + 0]) << 0);
        break;
    }

    // only enable those bits ( from LSB side of 32 -bit word ) which are
    // carrying decrypted bits
    //
    // note, decrypted bit count always divisible by 8 && ∈ [8..24]
    const uint32_t dec = state[2] ^ enc;
    switch (partial_byte_cnt) {
      case 1:
        state[3] = state[3] ^ (dec & 0xffu);
        break;
      case 2:
        state[3] = state[3] ^ (dec & 0xffffu);
        break;
      case 3:
        state[3] = state[3] ^ (dec & 0xffffffu);
        break;
    }

    switch (partial_byte_cnt) {
      case 1:
        text[partial_byte_off + 0] = static_cast<uint8_t>(dec);
        break;
      case 2:
#if defined __clang__
#pragma unroll 2
#endif
        for (size_t i = 0; i < 2; i++) {
          const uint8_t b = static_cast<uint8_t>(dec >> (i << 3));
          text[partial_byte_off + i] = b;
        }
        break;
      case 3:
#if defined __clang__
#pragma unroll 3
#endif
        for (size_t i = 0; i < 3; i++) {
          const uint8_t b = static_cast<uint8_t>(dec >> (i << 3));
          text[partial_byte_off + i] = b;
        }
        break;
    }

    state[1] = state[1] ^ static_cast<uint32_t>(partial_byte_cnt);
  }
}

// Finalization step, computing 64 -bit authentication tag for AEAD scheme
//
// See section 3.3.4 of TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const variant v>
static inline void
finalize(
  uint32_t* const __restrict state,     // 128 -bit state
  const uint32_t* const __restrict key, // {128, 192, 256} -bit secret key
  uint8_t* const __restrict tag         // 64 -bit authentication tag
)
{
  state[1] = state[1] ^ FRAMEBITS_TAG;
  switch (v) {
    case key_128:
      tinyjambu_128::state_update<1024ul>(state, key);
      break;
    case key_192:
      tinyjambu_192::state_update<1152ul>(state, key);
      break;
    case key_256:
      tinyjambu_256::state_update<1280ul>(state, key);
      break;
  }
  to_le_bytes(state[2], tag);

  state[1] = state[1] ^ FRAMEBITS_TAG;
  switch (v) {
    case key_128:
      tinyjambu_128::state_update<640ul>(state, key);
      break;
    case key_192:
      tinyjambu_192::state_update<640ul>(state, key);
      break;
    case key_256:
      tinyjambu_256::state_update<640ul>(state, key);
      break;
  }
  to_le_bytes(state[2], tag + 4);
}

}
