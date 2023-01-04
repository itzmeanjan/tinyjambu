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
FBK=64 make   # 2x32 feedback bits per iteration
FBK=128 make  # 4x32 feedback bits per iteration
```

> **Note** You may safely skip specifying `FBK`, default choice `FBK=32` is automatically set !

## Benchmarking

Find micro-benchmarking ( using `google-benchmark` ) results [here](./bench/README.md)

## Usage

`tinyjambu` being a simple, zero-dependency, header-only C++ library, it's pretty easy to start using it. Just add `./include` path while compiling your program ( inform your compiler about this include path using `-I ./include` syntax ), which uses `tinyjambu`.

I've written following example programs which demonstrate use of TinyJambu-{128, 192, 256} C++ API

- [TinyJambu-128](./example/tinyjambu_128.cpp)
- [TinyJambu-192](./example/tinyjambu_192.cpp)
- [TinyJambu-256](./example/tinyjambu_256.cpp)

You may also want to use Python API of `tinyjambu`, consider checking [here](https://github.com/itzmeanjan/tinyjambu/blob/1082f55/wrapper/python/example.py) for usage example.
