#pragma once
#include <cstddef>
#include <cstdint>

#if defined FBK_32
#pragma message("Computing 32 feedback bits in-parallel [REQUESTED]")
#elif defined FBK_128
#pragma message("Computing 128 feedback bits in-parallel [REQUESTED]")
#else
#define FBK_32
#pragma message("Computing 32 feedback bits in-parallel [DEFAULT]")
#endif

#if defined FBK_32
// Compile time check to ensure that 32 feedback bits can be safely computed per
// iteration round in following `state_update` function(s)
constexpr bool
check_rounds(const size_t rounds)
{
  return (rounds & 31ul) == 0ul;
}
#elif defined FBK_128
// Compile time check to ensure that 128 feedback bits can be safely computed
// per iteration round in following `state_update` function(s)
constexpr bool
check_rounds(const size_t rounds)
{
  return (rounds & 127ul) == 0ul;
}
#endif

// TinyJambu-128 Authenticated Encryption with Associated Data Implementation
namespace tinyjambu_128 {

// TinyJambu-128 `StateUpdate` function, computing 32/ 128 feedback bits ( based
// on compile-time decision ) during each iteration ( see inside body of for
// loop )
//
// Note, this function will update state of 128 -bit Non-Linear Feedback Shift
// Register `rounds` -many time; so ensure that ( a compile-time check is
// in-place )
//
// assert rounds % 32 == 0 or assert rounds % 128 == 0
//
// See section 3.2.3 in TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const size_t rounds>
static inline void
state_update(uint32_t* const __restrict state,    // 128 -bit permutation state
             const uint32_t* const __restrict key // 128 -bit secret key
             )
  requires(check_rounds(rounds))
{
#if defined FBK_32
  constexpr size_t itr_cnt = rounds >> 5;

  for (size_t i = 0; i < itr_cnt; i++) {
    const uint32_t s47 = (state[2] << 17) | (state[1] >> 15);
    const uint32_t s70 = (state[3] << 26) | (state[2] >> 6);
    const uint32_t s85 = (state[3] << 11) | (state[2] >> 21);
    const uint32_t s91 = (state[3] << 5) | (state[2] >> 27);

    // computed 32 feedback bits
    const uint32_t fbk = state[0] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[i & 3ul];

    state[0] = state[1];
    state[1] = state[2];
    state[2] = state[3];
    state[3] = fbk;
  }
#elif defined FBK_128
  constexpr size_t itr_cnt = rounds >> 7;

  for (size_t i = 0; i < itr_cnt; i++) {
    {
      const uint32_t s47 = (state[2] << 17) | (state[1] >> 15);
      const uint32_t s70 = (state[3] << 26) | (state[2] >> 6);
      const uint32_t s85 = (state[3] << 11) | (state[2] >> 21);
      const uint32_t s91 = (state[3] << 5) | (state[2] >> 27);

      state[0] = state[0] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[0];
    }
    {
      const uint32_t s47 = (state[3] << 17) | (state[2] >> 15);
      const uint32_t s70 = (state[0] << 26) | (state[3] >> 6);
      const uint32_t s85 = (state[0] << 11) | (state[3] >> 21);
      const uint32_t s91 = (state[0] << 5) | (state[3] >> 27);

      state[1] = state[1] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[1];
    }
    {
      const uint32_t s47 = (state[0] << 17) | (state[3] >> 15);
      const uint32_t s70 = (state[1] << 26) | (state[0] >> 6);
      const uint32_t s85 = (state[1] << 11) | (state[0] >> 21);
      const uint32_t s91 = (state[1] << 5) | (state[0] >> 27);

      state[2] = state[2] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[2];
    }
    {
      const uint32_t s47 = (state[1] << 17) | (state[0] >> 15);
      const uint32_t s70 = (state[2] << 26) | (state[1] >> 6);
      const uint32_t s85 = (state[2] << 11) | (state[1] >> 21);
      const uint32_t s91 = (state[2] << 5) | (state[1] >> 27);

      state[3] = state[3] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[3];
    }
  }
#endif
}

}

// TinyJambu-192 Authenticated Encryption with Associated Data Implementation
namespace tinyjambu_192 {

// TinyJambu-192 `StateUpdate` function, computing 32/ 128 feedback bits ( based
// on compile-time decision ) during each iteration ( see inside body of for
// loop )
//
// Note, this function will update state of 128 -bit Non-Linear Feedback Shift
// Register `rounds` -many time; so ensure that ( a compile-time check is
// in-place )
//
// assert rounds % 32 == 0 or assert rounds % 128 == 0
//
// See section 3.2.3 in TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const size_t rounds>
static inline void
state_update(uint32_t* const __restrict state,    // 128 -bit permutation state
             const uint32_t* const __restrict key // 192 -bit secret key
             )
  requires(check_rounds(rounds))
{
#if defined FBK_32
  constexpr size_t itr_cnt = rounds >> 5;

  for (size_t i = 0; i < itr_cnt; i++) {
    const size_t j = i % 6ul;

    const uint32_t s47 = (state[2] << 17) | (state[1] >> 15);
    const uint32_t s70 = (state[3] << 26) | (state[2] >> 6);
    const uint32_t s85 = (state[3] << 11) | (state[2] >> 21);
    const uint32_t s91 = (state[3] << 5) | (state[2] >> 27);

    // computed 32 feedback bits
    const uint32_t fbk = state[0] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[j];

    state[0] = state[1];
    state[1] = state[2];
    state[2] = state[3];
    state[3] = fbk;
  }
#elif defined FBK_128
  constexpr size_t itr_cnt = rounds >> 7;

  for (size_t i = 0, j = 0; i < itr_cnt; i++) {
    {
      const uint32_t s47 = (state[2] << 17) | (state[1] >> 15);
      const uint32_t s70 = (state[3] << 26) | (state[2] >> 6);
      const uint32_t s85 = (state[3] << 11) | (state[2] >> 21);
      const uint32_t s91 = (state[3] << 5) | (state[2] >> 27);

      state[0] = state[0] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[j++];
    }
    {
      const uint32_t s47 = (state[3] << 17) | (state[2] >> 15);
      const uint32_t s70 = (state[0] << 26) | (state[3] >> 6);
      const uint32_t s85 = (state[0] << 11) | (state[3] >> 21);
      const uint32_t s91 = (state[0] << 5) | (state[3] >> 27);

      state[1] = state[1] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[j++];
    }
    if (j == 6) {
      j = 0;
    }
    {
      const uint32_t s47 = (state[0] << 17) | (state[3] >> 15);
      const uint32_t s70 = (state[1] << 26) | (state[0] >> 6);
      const uint32_t s85 = (state[1] << 11) | (state[0] >> 21);
      const uint32_t s91 = (state[1] << 5) | (state[0] >> 27);

      state[2] = state[2] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[j++];
    }
    {
      const uint32_t s47 = (state[1] << 17) | (state[0] >> 15);
      const uint32_t s70 = (state[2] << 26) | (state[1] >> 6);
      const uint32_t s85 = (state[2] << 11) | (state[1] >> 21);
      const uint32_t s91 = (state[2] << 5) | (state[1] >> 27);

      state[3] = state[3] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[j++];
    }
    if (j == 6) {
      j = 0;
    }
  }
#endif
}

}

// TinyJambu-256 Authenticated Encryption with Associated Data Implementation
namespace tinyjambu_256 {

// TinyJambu-256 `StateUpdate` function, computing 32/ 128 feedback bits ( based
// on compile-time decision ) during each iteration ( see inside body of
// for loop )
//
// Note, this function will update state of 128 -bit Non-Linear Feedback Shift
// Register `rounds` -many time; so ensure that ( a compile-time check is
// in-place )
//
// assert rounds % 32 == 0 or assert rounds % 128 == 0
//
// See section 3.2.3 in TinyJambu specification
// https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf
template<const size_t rounds>
static inline void
state_update(uint32_t* const __restrict state,    // 128 -bit permutation state
             const uint32_t* const __restrict key // 256 -bit secret key
             )
  requires(check_rounds(rounds))
{
#if defined FBK_32
  constexpr size_t itr_cnt = rounds >> 5;

  for (size_t i = 0; i < itr_cnt; i++) {
    const uint32_t s47 = (state[2] << 17) | (state[1] >> 15);
    const uint32_t s70 = (state[3] << 26) | (state[2] >> 6);
    const uint32_t s85 = (state[3] << 11) | (state[2] >> 21);
    const uint32_t s91 = (state[3] << 5) | (state[2] >> 27);

    // computed 32 feedback bits
    const uint32_t fbk = state[0] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[i & 7ul];

    state[0] = state[1];
    state[1] = state[2];
    state[2] = state[3];
    state[3] = fbk;
  }
#elif defined FBK_128
  constexpr size_t itr_cnt = rounds >> 7;

  for (size_t i = 0, j = 0; i < itr_cnt; i++, j += 4) {
    {
      const uint32_t s47 = (state[2] << 17) | (state[1] >> 15);
      const uint32_t s70 = (state[3] << 26) | (state[2] >> 6);
      const uint32_t s85 = (state[3] << 11) | (state[2] >> 21);
      const uint32_t s91 = (state[3] << 5) | (state[2] >> 27);

      state[0] = state[0] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[(j ^ 0ul) & 7ul];
    }
    {
      const uint32_t s47 = (state[3] << 17) | (state[2] >> 15);
      const uint32_t s70 = (state[0] << 26) | (state[3] >> 6);
      const uint32_t s85 = (state[0] << 11) | (state[3] >> 21);
      const uint32_t s91 = (state[0] << 5) | (state[3] >> 27);

      state[1] = state[1] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[(j ^ 1ul) & 7ul];
    }
    {
      const uint32_t s47 = (state[0] << 17) | (state[3] >> 15);
      const uint32_t s70 = (state[1] << 26) | (state[0] >> 6);
      const uint32_t s85 = (state[1] << 11) | (state[0] >> 21);
      const uint32_t s91 = (state[1] << 5) | (state[0] >> 27);

      state[2] = state[2] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[(j ^ 2ul) & 7ul];
    }
    {
      const uint32_t s47 = (state[1] << 17) | (state[0] >> 15);
      const uint32_t s70 = (state[2] << 26) | (state[1] >> 6);
      const uint32_t s85 = (state[2] << 11) | (state[1] >> 21);
      const uint32_t s91 = (state[2] << 5) | (state[1] >> 27);

      state[3] = state[3] ^ s47 ^ (~(s70 & s85)) ^ s91 ^ key[(j ^ 3ul) & 7ul];
    }
  }
#endif
}

}
