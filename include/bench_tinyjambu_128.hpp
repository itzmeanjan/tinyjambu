#pragma once
#include "tinyjambu_128.hpp"
#include <benchmark/benchmark.h>
#include <string.h>

// Benchmark TinyJambu-128 authenticated encryption routine
static void
tinyjambu_128_encrypt(benchmark::State& state,
                      const size_t ct_len,
                      const size_t dt_len)
{
  // acquire memory resources
  uint8_t* text = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* enc = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* data = static_cast<uint8_t*>(malloc(dt_len));
  uint8_t* key = static_cast<uint8_t*>(malloc(16));
  uint8_t* nonce = static_cast<uint8_t*>(malloc(12));
  uint8_t* tag = static_cast<uint8_t*>(malloc(8));

  // random plain text bytes
  random_data(text, ct_len);
  // random associated data bytes
  random_data(data, dt_len);
  // random secret key ( = 128 -bit )
  random_data(key, 16);
  // random public message nonce ( = 96 -bit )
  random_data(nonce, 12);

  memset(enc, 0, ct_len);
  memset(tag, 0, 8);

  size_t itr = 0;
  for (auto _ : state) {
    tinyjambu_128::encrypt(key, nonce, data, dt_len, text, enc, ct_len, tag);

    benchmark::DoNotOptimize(enc);
    benchmark::DoNotOptimize(tag);
    benchmark::DoNotOptimize(itr++);
  }

  state.SetBytesProcessed(static_cast<int64_t>((dt_len + ct_len) * itr));
  state.SetItemsProcessed(static_cast<int64_t>(itr));

  // deallocate all resources
  free(text);
  free(enc);
  free(data);
  free(key);
  free(nonce);
  free(tag);
}

// Benchmark TinyJambu-128 verified decryption routine
static void
tinyjambu_128_decrypt(benchmark::State& state,
                      const size_t ct_len,
                      const size_t dt_len)
{
  // acquire memory resources
  uint8_t* text = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* enc = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* dec = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* data = static_cast<uint8_t*>(malloc(dt_len));
  uint8_t* key = static_cast<uint8_t*>(malloc(16));
  uint8_t* nonce = static_cast<uint8_t*>(malloc(12));
  uint8_t* tag = static_cast<uint8_t*>(malloc(8));

  // random plain text bytes
  random_data(text, ct_len);
  // random associated data bytes
  random_data(data, dt_len);
  // random secret key ( = 128 -bit )
  random_data(key, 16);
  // random public message nonce ( = 96 -bit )
  random_data(nonce, 12);

  memset(enc, 0, ct_len);
  memset(dec, 0, ct_len);
  memset(tag, 0, 8);

  tinyjambu_128::encrypt(key, nonce, data, dt_len, text, enc, ct_len, tag);

  size_t itr = 0;
  for (auto _ : state) {
    using namespace tinyjambu_128;
    using namespace benchmark;

    DoNotOptimize(decrypt(key, nonce, tag, data, dt_len, enc, dec, ct_len));
    DoNotOptimize(dec);
    DoNotOptimize(itr++);
  }

  state.SetBytesProcessed(static_cast<int64_t>((dt_len + ct_len) * itr));
  state.SetItemsProcessed(static_cast<int64_t>(itr));

  // deallocate all resources
  free(text);
  free(enc);
  free(dec);
  free(data);
  free(key);
  free(nonce);
  free(tag);
}

// Benchmark TinyJambu-128 encrypt routine with 64 -bytes plain text & 32 -bytes
// associated data
static void
tinyjambu_128_encrypt_64B_32B(benchmark::State& state)
{
  tinyjambu_128_encrypt(state, 64ul, 32ul);
}

// Benchmark TinyJambu-128 encrypt routine with 128 -bytes plain text & 32
// -bytes associated data
static void
tinyjambu_128_encrypt_128B_32B(benchmark::State& state)
{
  tinyjambu_128_encrypt(state, 128ul, 32ul);
}

// Benchmark TinyJambu-128 encrypt routine with 256 -bytes plain text & 32
// -bytes associated data
static void
tinyjambu_128_encrypt_256B_32B(benchmark::State& state)
{
  tinyjambu_128_encrypt(state, 256ul, 32ul);
}

// Benchmark TinyJambu-128 encrypt routine with 512 -bytes plain text & 32
// -bytes associated data
static void
tinyjambu_128_encrypt_512B_32B(benchmark::State& state)
{
  tinyjambu_128_encrypt(state, 512ul, 32ul);
}

// Benchmark TinyJambu-128 encrypt routine with 1024 -bytes plain text & 32
// -bytes associated data
static void
tinyjambu_128_encrypt_1024B_32B(benchmark::State& state)
{
  tinyjambu_128_encrypt(state, 1024ul, 32ul);
}

// Benchmark TinyJambu-128 encrypt routine with 2048 -bytes plain text & 32
// -bytes associated data
static void
tinyjambu_128_encrypt_2048B_32B(benchmark::State& state)
{
  tinyjambu_128_encrypt(state, 2048ul, 32ul);
}

// Benchmark TinyJambu-128 encrypt routine with 4096 -bytes plain text & 32
// -bytes associated data
static void
tinyjambu_128_encrypt_4096B_32B(benchmark::State& state)
{
  tinyjambu_128_encrypt(state, 4096ul, 32ul);
}

// Benchmark TinyJambu-128 decrypt routine with 64 -bytes cipher text & 32
// -bytes associated data
static void
tinyjambu_128_decrypt_64B_32B(benchmark::State& state)
{
  tinyjambu_128_decrypt(state, 64ul, 32ul);
}

// Benchmark TinyJambu-128 decrypt routine with 128 -bytes cipher text & 32
// -bytes associated data
static void
tinyjambu_128_decrypt_128B_32B(benchmark::State& state)
{
  tinyjambu_128_decrypt(state, 128ul, 32ul);
}

// Benchmark TinyJambu-128 decrypt routine with 256 -bytes cipher text & 32
// -bytes associated data
static void
tinyjambu_128_decrypt_256B_32B(benchmark::State& state)
{
  tinyjambu_128_decrypt(state, 256ul, 32ul);
}

// Benchmark TinyJambu-128 decrypt routine with 512 -bytes cipher text & 32
// -bytes associated data
static void
tinyjambu_128_decrypt_512B_32B(benchmark::State& state)
{
  tinyjambu_128_decrypt(state, 512ul, 32ul);
}

// Benchmark TinyJambu-128 decrypt routine with 1024 -bytes cipher text & 32
// -bytes associated data
static void
tinyjambu_128_decrypt_1024B_32B(benchmark::State& state)
{
  tinyjambu_128_decrypt(state, 1024ul, 32ul);
}

// Benchmark TinyJambu-128 decrypt routine with 2048 -bytes cipher text & 32
// -bytes associated data
static void
tinyjambu_128_decrypt_2048B_32B(benchmark::State& state)
{
  tinyjambu_128_decrypt(state, 2048ul, 32ul);
}

// Benchmark TinyJambu-128 decrypt routine with 4096 -bytes cipher text & 32
// -bytes associated data
static void
tinyjambu_128_decrypt_4096B_32B(benchmark::State& state)
{
  tinyjambu_128_decrypt(state, 4096ul, 32ul);
}