# ---------------------------------- #
# DO NOT MODIFY THIS FILE            #
# ---------------------------------- #

import argparse
import os
import subprocess

# Source: https://stackoverflow.com/questions/15008758/parsing-boolean-values-with-argparse
def str2bool(v):
  if isinstance(v, bool):
    return v
  if v.lower() in ('yes', 'true', 't', 'y', '1'):
    return "True"
  elif v.lower() in ('no', 'false', 'f', 'n', '0'):
    return "False"
  else:
    raise argparse.ArgumentTypeError('Boolean value expected.')

parser = argparse.ArgumentParser(description="Concurrent program testing")
parser.add_argument('-s', choices=["none", "pct", "random"], default="none", action="store", dest="algorithm")
parser.add_argument('-n', type=int, default=1, action="store", dest="iterations")
parser.add_argument('-st', type=str2bool, default=False, action="store", dest="stacktraces")
parser.add_argument('-seed', type=int, default=0, action="store", dest="seed")
args = parser.parse_args()

m_env = os.environ.copy()
m_env["STACKTRACES"] = str(args.stacktraces)
m_env["ALGORITHM"] = args.algorithm

directory = "tests/"
for filename in os.listdir(directory):
  if not filename.endswith("_test"):
    continue
  full_filename = directory + filename
  command = "python3 new_framework.py -s={} -n={} -st={} -seed={} {}".format(args.algorithm, args.iterations, args.stacktraces, args.seed, full_filename)
  print(command)
  print(command.split(" "))
  rc = subprocess.run(command.split(" "))
  
rc = subprocess.run(["gcov", "testlib.gcda"])
