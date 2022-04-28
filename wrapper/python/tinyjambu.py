#!/usr/bin/python3


'''
  Before using `tinyjambu` library module, make sure you've run
  `make lib` and generated shared library object, which is loaded
  here; then all function calls are forwarded to respective C++
  implementation, executed on host CPU.

  Author: Anjan Roy <hello@itzmeanjan.in>
  
  Project: https://github.com/itzmeanjan/tinyjambu
'''

import ctypes as ct
from genericpath import exists
from posixpath import abspath
from typing import Tuple
import numpy as np

# path to shared library object
SO_PATH: str = abspath("../libtinyjambu.so")
# ensure shared library object exists, before creating
# handle to it
assert exists(SO_PATH), "Use `make lib` to generate shared library object !"

# handle to shared library object
SO_LIB: ct.CDLL = ct.CDLL(SO_PATH)


# all possible data types for several function parameters & return types
u8 = np.uint8
len_t = ct.c_size_t
uint8_tp = np.ctypeslib.ndpointer(dtype=u8, ndim=1, flags="CONTIGUOUS")
bool_t = ct.c_bool


def tinyjambu_128_encrypt(
    key: bytes, nonce: bytes, data: bytes, text: bytes
) -> Tuple[bytes, bytes]:
    """
    Encrypts M ( >=0 ) -many plain text bytes, while using 16 -bytes secret key,
    12 -bytes public message nonce & N ( >=0 ) -bytes associated data, while producing
    M -bytes cipher text & 8 -bytes authentication tag ( in order )
    """
    assert len(key) == 16, "TinyJambu-128 takes 16 -bytes secret key !"
    assert len(nonce) == 12, "TinyJambu-128 takes 12 -bytes nonce !"

    ad_len = len(data)
    ct_len = len(text)

    key_ = np.frombuffer(key, dtype=u8)
    nonce_ = np.frombuffer(nonce, dtype=u8)
    data_ = np.frombuffer(data, dtype=u8)
    text_ = np.frombuffer(text, dtype=u8)
    enc = np.empty(ct_len, dtype=u8)
    tag = np.empty(8, dtype=u8)

    args = [uint8_tp, uint8_tp, uint8_tp, len_t, uint8_tp, uint8_tp, len_t, uint8_tp]
    SO_LIB.tinyjambu_128_encrypt.argtypes = args

    SO_LIB.tinyjambu_128_encrypt(key_, nonce_, data_, ad_len, text_, enc, ct_len, tag)

    enc_ = enc.tobytes()
    tag_ = tag.tobytes()

    return enc_, tag_


def tinyjambu_128_decrypt(
    key: bytes, nonce: bytes, tag: bytes, data: bytes, enc: bytes
) -> Tuple[bool, bytes]:
    """
    Decrypts M ( >=0 ) -many cipher text bytes, while using 16 -bytes secret key,
    12 -bytes public message nonce, 8 -bytes authentication tag & N ( >=0 ) -bytes associated data, while producing
    boolean flag denoting verification status & M -bytes plain text ( in order )
    """
    assert len(key) == 16, "TinyJambu-128 takes 16 -bytes secret key !"
    assert len(nonce) == 12, "TinyJambu-128 takes 12 -bytes nonce !"
    assert len(tag) == 8, "TinyJambu-128 takes 8 -bytes authentication tag !"

    ad_len = len(data)
    ct_len = len(enc)

    key_ = np.frombuffer(key, dtype=u8)
    nonce_ = np.frombuffer(nonce, dtype=u8)
    tag_ = np.frombuffer(tag, dtype=u8)
    data_ = np.frombuffer(data, dtype=u8)
    enc_ = np.frombuffer(enc, dtype=u8)
    dec = np.empty(ct_len, dtype=u8)

    args = [uint8_tp, uint8_tp, uint8_tp, uint8_tp, len_t, uint8_tp, uint8_tp, len_t]
    SO_LIB.tinyjambu_128_decrypt.argtypes = args
    SO_LIB.tinyjambu_128_decrypt.restype = bool_t

    f = SO_LIB.tinyjambu_128_decrypt(
        key_, nonce_, tag_, data_, ad_len, enc_, dec, ct_len
    )

    dec_ = dec.tobytes()

    return f, dec_

def tinyjambu_192_encrypt(
    key: bytes, nonce: bytes, data: bytes, text: bytes
) -> Tuple[bytes, bytes]:
    """
    Encrypts M ( >=0 ) -many plain text bytes, while using 24 -bytes secret key,
    12 -bytes public message nonce & N ( >=0 ) -bytes associated data, while producing
    M -bytes cipher text & 8 -bytes authentication tag ( in order )
    """
    assert len(key) == 24, "TinyJambu-192 takes 24 -bytes secret key !"
    assert len(nonce) == 12, "TinyJambu-192 takes 12 -bytes nonce !"

    ad_len = len(data)
    ct_len = len(text)

    key_ = np.frombuffer(key, dtype=u8)
    nonce_ = np.frombuffer(nonce, dtype=u8)
    data_ = np.frombuffer(data, dtype=u8)
    text_ = np.frombuffer(text, dtype=u8)
    enc = np.empty(ct_len, dtype=u8)
    tag = np.empty(8, dtype=u8)

    args = [uint8_tp, uint8_tp, uint8_tp, len_t, uint8_tp, uint8_tp, len_t, uint8_tp]
    SO_LIB.tinyjambu_192_encrypt.argtypes = args

    SO_LIB.tinyjambu_192_encrypt(key_, nonce_, data_, ad_len, text_, enc, ct_len, tag)

    enc_ = enc.tobytes()
    tag_ = tag.tobytes()

    return enc_, tag_


def tinyjambu_192_decrypt(
    key: bytes, nonce: bytes, tag: bytes, data: bytes, enc: bytes
) -> Tuple[bool, bytes]:
    """
    Decrypts M ( >=0 ) -many cipher text bytes, while using 24 -bytes secret key,
    12 -bytes public message nonce, 8 -bytes authentication tag & N ( >=0 ) -bytes associated data, while producing
    boolean flag denoting verification status & M -bytes plain text ( in order )
    """
    assert len(key) == 24, "TinyJambu-192 takes 24 -bytes secret key !"
    assert len(nonce) == 12, "TinyJambu-192 takes 12 -bytes nonce !"
    assert len(tag) == 8, "TinyJambu-192 takes 8 -bytes authentication tag !"

    ad_len = len(data)
    ct_len = len(enc)

    key_ = np.frombuffer(key, dtype=u8)
    nonce_ = np.frombuffer(nonce, dtype=u8)
    tag_ = np.frombuffer(tag, dtype=u8)
    data_ = np.frombuffer(data, dtype=u8)
    enc_ = np.frombuffer(enc, dtype=u8)
    dec = np.empty(ct_len, dtype=u8)

    args = [uint8_tp, uint8_tp, uint8_tp, uint8_tp, len_t, uint8_tp, uint8_tp, len_t]
    SO_LIB.tinyjambu_192_decrypt.argtypes = args
    SO_LIB.tinyjambu_192_decrypt.restype = bool_t

    f = SO_LIB.tinyjambu_192_decrypt(
        key_, nonce_, tag_, data_, ad_len, enc_, dec, ct_len
    )

    dec_ = dec.tobytes()

    return f, dec_

def tinyjambu_256_encrypt(
    key: bytes, nonce: bytes, data: bytes, text: bytes
) -> Tuple[bytes, bytes]:
    """
    Encrypts M ( >=0 ) -many plain text bytes, while using 32 -bytes secret key,
    12 -bytes public message nonce & N ( >=0 ) -bytes associated data, while producing
    M -bytes cipher text & 8 -bytes authentication tag ( in order )
    """
    assert len(key) == 32, "TinyJambu-192 takes 32 -bytes secret key !"
    assert len(nonce) == 12, "TinyJambu-192 takes 12 -bytes nonce !"

    ad_len = len(data)
    ct_len = len(text)

    key_ = np.frombuffer(key, dtype=u8)
    nonce_ = np.frombuffer(nonce, dtype=u8)
    data_ = np.frombuffer(data, dtype=u8)
    text_ = np.frombuffer(text, dtype=u8)
    enc = np.empty(ct_len, dtype=u8)
    tag = np.empty(8, dtype=u8)

    args = [uint8_tp, uint8_tp, uint8_tp, len_t, uint8_tp, uint8_tp, len_t, uint8_tp]
    SO_LIB.tinyjambu_256_encrypt.argtypes = args

    SO_LIB.tinyjambu_256_encrypt(key_, nonce_, data_, ad_len, text_, enc, ct_len, tag)

    enc_ = enc.tobytes()
    tag_ = tag.tobytes()

    return enc_, tag_


def tinyjambu_256_decrypt(
    key: bytes, nonce: bytes, tag: bytes, data: bytes, enc: bytes
) -> Tuple[bool, bytes]:
    """
    Decrypts M ( >=0 ) -many cipher text bytes, while using 32 -bytes secret key,
    12 -bytes public message nonce, 8 -bytes authentication tag & N ( >=0 ) -bytes associated data, while producing
    boolean flag denoting verification status & M -bytes plain text ( in order )
    """
    assert len(key) == 32, "TinyJambu-256 takes 24 -bytes secret key !"
    assert len(nonce) == 12, "TinyJambu-256 takes 12 -bytes nonce !"
    assert len(tag) == 8, "TinyJambu-256 takes 8 -bytes authentication tag !"

    ad_len = len(data)
    ct_len = len(enc)

    key_ = np.frombuffer(key, dtype=u8)
    nonce_ = np.frombuffer(nonce, dtype=u8)
    tag_ = np.frombuffer(tag, dtype=u8)
    data_ = np.frombuffer(data, dtype=u8)
    enc_ = np.frombuffer(enc, dtype=u8)
    dec = np.empty(ct_len, dtype=u8)

    args = [uint8_tp, uint8_tp, uint8_tp, uint8_tp, len_t, uint8_tp, uint8_tp, len_t]
    SO_LIB.tinyjambu_256_decrypt.argtypes = args
    SO_LIB.tinyjambu_256_decrypt.restype = bool_t

    f = SO_LIB.tinyjambu_256_decrypt(
        key_, nonce_, tag_, data_, ad_len, enc_, dec, ct_len
    )

    dec_ = dec.tobytes()

    return f, dec_
