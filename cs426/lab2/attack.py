#!/usr/bin/env python3

import argparse
import subprocess
import os

bad_padding = lambda output : "bad padding" in str(output)

BLOCK_SIZE = 16
tuner = "0000000000000000"

def attack(ciphertext):
    # oracle = subprocess.run(["./oracle.py", "-d", f"{ciphertext}"], capture_output=True)
    # returncode = oracle.returncode
    # output = oracle.stdout

    # Split ciphertext into blocks
    blocks = [ciphertext[i:i+BLOCK_SIZE] for i in range(0, len(ciphertext), BLOCK_SIZE)]

    # Current padding size working with (will be incremented)
    padding_size = 0x01

    # Plaintext string placeholder
    plaintext = ""

    for block_num, block in enumerate(blocks):
        if block_num == 1:
            continue

        # Work backwards from the last byte
        for i in range(BLOCK_SIZE - 1, -1, -1):
            # Find a value of tuner[i] such that ' tuner[i] || block ' is accepted by the oracle
            tuner = tune(tuner, block, i, padding_size)
            # Since we have a valid value of tuner, we can figure out X[i]
            x = int(tuner[15], 16) ^ padding_size               # X[15] = C'[15] ^ 0x01
            p = int(blocks[block_num - 1][i], 16) ^ x           # P2[15] = C1[15] ^ X[15]
            plaintext = plaintext + p
            
            # Find value of 
    






if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Attack the oracle.')
    parser.add_argument("-a", dest="ciphertext")
    args = parser.parse_args()
    attack(args.ciphertext)