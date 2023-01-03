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

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( compiled using Clang )

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

### On ARM Neoverse-V1 aka AWS Graviton3 ( compiled using GCC )

```bash
2023-01-03T18:36:28+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.13, 0.04, 0.01
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
tinyjambu_128_encrypt/64/32         96.5 ns         96.5 ns      7253865 bytes_per_second=948.387M/s
tinyjambu_128_decrypt/64/32          106 ns          106 ns      6598215 bytes_per_second=863.188M/s
tinyjambu_128_encrypt/128/32         138 ns          138 ns      5066435 bytes_per_second=1104.24M/s
tinyjambu_128_decrypt/128/32         153 ns          153 ns      4585540 bytes_per_second=999.76M/s
tinyjambu_128_encrypt/256/32         220 ns          220 ns      3176342 bytes_per_second=1.21674G/s
tinyjambu_128_decrypt/256/32         246 ns          246 ns      2847325 bytes_per_second=1118.29M/s
tinyjambu_128_encrypt/512/32         386 ns          386 ns      1813281 bytes_per_second=1.31183G/s
tinyjambu_128_decrypt/512/32         432 ns          432 ns      1620707 bytes_per_second=1.17201G/s
tinyjambu_128_encrypt/1024/32        717 ns          717 ns       978339 bytes_per_second=1.37131G/s
tinyjambu_128_decrypt/1024/32        805 ns          805 ns       874080 bytes_per_second=1.22207G/s
tinyjambu_128_encrypt/2048/32       1384 ns         1383 ns       505692 bytes_per_second=1.4002G/s
tinyjambu_128_decrypt/2048/32       1556 ns         1556 ns       448663 bytes_per_second=1.24505G/s
tinyjambu_128_encrypt/4096/32       2706 ns         2706 ns       258416 bytes_per_second=1.42058G/s
tinyjambu_128_decrypt/4096/32       3051 ns         3051 ns       229881 bytes_per_second=1.26017G/s
tinyjambu_192_encrypt/64/32          104 ns          104 ns      6708001 bytes_per_second=877.101M/s
tinyjambu_192_decrypt/64/32          115 ns          115 ns      6096908 bytes_per_second=797.752M/s
tinyjambu_192_encrypt/128/32         146 ns          146 ns      4798290 bytes_per_second=1045.77M/s
tinyjambu_192_decrypt/128/32         162 ns          162 ns      4306249 bytes_per_second=939.019M/s
tinyjambu_192_encrypt/256/32         230 ns          230 ns      3048561 bytes_per_second=1.16805G/s
tinyjambu_192_decrypt/256/32         258 ns          258 ns      2711038 bytes_per_second=1063.46M/s
tinyjambu_192_encrypt/512/32         396 ns          396 ns      1766788 bytes_per_second=1.27877G/s
tinyjambu_192_decrypt/512/32         450 ns          450 ns      1558383 bytes_per_second=1.12667G/s
tinyjambu_192_encrypt/1024/32        730 ns          730 ns       962842 bytes_per_second=1.34792G/s
tinyjambu_192_decrypt/1024/32        832 ns          832 ns       839898 bytes_per_second=1.1814G/s
tinyjambu_192_encrypt/2048/32       1403 ns         1403 ns       499219 bytes_per_second=1.38092G/s
tinyjambu_192_decrypt/2048/32       1603 ns         1603 ns       436695 bytes_per_second=1.20823G/s
tinyjambu_192_encrypt/4096/32       2740 ns         2740 ns       255649 bytes_per_second=1.4033G/s
tinyjambu_192_decrypt/4096/32       3131 ns         3131 ns       223764 bytes_per_second=1.2278G/s
tinyjambu_256_encrypt/64/32         96.4 ns         96.4 ns      7266925 bytes_per_second=950.174M/s
tinyjambu_256_decrypt/64/32          108 ns          108 ns      6511191 bytes_per_second=851.602M/s
tinyjambu_256_encrypt/128/32         138 ns          138 ns      5062154 bytes_per_second=1104M/s
tinyjambu_256_decrypt/128/32         156 ns          156 ns      4490840 bytes_per_second=980.265M/s
tinyjambu_256_encrypt/256/32         221 ns          221 ns      3167103 bytes_per_second=1.21209G/s
tinyjambu_256_decrypt/256/32         252 ns          252 ns      2779985 bytes_per_second=1090.91M/s
tinyjambu_256_encrypt/512/32         388 ns          388 ns      1805046 bytes_per_second=1.30624G/s
tinyjambu_256_decrypt/512/32         444 ns          444 ns      1576240 bytes_per_second=1.14061G/s
tinyjambu_256_encrypt/1024/32        720 ns          720 ns       970148 bytes_per_second=1.36543G/s
tinyjambu_256_decrypt/1024/32        829 ns          829 ns       845395 bytes_per_second=1.18669G/s
tinyjambu_256_encrypt/2048/32       1394 ns         1394 ns       504158 bytes_per_second=1.38991G/s
tinyjambu_256_decrypt/2048/32       1605 ns         1605 ns       436268 bytes_per_second=1.20675G/s
tinyjambu_256_encrypt/4096/32       2724 ns         2724 ns       257125 bytes_per_second=1.41142G/s
tinyjambu_256_decrypt/4096/32       3140 ns         3140 ns       222869 bytes_per_second=1.22436G/s
```

### On ARM Neoverse-V1 aka AWS Graviton3 ( compiled using Clang )

```bash
2023-01-03T18:38:24+00:00
Running ./bench/a.out
Run on (64 X 2100 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x64)
  L1 Instruction 64 KiB (x64)
  L2 Unified 1024 KiB (x64)
  L3 Unified 32768 KiB (x1)
Load Average: 0.23, 0.14, 0.05
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
tinyjambu_128_encrypt/64/32          196 ns          196 ns      3563341 bytes_per_second=466.267M/s
tinyjambu_128_decrypt/64/32          209 ns          209 ns      3352745 bytes_per_second=438.617M/s
tinyjambu_128_encrypt/128/32         274 ns          274 ns      2553490 bytes_per_second=556.604M/s
tinyjambu_128_decrypt/128/32         293 ns          293 ns      2391877 bytes_per_second=521.536M/s
tinyjambu_128_encrypt/256/32         431 ns          430 ns      1625655 bytes_per_second=638.006M/s
tinyjambu_128_decrypt/256/32         461 ns          461 ns      1519185 bytes_per_second=596.101M/s
tinyjambu_128_encrypt/512/32         741 ns          741 ns       944315 bytes_per_second=699.928M/s
tinyjambu_128_decrypt/512/32         796 ns          796 ns       879976 bytes_per_second=652.075M/s
tinyjambu_128_encrypt/1024/32       1363 ns         1363 ns       513612 bytes_per_second=738.863M/s
tinyjambu_128_decrypt/1024/32       1465 ns         1465 ns       477733 bytes_per_second=687.277M/s
tinyjambu_128_encrypt/2048/32       2607 ns         2607 ns       268594 bytes_per_second=761.017M/s
tinyjambu_128_decrypt/2048/32       2804 ns         2804 ns       249653 bytes_per_second=707.38M/s
tinyjambu_128_encrypt/4096/32       5094 ns         5094 ns       137313 bytes_per_second=772.802M/s
tinyjambu_128_decrypt/4096/32       5488 ns         5488 ns       127539 bytes_per_second=717.358M/s
tinyjambu_192_encrypt/64/32          197 ns          197 ns      3555524 bytes_per_second=464.909M/s
tinyjambu_192_decrypt/64/32          209 ns          209 ns      3352037 bytes_per_second=437.763M/s
tinyjambu_192_encrypt/128/32         275 ns          275 ns      2547980 bytes_per_second=555.513M/s
tinyjambu_192_decrypt/128/32         292 ns          292 ns      2397478 bytes_per_second=522.479M/s
tinyjambu_192_encrypt/256/32         431 ns          431 ns      1623358 bytes_per_second=637.072M/s
tinyjambu_192_decrypt/256/32         461 ns          461 ns      1518664 bytes_per_second=595.839M/s
tinyjambu_192_encrypt/512/32         742 ns          742 ns       943571 bytes_per_second=699.383M/s
tinyjambu_192_decrypt/512/32         796 ns          796 ns       880174 bytes_per_second=652.164M/s
tinyjambu_192_encrypt/1024/32       1363 ns         1363 ns       513499 bytes_per_second=738.83M/s
tinyjambu_192_decrypt/1024/32       1466 ns         1466 ns       477937 bytes_per_second=687.132M/s
tinyjambu_192_encrypt/2048/32       2606 ns         2606 ns       268753 bytes_per_second=761.311M/s
tinyjambu_192_decrypt/2048/32       2807 ns         2807 ns       249354 bytes_per_second=706.798M/s
tinyjambu_192_encrypt/4096/32       5091 ns         5091 ns       137510 bytes_per_second=773.268M/s
tinyjambu_192_decrypt/4096/32       5486 ns         5486 ns       127596 bytes_per_second=717.579M/s
tinyjambu_256_encrypt/64/32          197 ns          197 ns      3552618 bytes_per_second=464.675M/s
tinyjambu_256_decrypt/64/32          209 ns          209 ns      3351161 bytes_per_second=438.795M/s
tinyjambu_256_encrypt/128/32         275 ns          275 ns      2547565 bytes_per_second=555.245M/s
tinyjambu_256_decrypt/128/32         292 ns          292 ns      2399484 bytes_per_second=523.26M/s
tinyjambu_256_encrypt/256/32         432 ns          432 ns      1622773 bytes_per_second=636.351M/s
tinyjambu_256_decrypt/256/32         460 ns          460 ns      1516588 bytes_per_second=596.731M/s
tinyjambu_256_encrypt/512/32         742 ns          742 ns       943274 bytes_per_second=698.828M/s
tinyjambu_256_decrypt/512/32         795 ns          795 ns       880363 bytes_per_second=652.804M/s
tinyjambu_256_encrypt/1024/32       1364 ns         1364 ns       513235 bytes_per_second=738.4M/s
tinyjambu_256_decrypt/1024/32       1464 ns         1464 ns       478214 bytes_per_second=688.064M/s
tinyjambu_256_encrypt/2048/32       2608 ns         2607 ns       268474 bytes_per_second=760.748M/s
tinyjambu_256_decrypt/2048/32       2802 ns         2801 ns       249812 bytes_per_second=708.065M/s
tinyjambu_256_encrypt/4096/32       5094 ns         5094 ns       137429 bytes_per_second=772.807M/s
tinyjambu_256_decrypt/4096/32       5477 ns         5476 ns       127790 bytes_per_second=718.855M/s
```

### On Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz ( compiled using GCC )

```bash
2023-01-03T18:41:15+00:00
Running ./bench/a.out
Run on (128 X 1261.36 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x64)
  L1 Instruction 32 KiB (x64)
  L2 Unified 1280 KiB (x64)
  L3 Unified 55296 KiB (x2)
Load Average: 0.32, 0.09, 0.03
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
tinyjambu_128_encrypt/64/32         83.4 ns         83.4 ns      8394527 bytes_per_second=1098.24M/s
tinyjambu_128_decrypt/64/32         88.5 ns         88.5 ns      7884714 bytes_per_second=1034.54M/s
tinyjambu_128_encrypt/128/32         109 ns          109 ns      6392854 bytes_per_second=1.36459G/s
tinyjambu_128_decrypt/128/32         116 ns          116 ns      6070254 bytes_per_second=1.28906G/s
tinyjambu_128_encrypt/256/32         162 ns          162 ns      4331509 bytes_per_second=1.65479G/s
tinyjambu_128_decrypt/256/32         171 ns          171 ns      4107331 bytes_per_second=1.57135G/s
tinyjambu_128_encrypt/512/32         267 ns          267 ns      2621591 bytes_per_second=1.89727G/s
tinyjambu_128_decrypt/512/32         291 ns          291 ns      2400346 bytes_per_second=1.73981G/s
tinyjambu_128_encrypt/1024/32        488 ns          488 ns      1431805 bytes_per_second=2.01655G/s
tinyjambu_128_decrypt/1024/32        505 ns          505 ns      1385783 bytes_per_second=1.94586G/s
tinyjambu_128_encrypt/2048/32        904 ns          904 ns       773544 bytes_per_second=2.142G/s
tinyjambu_128_decrypt/2048/32        945 ns          945 ns       741207 bytes_per_second=2.05082G/s
tinyjambu_128_encrypt/4096/32       1778 ns         1778 ns       393233 bytes_per_second=2.16181G/s
tinyjambu_128_decrypt/4096/32       1842 ns         1842 ns       379701 bytes_per_second=2.08688G/s
tinyjambu_192_encrypt/64/32         87.9 ns         87.9 ns      7967523 bytes_per_second=1041.86M/s
tinyjambu_192_decrypt/64/32         91.6 ns         91.6 ns      7635884 bytes_per_second=999.032M/s
tinyjambu_192_encrypt/128/32         113 ns          113 ns      6193001 bytes_per_second=1.31773G/s
tinyjambu_192_decrypt/128/32         119 ns          119 ns      5886398 bytes_per_second=1.25386G/s
tinyjambu_192_encrypt/256/32         164 ns          164 ns      4262440 bytes_per_second=1.63352G/s
tinyjambu_192_decrypt/256/32         174 ns          174 ns      4029690 bytes_per_second=1.54392G/s
tinyjambu_192_encrypt/512/32         267 ns          267 ns      2626044 bytes_per_second=1.90058G/s
tinyjambu_192_decrypt/512/32         290 ns          290 ns      2410317 bytes_per_second=1.74544G/s
tinyjambu_192_encrypt/1024/32        479 ns          479 ns      1463044 bytes_per_second=2.05495G/s
tinyjambu_192_decrypt/1024/32        510 ns          510 ns      1373501 bytes_per_second=1.92922G/s
tinyjambu_192_encrypt/2048/32        888 ns          888 ns       788271 bytes_per_second=2.18144G/s
tinyjambu_192_decrypt/2048/32        949 ns          949 ns       737954 bytes_per_second=2.0413G/s
tinyjambu_192_encrypt/4096/32       1741 ns         1741 ns       402041 bytes_per_second=2.20816G/s
tinyjambu_192_decrypt/4096/32       1829 ns         1829 ns       382602 bytes_per_second=2.1016G/s
tinyjambu_256_encrypt/64/32         84.0 ns         84.0 ns      8336015 bytes_per_second=1090.31M/s
tinyjambu_256_decrypt/64/32         89.3 ns         89.3 ns      7834471 bytes_per_second=1025.66M/s
tinyjambu_256_encrypt/128/32         109 ns          109 ns      6441645 bytes_per_second=1.36997G/s
tinyjambu_256_decrypt/128/32         116 ns          116 ns      6030520 bytes_per_second=1.28316G/s
tinyjambu_256_encrypt/256/32         160 ns          160 ns      4391421 bytes_per_second=1.67742G/s
tinyjambu_256_decrypt/256/32         171 ns          171 ns      4097783 bytes_per_second=1.56993G/s
tinyjambu_256_encrypt/512/32         262 ns          262 ns      2675348 bytes_per_second=1.93579G/s
tinyjambu_256_decrypt/512/32         287 ns          287 ns      2433259 bytes_per_second=1.76306G/s
tinyjambu_256_encrypt/1024/32        473 ns          473 ns      1480701 bytes_per_second=2.08076G/s
tinyjambu_256_decrypt/1024/32        505 ns          505 ns      1385058 bytes_per_second=1.94578G/s
tinyjambu_256_encrypt/2048/32        882 ns          882 ns       793800 bytes_per_second=2.19674G/s
tinyjambu_256_decrypt/2048/32        944 ns          944 ns       741266 bytes_per_second=2.05106G/s
tinyjambu_256_encrypt/4096/32       1735 ns         1735 ns       403539 bytes_per_second=2.21558G/s
tinyjambu_256_decrypt/4096/32       1826 ns         1826 ns       383363 bytes_per_second=2.10531G/s
```

### On Intel(R) Xeon(R) Platinum 8375C CPU @ 2.90GHz ( compiled using Clang )

```bash
2023-01-03T18:43:10+00:00
Running ./bench/a.out
Run on (128 X 1280.28 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x64)
  L1 Instruction 32 KiB (x64)
  L2 Unified 1280 KiB (x64)
  L3 Unified 55296 KiB (x2)
Load Average: 0.16, 0.14, 0.06
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
tinyjambu_128_encrypt/64/32          148 ns          148 ns      4732242 bytes_per_second=619.02M/s
tinyjambu_128_decrypt/64/32          158 ns          158 ns      4432619 bytes_per_second=579.828M/s
tinyjambu_128_encrypt/128/32         222 ns          222 ns      3138530 bytes_per_second=688.315M/s
tinyjambu_128_decrypt/128/32         233 ns          233 ns      3003385 bytes_per_second=654.68M/s
tinyjambu_128_encrypt/256/32         356 ns          356 ns      1963945 bytes_per_second=770.524M/s
tinyjambu_128_decrypt/256/32         384 ns          384 ns      1822432 bytes_per_second=715.755M/s
tinyjambu_128_encrypt/512/32         635 ns          635 ns      1101821 bytes_per_second=816.672M/s
tinyjambu_128_decrypt/512/32         686 ns          686 ns      1020952 bytes_per_second=756.766M/s
tinyjambu_128_encrypt/1024/32       1191 ns         1191 ns       587434 bytes_per_second=845.269M/s
tinyjambu_128_decrypt/1024/32       1288 ns         1288 ns       543497 bytes_per_second=781.875M/s
tinyjambu_128_encrypt/2048/32       2305 ns         2305 ns       303690 bytes_per_second=860.613M/s
tinyjambu_128_decrypt/2048/32       2492 ns         2492 ns       280852 bytes_per_second=795.984M/s
tinyjambu_128_encrypt/4096/32       5027 ns         5027 ns       139278 bytes_per_second=783.128M/s
tinyjambu_128_decrypt/4096/32       5346 ns         5346 ns       130962 bytes_per_second=736.425M/s
tinyjambu_192_encrypt/64/32          152 ns          152 ns      4637487 bytes_per_second=601.755M/s
tinyjambu_192_decrypt/64/32          159 ns          159 ns      4382066 bytes_per_second=574.12M/s
tinyjambu_192_encrypt/128/32         219 ns          219 ns      3192237 bytes_per_second=695.772M/s
tinyjambu_192_decrypt/128/32         235 ns          235 ns      2983522 bytes_per_second=650.405M/s
tinyjambu_192_encrypt/256/32         359 ns          359 ns      1953998 bytes_per_second=765.7M/s
tinyjambu_192_decrypt/256/32         385 ns          385 ns      1818517 bytes_per_second=712.878M/s
tinyjambu_192_encrypt/512/32         639 ns          639 ns      1095354 bytes_per_second=811.94M/s
tinyjambu_192_decrypt/512/32         688 ns          688 ns      1016688 bytes_per_second=753.601M/s
tinyjambu_192_encrypt/1024/32       1196 ns         1196 ns       585869 bytes_per_second=841.974M/s
tinyjambu_192_decrypt/1024/32       1290 ns         1290 ns       542622 bytes_per_second=780.753M/s
tinyjambu_192_encrypt/2048/32       2308 ns         2308 ns       303324 bytes_per_second=859.567M/s
tinyjambu_192_decrypt/2048/32       2494 ns         2494 ns       280717 bytes_per_second=795.481M/s
tinyjambu_192_encrypt/4096/32       5022 ns         5022 ns       139364 bytes_per_second=783.896M/s
tinyjambu_192_decrypt/4096/32       5341 ns         5341 ns       131077 bytes_per_second=737.087M/s
tinyjambu_256_encrypt/64/32          151 ns          151 ns      4634163 bytes_per_second=608.223M/s
tinyjambu_256_decrypt/64/32          158 ns          158 ns      4420970 bytes_per_second=577.738M/s
tinyjambu_256_encrypt/128/32         218 ns          218 ns      3215416 bytes_per_second=701.529M/s
tinyjambu_256_decrypt/128/32         234 ns          234 ns      2996503 bytes_per_second=653.395M/s
tinyjambu_256_encrypt/256/32         357 ns          357 ns      1959918 bytes_per_second=770.278M/s
tinyjambu_256_decrypt/256/32         384 ns          384 ns      1817890 bytes_per_second=715.08M/s
tinyjambu_256_encrypt/512/32         636 ns          636 ns      1101838 bytes_per_second=816.194M/s
tinyjambu_256_decrypt/512/32         686 ns          686 ns      1020388 bytes_per_second=756.325M/s
tinyjambu_256_encrypt/1024/32       1193 ns         1193 ns       587467 bytes_per_second=844.285M/s
tinyjambu_256_decrypt/1024/32       1288 ns         1288 ns       543318 bytes_per_second=781.728M/s
tinyjambu_256_encrypt/2048/32       2305 ns         2305 ns       303667 bytes_per_second=860.505M/s
tinyjambu_256_decrypt/2048/32       2493 ns         2493 ns       280734 bytes_per_second=795.581M/s
tinyjambu_256_encrypt/4096/32       5025 ns         5025 ns       139237 bytes_per_second=783.384M/s
tinyjambu_256_decrypt/4096/32       5349 ns         5349 ns       130832 bytes_per_second=735.957M/s
```

## Usage

`tinyjambu` being a simple, zero-dependency, header-only C++ library, it's pretty easy to start using it. Just add `./include` path while compiling your program ( inform your compiler about this include path using `-I ./include` syntax ), which uses `tinyjambu`.

I've written following example programs which demonstrate use of TinyJambu-{128, 192, 256} C++ API

- [TinyJambu-128](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_128.cpp)
- [TinyJambu-193](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_193.cpp)
- [TinyJambu-256](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_256.cpp)

You may also want to use Python API of `tinyjambu`, consider checking [here](https://github.com/itzmeanjan/tinyjambu/blob/1082f55/wrapper/python/example.py) for usage example.
