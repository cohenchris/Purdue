#!/usr/bin/env python3

# AES-CBC Oracle
# Feb 21, 2021
# root@davejingtian.org

from Crypto.Cipher import AES
from Crypto import Random
import argparse
import binascii
import re


key_def = b'1122334455667788'
iv_def = b'1234567812345678'
block_size = AES.block_size
key_rand = Random.new().read(AES.block_size)
iv_rand = Random.new().read(AES.block_size)
debug = False
use_rand = False


def pad(s):
    '''
    PKCS#7
    '''
    pad_byte = block_size - len(s) % block_size
    for i in range(pad_byte):
        s.append(pad_byte)
    return s


def unpad(s):
    if debug:
        print("Debug: raw plaintxt=[" + str(s) + "], hex=[" + str(s.hex()) + "]")
    pad_num = s[-1]

    if pad_num == 0:
        return None

    for i in range(1, pad_num+1):
        if s[-i] != pad_num:
            return None
    return s[:-pad_num]


def encrypt(msg, iv):
    raw = pad(msg)
    if debug:
        print("Debug: padding=[" + str(raw) + "]")
    if use_rand:
        cipher = AES.new(key_rand, AES.MODE_CBC, iv_rand)
    else:
        cipher = AES.new(key_def, AES.MODE_CBC, iv)
    return cipher.encrypt(raw), iv


def decrypt(enc, iv):
    if use_rand:
        decipher = AES.new(key_rand, AES.MODE_CBC, iv_rand)
    else:
        decipher = AES.new(key_def, AES.MODE_CBC, iv)
    return unpad(decipher.decrypt(enc))



if __name__ == "__main__":
    iv = iv_def

    parser = argparse.ArgumentParser(description="AES-CBC Encryption/Decryption Oracle")
    parser.add_argument("-e", "--encrypt", help="encrypt a plaintxt in string format")
    parser.add_argument("-d", "--decrypt", help="decrypt a ciphertxt in hex string format")
    parser.add_argument("-i", "--iv", help="iv in string format (16-byte)")
    args = parser.parse_args()

    if args.iv:
        iv = bytearray(args.iv, "UTF-8")
    if debug:
        print('Using iv: ' + str(iv))

    if args.encrypt:
        if debug:
            print("Debug: plaintxt=[" + args.encrypt +"]")
        (cipher, iv) = encrypt(bytearray(args.encrypt, "UTF-8"), iv)
        print("Ciphertxt: " + str(binascii.hexlify(cipher)) + ", iv: " + str(iv))
    elif args.decrypt:
        if debug:
            print("Debug: ciphertxt=[" + args.decrypt + "]")
        rtn = decrypt(bytes.fromhex(args.decrypt), iv)
        if rtn == None:
            print("Error: bad padding")
        else:
            print("Plaintxt: " + str(rtn))
    else:
        print('Error: Wrong usage')

