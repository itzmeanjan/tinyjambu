#pragma once
#include <cstdint>

using size_t = std::size_t;

// TinyJambu-128 Authenticated Encryption with Associated Data Implementation
namespace tinyjambu_128 {

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

}
