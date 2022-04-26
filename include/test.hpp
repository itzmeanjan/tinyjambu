#pragma once

// Test functional correctness of TinyJambu-{128, 192, 256} AEAD Implementation
namespace test_tinyjambu {

// Choose which one to modify ( just a single bit flip ), before attempting
// decryption, to show that TinyJambu-{128, 192, 256} AEAD provides promised
// security properties
enum mutate_t
{
  key,   // secret key
  nonce, // public message nonce
  tag,   // authentication tag
  data,  // associated data
  enc,   // encrypted data
  none   // don't modify anything !
};

}
