#pragma once
#include "tinyjambu_128.hpp"
#include <benchmark/benchmark.h>
#include <string.h>

// Benchmark TinyJambu-128 authenticated encryption routine
static void
tinyjambu_128_encrypt(benchmark::State& state)
{
  const size_t ct_len = state.range(0);
  const size_t dt_len = state.range(1);

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

  for (auto _ : state) {
    tinyjambu_128::encrypt(key, nonce, data, dt_len, text, enc, ct_len, tag);

    benchmark::DoNotOptimize(enc);
    benchmark::DoNotOptimize(tag);
  }

  const size_t per_itr_data = dt_len + ct_len;
  const size_t total_data = per_itr_data * state.iterations();

  state.SetBytesProcessed(static_cast<int64_t>(total_data));

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
tinyjambu_128_decrypt(benchmark::State& state)
{
  const size_t ct_len = state.range(0);
  const size_t dt_len = state.range(1);

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

  for (auto _ : state) {
    using namespace tinyjambu_128;
    using namespace benchmark;

    DoNotOptimize(decrypt(key, nonce, tag, data, dt_len, enc, dec, ct_len));
    DoNotOptimize(dec);
  }

  const size_t per_itr_data = dt_len + ct_len;
  const size_t total_data = per_itr_data * state.iterations();

  state.SetBytesProcessed(static_cast<int64_t>(total_data));

  // deallocate all resources
  free(text);
  free(enc);
  free(dec);
  free(data);
  free(key);
  free(nonce);
  free(tag);
}
