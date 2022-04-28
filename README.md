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
2022-04-26T20:35:01+05:30
Running ./bench/a.out
Run on (4 X 1800 MHz CPU s)
Load Average: 0.96, 1.06, 0.59
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
tinyjambu_128_encrypt_64B_32B         4215 ns         4214 ns       166119 bytes_per_second=21.7237M/s items_per_second=237.28k/s
tinyjambu_128_encrypt_128B_32B        6809 ns         6809 ns       102635 bytes_per_second=22.4105M/s items_per_second=146.87k/s
tinyjambu_128_encrypt_256B_32B       12153 ns        12090 ns        58248 bytes_per_second=22.7173M/s items_per_second=82.7113k/s
tinyjambu_128_encrypt_512B_32B       22434 ns        22420 ns        31176 bytes_per_second=23.1401M/s items_per_second=44.6032k/s
tinyjambu_128_encrypt_1024B_32B      43237 ns        43212 ns        16193 bytes_per_second=23.3057M/s items_per_second=23.1418k/s
tinyjambu_128_encrypt_2048B_32B      84846 ns        84804 ns         8228 bytes_per_second=23.3908M/s items_per_second=11.7919k/s
tinyjambu_128_encrypt_4096B_32B     168406 ns       168228 ns         4155 bytes_per_second=23.4013M/s items_per_second=5.9443k/s
tinyjambu_192_encrypt_64B_32B         4475 ns         4472 ns       156526 bytes_per_second=20.4713M/s items_per_second=223.601k/s
tinyjambu_192_encrypt_128B_32B        7390 ns         7383 ns        94506 bytes_per_second=20.668M/s items_per_second=135.45k/s
tinyjambu_192_encrypt_256B_32B       13161 ns        13154 ns        53144 bytes_per_second=20.8808M/s items_per_second=76.0247k/s
tinyjambu_192_encrypt_512B_32B       24621 ns        24613 ns        28429 bytes_per_second=21.0783M/s items_per_second=40.6291k/s
tinyjambu_192_encrypt_1024B_32B      47646 ns        47629 ns        14699 bytes_per_second=21.1441M/s items_per_second=20.9955k/s
tinyjambu_192_encrypt_2048B_32B      93717 ns        93659 ns         7455 bytes_per_second=21.1795M/s items_per_second=10.6771k/s
tinyjambu_192_encrypt_4096B_32B     185729 ns       185659 ns         3767 bytes_per_second=21.2043M/s items_per_second=5.38622k/s
tinyjambu_256_encrypt_64B_32B         5111 ns         5111 ns       136325 bytes_per_second=17.9146M/s items_per_second=195.675k/s
tinyjambu_256_encrypt_128B_32B        8521 ns         8518 ns        81827 bytes_per_second=17.9129M/s items_per_second=117.394k/s
tinyjambu_256_encrypt_256B_32B       15340 ns        15335 ns        45570 bytes_per_second=17.9102M/s items_per_second=65.2089k/s
tinyjambu_256_encrypt_512B_32B       28906 ns        28897 ns        24212 bytes_per_second=17.9531M/s items_per_second=34.6052k/s
tinyjambu_256_encrypt_1024B_32B      56131 ns        56110 ns        12422 bytes_per_second=17.9483M/s items_per_second=17.8221k/s
tinyjambu_256_encrypt_2048B_32B     110598 ns       110551 ns         6321 bytes_per_second=17.9432M/s items_per_second=9.04559k/s
tinyjambu_256_encrypt_4096B_32B     219481 ns       219395 ns         3191 bytes_per_second=17.9437M/s items_per_second=4.55798k/s
tinyjambu_128_decrypt_64B_32B         4223 ns         4221 ns       165910 bytes_per_second=21.6887M/s items_per_second=236.899k/s
tinyjambu_128_decrypt_128B_32B        6817 ns         6814 ns       102220 bytes_per_second=22.3917M/s items_per_second=146.746k/s
tinyjambu_128_decrypt_256B_32B       11998 ns        11996 ns        58113 bytes_per_second=22.8953M/s items_per_second=83.3591k/s
tinyjambu_128_decrypt_512B_32B       22388 ns        22385 ns        31228 bytes_per_second=23.1766M/s items_per_second=44.6737k/s
tinyjambu_128_decrypt_1024B_32B      43145 ns        43138 ns        16215 bytes_per_second=23.3453M/s items_per_second=23.1812k/s
tinyjambu_128_decrypt_2048B_32B      84693 ns        84671 ns         8221 bytes_per_second=23.4276M/s items_per_second=11.8104k/s
tinyjambu_128_decrypt_4096B_32B     167800 ns       167737 ns         4171 bytes_per_second=23.4699M/s items_per_second=5.96171k/s
tinyjambu_192_decrypt_64B_32B         4543 ns         4542 ns       154000 bytes_per_second=20.1565M/s items_per_second=220.163k/s
tinyjambu_192_decrypt_128B_32B        7412 ns         7409 ns        94132 bytes_per_second=20.5949M/s items_per_second=134.971k/s
tinyjambu_192_decrypt_256B_32B       13168 ns        13164 ns        53066 bytes_per_second=20.865M/s items_per_second=75.9673k/s
tinyjambu_192_decrypt_512B_32B       24672 ns        24663 ns        28363 bytes_per_second=21.0354M/s items_per_second=40.5464k/s
tinyjambu_192_decrypt_1024B_32B      47550 ns        47532 ns        14731 bytes_per_second=21.1873M/s items_per_second=21.0383k/s
tinyjambu_192_decrypt_2048B_32B      93716 ns        93683 ns         7439 bytes_per_second=21.174M/s items_per_second=10.6743k/s
tinyjambu_192_decrypt_4096B_32B     185200 ns       185133 ns         3778 bytes_per_second=21.2645M/s items_per_second=5.40152k/s
tinyjambu_256_decrypt_64B_32B         5107 ns         5105 ns       136495 bytes_per_second=17.9344M/s items_per_second=195.891k/s
tinyjambu_256_decrypt_128B_32B        8521 ns         8520 ns        81836 bytes_per_second=17.9101M/s items_per_second=117.376k/s
tinyjambu_256_decrypt_256B_32B       15356 ns        15353 ns        45496 bytes_per_second=17.889M/s items_per_second=65.1319k/s
tinyjambu_256_decrypt_512B_32B       28977 ns        28974 ns        24129 bytes_per_second=17.9058M/s items_per_second=34.5139k/s
tinyjambu_256_decrypt_1024B_32B      56448 ns        56427 ns        12350 bytes_per_second=17.8476M/s items_per_second=17.7222k/s
tinyjambu_256_decrypt_2048B_32B     111090 ns       111001 ns         6303 bytes_per_second=17.8705M/s items_per_second=9.00893k/s
tinyjambu_256_decrypt_4096B_32B     220698 ns       220582 ns         3173 bytes_per_second=17.8471M/s items_per_second=4.53345k/s
```

or 

```bash
FBK=128 make benchmark
```

```bash
2022-04-26T20:35:56+05:30
Running ./bench/a.out
Run on (4 X 1800 MHz CPU s)
Load Average: 1.05, 1.06, 0.62
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
tinyjambu_128_encrypt_64B_32B         3005 ns         3003 ns       232903 bytes_per_second=30.4835M/s items_per_second=332.961k/s
tinyjambu_128_encrypt_128B_32B        4855 ns         4852 ns       144324 bytes_per_second=31.4466M/s items_per_second=206.088k/s
tinyjambu_128_encrypt_256B_32B        8576 ns         8566 ns        81475 bytes_per_second=32.0624M/s items_per_second=116.736k/s
tinyjambu_128_encrypt_512B_32B       15961 ns        15958 ns        43806 bytes_per_second=32.51M/s items_per_second=62.664k/s
tinyjambu_128_encrypt_1024B_32B      30774 ns        30770 ns        22732 bytes_per_second=32.7298M/s items_per_second=32.4997k/s
tinyjambu_128_encrypt_2048B_32B      60453 ns        60422 ns        11537 bytes_per_second=32.8297M/s items_per_second=16.5502k/s
tinyjambu_128_encrypt_4096B_32B     119725 ns       119672 ns         5837 bytes_per_second=32.8962M/s items_per_second=8.35614k/s
tinyjambu_192_encrypt_64B_32B         3410 ns         3409 ns       205325 bytes_per_second=26.8574M/s items_per_second=293.355k/s
tinyjambu_192_encrypt_128B_32B        5481 ns         5478 ns       127289 bytes_per_second=27.8537M/s items_per_second=182.542k/s
tinyjambu_192_encrypt_256B_32B        9648 ns         9645 ns        72399 bytes_per_second=28.4768M/s items_per_second=103.681k/s
tinyjambu_192_encrypt_512B_32B       17987 ns        17980 ns        38876 bytes_per_second=28.8536M/s items_per_second=55.6161k/s
tinyjambu_192_encrypt_1024B_32B      34681 ns        34665 ns        20197 bytes_per_second=29.052M/s items_per_second=28.8478k/s
tinyjambu_192_encrypt_2048B_32B      68008 ns        67997 ns        10264 bytes_per_second=29.1726M/s items_per_second=14.7066k/s
tinyjambu_192_encrypt_4096B_32B     134733 ns       134712 ns         5180 bytes_per_second=29.2237M/s items_per_second=7.42327k/s
tinyjambu_256_encrypt_64B_32B         3463 ns         3463 ns       201906 bytes_per_second=26.4397M/s items_per_second=288.792k/s
tinyjambu_256_encrypt_128B_32B        5640 ns         5637 ns       123635 bytes_per_second=27.0697M/s items_per_second=177.404k/s
tinyjambu_256_encrypt_256B_32B        9983 ns         9980 ns        69865 bytes_per_second=27.5207M/s items_per_second=100.2k/s
tinyjambu_256_encrypt_512B_32B       18678 ns        18671 ns        37453 bytes_per_second=27.7867M/s items_per_second=53.5596k/s
tinyjambu_256_encrypt_1024B_32B      36061 ns        36049 ns        19404 bytes_per_second=27.9363M/s items_per_second=27.7399k/s
tinyjambu_256_encrypt_2048B_32B      70840 ns        70813 ns         9851 bytes_per_second=28.0123M/s items_per_second=14.1217k/s
tinyjambu_256_encrypt_4096B_32B     140363 ns       140315 ns         4975 bytes_per_second=28.0566M/s items_per_second=7.1268k/s
tinyjambu_128_decrypt_64B_32B         3019 ns         3018 ns       231971 bytes_per_second=30.3375M/s items_per_second=331.367k/s
tinyjambu_128_decrypt_128B_32B        4871 ns         4869 ns       143809 bytes_per_second=31.3356M/s items_per_second=205.361k/s
tinyjambu_128_decrypt_256B_32B        8576 ns         8573 ns        81408 bytes_per_second=32.0378M/s items_per_second=116.646k/s
tinyjambu_128_decrypt_512B_32B       15986 ns        15980 ns        43743 bytes_per_second=32.465M/s items_per_second=62.5772k/s
tinyjambu_128_decrypt_1024B_32B      30808 ns        30795 ns        22714 bytes_per_second=32.7024M/s items_per_second=32.4725k/s
tinyjambu_128_decrypt_2048B_32B      60419 ns        60409 ns        11534 bytes_per_second=32.8366M/s items_per_second=16.5537k/s
tinyjambu_128_decrypt_4096B_32B     119736 ns       119695 ns         5835 bytes_per_second=32.8899M/s items_per_second=8.35456k/s
tinyjambu_192_decrypt_64B_32B         3631 ns         3630 ns       192712 bytes_per_second=25.2201M/s items_per_second=275.471k/s
tinyjambu_192_decrypt_128B_32B        5488 ns         5485 ns       127072 bytes_per_second=27.8175M/s items_per_second=182.305k/s
tinyjambu_192_decrypt_256B_32B       10578 ns        10572 ns        66078 bytes_per_second=25.98M/s items_per_second=94.5904k/s
tinyjambu_192_decrypt_512B_32B       19836 ns        19830 ns        35232 bytes_per_second=26.1619M/s items_per_second=50.4278k/s
tinyjambu_192_decrypt_1024B_32B      38378 ns        38357 ns        18257 bytes_per_second=26.2553M/s items_per_second=26.0707k/s
tinyjambu_192_decrypt_2048B_32B      75404 ns        75382 ns         9246 bytes_per_second=26.3147M/s items_per_second=13.2658k/s
tinyjambu_192_decrypt_4096B_32B     134789 ns       134739 ns         5189 bytes_per_second=29.2177M/s items_per_second=7.42175k/s
tinyjambu_256_decrypt_64B_32B         3468 ns         3467 ns       201869 bytes_per_second=26.4071M/s items_per_second=288.436k/s
tinyjambu_256_decrypt_128B_32B        5639 ns         5638 ns       123621 bytes_per_second=27.0637M/s items_per_second=177.365k/s
tinyjambu_256_decrypt_256B_32B        9986 ns         9983 ns        69786 bytes_per_second=27.5122M/s items_per_second=100.169k/s
tinyjambu_256_decrypt_512B_32B       18679 ns        18673 ns        37466 bytes_per_second=27.7833M/s items_per_second=53.5531k/s
tinyjambu_256_decrypt_1024B_32B      36058 ns        36049 ns        19404 bytes_per_second=27.9368M/s items_per_second=27.7404k/s
tinyjambu_256_decrypt_2048B_32B      70835 ns        70810 ns         9853 bytes_per_second=28.0136M/s items_per_second=14.1223k/s
tinyjambu_256_decrypt_4096B_32B     140386 ns       140327 ns         4977 bytes_per_second=28.0542M/s items_per_second=7.12621k/s
```

> Not mentioning any value for `FBK` parameter, results in setting `FBK=32` !

For benchmarking Python API of `tinyjambu`, issue following command

```bash
FBK=32 make bench_python
```

```bash
2022-04-28T12:57:04+05:30
Running bench_tinyjambu.py
Run on (4 X 1897.34 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.46, 0.45, 0.40
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
tinyjambu_128_encrypt      60135 ns        60132 ns        11183
tinyjambu_128_decrypt      59994 ns        59993 ns        11085
tinyjambu_192_encrypt      60280 ns        60278 ns        11107
tinyjambu_192_decrypt      63265 ns        63260 ns        11010
tinyjambu_256_encrypt      62988 ns        62980 ns        10757
tinyjambu_256_decrypt      60500 ns        60496 ns        10829
```

or when interested in computing 128 feedback bits each iteration

```bash
FBK=128 make bench_python
```

```bash
2022-04-28T12:56:35+05:30
Running bench_tinyjambu.py
Run on (4 X 1895.73 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 3072 KiB (x1)
Load Average: 0.54, 0.46, 0.41
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
tinyjambu_128_encrypt      70637 ns        70631 ns         9964
tinyjambu_128_decrypt      67828 ns        67823 ns        10086
tinyjambu_192_encrypt      68471 ns        68468 ns         9714
tinyjambu_192_decrypt      68530 ns        68525 ns         9193
tinyjambu_256_encrypt      69690 ns        69683 ns         9614
tinyjambu_256_decrypt      76305 ns        76295 ns         9593
```

## Usage

`tinyjambu` being a simple, zero-dependency, header-only C++ library, it's pretty easy to start using it. Just add `./include` path while compiling your program ( inform your compiler about this include path using `-I ./include` syntax ), which uses `tinyjambu`.

I've written following example programs which demonstrate use of TinyJambu-{128, 192, 256} C++ API

- [TinyJambu-128](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_128.cpp)
- [TinyJambu-193](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_193.cpp)
- [TinyJambu-256](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_256.cpp)
