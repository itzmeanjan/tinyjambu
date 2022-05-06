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

Here I'm maintaining one easy to use, zero-dependency ( excluding C++20 standard library ), header-only C++ library, which implements all three variants of TinyJambu. I've also written Python interface for underlying C++ implementation, which one might want to use in their Python programs.

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

- If you want to use/ test/ benchmark Python API, you need to have `python3`, along with some dependencies, which can be installed using `pip`.

> I'm using

```bash
$ python3 --version

Python 3.10.4
```

> For downloading Python API dependencies

```bash
pushd wrapper/python
python3 -m pip install -r requirements.txt # given that you've pip installed !
popd
```

> If `pip` is not yet installed

```bash
sudo apt-get install python3-pip
```

> But probably, you want to use `virtualenv` instead of polluting your global namespace. [ **Recommended** ]

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

For testing using Known Answer Tests provided with NIST LWC submission of TinyJambu, issue

```bash
make test_kat
```

## Benchmarking

For benchmarking TinyJambu-{128, 192, 256} authenticated encryption/ verified decryption on CPU based systems, issue

```bash
FBK=32 make benchmark
```

```bash
2022-05-06T13:22:06+05:30
Running ./bench/a.out
Run on (4 X 1800 MHz CPU s)
Load Average: 1.26, 1.60, 1.97
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
tinyjambu_128_encrypt_64B_32B          214 ns          214 ns      3258175 bytes_per_second=427.413M/s
tinyjambu_128_decrypt_64B_32B          258 ns          258 ns      2710619 bytes_per_second=355.428M/s
tinyjambu_128_encrypt_128B_32B         304 ns          304 ns      2300939 bytes_per_second=502.541M/s
tinyjambu_128_decrypt_128B_32B         382 ns          382 ns      1829325 bytes_per_second=399.145M/s
tinyjambu_128_encrypt_256B_32B         492 ns          492 ns      1424379 bytes_per_second=558.672M/s
tinyjambu_128_decrypt_256B_32B         638 ns          638 ns      1091766 bytes_per_second=430.7M/s
tinyjambu_128_encrypt_512B_32B         847 ns          847 ns       821184 bytes_per_second=612.299M/s
tinyjambu_128_decrypt_512B_32B        1137 ns         1137 ns       614322 bytes_per_second=456.451M/s
tinyjambu_128_encrypt_1024B_32B       1561 ns         1560 ns       448022 bytes_per_second=645.468M/s
tinyjambu_128_decrypt_1024B_32B       2141 ns         2135 ns       327782 bytes_per_second=471.771M/s
tinyjambu_128_encrypt_2048B_32B       2986 ns         2985 ns       234390 bytes_per_second=664.436M/s
tinyjambu_128_decrypt_2048B_32B       4130 ns         4129 ns       169542 bytes_per_second=480.452M/s
tinyjambu_128_encrypt_4096B_32B       5837 ns         5835 ns       119494 bytes_per_second=674.676M/s
tinyjambu_128_decrypt_4096B_32B       8125 ns         8121 ns        85926 bytes_per_second=484.789M/s
tinyjambu_192_encrypt_64B_32B          217 ns          217 ns      3223200 bytes_per_second=421.75M/s
tinyjambu_192_decrypt_64B_32B          259 ns          259 ns      2702900 bytes_per_second=353.56M/s
tinyjambu_192_encrypt_128B_32B         306 ns          306 ns      2286668 bytes_per_second=498.285M/s
tinyjambu_192_decrypt_128B_32B         384 ns          383 ns      1825778 bytes_per_second=397.912M/s
tinyjambu_192_encrypt_256B_32B         494 ns          494 ns      1418283 bytes_per_second=556.397M/s
tinyjambu_192_decrypt_256B_32B         641 ns          641 ns      1086930 bytes_per_second=428.255M/s
tinyjambu_192_encrypt_512B_32B         850 ns          850 ns       821175 bytes_per_second=610.506M/s
tinyjambu_192_decrypt_512B_32B        1139 ns         1139 ns       612299 bytes_per_second=455.361M/s
tinyjambu_192_encrypt_1024B_32B       1562 ns         1562 ns       446680 bytes_per_second=644.903M/s
tinyjambu_192_decrypt_1024B_32B       2138 ns         2137 ns       327171 bytes_per_second=471.192M/s
tinyjambu_192_encrypt_2048B_32B       2987 ns         2986 ns       234165 bytes_per_second=664.25M/s
tinyjambu_192_decrypt_2048B_32B       4130 ns         4130 ns       169251 bytes_per_second=480.296M/s
tinyjambu_192_encrypt_4096B_32B       5838 ns         5836 ns       119422 bytes_per_second=674.509M/s
tinyjambu_192_decrypt_4096B_32B       8128 ns         8125 ns        85811 bytes_per_second=484.551M/s
tinyjambu_256_encrypt_64B_32B          223 ns          223 ns      3133036 bytes_per_second=409.942M/s
tinyjambu_256_decrypt_64B_32B          260 ns          260 ns      2684956 bytes_per_second=351.523M/s
tinyjambu_256_encrypt_128B_32B         307 ns          307 ns      2281623 bytes_per_second=497.543M/s
tinyjambu_256_decrypt_128B_32B         386 ns          385 ns      1817545 bytes_per_second=395.943M/s
tinyjambu_256_encrypt_256B_32B         493 ns          493 ns      1419653 bytes_per_second=557.167M/s
tinyjambu_256_decrypt_256B_32B         641 ns          641 ns      1087613 bytes_per_second=428.347M/s
tinyjambu_256_encrypt_512B_32B         849 ns          849 ns       821790 bytes_per_second=610.939M/s
tinyjambu_256_decrypt_512B_32B        1140 ns         1140 ns       612646 bytes_per_second=455.057M/s
tinyjambu_256_encrypt_1024B_32B       1562 ns         1561 ns       447705 bytes_per_second=645.044M/s
tinyjambu_256_decrypt_1024B_32B       2137 ns         2136 ns       327512 bytes_per_second=471.37M/s
tinyjambu_256_encrypt_2048B_32B       2985 ns         2985 ns       233892 bytes_per_second=664.544M/s
tinyjambu_256_decrypt_2048B_32B       4133 ns         4132 ns       169384 bytes_per_second=480.03M/s
tinyjambu_256_encrypt_4096B_32B       5836 ns         5835 ns       119364 bytes_per_second=674.731M/s
tinyjambu_256_decrypt_4096B_32B       8125 ns         8122 ns        85866 bytes_per_second=484.692M/s
```

or 

```bash
FBK=128 make benchmark
```

```bash
2022-05-06T13:25:54+05:30
Running ./bench/a.out
Run on (4 X 1800 MHz CPU s)
Load Average: 0.84, 1.05, 1.66
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
tinyjambu_128_encrypt_64B_32B          191 ns          189 ns      3719449 bytes_per_second=484.043M/s
tinyjambu_128_decrypt_64B_32B          228 ns          227 ns      3097954 bytes_per_second=403.955M/s
tinyjambu_128_encrypt_128B_32B         299 ns          281 ns      2506384 bytes_per_second=542.858M/s
tinyjambu_128_decrypt_128B_32B         341 ns          341 ns      2051842 bytes_per_second=447.173M/s
tinyjambu_128_encrypt_256B_32B         463 ns          463 ns      1512329 bytes_per_second=593.157M/s
tinyjambu_128_decrypt_256B_32B         582 ns          579 ns      1210753 bytes_per_second=474.037M/s
tinyjambu_128_encrypt_512B_32B         822 ns          821 ns       845418 bytes_per_second=632.034M/s
tinyjambu_128_decrypt_512B_32B        1057 ns         1050 ns       668798 bytes_per_second=494.208M/s
tinyjambu_128_encrypt_1024B_32B       1547 ns         1540 ns       455576 bytes_per_second=653.989M/s
tinyjambu_128_decrypt_1024B_32B       1989 ns         1972 ns       351060 bytes_per_second=510.669M/s
tinyjambu_128_encrypt_2048B_32B       3083 ns         3003 ns       236531 bytes_per_second=660.455M/s
tinyjambu_128_decrypt_2048B_32B       3867 ns         3838 ns       182978 bytes_per_second=516.883M/s
tinyjambu_128_encrypt_4096B_32B       5916 ns         5846 ns       120283 bytes_per_second=673.424M/s
tinyjambu_128_decrypt_4096B_32B       7533 ns         7528 ns        92978 bytes_per_second=522.957M/s
tinyjambu_192_encrypt_64B_32B          192 ns          191 ns      3660487 bytes_per_second=479.165M/s
tinyjambu_192_decrypt_64B_32B          235 ns          235 ns      2978014 bytes_per_second=389.797M/s
tinyjambu_192_encrypt_128B_32B         280 ns          279 ns      2503782 bytes_per_second=545.94M/s
tinyjambu_192_decrypt_128B_32B         360 ns          360 ns      1946667 bytes_per_second=424.289M/s
tinyjambu_192_encrypt_256B_32B         467 ns          466 ns      1501211 bytes_per_second=588.918M/s
tinyjambu_192_decrypt_256B_32B         615 ns          615 ns      1133446 bytes_per_second=446.529M/s
tinyjambu_192_encrypt_512B_32B         823 ns          823 ns       848092 bytes_per_second=630.676M/s
tinyjambu_192_decrypt_512B_32B        1114 ns         1114 ns       626909 bytes_per_second=465.849M/s
tinyjambu_192_encrypt_1024B_32B       1534 ns         1534 ns       455510 bytes_per_second=656.435M/s
tinyjambu_192_decrypt_1024B_32B       2111 ns         2111 ns       330364 bytes_per_second=477.174M/s
tinyjambu_192_encrypt_2048B_32B       2959 ns         2958 ns       236145 bytes_per_second=670.514M/s
tinyjambu_192_decrypt_2048B_32B       4105 ns         4105 ns       170318 bytes_per_second=483.258M/s
tinyjambu_192_encrypt_4096B_32B       5807 ns         5807 ns       120118 bytes_per_second=677.962M/s
tinyjambu_192_decrypt_4096B_32B       8100 ns         8097 ns        86294 bytes_per_second=486.2M/s
tinyjambu_256_encrypt_64B_32B          190 ns          190 ns      3674394 bytes_per_second=480.861M/s
tinyjambu_256_decrypt_64B_32B          235 ns          234 ns      2983217 bytes_per_second=390.584M/s
tinyjambu_256_encrypt_128B_32B         281 ns          280 ns      2495984 bytes_per_second=544.037M/s
tinyjambu_256_decrypt_128B_32B         359 ns          359 ns      1950381 bytes_per_second=425.006M/s
tinyjambu_256_encrypt_256B_32B         465 ns          465 ns      1503744 bytes_per_second=590.527M/s
tinyjambu_256_decrypt_256B_32B         615 ns          614 ns      1133454 bytes_per_second=446.968M/s
tinyjambu_256_encrypt_512B_32B         822 ns          821 ns       849130 bytes_per_second=631.543M/s
tinyjambu_256_decrypt_512B_32B        1114 ns         1113 ns       627237 bytes_per_second=466.045M/s
tinyjambu_256_encrypt_1024B_32B       1533 ns         1533 ns       455629 bytes_per_second=656.905M/s
tinyjambu_256_decrypt_1024B_32B       2110 ns         2109 ns       331333 bytes_per_second=477.414M/s
tinyjambu_256_encrypt_2048B_32B       2958 ns         2957 ns       236159 bytes_per_second=670.757M/s
tinyjambu_256_decrypt_2048B_32B       4109 ns         4107 ns       170346 bytes_per_second=482.966M/s
tinyjambu_256_encrypt_4096B_32B       5806 ns         5806 ns       119779 bytes_per_second=678.094M/s
tinyjambu_256_decrypt_4096B_32B       8100 ns         8096 ns        86107 bytes_per_second=486.235M/s
```

> Not mentioning any value for `FBK` parameter, results in setting `FBK=32` !

For benchmarking Python API of `tinyjambu`, issue following command

```bash
FBK=32 make bench_python
```

```bash
2022-05-06T08:53:50+00:00
Running bench_tinyjambu.py
Run on (4 X 2300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.23, 0.23, 0.12
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
tinyjambu_128_encrypt      35807 ns        35806 ns        19346
tinyjambu_128_decrypt      36383 ns        36381 ns        19390
tinyjambu_192_encrypt      36396 ns        36395 ns        19260
tinyjambu_192_decrypt      36662 ns        36657 ns        19093
tinyjambu_256_encrypt      36532 ns        36529 ns        19344
tinyjambu_256_decrypt      35906 ns        35902 ns        19545
```

or when interested in computing 128 feedback bits each iteration

```bash
FBK=128 make bench_python
```

```bash
2022-05-06T08:53:14+00:00
Running bench_tinyjambu.py
Run on (4 X 2300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.11, 0.22, 0.11
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
tinyjambu_128_encrypt      36252 ns        36248 ns        19124
tinyjambu_128_decrypt      36922 ns        36919 ns        19136
tinyjambu_192_encrypt      36976 ns        36974 ns        19086
tinyjambu_192_decrypt      37122 ns        37120 ns        18719
tinyjambu_256_encrypt      36729 ns        36730 ns        19045
tinyjambu_256_decrypt      36435 ns        36433 ns        19436
```

## Usage

`tinyjambu` being a simple, zero-dependency, header-only C++ library, it's pretty easy to start using it. Just add `./include` path while compiling your program ( inform your compiler about this include path using `-I ./include` syntax ), which uses `tinyjambu`.

I've written following example programs which demonstrate use of TinyJambu-{128, 192, 256} C++ API

- [TinyJambu-128](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_128.cpp)
- [TinyJambu-193](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_193.cpp)
- [TinyJambu-256](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_256.cpp)

You may also want to use Python API of `tinyjambu`, consider checking [here](https://github.com/itzmeanjan/tinyjambu/blob/1082f55/wrapper/python/example.py) for usage example.
