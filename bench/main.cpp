#include "bench_tinyjambu_128.hpp"
#include "bench_tinyjambu_192.hpp"
#include "bench_tinyjambu_256.hpp"

// Register for benchmarking
//
// Note, associated data size is kept constant for all benchmark cases !

// TinyJambu-128 encrypt/ decrypt routines
BENCHMARK(tinyjambu_128_encrypt)->Args({ 64, 32 });
BENCHMARK(tinyjambu_128_decrypt)->Args({ 64, 32 });

BENCHMARK(tinyjambu_128_encrypt)->Args({ 128, 32 });
BENCHMARK(tinyjambu_128_decrypt)->Args({ 128, 32 });

BENCHMARK(tinyjambu_128_encrypt)->Args({ 256, 32 });
BENCHMARK(tinyjambu_128_decrypt)->Args({ 256, 32 });

BENCHMARK(tinyjambu_128_encrypt)->Args({ 512, 32 });
BENCHMARK(tinyjambu_128_decrypt)->Args({ 512, 32 });

BENCHMARK(tinyjambu_128_encrypt)->Args({ 1024, 32 });
BENCHMARK(tinyjambu_128_decrypt)->Args({ 1024, 32 });

BENCHMARK(tinyjambu_128_encrypt)->Args({ 2048, 32 });
BENCHMARK(tinyjambu_128_decrypt)->Args({ 2048, 32 });

BENCHMARK(tinyjambu_128_encrypt)->Args({ 4096, 32 });
BENCHMARK(tinyjambu_128_decrypt)->Args({ 4096, 32 });

// TinyJambu-192 encrypt/ decrypt routines
BENCHMARK(tinyjambu_192_encrypt)->Args({ 64, 32 });
BENCHMARK(tinyjambu_192_decrypt)->Args({ 64, 32 });

BENCHMARK(tinyjambu_192_encrypt)->Args({ 128, 32 });
BENCHMARK(tinyjambu_192_decrypt)->Args({ 128, 32 });

BENCHMARK(tinyjambu_192_encrypt)->Args({ 256, 32 });
BENCHMARK(tinyjambu_192_decrypt)->Args({ 256, 32 });

BENCHMARK(tinyjambu_192_encrypt)->Args({ 512, 32 });
BENCHMARK(tinyjambu_192_decrypt)->Args({ 512, 32 });

BENCHMARK(tinyjambu_192_encrypt)->Args({ 1024, 32 });
BENCHMARK(tinyjambu_192_decrypt)->Args({ 1024, 32 });

BENCHMARK(tinyjambu_192_encrypt)->Args({ 2048, 32 });
BENCHMARK(tinyjambu_192_decrypt)->Args({ 2048, 32 });

BENCHMARK(tinyjambu_192_encrypt)->Args({ 4096, 32 });
BENCHMARK(tinyjambu_192_decrypt)->Args({ 4096, 32 });

// TinyJambu-256 encrypt/ decrypt routines
BENCHMARK(tinyjambu_256_encrypt)->Args({ 64, 32 });
BENCHMARK(tinyjambu_256_decrypt)->Args({ 64, 32 });

BENCHMARK(tinyjambu_256_encrypt)->Args({ 128, 32 });
BENCHMARK(tinyjambu_256_decrypt)->Args({ 128, 32 });

BENCHMARK(tinyjambu_256_encrypt)->Args({ 256, 32 });
BENCHMARK(tinyjambu_256_decrypt)->Args({ 256, 32 });

BENCHMARK(tinyjambu_256_encrypt)->Args({ 512, 32 });
BENCHMARK(tinyjambu_256_decrypt)->Args({ 512, 32 });

BENCHMARK(tinyjambu_256_encrypt)->Args({ 1024, 32 });
BENCHMARK(tinyjambu_256_decrypt)->Args({ 1024, 32 });

BENCHMARK(tinyjambu_256_encrypt)->Args({ 2048, 32 });
BENCHMARK(tinyjambu_256_decrypt)->Args({ 2048, 32 });

BENCHMARK(tinyjambu_256_encrypt)->Args({ 4096, 32 });
BENCHMARK(tinyjambu_256_decrypt)->Args({ 4096, 32 });

// main function to make it executable
BENCHMARK_MAIN();
