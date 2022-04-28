#!/usr/bin/python3

import tinyjambu as tj
import random as rnd


def main():
    AD_LEN = 32  # 32 -bytes of associated data
    CT_LEN = 64  # 64 -bytes of plain/ cipher text

    # random secret key of 128 -bit
    key = rnd.randbytes(16)
    # random nonce of 96 -bit
    nonce = rnd.randbytes(12)
    # random associated data
    data = rnd.randbytes(AD_LEN)
    # random plain text
    text = rnd.randbytes(CT_LEN)

    enc, tag = tj.tinyjambu_128_encrypt(key, nonce, data, text)
    flag, dec = tj.tinyjambu_128_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec

    print("TinyJambu-128 AEAD\n")
    print(f"Key                 : 0x{key.hex()}")
    print(f"Nonce               : 0x{nonce.hex()}")
    print(f"Associated Data     : 0x{data.hex()}")
    print(f"Plain Text          : 0x{text.hex()}")
    print(f"Cipher Text         : 0x{enc.hex()}")
    print(f"Authentication Tag  : 0x{tag.hex()}")
    print(f"Decrypted Text      : 0x{dec.hex()}")

    # ---

    # random secret key of 192 -bit
    key = rnd.randbytes(24)
    # random nonce of 96 -bit
    nonce = rnd.randbytes(12)
    # random associated data
    data = rnd.randbytes(AD_LEN)
    # random plain text
    text = rnd.randbytes(CT_LEN)

    enc, tag = tj.tinyjambu_192_encrypt(key, nonce, data, text)
    flag, dec = tj.tinyjambu_192_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec

    print("\nTinyJambu-192 AEAD\n")
    print(f"Key                 : 0x{key.hex()}")
    print(f"Nonce               : 0x{nonce.hex()}")
    print(f"Associated Data     : 0x{data.hex()}")
    print(f"Plain Text          : 0x{text.hex()}")
    print(f"Cipher Text         : 0x{enc.hex()}")
    print(f"Authentication Tag  : 0x{tag.hex()}")
    print(f"Decrypted Text      : 0x{dec.hex()}")

    # ---

    # random secret key of 256 -bit
    key = rnd.randbytes(32)
    # random nonce of 96 -bit
    nonce = rnd.randbytes(12)
    # random associated data
    data = rnd.randbytes(AD_LEN)
    # random plain text
    text = rnd.randbytes(CT_LEN)

    enc, tag = tj.tinyjambu_256_encrypt(key, nonce, data, text)
    flag, dec = tj.tinyjambu_256_decrypt(key, nonce, tag, data, enc)

    assert flag
    assert text == dec

    print("\nTinyJambu-256 AEAD\n")
    print(f"Key                 : 0x{key.hex()}")
    print(f"Nonce               : 0x{nonce.hex()}")
    print(f"Associated Data     : 0x{data.hex()}")
    print(f"Plain Text          : 0x{text.hex()}")
    print(f"Cipher Text         : 0x{enc.hex()}")
    print(f"Authentication Tag  : 0x{tag.hex()}")
    print(f"Decrypted Text      : 0x{dec.hex()}")


if __name__ == "__main__":
    main()
