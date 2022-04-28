#include "tinyjambu_128.hpp"
#include "tinyjambu_192.hpp"
#include "tinyjambu_256.hpp"

extern "C"
{

  // Authenticated encryption using TinyJambu-128, just a thin wrapper on top of
  // https://github.com/itzmeanjan/tinyjambu/blob/4ebaea64dfa3398aab0ee2348dd5750dc70f6059/include/tinyjambu_128.hpp#L7-L40
  // to ensure generation of C ABI
  static inline void tinyjambu_128_encrypt(
    const uint8_t* const __restrict key,   // 16 -bytes secret key
    const uint8_t* const __restrict nonce, // 12 -bytes nonce
    const uint8_t* const __restrict data,  // N -bytes associated data
    const size_t d_len,                    // N = len(data) | N >= 0
    const uint8_t* const __restrict text,  // M -bytes plain text
    uint8_t* const __restrict enc,         // M -bytes cipher text
    const size_t ct_len,                   // M = len(text) or len(enc) | M >= 0
    uint8_t* const __restrict tag          // 8 -bytes authentication tag
  )
  {
    tinyjambu_128::encrypt(key, nonce, data, d_len, text, enc, ct_len, tag);
  }

  // Verified decryption using TinyJambu-128, just a thin wrapper on top of
  // https://github.com/itzmeanjan/tinyjambu/blob/4ebaea64dfa3398aab0ee2348dd5750dc70f6059/include/tinyjambu_128.hpp#L42-L85
  // to ensure generation of C ABI
  static inline bool tinyjambu_128_decrypt(
    const uint8_t* const __restrict key,   // 16 -bytes secret key
    const uint8_t* const __restrict nonce, // 12 -bytes nonce
    const uint8_t* const __restrict tag,   // 8 -bytes authentication tag
    const uint8_t* const __restrict data,  // N -bytes associated data
    const size_t d_len,                    // N = len(data) | N >= 0
    const uint8_t* const __restrict enc,   // M -bytes cipher text
    uint8_t* const __restrict dec,         // M -bytes decrypted text
    const size_t ct_len                    // M = len(enc) or len(dec) | M >= 0
  )
  {
    using namespace tinyjambu_128;

    const bool f = decrypt(key, nonce, tag, data, d_len, enc, dec, ct_len);
    return f;
  }

  // Authenticated encryption using TinyJambu-192, just a thin wrapper on top of
  // https://github.com/itzmeanjan/tinyjambu/blob/4ebaea64dfa3398aab0ee2348dd5750dc70f6059/include/tinyjambu_192.hpp#L7-L40
  // to ensure generation of C ABI
  static inline void tinyjambu_192_encrypt(
    const uint8_t* const __restrict key,   // 24 -bytes secret key
    const uint8_t* const __restrict nonce, // 12 -bytes nonce
    const uint8_t* const __restrict data,  // N -bytes associated data
    const size_t d_len,                    // N = len(data) | N >= 0
    const uint8_t* const __restrict text,  // M -bytes plain text
    uint8_t* const __restrict enc,         // M -bytes cipher text
    const size_t ct_len,                   // M = len(text) or len(enc) | M >= 0
    uint8_t* const __restrict tag          // 8 -bytes authentication tag
  )
  {
    tinyjambu_192::encrypt(key, nonce, data, d_len, text, enc, ct_len, tag);
  }

  // Verified decryption using TinyJambu-192, just a thin wrapper on top of
  // https://github.com/itzmeanjan/tinyjambu/blob/4ebaea64dfa3398aab0ee2348dd5750dc70f6059/include/tinyjambu_192.hpp#L42-L85
  // to ensure generation of C ABI
  static inline bool tinyjambu_192_decrypt(
    const uint8_t* const __restrict key,   // 24 -bytes secret key
    const uint8_t* const __restrict nonce, // 12 -bytes nonce
    const uint8_t* const __restrict tag,   // 8 -bytes authentication tag
    const uint8_t* const __restrict data,  // N -bytes associated data
    const size_t d_len,                    // N = len(data) | N >= 0
    const uint8_t* const __restrict enc,   // M -bytes cipher text
    uint8_t* const __restrict dec,         // M -bytes decrypted text
    const size_t ct_len                    // M = len(enc) or len(dec) | M >= 0
  )
  {
    using namespace tinyjambu_192;

    const bool f = decrypt(key, nonce, tag, data, d_len, enc, dec, ct_len);
    return f;
  }

  // Authenticated encryption using TinyJambu-256, just a thin wrapper on top of
  // https://github.com/itzmeanjan/tinyjambu/blob/4ebaea64dfa3398aab0ee2348dd5750dc70f6059/include/tinyjambu_256.hpp#L7-L40
  // to ensure generation of C ABI
  static inline void tinyjambu_256_encrypt(
    const uint8_t* const __restrict key,   // 32 -bytes secret key
    const uint8_t* const __restrict nonce, // 12 -bytes nonce
    const uint8_t* const __restrict data,  // N -bytes associated data
    const size_t d_len,                    // N = len(data) | N >= 0
    const uint8_t* const __restrict text,  // M -bytes plain text
    uint8_t* const __restrict enc,         // M -bytes cipher text
    const size_t ct_len,                   // M = len(text) or len(enc) | M >= 0
    uint8_t* const __restrict tag          // 8 -bytes authentication tag
  )
  {
    tinyjambu_256::encrypt(key, nonce, data, d_len, text, enc, ct_len, tag);
  }

  // Verified decryption using TinyJambu-256, just a thin wrapper on top of
  // https://github.com/itzmeanjan/tinyjambu/blob/4ebaea64dfa3398aab0ee2348dd5750dc70f6059/include/tinyjambu_256.hpp#L42-L85
  // to ensure generation of C ABI
  static inline bool tinyjambu_256_decrypt(
    const uint8_t* const __restrict key,   // 32 -bytes secret key
    const uint8_t* const __restrict nonce, // 12 -bytes nonce
    const uint8_t* const __restrict tag,   // 8 -bytes authentication tag
    const uint8_t* const __restrict data,  // N -bytes associated data
    const size_t d_len,                    // N = len(data) | N >= 0
    const uint8_t* const __restrict enc,   // M -bytes cipher text
    uint8_t* const __restrict dec,         // M -bytes decrypted text
    const size_t ct_len                    // M = len(enc) or len(dec) | M >= 0
  )
  {
    using namespace tinyjambu_256;

    const bool f = decrypt(key, nonce, tag, data, d_len, enc, dec, ct_len);
    return f;
  }
}
