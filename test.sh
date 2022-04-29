#!/bin/bash

# generate shared library object
make lib

# ---

mkdir -p tmp
pushd tmp

# download compressed NIST LWC submission of TinyJambu
wget -O tinyjambu.zip https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/finalist-round/updated-submissions/tinyjambu.zip
# uncomress
unzip tinyjambu.zip

# copy Known Answer Tests outside of uncompressed NIST LWC submission directory
cp tinyjambu/Implementations/crypto_aead/tinyjambu128v2/LWC_AEAD_KAT_128_96.txt ../
cp tinyjambu/Implementations/crypto_aead/tinyjambu192v2/LWC_AEAD_KAT_192_96.txt ../
cp tinyjambu/Implementations/crypto_aead/tinyjambu256v2/LWC_AEAD_KAT_256_96.txt ../

popd

# ---

# remove NIST LWC submission zip
rm -rf tmp

# move Known Answer Tests to execution directory
mv LWC_AEAD_KAT_128_96.txt wrapper/python/
mv LWC_AEAD_KAT_192_96.txt wrapper/python/
mv LWC_AEAD_KAT_256_96.txt wrapper/python/

# ---

pushd wrapper/python

# run tests
pytest -v

# clean up
rm LWC_AEAD_KAT_*_96.txt

popd

# ---
