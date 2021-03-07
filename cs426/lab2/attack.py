#!/usr/bin/env python3

import argparse
import subprocess
import os

bad_padding = lambda output : "bad padding" in str(output)

BLOCK_SIZE = 16


# TODO
# - 2 hex values is one byte!
# - change everything to reflect that

def tune(tuner, block, byte_index, padding_size):
    for i in range(16): # Go through each hex
        # First, concatenate tuner and block
        test_block = tuner + block
        print(f"\tTrying test_block: {test_block}")
        test_xor = int(tuner, 16) ^ int(block, 16)
        print(f"\t{int(tuner, 16)} ^ {int(block, 16)} = {hex(test_xor)}")

        # Send this block to the oracle for decryption
        oracle = subprocess.run(["./oracle.py", "-d", f"{test_block}"], capture_output=True)
        output = oracle.stdout
        print(f"\t{oracle.stdout}")

        # If the oracle throws a bad padding error, the test_block has invalid padding.
        if bad_padding(output):
            if tuner[byte_index] == "f":
                # Should never get here
                print("Something has gone horribly wrong!")
                exit(1)

            # Try the next hex value
            new_byte_val = str(hex(int(tuner[byte_index], 16) + 1)).strip("0x")
            tuner = tuner[:byte_index] + new_byte_val
            print(f"\t{tuner}")
        else:
            # Success!
            return tuner
        print()



def attack(ciphertext):
    # Split ciphertext into blocks
    blocks = [ciphertext[i:i+BLOCK_SIZE] for i in range(0, len(ciphertext), BLOCK_SIZE)]
    print(blocks)

    # Current padding size working with (will be incremented)
    padding_size = 0x01

    # Ciphertext block to test with
    tuner = "0000000000000000"

    # Plaintext string placeholder
    plaintext = ""

    for block_num, block in enumerate(blocks):
        if block_num == 0:
            continue

        print(f"Working with block #{block_num}")

        # Work backwards from the last byte
        for i in range(BLOCK_SIZE - 1, -1, -1):
            # Find a value of tuner[i] such that ' tuner || block ' is accepted by the oracle
            tuner = tune(tuner, block, i, padding_size)
            # Since we have a valid value of tuner, we can figure out X[i]
            x = hex(int(tuner[14:16], 16)) ^ padding_size               # X[15] = C'[15] ^ 0x01
            p = int(blocks[block_num - 1][i], 16) ^ x           # P2[15] = C1[15] ^ X[15]
            plaintext = plaintext + p
            
            # Change value of C[i] such that C[i] ^ X[i] = (padding_size + 1)
            # This is to prepare for the next iteration of the loop
            tuner = (padding_size + 1) ^ x
    return plaintext


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Attack the oracle.')
    parser.add_argument("-a", dest="ciphertext")
    args = parser.parse_args()
    plaintext = attack(args.ciphertext)
    print(plaintext)