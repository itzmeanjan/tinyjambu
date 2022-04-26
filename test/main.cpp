#include "test_tinyjambu_128.hpp"
#include "test_tinyjambu_192.hpp"
#include "test_tinyjambu_256.hpp"
#include <iostream>

int
main()
{
  constexpr size_t MIN_DT_LEN = 0ul;
  constexpr size_t MAX_DT_LEN = 32ul;
  constexpr size_t MIN_CT_LEN = 0ul;
  constexpr size_t MAX_CT_LEN = 64ul;

  for (size_t i = MIN_CT_LEN; i < MAX_CT_LEN; i++) {
    for (size_t j = MIN_DT_LEN; j < MAX_DT_LEN; j++) {
      test_tinyjambu::key_128(j, i, test_tinyjambu::mutate_t::key);
      test_tinyjambu::key_128(j, i, test_tinyjambu::mutate_t::nonce);
      test_tinyjambu::key_128(j, i, test_tinyjambu::mutate_t::tag);
      test_tinyjambu::key_128(j, i, test_tinyjambu::mutate_t::data);
      test_tinyjambu::key_128(j, i, test_tinyjambu::mutate_t::enc);
      test_tinyjambu::key_128(j, i, test_tinyjambu::mutate_t::none);
    }
  }

  std::cout << "[test] passed TinyJambu-128 AEAD" << std::endl;

  for (size_t i = MIN_CT_LEN; i < MAX_CT_LEN; i++) {
    for (size_t j = MIN_DT_LEN; j < MAX_DT_LEN; j++) {
      test_tinyjambu::key_192(j, i, test_tinyjambu::mutate_t::key);
      test_tinyjambu::key_192(j, i, test_tinyjambu::mutate_t::nonce);
      test_tinyjambu::key_192(j, i, test_tinyjambu::mutate_t::tag);
      test_tinyjambu::key_192(j, i, test_tinyjambu::mutate_t::data);
      test_tinyjambu::key_192(j, i, test_tinyjambu::mutate_t::enc);
      test_tinyjambu::key_192(j, i, test_tinyjambu::mutate_t::none);
    }
  }

  std::cout << "[test] passed TinyJambu-192 AEAD" << std::endl;

  for (size_t i = MIN_CT_LEN; i < MAX_CT_LEN; i++) {
    for (size_t j = MIN_DT_LEN; j < MAX_DT_LEN; j++) {
      test_tinyjambu::key_256(j, i, test_tinyjambu::mutate_t::key);
      test_tinyjambu::key_256(j, i, test_tinyjambu::mutate_t::nonce);
      test_tinyjambu::key_256(j, i, test_tinyjambu::mutate_t::tag);
      test_tinyjambu::key_256(j, i, test_tinyjambu::mutate_t::data);
      test_tinyjambu::key_256(j, i, test_tinyjambu::mutate_t::enc);
      test_tinyjambu::key_256(j, i, test_tinyjambu::mutate_t::none);
    }
  }

  std::cout << "[test] passed TinyJambu-256 AEAD" << std::endl;

  return EXIT_SUCCESS;
}
