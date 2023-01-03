# tinyjambu
Accelerated TinyJambu - Lightweight Authenticated Encryption Algorithms

## Overview

After working on `ascon` **A**uthenticated **E**ncryption with **A**ssociated **D**ata suite, I've taken up another final round candidate of NIST **L**ight **W**eight **C**ryptography competition `tinyjambu`, which offers three authenticated encryption algorithms, variated based on secret key bit length.

AEAD Algorithm | Secret Key ( Bit Length )
--- | --:
TinyJambu-128 | 128
TinyJambu-192 | 192
TinyJambu-256 | 256

> **Note** Learn more about AEAD [here](https://en.wikipedia.org/wiki/Authenticated_encryption)

> **Note** If interested in learning more about my work on Ascon AEAD, see [here](https://github.com/itzmeanjan/ascon)

TinyJambu-{128, 192, 256} offers lightweight authenticated encryption/ verified decryption algorithms. Encryption/ Decryption procedures take following inputs and computes these outputs.

Algorithm | Input | Output
--- | --: | --:
`encrypt` | 16/ 24/ 32 -bytes secret key, 12 -bytes public message nonce, N -bytes of plain text s.t. N >= 0 and N -bytes of associated data s.t. N >= 0 | N -bytes encrypted data s.t. N >= 0 && len(text) == len(cipher) and 8 -bytes authentication tag
`decrypt` | 16/ 24/ 32 -bytes secret key, 12 -bytes public message nonce, 8 -bytes of authentication tag, N -bytes encrypted data s.t. N >= 0 and N -bytes of associated data s.t. N >= 0 | N -bytes decrypted data s.t. N >= 0 && len(cipher) == len(text) and boolean flag denoting success of verification

Here I'm maintaining one easy-to-use, zero-dependency, header-only C++ library, which implements all three variants of TinyJambu. I've also written a Python wrapper interface for underlying C++ implementation, which one might want to use in their Python programs.

> **Note** During implementation, I followed [this](https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-spec-doc/tinyjambu-spec-final.pdf) specification of TinyJambu.

## Prerequisites

- C++ compilers like `clang++`/ `g++`, along C++ standard library, implementing C++20 specification

```bash
$ clang++ --version
Ubuntu clang version 14.0.0-1ubuntu1
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

$ g++ --version
g++ (Ubuntu 11.2.0-19ubuntu1) 11.2.0
```

- System development utilities like `make`, `cmake` and `git`

```bash
$ make --version
GNU Make 4.3

$ cmake --version
cmake version 3.22.1

$ git --version
git version 2.34.1
```

- For benchmarking TinyJambu-{128, 192, 256} on CPU, you'll need to have `google-benchmark` library globally installed; see [this](https://github.com/google/benchmark/tree/60b16f1#installation) guide.

- If you want to use/ test/ benchmark Python API, you need to have `python3`, along with some dependencies, which can be installed using `pip`.

```bash
$ python3 --version
Python 3.10.8

# If you don't have pip installed
$ sudo apt-get install python3-pip

# Download Python dependencies
$ python3 -m pip install -r wrapper/python/requirements.txt --user
```

> **Note** But probably, you want to use `virtualenv` instead of polluting your global Python installation.

```bash
# download virtualenv
python3 -m pip install --user virtualenv

pushd wrapper/python

# create virtualenv work directory
python3 -m virtualenv .
# enable virtualenv
source bin/activate # notice shell prompt change

# download dependencies inside virtualenv
python3 -m pip install -r requirements.txt

# do whatever you want to do inside virtualenv workspace
# ...
# ...

# disable virtualenv
deactivate # notice shell prompt change

popd
```

## Testing

For ensuring functional correctness of TinyJambu-{128, 192, 256} AEAD, I've written following test cases

- Given randomly generated secret key, public message nonce, plain text bytes ( length >= 0 ) & associated data bytes ( length >= 0 )
    - first, compute encrypted bytes & authentication tag
    - then, attempt to decrypt, ensure that verification passes, while also employing byte-by-byte comparison of decrypted and original plain text
- Given randomly generated secret key, public message nonce, plain text bytes ( length >= 0 ) & associated data bytes ( length >= 0 )
    - first, compute encrypted bytes & authentication tag
    - then, mutate ( just a single bit flip should suffice ) either of secret key/ public message nonce/ authentication tag/ encrypted bytes/ associated data
    - finally, attempt to decrypt, authentication tag verification must fail. Alongside check that unverified plain text is never released i.e. plain text bytes should be zeroed in case of tag verification failure.
- Test correctness and compatibility using Known Answer Tests provided with NIST LWC submission of TinyJambu.

Issue following command(s) to run test cases on all variants of TinyJambu

```bash
FBK=32 make   # 32 feedback bits in-parallel
FBK=128 make  # 4x32 feedback bits per iteration
```

> **Note** You may safely skip specifying `FBK`, default choice `FBK=32` is automatically set !

## Benchmarking

For benchmarking TinyJambu-{128, 192, 256} authenticated encryption/ verified decryption on CPU, issue

```bash
FBK=32 make benchmark
```

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled with Clang )

```bash
2023-01-03T21:53:59+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 1.63, 1.81, 1.87
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
tinyjambu_128_encrypt/64/32          141 ns          141 ns      4901515 bytes_per_second=648.653M/s
tinyjambu_128_decrypt/64/32          152 ns          152 ns      4534560 bytes_per_second=603.67M/s
tinyjambu_128_encrypt/128/32         247 ns          245 ns      2999182 bytes_per_second=623.62M/s
tinyjambu_128_decrypt/128/32         246 ns          245 ns      2752882 bytes_per_second=622.835M/s
tinyjambu_128_encrypt/256/32         410 ns          408 ns      1734296 bytes_per_second=672.966M/s
tinyjambu_128_decrypt/256/32         426 ns          425 ns      1625246 bytes_per_second=646.384M/s
tinyjambu_128_encrypt/512/32         794 ns          782 ns       911162 bytes_per_second=663.527M/s
tinyjambu_128_decrypt/512/32         799 ns          787 ns       818618 bytes_per_second=658.906M/s
tinyjambu_128_encrypt/1024/32       1480 ns         1467 ns       435847 bytes_per_second=686.462M/s
tinyjambu_128_decrypt/1024/32       1537 ns         1525 ns       462293 bytes_per_second=660.339M/s
tinyjambu_128_encrypt/2048/32       2909 ns         2868 ns       240999 bytes_per_second=691.532M/s
tinyjambu_128_decrypt/2048/32       3070 ns         3036 ns       239672 bytes_per_second=653.442M/s
tinyjambu_128_encrypt/4096/32       6302 ns         6048 ns       126846 bytes_per_second=650.892M/s
tinyjambu_128_decrypt/4096/32       6128 ns         6061 ns       114161 bytes_per_second=649.487M/s
tinyjambu_192_encrypt/64/32          168 ns          166 ns      4271861 bytes_per_second=550.647M/s
tinyjambu_192_decrypt/64/32          173 ns          170 ns      4066127 bytes_per_second=537.702M/s
tinyjambu_192_encrypt/128/32         247 ns          247 ns      2813053 bytes_per_second=617.995M/s
tinyjambu_192_decrypt/128/32         261 ns          260 ns      2698119 bytes_per_second=587.062M/s
tinyjambu_192_encrypt/256/32         455 ns          452 ns      1546756 bytes_per_second=607.028M/s
tinyjambu_192_decrypt/256/32         511 ns          494 ns      1568160 bytes_per_second=556.419M/s
tinyjambu_192_encrypt/512/32         890 ns          882 ns       789702 bytes_per_second=587.98M/s
tinyjambu_192_decrypt/512/32         847 ns          844 ns       819576 bytes_per_second=615.042M/s
tinyjambu_192_encrypt/1024/32       1546 ns         1545 ns       442553 bytes_per_second=651.8M/s
tinyjambu_192_decrypt/1024/32       1530 ns         1529 ns       452925 bytes_per_second=658.561M/s
tinyjambu_192_encrypt/2048/32       2995 ns         2994 ns       228835 bytes_per_second=662.645M/s
tinyjambu_192_decrypt/2048/32       3158 ns         3129 ns       234266 bytes_per_second=633.911M/s
tinyjambu_192_encrypt/4096/32       6423 ns         6362 ns       106851 bytes_per_second=618.8M/s
tinyjambu_192_decrypt/4096/32       6250 ns         6208 ns       106725 bytes_per_second=634.184M/s
tinyjambu_256_encrypt/64/32          152 ns          151 ns      4886801 bytes_per_second=607.999M/s
tinyjambu_256_decrypt/64/32          161 ns          160 ns      4068656 bytes_per_second=570.484M/s
tinyjambu_256_encrypt/128/32         234 ns          233 ns      3070526 bytes_per_second=653.58M/s
tinyjambu_256_decrypt/128/32         261 ns          260 ns      2698213 bytes_per_second=586.16M/s
tinyjambu_256_encrypt/256/32         400 ns          399 ns      1727836 bytes_per_second=688.072M/s
tinyjambu_256_decrypt/256/32         487 ns          483 ns      1493094 bytes_per_second=568.081M/s
tinyjambu_256_encrypt/512/32         768 ns          762 ns       954094 bytes_per_second=681.143M/s
tinyjambu_256_decrypt/512/32         914 ns          890 ns       776518 bytes_per_second=583.202M/s
tinyjambu_256_encrypt/1024/32       1383 ns         1381 ns       479531 bytes_per_second=728.979M/s
tinyjambu_256_decrypt/1024/32       1599 ns         1597 ns       433582 bytes_per_second=630.517M/s
tinyjambu_256_encrypt/2048/32       2685 ns         2684 ns       259033 bytes_per_second=739.053M/s
tinyjambu_256_decrypt/2048/32       3139 ns         3137 ns       224072 bytes_per_second=632.285M/s
tinyjambu_256_encrypt/4096/32       5336 ns         5333 ns       127003 bytes_per_second=738.174M/s
tinyjambu_256_decrypt/4096/32       6188 ns         6184 ns       111905 bytes_per_second=636.624M/s
```

## Usage

`tinyjambu` being a simple, zero-dependency, header-only C++ library, it's pretty easy to start using it. Just add `./include` path while compiling your program ( inform your compiler about this include path using `-I ./include` syntax ), which uses `tinyjambu`.

I've written following example programs which demonstrate use of TinyJambu-{128, 192, 256} C++ API

- [TinyJambu-128](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_128.cpp)
- [TinyJambu-193](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_193.cpp)
- [TinyJambu-256](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_256.cpp)

You may also want to use Python API of `tinyjambu`, consider checking [here](https://github.com/itzmeanjan/tinyjambu/blob/1082f55/wrapper/python/example.py) for usage example.
