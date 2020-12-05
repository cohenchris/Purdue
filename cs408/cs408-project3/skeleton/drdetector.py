#!/usr/bin/env python3
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

if not os.path.exists("testlib.so"):
  print("testlib.so not found! Make sure to compile it with \"make library\"!")
  exit(1)


  
parser = argparse.ArgumentParser(description="Concurrent program testing")
parser.add_argument('-a', choices=["lockset", "happens"], action="store", dest="algorithm")
parser.add_argument("target", nargs="+", action="store")
args = parser.parse_args()

m_env = os.environ.copy()
m_env["ALGORITHM"] = args.algorithm
m_env["LD_PRELOAD"] = "./testlib.so"
m_env["LD_LIBRARY_PATH"] = os.getcwd()
exit_status = 0



rc = subprocess.run(args.target, env=m_env)
if rc.returncode > exit_status:
  exit_status = rc.returncode

exit(exit_status)