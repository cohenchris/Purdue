# ----------------------------------------------------------------------#
# In this file you need to add code to run your unit testing framework. #
# You can refer to coverage.py for ideas on how you could do this.      #
# This program gets called by the Makefile when running "make test"     #
# ----------------------------------------------------------------------#

import argparse
import os
import subprocess

import time
start_time = time.time()

# This is only a placeholder, you can do much better!
# rc = subprocess.run(["python3", "coverage.py"])

# +-----------+-------------------+-------------------+
# | flag      |   description     |       input       |
# +-----------+-------------------+-------------------+
# | -s        | scheduling policy | none, pct, random |
# | -n        | # times to run    | <some int > 0>    |
# | -st       | stacktrace enable | true, false       |
# | -seed     | seed for tester   | <some int>        |
# +-----------+-------------------+-------------------+

SCHEDULING_POLICIES = ["none", "pct", "random"] # -s    (Different vals for the scheduling policy)
MAX_RUN = 1                                     # -n    (Max number of times to run).
STACKTRACE_VALS = ["true", "false"]             # -st   (enable/disable stack tracing)
MAX_SEED = 1                                    # -seed (Max seed to use)

return_codes = []                               # Will end up being an array of all return codes

for seed in range(MAX_SEED):
    for run in range(MAX_RUN):
        for stacktrace in STACKTRACE_VALS:
            for scheduling_policy in SCHEDULING_POLICIES:
                run_command = ["python3", "coverage.py", "-s", f"{scheduling_policy}", "-n", f"{run + 1}", "-st", f"{stacktrace}", "-seed" , f"{seed}"]
                rc = subprocess.run(run_command)

elapsed_secs = time.time() - start_time
elapsed_mins = elapsed_secs / 60.0
print(f"\n\n TIME ELAPSED: {elapsed_secs:.2f} seconds, {elapsed_mins:.2f} mins\n")