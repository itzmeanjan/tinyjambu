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
2022-05-09T10:43:10+05:30
Running ./bench/a.out
Run on (4 X 1800 MHz CPU s)
Load Average: 2.62, 3.21, 2.62
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
tinyjambu_128_encrypt/64/32          216 ns          214 ns      3182810 bytes_per_second=427.007M/s
tinyjambu_128_decrypt/64/32          229 ns          223 ns      3176595 bytes_per_second=410.417M/s
tinyjambu_128_encrypt/128/32         313 ns          312 ns      2226953 bytes_per_second=489.541M/s
tinyjambu_128_decrypt/128/32         328 ns          322 ns      2177845 bytes_per_second=473.611M/s
tinyjambu_128_encrypt/256/32         504 ns          491 ns      1415706 bytes_per_second=559.491M/s
tinyjambu_128_decrypt/256/32         499 ns          499 ns      1403780 bytes_per_second=550.961M/s
tinyjambu_128_encrypt/512/32         849 ns          849 ns       826057 bytes_per_second=611.308M/s
tinyjambu_128_decrypt/512/32         856 ns          856 ns       817213 bytes_per_second=606.2M/s
tinyjambu_128_encrypt/1024/32       1562 ns         1562 ns       447894 bytes_per_second=644.878M/s
tinyjambu_128_decrypt/1024/32       1570 ns         1570 ns       445427 bytes_per_second=641.444M/s
tinyjambu_128_encrypt/2048/32       3160 ns         3010 ns       233740 bytes_per_second=659.115M/s
tinyjambu_128_decrypt/2048/32       3010 ns         3000 ns       231534 bytes_per_second=661.164M/s
tinyjambu_128_encrypt/4096/32       5839 ns         5835 ns       119589 bytes_per_second=674.664M/s
tinyjambu_128_decrypt/4096/32       5849 ns         5846 ns       119334 bytes_per_second=673.464M/s
tinyjambu_192_encrypt/64/32          231 ns          231 ns      3028652 bytes_per_second=396.129M/s
tinyjambu_192_decrypt/64/32          241 ns          241 ns      2902854 bytes_per_second=379.852M/s
tinyjambu_192_encrypt/128/32         328 ns          327 ns      2139179 bytes_per_second=465.974M/s
tinyjambu_192_decrypt/128/32         338 ns          338 ns      2072097 bytes_per_second=451.596M/s
tinyjambu_192_encrypt/256/32         506 ns          506 ns      1380932 bytes_per_second=543.109M/s
tinyjambu_192_decrypt/256/32         516 ns          516 ns      1352568 bytes_per_second=532.438M/s
tinyjambu_192_encrypt/512/32         862 ns          862 ns       810011 bytes_per_second=602.183M/s
tinyjambu_192_decrypt/512/32         873 ns          873 ns       800428 bytes_per_second=594.592M/s
tinyjambu_192_encrypt/1024/32       1575 ns         1574 ns       444179 bytes_per_second=639.796M/s
tinyjambu_192_decrypt/1024/32       1586 ns         1585 ns       441155 bytes_per_second=635.237M/s
tinyjambu_192_encrypt/2048/32       3000 ns         2999 ns       233440 bytes_per_second=661.404M/s
tinyjambu_192_decrypt/2048/32       3015 ns         3013 ns       232417 bytes_per_second=658.299M/s
tinyjambu_192_encrypt/4096/32       5848 ns         5847 ns       119220 bytes_per_second=673.345M/s
tinyjambu_192_decrypt/4096/32       5862 ns         5860 ns       118927 bytes_per_second=671.799M/s
tinyjambu_256_encrypt/64/32          215 ns          215 ns      3250097 bytes_per_second=426.069M/s
tinyjambu_256_decrypt/64/32          221 ns          221 ns      3157166 bytes_per_second=413.467M/s
tinyjambu_256_encrypt/128/32         313 ns          313 ns      2236083 bytes_per_second=487.729M/s
tinyjambu_256_decrypt/128/32         319 ns          319 ns      2190362 bytes_per_second=477.855M/s
tinyjambu_256_encrypt/256/32         491 ns          491 ns      1424632 bytes_per_second=559.742M/s
tinyjambu_256_decrypt/256/32         498 ns          498 ns      1407189 bytes_per_second=551.76M/s
tinyjambu_256_encrypt/512/32         847 ns          847 ns       824256 bytes_per_second=612.598M/s
tinyjambu_256_decrypt/512/32         854 ns          853 ns       817900 bytes_per_second=607.954M/s
tinyjambu_256_encrypt/1024/32       1561 ns         1560 ns       447668 bytes_per_second=645.549M/s
tinyjambu_256_decrypt/1024/32       1566 ns         1565 ns       446041 bytes_per_second=643.338M/s
tinyjambu_256_encrypt/2048/32       2986 ns         2985 ns       234492 bytes_per_second=664.52M/s
tinyjambu_256_decrypt/2048/32       2991 ns         2991 ns       233925 bytes_per_second=663.251M/s
tinyjambu_256_encrypt/4096/32       5842 ns         5838 ns       119566 bytes_per_second=674.298M/s
tinyjambu_256_decrypt/4096/32       5844 ns         5842 ns       119292 bytes_per_second=673.887M/s
```

or 

```bash
FBK=128 make benchmark
```

```bash
2022-05-09T10:44:52+05:30
Running ./bench/a.out
Run on (4 X 1800 MHz CPU s)
Load Average: 2.14, 2.91, 2.57
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
tinyjambu_128_encrypt/64/32          193 ns          191 ns      3641451 bytes_per_second=480.273M/s
tinyjambu_128_decrypt/64/32          201 ns          198 ns      3558422 bytes_per_second=463.242M/s
tinyjambu_128_encrypt/128/32         292 ns          286 ns      2462547 bytes_per_second=533.676M/s
tinyjambu_128_decrypt/128/32         291 ns          291 ns      2400488 bytes_per_second=525.116M/s
tinyjambu_128_encrypt/256/32         478 ns          469 ns      1508424 bytes_per_second=586.072M/s
tinyjambu_128_decrypt/256/32         475 ns          474 ns      1448338 bytes_per_second=579.215M/s
tinyjambu_128_encrypt/512/32         833 ns          826 ns       841159 bytes_per_second=628.288M/s
tinyjambu_128_decrypt/512/32         853 ns          840 ns       829213 bytes_per_second=617.852M/s
tinyjambu_128_encrypt/1024/32       1623 ns         1553 ns       456523 bytes_per_second=648.398M/s
tinyjambu_128_decrypt/1024/32       1559 ns         1548 ns       450491 bytes_per_second=650.563M/s
tinyjambu_128_encrypt/2048/32       2988 ns         2971 ns       233836 bytes_per_second=667.718M/s
tinyjambu_128_decrypt/2048/32       3093 ns         2995 ns       235562 bytes_per_second=662.317M/s
tinyjambu_128_encrypt/4096/32       5916 ns         5861 ns       119073 bytes_per_second=671.728M/s
tinyjambu_128_decrypt/4096/32       5817 ns         5814 ns       119346 bytes_per_second=677.112M/s
tinyjambu_192_encrypt/64/32          188 ns          188 ns      3724889 bytes_per_second=487.983M/s
tinyjambu_192_decrypt/64/32          196 ns          196 ns      3552895 bytes_per_second=466.802M/s
tinyjambu_192_encrypt/128/32         286 ns          286 ns      2446996 bytes_per_second=533.318M/s
tinyjambu_192_decrypt/128/32         293 ns          293 ns      2389541 bytes_per_second=521.086M/s
tinyjambu_192_encrypt/256/32         464 ns          464 ns      1507097 bytes_per_second=591.852M/s
tinyjambu_192_decrypt/256/32         471 ns          471 ns      1485882 bytes_per_second=583.323M/s
tinyjambu_192_encrypt/512/32         820 ns          820 ns       850925 bytes_per_second=632.669M/s
tinyjambu_192_decrypt/512/32         828 ns          827 ns       844401 bytes_per_second=627.072M/s
tinyjambu_192_encrypt/1024/32       1533 ns         1533 ns       456060 bytes_per_second=657.011M/s
tinyjambu_192_decrypt/1024/32       1541 ns         1540 ns       453892 bytes_per_second=653.872M/s
tinyjambu_192_encrypt/2048/32       2964 ns         2962 ns       236348 bytes_per_second=669.619M/s
tinyjambu_192_decrypt/2048/32       2967 ns         2966 ns       235909 bytes_per_second=668.893M/s
tinyjambu_192_encrypt/4096/32       5812 ns         5808 ns       119998 bytes_per_second=677.765M/s
tinyjambu_192_decrypt/4096/32       5818 ns         5816 ns       119841 bytes_per_second=676.898M/s
tinyjambu_256_encrypt/64/32          186 ns          186 ns      3743209 bytes_per_second=491.209M/s
tinyjambu_256_decrypt/64/32          194 ns          194 ns      3603944 bytes_per_second=472.872M/s
tinyjambu_256_encrypt/128/32         285 ns          285 ns      2449205 bytes_per_second=534.658M/s
tinyjambu_256_decrypt/128/32         291 ns          291 ns      2400698 bytes_per_second=524.439M/s
tinyjambu_256_encrypt/256/32         464 ns          464 ns      1508813 bytes_per_second=592.532M/s
tinyjambu_256_decrypt/256/32         470 ns          469 ns      1491662 bytes_per_second=585.198M/s
tinyjambu_256_encrypt/512/32         820 ns          820 ns       851391 bytes_per_second=632.875M/s
tinyjambu_256_decrypt/512/32         825 ns          825 ns       844338 bytes_per_second=628.799M/s
tinyjambu_256_encrypt/1024/32       1533 ns         1532 ns       456028 bytes_per_second=657.202M/s
tinyjambu_256_decrypt/1024/32       1539 ns         1538 ns       454670 bytes_per_second=654.627M/s
tinyjambu_256_encrypt/2048/32       2959 ns         2958 ns       236706 bytes_per_second=670.618M/s
tinyjambu_256_decrypt/2048/32       2966 ns         2964 ns       236129 bytes_per_second=669.179M/s
tinyjambu_256_encrypt/4096/32       5811 ns         5808 ns       120053 bytes_per_second=677.848M/s
tinyjambu_256_decrypt/4096/32       5820 ns         5817 ns       119927 bytes_per_second=676.767M/s
```

> Not mentioning any value for `FBK` parameter, results in setting `FBK=32` !

For benchmarking Python API of `tinyjambu`, issue following command

```bash
FBK=32 make bench_python
```

```bash
2022-05-09T05:20:22+00:00
Running bench_tinyjambu.py
Run on (4 X 2300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.14, 0.03, 0.01
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
tinyjambu_128_encrypt      36198 ns        36198 ns        19178
tinyjambu_128_decrypt      36955 ns        36954 ns        19055
tinyjambu_192_encrypt      36808 ns        36808 ns        19155
tinyjambu_192_decrypt      37485 ns        37483 ns        18694
tinyjambu_256_encrypt      37357 ns        37355 ns        18879
tinyjambu_256_decrypt      36487 ns        36486 ns        19137
```

or when interested in computing 128 feedback bits each iteration

```bash
FBK=128 make bench_python
```

```bash
2022-05-09T05:21:00+00:00
Running bench_tinyjambu.py
Run on (4 X 2300 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 46080 KiB (x1)
Load Average: 0.13, 0.04, 0.01
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
tinyjambu_128_encrypt      35533 ns        35530 ns        19457
tinyjambu_128_decrypt      36372 ns        36367 ns        19464
tinyjambu_192_encrypt      36386 ns        36385 ns        19260
tinyjambu_192_decrypt      36435 ns        36430 ns        19160
tinyjambu_256_encrypt      36534 ns        36534 ns        19196
tinyjambu_256_decrypt      35389 ns        35388 ns        19920
```

## Usage

`tinyjambu` being a simple, zero-dependency, header-only C++ library, it's pretty easy to start using it. Just add `./include` path while compiling your program ( inform your compiler about this include path using `-I ./include` syntax ), which uses `tinyjambu`.

I've written following example programs which demonstrate use of TinyJambu-{128, 192, 256} C++ API

- [TinyJambu-128](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_128.cpp)
- [TinyJambu-193](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_193.cpp)
- [TinyJambu-256](https://github.com/itzmeanjan/tinyjambu/blob/dc631cd/example/tinyjambu_256.cpp)

You may also want to use Python API of `tinyjambu`, consider checking [here](https://github.com/itzmeanjan/tinyjambu/blob/1082f55/wrapper/python/example.py) for usage example.
