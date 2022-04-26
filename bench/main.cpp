#include "bench_tinyjambu_128.hpp"
#include "bench_tinyjambu_192.hpp"
#include "bench_tinyjambu_256.hpp"

// Register for benchmarking
//
// Note, associated data size is kept constant for all benchmark cases !

// TinyJambu-128 encrypt routine
BENCHMARK(tinyjambu_128_encrypt_64B_32B);
BENCHMARK(tinyjambu_128_encrypt_128B_32B);
BENCHMARK(tinyjambu_128_encrypt_256B_32B);
BENCHMARK(tinyjambu_128_encrypt_512B_32B);
BENCHMARK(tinyjambu_128_encrypt_1024B_32B);
BENCHMARK(tinyjambu_128_encrypt_2048B_32B);
BENCHMARK(tinyjambu_128_encrypt_4096B_32B);

// TinyJambu-192 encrypt routine
BENCHMARK(tinyjambu_192_encrypt_64B_32B);
BENCHMARK(tinyjambu_192_encrypt_128B_32B);
BENCHMARK(tinyjambu_192_encrypt_256B_32B);
BENCHMARK(tinyjambu_192_encrypt_512B_32B);
BENCHMARK(tinyjambu_192_encrypt_1024B_32B);
BENCHMARK(tinyjambu_192_encrypt_2048B_32B);
BENCHMARK(tinyjambu_192_encrypt_4096B_32B);

// TinyJambu-256 encrypt routine
BENCHMARK(tinyjambu_256_encrypt_64B_32B);
BENCHMARK(tinyjambu_256_encrypt_128B_32B);
BENCHMARK(tinyjambu_256_encrypt_256B_32B);
BENCHMARK(tinyjambu_256_encrypt_512B_32B);
BENCHMARK(tinyjambu_256_encrypt_1024B_32B);
BENCHMARK(tinyjambu_256_encrypt_2048B_32B);
BENCHMARK(tinyjambu_256_encrypt_4096B_32B);

// ---

// TinyJambu-128 decrypt routine
BENCHMARK(tinyjambu_128_decrypt_64B_32B);
BENCHMARK(tinyjambu_128_decrypt_128B_32B);
BENCHMARK(tinyjambu_128_decrypt_256B_32B);
BENCHMARK(tinyjambu_128_decrypt_512B_32B);
BENCHMARK(tinyjambu_128_decrypt_1024B_32B);
BENCHMARK(tinyjambu_128_decrypt_2048B_32B);
BENCHMARK(tinyjambu_128_decrypt_4096B_32B);

// TinyJambu-192 decrypt routine
BENCHMARK(tinyjambu_192_decrypt_64B_32B);
BENCHMARK(tinyjambu_192_decrypt_128B_32B);
BENCHMARK(tinyjambu_192_decrypt_256B_32B);
BENCHMARK(tinyjambu_192_decrypt_512B_32B);
BENCHMARK(tinyjambu_192_decrypt_1024B_32B);
BENCHMARK(tinyjambu_192_decrypt_2048B_32B);
BENCHMARK(tinyjambu_192_decrypt_4096B_32B);

// TinyJambu-256 decrypt routine
BENCHMARK(tinyjambu_256_decrypt_64B_32B);
BENCHMARK(tinyjambu_256_decrypt_128B_32B);
BENCHMARK(tinyjambu_256_decrypt_256B_32B);
BENCHMARK(tinyjambu_256_decrypt_512B_32B);
BENCHMARK(tinyjambu_256_decrypt_1024B_32B);
BENCHMARK(tinyjambu_256_decrypt_2048B_32B);
BENCHMARK(tinyjambu_256_decrypt_4096B_32B);

// main function to make it executable
BENCHMARK_MAIN();
