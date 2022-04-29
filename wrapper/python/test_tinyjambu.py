#!/usr/bin/python3

import tinyjambu as tj
import numpy as np

# type alias
u8 = np.uint8


def test_tinyjambu_128_kat():
    """
    Tests functional correctness of TinyJambu-128 implementation, using
    Known Answer Tests submitted along with final round submission of TinyJambu in NIST LWC

    See https://csrc.nist.gov/projects/lightweight-cryptography/finalists
    """
    count = 0  # -many KATs to be run

    with open("LWC_AEAD_KAT_128_96.txt", "r") as fd:
        while True:
            cnt = fd.readline()
            if not cnt:
                # no more KATs remaining
                break

            key = fd.readline()
            nonce = fd.readline()
            pt = fd.readline()
            ad = fd.readline()
            ct = fd.readline()

            # extract out required fields
            cnt = int([i.strip() for i in cnt.split("=")][-1])
            key = [i.strip() for i in key.split("=")][-1]
            nonce = [i.strip() for i in nonce.split("=")][-1]
            pt = [i.strip() for i in pt.split("=")][-1]
            ad = [i.strip() for i in ad.split("=")][-1]

            # 128 -bit secret key
            key = int(f"0x{key}", base=16).to_bytes(16, "big")
            # 96 -bit nonce
            nonce = int(f"0x{nonce}", base=16).to_bytes(12, "big")
            # plain text
            pt = bytes(
                [
                    int(f"0x{pt[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(pt) >> 1)
                ]
            )
            # associated data
            ad = bytes(
                [
                    int(f"0x{ad[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ad) >> 1)
                ]
            )

            cipher, tag = tj.tinyjambu_128_encrypt(key, nonce, ad, pt)
            flag, text = tj.tinyjambu_128_decrypt(key, nonce, tag, ad, cipher)

            assert (
                pt == text and flag
            ), f"[TinyJambu-128 KAT {cnt}] expected 0x{pt.hex()}, found 0x{text.hex()} !"

            # don't need this line, so discard
            fd.readline()
            # to keep track of how many KATs executed !
            count = cnt

    print(f"[test] passed {count} -many TinyJambu-128 KAT(s)")


def test_tinyjambu_192_kat():
    """
    Tests functional correctness of TinyJambu-192 implementation, using
    Known Answer Tests submitted along with final round submission of TinyJambu in NIST LWC

    See https://csrc.nist.gov/projects/lightweight-cryptography/finalists
    """
    count = 0  # -many KATs to be run

    with open("LWC_AEAD_KAT_192_96.txt", "r") as fd:
        while True:
            cnt = fd.readline()
            if not cnt:
                # no more KATs remaining
                break

            key = fd.readline()
            nonce = fd.readline()
            pt = fd.readline()
            ad = fd.readline()
            ct = fd.readline()

            # extract out required fields
            cnt = int([i.strip() for i in cnt.split("=")][-1])
            key = [i.strip() for i in key.split("=")][-1]
            nonce = [i.strip() for i in nonce.split("=")][-1]
            pt = [i.strip() for i in pt.split("=")][-1]
            ad = [i.strip() for i in ad.split("=")][-1]

            # 192 -bit secret key
            key = int(f"0x{key}", base=16).to_bytes(24, "big")
            # 96 -bit nonce
            nonce = int(f"0x{nonce}", base=16).to_bytes(12, "big")
            # plain text
            pt = bytes(
                [
                    int(f"0x{pt[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(pt) >> 1)
                ]
            )
            # associated data
            ad = bytes(
                [
                    int(f"0x{ad[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ad) >> 1)
                ]
            )

            cipher, tag = tj.tinyjambu_192_encrypt(key, nonce, ad, pt)
            flag, text = tj.tinyjambu_192_decrypt(key, nonce, tag, ad, cipher)

            assert (
                pt == text and flag
            ), f"[TinyJambu-192 KAT {cnt}] expected 0x{pt.hex()}, found 0x{text.hex()} !"

            # don't need this line, so discard
            fd.readline()
            # to keep track of how many KATs executed !
            count = cnt

    print(f"[test] passed {count} -many TinyJambu-192 KAT(s)")


def test_tinyjambu_256_kat():
    """
    Tests functional correctness of TinyJambu-256 implementation, using
    Known Answer Tests submitted along with final round submission of TinyJambu in NIST LWC

    See https://csrc.nist.gov/projects/lightweight-cryptography/finalists
    """
    count = 0  # -many KATs to be run

    with open("LWC_AEAD_KAT_256_96.txt", "r") as fd:
        while True:
            cnt = fd.readline()
            if not cnt:
                # no more KATs remaining
                break

            key = fd.readline()
            nonce = fd.readline()
            pt = fd.readline()
            ad = fd.readline()
            ct = fd.readline()

            # extract out required fields
            cnt = int([i.strip() for i in cnt.split("=")][-1])
            key = [i.strip() for i in key.split("=")][-1]
            nonce = [i.strip() for i in nonce.split("=")][-1]
            pt = [i.strip() for i in pt.split("=")][-1]
            ad = [i.strip() for i in ad.split("=")][-1]

            # 256 -bit secret key
            key = int(f"0x{key}", base=16).to_bytes(32, "big")
            # 96 -bit nonce
            nonce = int(f"0x{nonce}", base=16).to_bytes(12, "big")
            # plain text
            pt = bytes(
                [
                    int(f"0x{pt[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(pt) >> 1)
                ]
            )
            # associated data
            ad = bytes(
                [
                    int(f"0x{ad[(i << 1): ((i+1) << 1)]}", base=16)
                    for i in range(len(ad) >> 1)
                ]
            )

            cipher, tag = tj.tinyjambu_256_encrypt(key, nonce, ad, pt)
            flag, text = tj.tinyjambu_256_decrypt(key, nonce, tag, ad, cipher)

            assert (
                pt == text and flag
            ), f"[TinyJambu-256 KAT {cnt}] expected 0x{pt.hex()}, found 0x{text.hex()} !"

            # don't need this line, so discard
            fd.readline()
            # to keep track of how many KATs executed !
            count = cnt

    print(f"[test] passed {count} -many TinyJambu-256 KAT(s)")


if __name__ == "__main__":
    print("use `pytest` for running test cases !")
