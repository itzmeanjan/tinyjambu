#!/usr/bin/python3

"""
Took inspiration from https://github.com/google/benchmark/blob/dc901ff9090e2b931433790cc44afc3af3b09ab2/bindings/python/google_benchmark/example.py
when writing following benchmark suite !
"""

import tinyjambu as tj
import google_benchmark as benchmark
import random as rnd

AD_LEN = 32  # associated data byte length [ fixed ]
CT_LEN = 128  # plain/ cipher text byte length [ fixed ]


@benchmark.register
def tinyjambu_128_encrypt(state):
    key = rnd.randbytes(16)
    nonce = rnd.randbytes(12)
    data = rnd.randbytes(AD_LEN)
    text = rnd.randbytes(CT_LEN)

    while state:
        enc, tag = tj.tinyjambu_128_encrypt(key, nonce, data, text)

    flag, dec = tj.tinyjambu_128_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec


@benchmark.register
def tinyjambu_128_decrypt(state):
    key = rnd.randbytes(16)
    nonce = rnd.randbytes(12)
    data = rnd.randbytes(AD_LEN)
    text = rnd.randbytes(CT_LEN)

    enc, tag = tj.tinyjambu_128_encrypt(key, nonce, data, text)

    while state:
        flag, dec = tj.tinyjambu_128_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec


@benchmark.register
def tinyjambu_192_encrypt(state):
    key = rnd.randbytes(24)
    nonce = rnd.randbytes(12)
    data = rnd.randbytes(AD_LEN)
    text = rnd.randbytes(CT_LEN)

    while state:
        enc, tag = tj.tinyjambu_192_encrypt(key, nonce, data, text)

    flag, dec = tj.tinyjambu_192_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec


@benchmark.register
def tinyjambu_192_decrypt(state):
    key = rnd.randbytes(24)
    nonce = rnd.randbytes(12)
    data = rnd.randbytes(AD_LEN)
    text = rnd.randbytes(CT_LEN)

    enc, tag = tj.tinyjambu_192_encrypt(key, nonce, data, text)

    while state:
        flag, dec = tj.tinyjambu_192_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec


@benchmark.register
def tinyjambu_256_encrypt(state):
    key = rnd.randbytes(32)
    nonce = rnd.randbytes(12)
    data = rnd.randbytes(AD_LEN)
    text = rnd.randbytes(CT_LEN)

    while state:
        enc, tag = tj.tinyjambu_256_encrypt(key, nonce, data, text)

    flag, dec = tj.tinyjambu_256_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec


@benchmark.register
def tinyjambu_256_decrypt(state):
    key = rnd.randbytes(32)
    nonce = rnd.randbytes(12)
    data = rnd.randbytes(AD_LEN)
    text = rnd.randbytes(CT_LEN)

    enc, tag = tj.tinyjambu_256_encrypt(key, nonce, data, text)

    while state:
        flag, dec = tj.tinyjambu_256_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec


if __name__ == "__main__":
    benchmark.main()
