# Chris Cohen
# Intro to Cryptography - Fall 2020
# HW06

import time


def group_by_threes(factors):
    return [factors[n:n+3] for n in range(0, len(factors), 3)]

def find_cube_root(num):
    # Binary search to find cube root    
    low = 1
    high = num
    mid = 0

    while low <= high:
        mid = (low + high) // 2
        cube = mid**3

        if cube == num:
            # Found cube root - break
            return mid
        elif cube > num:
            high = mid - 1
        else: # cube < num
            low = mid + 1

    # No cube root found
    return -1


if __name__ == "__main__":
    start_time = time.time()
    inputs = []

    # Parse inputs from inputs.txt
    with open("inputs.txt", "r") as f:
        inputs = [line.strip() for line in f.readlines() if line.strip()]

    # Find cube root for each input
    cube_roots = [find_cube_root(int(num, base=2)) for num in inputs]
    # Convert each root back to binary
    cube_roots = [bin(root)[2:] for root in cube_roots]

    # Write all results to outputs.txt
    with open("outputs.txt", "w") as f:
        [f.write(str(root) + "\n") for root in cube_roots]

    elapsed_time = time.time() - start_time
    print(f"\nElapsed time: {elapsed_time:.10f}s")