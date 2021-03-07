#!/usr/bin/env python3

import argparse
import subprocess
import os

bad_padding = lambda output : "bad padding" in str(output)

BLOCK_SIZE = 16 # 16 byte fixed block size
NUM_BLOCK_VALUES = BLOCK_SIZE * BLOCK_SIZE

def tune(tuner, block, byte_index, padding_size):
    for i in range(NUM_BLOCK_VALUES): # Go through each possible hex value for each byte
        # First, concatenate tuner and block
        test_block = tuner + block

        # Send this block to the oracle for decryption
        oracle = subprocess.run(["./oracle.py", "-d", f"{test_block}"], capture_output=True)
        output = oracle.stdout

        # If the oracle throws a bad padding error, the test_block has invalid padding.
        if bad_padding(output):
            if tuner[byte_index:byte_index+2] == "ff":
                # Should never get here
                return
            # Try the next hex value
            # Increment current byte by one
            new_byte_val = str(hex(int(tuner[byte_index:byte_index+2], 16) + 1))[2:]
            # Edit string to reflect new value
            new_byte_val = new_byte_val.zfill(2)    # Pad with zeroes to make sure it's always length 2\

            if byte_index == 30:
                # Avoid calling tuner[32], which doesn't exist and could cause issues
                tuner = tuner[:byte_index] + new_byte_val
            else:
                tuner = tuner[:byte_index] + new_byte_val + tuner[byte_index+2:]
        else:
            # Success!
            return tuner


def update_tuner(padding_size, x_vals, byte_index):
    # byte_num = byte_index / 2
    # tuner[15] = x[15] ^ padding_size
    # tuner[14] = x[14] ^ padding_size
    # ...
    # tuner[byte_num] = x[byte_num] ^ padding_size

    new_tuner = ""

    byte_num = int(byte_index / 2)

    for i in range(15, byte_num - 1, -1):
        new_tuner = hex(int(padding_size, 16) ^ int(x_vals[i], 16))[2:].zfill(2) + new_tuner


    new_tuner = new_tuner.zfill(32)
    return new_tuner

def attack(ciphertext):
    # Plaintext string placeholder
    plaintext = ""
    try:
        # Split ciphertext into blocks
        blocks = [ciphertext[i:i+(BLOCK_SIZE*2)] for i in range(0, len(ciphertext), BLOCK_SIZE*2)]

        for block_num, block in reversed(list(enumerate(blocks))):
            if block_num == 0:
                continue

            # Current padding size working with (will be incremented)
            padding_size = "1"

            # Ciphertext block to test with
            tuner = "00000000000000000000000000000000"

            # values of X for later use in updating tuner
            x_vals = ["ff"] * 16


            # Work backwards from the last byte
            for i in range((BLOCK_SIZE*2) - 2, -1, -2):
                # byte = block[i:i+2]
                # Find a value of tuner[i] such that ' tuner || block ' is accepted by the oracle
                tuner = tune(tuner, block, i, padding_size)
                # Since we have a valid value of tuner, we can figure out X[i]
                x = int(tuner[i:i+2], 16) ^ int(padding_size, 16)   # X[15] = C'[15] ^ 0x01
                # Store 'x' in x_vals[i/2] (for later use in updating the tuner)
                x_vals[int(i/2)] = hex(x)[2:].zfill(2)
                p = int(blocks[block_num - 1][i:i+2], 16) ^ x           # P2[15] = C1[15] ^ X[15]
                plaintext = hex(p)[2:].zfill(2) + plaintext
                
                # Increment padding_size
                padding_size = hex(int(padding_size, 16) + 1)[2:].zfill(2)
                # Change value of C[i] such that C[i] ^ X[i] = (old_padding_size + 1)
                # This is to prepare for the next iteration of the loop
                tuner = update_tuner(padding_size, x_vals, i)
    except Exception as e:
        print(e)

    return plaintext


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Attack the oracle.')
    parser.add_argument("-a", dest="ciphertext")
    args = parser.parse_args()
    plaintext = attack(args.ciphertext)
    print(plaintext)