#pragma once
#include "tinyjambu_256.hpp"
#include <benchmark/benchmark.h>
#include <string.h>

// Benchmark TinyJambu-256 authenticated encryption routine
void
tinyjambu_256_encrypt(benchmark::State& state)
{
  const size_t ct_len = state.range(0);
  const size_t dt_len = state.range(1);

  // acquire memory resources
  uint8_t* text = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* enc = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* dec = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* data = static_cast<uint8_t*>(malloc(dt_len));
  uint8_t* key = static_cast<uint8_t*>(malloc(32));
  uint8_t* nonce = static_cast<uint8_t*>(malloc(12));
  uint8_t* tag = static_cast<uint8_t*>(malloc(8));

  // random plain text bytes
  random_data(text, ct_len);
  // random associated data bytes
  random_data(data, dt_len);
  // random secret key ( = 256 -bit )
  random_data(key, 32);
  // random public message nonce ( = 96 -bit )
  random_data(nonce, 12);

  for (auto _ : state) {
    tinyjambu_256::encrypt(key, nonce, data, dt_len, text, enc, ct_len, tag);

    benchmark::DoNotOptimize(key);
    benchmark::DoNotOptimize(nonce);
    benchmark::DoNotOptimize(data);
    benchmark::DoNotOptimize(text);
    benchmark::DoNotOptimize(enc);
    benchmark::DoNotOptimize(tag);
    benchmark::ClobberMemory();
  }

  bool flg = false;
  flg = tinyjambu_256::decrypt(key, nonce, tag, data, dt_len, enc, dec, ct_len);
  assert(flg);

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

// Benchmark TinyJambu-256 verified decryption routine
void
tinyjambu_256_decrypt(benchmark::State& state)
{
  const size_t ct_len = state.range(0);
  const size_t dt_len = state.range(1);

  // acquire memory resources
  uint8_t* text = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* enc = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* dec = static_cast<uint8_t*>(malloc(ct_len));
  uint8_t* data = static_cast<uint8_t*>(malloc(dt_len));
  uint8_t* key = static_cast<uint8_t*>(malloc(32));
  uint8_t* nonce = static_cast<uint8_t*>(malloc(12));
  uint8_t* tag = static_cast<uint8_t*>(malloc(8));

  // random plain text bytes
  random_data(text, ct_len);
  // random associated data bytes
  random_data(data, dt_len);
  // random secret key ( = 256 -bit )
  random_data(key, 32);
  // random public message nonce ( = 96 -bit )
  random_data(nonce, 12);

  tinyjambu_256::encrypt(key, nonce, data, dt_len, text, enc, ct_len, tag);

  for (auto _ : state) {
    using namespace tinyjambu_256;
    using namespace benchmark;

    bool flg = false;
    flg = decrypt(key, nonce, tag, data, dt_len, enc, dec, ct_len);
    assert(flg);

    DoNotOptimize(key);
    DoNotOptimize(nonce);
    DoNotOptimize(tag);
    DoNotOptimize(data);
    DoNotOptimize(enc);
    DoNotOptimize(dec);
    benchmark::ClobberMemory();
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
