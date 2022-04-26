#include "test_tinyjambu_128.hpp"
#include <iostream>

int
main()
{
  constexpr size_t MIN_DT_LEN = 0ul;
  constexpr size_t MAX_DT_LEN = 32ul;
  constexpr size_t MIN_CT_LEN = 0ul;
  constexpr size_t MAX_CT_LEN = 32ul;

  for (size_t i = MIN_CT_LEN; i < MAX_CT_LEN; i++) {
    for (size_t j = MIN_DT_LEN; j < MAX_DT_LEN; j++) {
      test_tinyjambu::key_128(j, i);
    }
  }

  std::cout << "[test] passed TinyJambu-128 AEAD" << std::endl;

  return EXIT_SUCCESS;
}
