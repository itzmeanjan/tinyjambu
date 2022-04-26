# tinyjambu
Accelerated TinyJambu - Lightweight Authenticated Encryption Algorithms

## Overview

After working on `ascon` **A**uthenticated **E**ncryption with **A**ssociated **D**ata suite, I've taken up another final round candidate of NIST **L**ight **W**eight **C**ryptography competition `tinyjambu`, which has three authenticated encryption algorithms, variated based on secret key bit length.

- TinyJambu-128 [ works with 16 -bytes secret key ]
- TinyJambu-192 [ works with 24 -bytes secret key ]
- TinyJambu-256 [ works with 32 -bytes secret key ]

> Learn more about AEAD [here](https://en.wikipedia.org/wiki/Authenticated_encryption)

> If interested in my work on Ascon AEAD, see [here](https://github.com/itzmeanjan/ascon)

TinyJambu-{128, 192, 256} offers lightweight authenticated encryption/ verified decryption algorithms. Encryption routine expects following as input

- 16/ 24/ 32 -bytes secret key
- 12 -bytes public message nonce
- N -bytes of plain text | N >= 0
- N -bytes of associated data | N >= 0

And produces following as output

- N -bytes encrypted data | N >= 0 && len(text) == len(cipher)
- 8 -bytes authentication tag

During decryption following are expected as input

- 16/ 24/ 32 -bytes secret key
- 12 -bytes public message nonce
- 8 -bytes of authentication tag
- N -bytes encrypted data | N >= 0
- N -bytes of associated data | N >= 0

While computing below as output

- N -bytes decrypted data | N >= 0 && len(cipher) == len(text)
- Boolean flag denoting success of verification

Here I'm maintaining one easy to use, zero-dependency ( excluding C++20 standard library ), header-only C++ library, which implements all three variants of TinyJambu.

> During implementation, I followed [this](https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf) specification of TinyJambu.

## Prerequisites

- C++ compilers like `dpcpp`/ `clang++`/ `g++`, along C++ standard library, implementing C++20 specification

> I'm using 

```bash
$ dpcpp --version

Intel(R) oneAPI DPC++/C++ Compiler 2022.0.0 (2022.0.0.20211123)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/intel/oneapi/compiler/2022.0.2/linux/bin-llvm
```

- System development utilities like `make`, `cmake`

> I'm using

```bash
$ make --version

GNU Make 4.3
```

```bash
$ cmake --version

cmake version 3.22.1
```

- For benchmarking TinyJambu-{128, 192, 256} on CPU -based systems, you'll need to have `google-benchmark` library globally installed; see [this](https://github.com/google/benchmark/tree/60b16f1#installation)

## Testing

For ensuring functional correctness of TinyJambu-{128, 192, 256} AEAD, I've written following test cases

- Given randomly generated secret key, public message nonce, plain text bytes ( length >= 0 ) & associated data bytes ( length >= 0 )
    - first, compute encrypted bytes & authentication tag
    - then, attempt to decrypt, ensure that verification passes, while also going through one byte-by-byte comparison
- Given randomly generated secret key, public message nonce, plain text bytes ( length >= 0 ) & associated data bytes ( length >= 0 )
    - first, compute encrypted bytes & authentication tag
    - then, mutate ( just a single bit flip ) either of secret key/ public message nonce/ authentication tag/ encrypted bytes/ associated data
    - finally, attempt to decrypt, authentication tag verification must fail !

Issue following command(s) to run test cases on all variants of TinyJambu

- While computing 32 feedback bits in-parallel

```bash
FBK=32 make
```

- When interested in testing variant, where 128 feedback bits are computed in-parallel

```bash
FBK=128 make
```

> You may safely skip specifying `FBK`, default choice `FBK=32` is automatically set !

## Benchmarking

For benchmarking TinyJambu-{128, 192, 256} authenticated encryption/ verified decryption on CPU based systems, issue

```bash
FBK=32 make benchmark
```

```bash
2022-04-26T13:17:02+00:00
Running ./bench/a.out
Run on (4 X 2300.07 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.08, 0.02, 0.01
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
tinyjambu_128_encrypt_64B_32B         2622 ns         2622 ns       267120 bytes_per_second=34.9158M/s items_per_second=381.374k/s
tinyjambu_128_encrypt_128B_32B        4242 ns         4242 ns       164803 bytes_per_second=35.9689M/s items_per_second=235.726k/s
tinyjambu_128_encrypt_256B_32B        7487 ns         7487 ns        93525 bytes_per_second=36.6857M/s items_per_second=133.569k/s
tinyjambu_128_encrypt_512B_32B       13981 ns        13980 ns        50098 bytes_per_second=37.1092M/s items_per_second=71.529k/s
tinyjambu_128_encrypt_1024B_32B      26972 ns        26970 ns        25959 bytes_per_second=37.3412M/s items_per_second=37.0787k/s
tinyjambu_128_encrypt_2048B_32B      52920 ns        52918 ns        13228 bytes_per_second=37.485M/s items_per_second=18.897k/s
tinyjambu_128_encrypt_4096B_32B     104853 ns       104846 ns         6678 bytes_per_second=37.5482M/s items_per_second=9.53781k/s
tinyjambu_192_encrypt_64B_32B         2878 ns         2878 ns       243282 bytes_per_second=31.8141M/s items_per_second=347.494k/s
tinyjambu_192_encrypt_128B_32B        4713 ns         4713 ns       148460 bytes_per_second=32.3748M/s items_per_second=212.171k/s
tinyjambu_192_encrypt_256B_32B        8385 ns         8384 ns        83497 bytes_per_second=32.758M/s items_per_second=119.268k/s
tinyjambu_192_encrypt_512B_32B       15719 ns        15718 ns        44535 bytes_per_second=33.0075M/s items_per_second=63.6229k/s
tinyjambu_192_encrypt_1024B_32B      30410 ns        30410 ns        23018 bytes_per_second=33.1169M/s items_per_second=32.8841k/s
tinyjambu_192_encrypt_2048B_32B      59762 ns        59758 ns        11707 bytes_per_second=33.1944M/s items_per_second=16.7341k/s
tinyjambu_192_encrypt_4096B_32B     118411 ns       118406 ns         5914 bytes_per_second=33.2481M/s items_per_second=8.44553k/s
tinyjambu_256_encrypt_64B_32B         3047 ns         3047 ns       229776 bytes_per_second=30.0446M/s items_per_second=328.167k/s
tinyjambu_256_encrypt_128B_32B        5053 ns         5053 ns       138833 bytes_per_second=30.1962M/s items_per_second=197.894k/s
tinyjambu_256_encrypt_256B_32B        9037 ns         9036 ns        77557 bytes_per_second=30.3951M/s items_per_second=110.665k/s
tinyjambu_256_encrypt_512B_32B       17012 ns        17011 ns        41153 bytes_per_second=30.4985M/s items_per_second=58.7868k/s
tinyjambu_256_encrypt_1024B_32B      32991 ns        32989 ns        21225 bytes_per_second=30.5277M/s items_per_second=30.313k/s
tinyjambu_256_encrypt_2048B_32B      64888 ns        64884 ns        10787 bytes_per_second=30.5721M/s items_per_second=15.4121k/s
tinyjambu_256_encrypt_4096B_32B     128745 ns       128744 ns         5436 bytes_per_second=30.5783M/s items_per_second=7.76737k/s
tinyjambu_128_decrypt_64B_32B         2587 ns         2587 ns       270197 bytes_per_second=35.3866M/s items_per_second=386.516k/s
tinyjambu_128_decrypt_128B_32B        4179 ns         4179 ns       167499 bytes_per_second=36.5167M/s items_per_second=239.316k/s
tinyjambu_128_decrypt_256B_32B        7362 ns         7361 ns        95070 bytes_per_second=37.3111M/s items_per_second=135.846k/s
tinyjambu_128_decrypt_512B_32B       13722 ns        13722 ns        51025 bytes_per_second=37.8083M/s items_per_second=72.8765k/s
tinyjambu_128_decrypt_1024B_32B      26480 ns        26479 ns        26429 bytes_per_second=38.0332M/s items_per_second=37.7658k/s
tinyjambu_128_decrypt_2048B_32B      51925 ns        51920 ns        13472 bytes_per_second=38.2056M/s items_per_second=19.2603k/s
tinyjambu_128_decrypt_4096B_32B     102852 ns       102846 ns         6806 bytes_per_second=38.2783M/s items_per_second=9.72328k/s
tinyjambu_192_decrypt_64B_32B         2858 ns         2857 ns       245090 bytes_per_second=32.0421M/s items_per_second=349.985k/s
tinyjambu_192_decrypt_128B_32B        4692 ns         4692 ns       149130 bytes_per_second=32.5243M/s items_per_second=213.151k/s
tinyjambu_192_decrypt_256B_32B        8359 ns         8358 ns        83772 bytes_per_second=32.8613M/s items_per_second=119.644k/s
tinyjambu_192_decrypt_512B_32B       15691 ns        15691 ns        44626 bytes_per_second=33.064M/s items_per_second=63.7318k/s
tinyjambu_192_decrypt_1024B_32B      30409 ns        30408 ns        23033 bytes_per_second=33.1188M/s items_per_second=32.886k/s
tinyjambu_192_decrypt_2048B_32B      59760 ns        59760 ns        11718 bytes_per_second=33.1937M/s items_per_second=16.7337k/s
tinyjambu_192_decrypt_4096B_32B     118483 ns       118476 ns         5910 bytes_per_second=33.2285M/s items_per_second=8.44054k/s
tinyjambu_256_decrypt_64B_32B         3051 ns         3051 ns       229435 bytes_per_second=30.0055M/s items_per_second=327.74k/s
tinyjambu_256_decrypt_128B_32B        5046 ns         5046 ns       138765 bytes_per_second=30.2407M/s items_per_second=198.185k/s
tinyjambu_256_decrypt_256B_32B        9040 ns         9040 ns        77423 bytes_per_second=30.3836M/s items_per_second=110.623k/s
tinyjambu_256_decrypt_512B_32B       17039 ns        17039 ns        41062 bytes_per_second=30.4486M/s items_per_second=58.6905k/s
tinyjambu_256_decrypt_1024B_32B      33039 ns        33036 ns        21188 bytes_per_second=30.4843M/s items_per_second=30.27k/s
tinyjambu_256_decrypt_2048B_32B      65024 ns        65021 ns        10767 bytes_per_second=30.5079M/s items_per_second=15.3797k/s
tinyjambu_256_decrypt_4096B_32B     128970 ns       128966 ns         5426 bytes_per_second=30.5256M/s items_per_second=7.75396k/s
```

or 

```bash
FBK=128 make benchmark
```

```bash
2022-04-26T13:21:01+00:00
Running ./bench/a.out
Run on (4 X 2300.07 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.04, 0.07, 0.03
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
tinyjambu_128_encrypt_64B_32B         2729 ns         2729 ns       256407 bytes_per_second=33.5471M/s items_per_second=366.424k/s
tinyjambu_128_encrypt_128B_32B        4426 ns         4426 ns       158089 bytes_per_second=34.473M/s items_per_second=225.922k/s
tinyjambu_128_encrypt_256B_32B        7826 ns         7826 ns        89503 bytes_per_second=35.0977M/s items_per_second=127.787k/s
tinyjambu_128_encrypt_512B_32B       14622 ns        14622 ns        47882 bytes_per_second=35.4813M/s items_per_second=68.3913k/s
tinyjambu_128_encrypt_1024B_32B      28213 ns        28212 ns        24808 bytes_per_second=35.6969M/s items_per_second=35.4459k/s
tinyjambu_128_encrypt_2048B_32B      55421 ns        55417 ns        12629 bytes_per_second=35.7947M/s items_per_second=18.0449k/s
tinyjambu_128_encrypt_4096B_32B     109808 ns       109800 ns         6372 bytes_per_second=35.854M/s items_per_second=9.10747k/s
tinyjambu_192_encrypt_64B_32B         2957 ns         2957 ns       236662 bytes_per_second=30.9599M/s items_per_second=338.164k/s
tinyjambu_192_encrypt_128B_32B        4858 ns         4858 ns       144077 bytes_per_second=31.4128M/s items_per_second=205.867k/s
tinyjambu_192_encrypt_256B_32B        8660 ns         8659 ns        80851 bytes_per_second=31.7187M/s items_per_second=115.484k/s
tinyjambu_192_encrypt_512B_32B       16260 ns        16260 ns        43048 bytes_per_second=31.907M/s items_per_second=61.5017k/s
tinyjambu_192_encrypt_1024B_32B      31463 ns        31461 ns        22246 bytes_per_second=32.0102M/s items_per_second=31.7852k/s
tinyjambu_192_encrypt_2048B_32B      61894 ns        61894 ns        11306 bytes_per_second=32.0492M/s items_per_second=16.1568k/s
tinyjambu_192_encrypt_4096B_32B     122817 ns       122815 ns         5706 bytes_per_second=32.0546M/s items_per_second=8.14236k/s
tinyjambu_256_encrypt_64B_32B         3199 ns         3199 ns       218752 bytes_per_second=28.6192M/s items_per_second=312.598k/s
tinyjambu_256_encrypt_128B_32B        5320 ns         5320 ns       131660 bytes_per_second=28.6845M/s items_per_second=187.987k/s
tinyjambu_256_encrypt_256B_32B        9564 ns         9564 ns        73229 bytes_per_second=28.7188M/s items_per_second=104.562k/s
tinyjambu_256_encrypt_512B_32B       18050 ns        18049 ns        38804 bytes_per_second=28.7446M/s items_per_second=55.406k/s
tinyjambu_256_encrypt_1024B_32B      35028 ns        35025 ns        19988 bytes_per_second=28.7533M/s items_per_second=28.5512k/s
tinyjambu_256_encrypt_2048B_32B      68997 ns        68992 ns        10146 bytes_per_second=28.7516M/s items_per_second=14.4943k/s
tinyjambu_256_encrypt_4096B_32B     136886 ns       136882 ns         5112 bytes_per_second=28.7604M/s items_per_second=7.30559k/s
tinyjambu_128_decrypt_64B_32B         2745 ns         2745 ns       254954 bytes_per_second=33.3537M/s items_per_second=364.312k/s
tinyjambu_128_decrypt_128B_32B        4460 ns         4460 ns       156865 bytes_per_second=34.2112M/s items_per_second=224.207k/s
tinyjambu_128_decrypt_256B_32B        7895 ns         7895 ns        88620 bytes_per_second=34.788M/s items_per_second=126.659k/s
tinyjambu_128_decrypt_512B_32B       14761 ns        14761 ns        47433 bytes_per_second=35.1468M/s items_per_second=67.7464k/s
tinyjambu_128_decrypt_1024B_32B      28502 ns        28501 ns        24565 bytes_per_second=35.3346M/s items_per_second=35.0862k/s
tinyjambu_128_decrypt_2048B_32B      55974 ns        55973 ns        12513 bytes_per_second=35.4391M/s items_per_second=17.8657k/s
tinyjambu_128_decrypt_4096B_32B     110950 ns       110942 ns         6308 bytes_per_second=35.4851M/s items_per_second=9.01376k/s
tinyjambu_192_decrypt_64B_32B         2977 ns         2977 ns       235178 bytes_per_second=30.7517M/s items_per_second=335.891k/s
tinyjambu_192_decrypt_128B_32B        4894 ns         4894 ns       143000 bytes_per_second=31.1806M/s items_per_second=204.345k/s
tinyjambu_192_decrypt_256B_32B        8735 ns         8735 ns        80189 bytes_per_second=31.4444M/s items_per_second=114.486k/s
tinyjambu_192_decrypt_512B_32B       16416 ns        16416 ns        42660 bytes_per_second=31.604M/s items_per_second=60.9177k/s
tinyjambu_192_decrypt_1024B_32B      31769 ns        31769 ns        22038 bytes_per_second=31.6998M/s items_per_second=31.4769k/s
tinyjambu_192_decrypt_2048B_32B      62485 ns        62484 ns        11181 bytes_per_second=31.7462M/s items_per_second=16.004k/s
tinyjambu_192_decrypt_4096B_32B     123876 ns       123874 ns         5652 bytes_per_second=31.7805M/s items_per_second=8.07274k/s
tinyjambu_256_decrypt_64B_32B         3216 ns         3216 ns       217676 bytes_per_second=28.4687M/s items_per_second=310.954k/s
tinyjambu_256_decrypt_128B_32B        5351 ns         5351 ns       130874 bytes_per_second=28.5184M/s items_per_second=186.898k/s
tinyjambu_256_decrypt_256B_32B        9616 ns         9616 ns        72820 bytes_per_second=28.5621M/s items_per_second=103.992k/s
tinyjambu_256_decrypt_512B_32B       18156 ns        18156 ns        38573 bytes_per_second=28.5749M/s items_per_second=55.079k/s
tinyjambu_256_decrypt_1024B_32B      35217 ns        35217 ns        19876 bytes_per_second=28.5963M/s items_per_second=28.3952k/s
tinyjambu_256_decrypt_2048B_32B      69362 ns        69358 ns        10096 bytes_per_second=28.6001M/s items_per_second=14.418k/s
tinyjambu_256_decrypt_4096B_32B     137634 ns       137631 ns         5088 bytes_per_second=28.6038M/s items_per_second=7.26582k/s
```

> Not mentioning any value for `FBK` parameter, results in setting `FBK=32` !

## Usage

`tinyjambu` being a simple, zero-dependency, header-only C++ library, it's pretty easy to start using it. Just add `./include` path while compiling your program ( inform your compiler about this include path using `-I ./include` syntax ), which uses `tinyjambu`.

I've written following example programs which demonstrate use of TinyJambu-{128, 192, 256} C++ API

- [TinyJambu-128](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_128.cpp)
- [TinyJambu-193](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_193.cpp)
- [TinyJambu-256](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_256.cpp)
