#include "tinyjambu_128.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

// Example demontrating how to use TinyJambu-128 AEAD C++20 header-only
// library API.
//
// Compile with
//
// g++ -Wall -std=c++20 -O3 -I ./include example/tinyjambu_128.cpp
int
main()
{
  // 128 -bit secret key
  constexpr size_t k_len = 16ul;
  // 96 -bit public message nonce
  constexpr size_t n_len = 12ul;
  // 64 -bit authentication tag
  constexpr size_t t_len = 8ul;
  // 32 -bytes associated data ( never encrypted )
  constexpr size_t dt_len = 32ul;
  // 64 -bytes plain text ( only encrypted )
  constexpr size_t ct_len = 64ul;

  // allocate memory for secret key
  uint8_t* key = static_cast<uint8_t*>(malloc(k_len));
  // allocate memory for public message nonce
  uint8_t* nonce = static_cast<uint8_t*>(malloc(n_len));
  // allocate memory for authentication tag
  uint8_t* tag = static_cast<uint8_t*>(malloc(t_len));
  // allocate memory for associated data
  uint8_t* data = static_cast<uint8_t*>(malloc(dt_len));
  // allocate memory for plain text
  uint8_t* text = static_cast<uint8_t*>(malloc(ct_len));
  // allocate memory for encrypted text
  uint8_t* enc = static_cast<uint8_t*>(malloc(ct_len));
  // allocate memory for decrypted text
  uint8_t* dec = static_cast<uint8_t*>(malloc(ct_len));

  // generate random secret key
  random_data(key, k_len);
  // generate random nonce
  random_data(nonce, n_len);
  // generate random associated data
  random_data(data, dt_len);
  // generate random plain text
  random_data(text, ct_len);

  // to be written memory locations explicitly set to zero
  memset(tag, 0, t_len);
  memset(enc, 0, ct_len);
  memset(dec, 0, ct_len);

  using namespace tinyjambu_128;

  // authenticated encryption
  encrypt(key, nonce, data, dt_len, text, enc, ct_len, tag);
  // verified decryption
  const bool f = decrypt(key, nonce, tag, data, dt_len, enc, dec, ct_len);

  // verify authenticity & integrity !
  assert(f);

  // byte-by-byte comparison to be sure that decrypted bytes are same as input
  // plain text bytes
  for (size_t i = 0; i < ct_len; i++) {
    assert(text[i] == dec[i]);
  }

  std::cout << "TinyJambu-128 AEAD" << std::endl << std::endl;
  std::cout << "key         : " << to_hex(key, k_len) << std::endl;
  std::cout << "nonce       : " << to_hex(nonce, n_len) << std::endl;
  std::cout << "text        : " << to_hex(text, ct_len) << std::endl;
  std::cout << "data        : " << to_hex(data, dt_len) << std::endl;
  std::cout << "ciphered    : " << to_hex(enc, ct_len) << std::endl;
  std::cout << "tag         : " << to_hex(tag, t_len) << std::endl;
  std::cout << "deciphered  : " << to_hex(dec, ct_len) << std::endl;

  // deallocate resources
  free(key);
  free(nonce);
  free(tag);
  free(data);
  free(text);
  free(enc);
  free(dec);

  return EXIT_SUCCESS;
}
