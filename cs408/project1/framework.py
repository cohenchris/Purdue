import argparse
import subprocess
import os


# These are some imports that I found useful when implementing this project, but you can use
# other options if you prefer
import telnetlib
import time
import sys
import re


from test_cases import test_compare
import global_vars


class bcolors:
    """
    Used for colored text
    Pulled from https://stackoverflow.com/questions/287871/how-to-print-colored-text-in-python
    """
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def log(statement):
    if VERBOSE:
        print(statement)

def get_configuration(input_file):
    """
    Parses the input file for the configuration line.
    Returns the path of the configuration file.
    """
    configuration_file = "./testing_environment/configurations/"
    try:
        with open(input_file, "r") as input:
            configuration_file += input.readline().strip()
    except:
        # Use default config file
        configuration_file += "bftpd.conf"

    # Double check that config file existss
    if not os.path.isfile(configuration_file):
        print("Configuration file is invalid or missing!")
        sys.exit(2) # Exit code 2, can't run the program with the given arguments
    return configuration_file


def start_server(execute_line):
    """
    Starts BFTPD server with the required arguments
    """
    # The tweaks I made here to the original code suppress the 'Killed' message.
    # Got it from here: https://stackoverflow.com/questions/11269575/how-to-hide-output-of-subprocess-in-python-2-7
    FNULL = open(os.devnull, 'w')
    proc = subprocess.Popen(execute_line, shell=True, stdout=FNULL, stderr=subprocess.STDOUT) #Runs the BFTPD server as a subprocess. Your program can now connect to it. The python telnetlib framework would be useful for this. 
    time.sleep(2)
    if proc.poll() != None:
        print("BFTPD failed to start, try changing your port")
        exit(2)


def read_input(input_file):
    """
    This function will parse the input file and return an array of commands in the following format:
    [
        [command_1, client_num],
        [command_2, client_num],
        ...,
        [command_n, client_num],
    ]
    """
    commands = []
    try:
        with open(input_file, "r") as input:
            # Get all lines (excluding first line, which is config file)
            lines = [line.strip() for line in input.readlines()][1:]
            for line in lines:
                commands.append(line.split(":"))
    except IOError:
        print("Input file " + input_file + " does not exist.")
        sys.exit(2) # Exit code 2, can't run the program with the given arguments
    return commands


def execute_command(command):
    """
    This function executes the given command for the given client

    It will receive a command in the format:
    [command, client_num]

    It will return the server's response code
    """

    client = int(command[0])
    cmd = command[1]
    output = []


    if client is 1: # Referring to control connection client
        if global_vars.CTRL is None: # Add control connection client first
            global_vars.CTRL = telnetlib.Telnet("localhost", 12000)
            output.append(global_vars.CTRL.read_until("\r\n"))

        # Then, execute normal command w/ control connection client
        global_vars.CTRL.write(cmd + "\r\n")
        if cmd.upper().split()[0] in global_vars.DISABLED:
            # Command is disabled, should only return a single line
            output.append(global_vars.CTRL.read_until("\r\n"))
            return output
        elif "PASS" in cmd.upper():
            output.append(global_vars.CTRL.read_until("\r\n"))
            if "230" in output[-1]:
                # Successfully logged in
                global_vars.LOGGED_IN = True
        elif not global_vars.LOGGED_IN:
            output.append(global_vars.CTRL.read_until("\r\n"))
            return output
        elif cmd.upper().split()[0] in global_vars.PASV_COMMANDS and global_vars.PASV_ACTIVE:
            # If needed, read in multiple lines here
            output.append(global_vars.CTRL.read_until("\r\n"))
            if cmd.upper().split()[0] != "STOR":
                output.append(global_vars.CTRL.read_until("\r\n"))
        elif cmd.upper().split()[0] in global_vars.PASV_COMMANDS and not global_vars.PASV_ACTIVE:
            # Should return 2 lines
            output.append(global_vars.CTRL.read_until("\r\n"))
            output.append(global_vars.CTRL.read_until("\r\n"))
            return output
        elif "STAT" in cmd.upper():
            output.append(global_vars.CTRL.read_until("\r\n"))
            output.append(global_vars.CTRL.read_until("\r\n"))
            output.append(global_vars.CTRL.read_until("\r\n"))
        elif "REST" in cmd.upper():
            output.append(global_vars.CTRL.read_until("\r\n"))
            global_vars.REST_VALUE = int(cmd.upper().split()[1])
        else:
            output.append(global_vars.CTRL.read_until("\r\n"))

        if cmd.upper() == "PASV":   # This command is used to open up a port for the DATA connection. Parse the port.
            if "227" in output[-1]:
                # Successfully set to passive mode
                global_vars.PASV_ACTIVE = True
            nums = output[-1].split("(")[-1].split(")")[0].split(",")
            DATA_PORT = int(nums[-2]) * 256 + int(nums[-1])
            # Connect second client to prepare to receive data
            if global_vars.DATA is None:
                global_vars.DATA = telnetlib.Telnet("localhost", DATA_PORT)
        if cmd.upper().split()[0] in global_vars.PASV_COMMANDS:
            # Receive data and quit
            if "STOR" in cmd.upper():
                # Reset the REST value (according to handout), close DATA client, read in last response line
                global_vars.DATA.close()
                output.append(global_vars.CTRL.read_until("\r\n"))
                global_vars.REST_VALUE = 0
                global_vars.DATA = None
                global_vars.PASV_ACTIVE = False
                global_vars.DATA_PORT = -1
                return output
            received = global_vars.DATA.read_very_eager()
            output.append(received)
            if "RETR" in cmd.upper():
                global_vars.REST_VALUE = 0

            global_vars.PASV_ACTIVE = False
            global_vars.DATA = None
            global_vars.DATA_PORT = -1
    return output


def check_test(commands, actual, expected_file):
    """
    Compares the actual and expected output of a test case.
    'commands' is an array of the commands given to the server
    'actual' is an array of the received responses from the server
    'expected_file' is a file containing expected output
    """

    global_vars.PASV_ACTIVE = False
    passed = True
    expected = []
    with open(expected_file, "r") as e:
        expected = e.readlines()

    commands.insert(0, ["0","PLACEHOLDER"]) # Placeholder for the server connection. This is to keep the commands, expected, and actual arrays synced

    e = 0   # expected counter
    a = 0   # actual counter
    c = 0   # commands counter

    while e < len(expected) and a < len(actual) and c < len(commands):
        if commands[c][1].upper() == "PASV":
            global_vars.PASV_ACTIVE = True

        if commands[c][1].upper() == "PWD":
            expected[e] = expected[e].replace("/<<cwd>>", global_vars.CWD)

        if commands[c][1].upper().split()[0] in ["LIST", "NLST", "RETR"] and commands[c][1].upper().split()[0] not in global_vars.DISABLED and global_vars.LOGGED_IN and global_vars.PASV_ACTIVE:
            list_expected = [expected[e], expected[e + 1]]
            list_actual = [actual[a], actual[a + 1], actual[a + 2]]
            if commands[c][1].upper().split()[0] == "LIST":
                task = subprocess.Popen("ls -l " + global_vars.CWD, shell=True, stdout=subprocess.PIPE)
                contents = task.stdout.read().split("\n")[1:]
                contents = [f for f in contents if "forbidden" not in f]
                contents = "\n".join(contents)
                list_expected.append(contents)
            elif commands[c][1].upper().split()[0] == "NLST":
                contents = "\n".join([f for f in os.listdir(global_vars.CWD) if os.access(global_vars.CWD + "/" + f, os.R_OK)])
                list_expected.append(contents)
            elif commands[c][1].upper().split()[0] == "RETR":
                offset = 0
                if "REST" in commands[c - 1][1].upper():
                    offset = int(commands[c - 1][1].split()[-1])
                with open(global_vars.CWD + "/" + commands[c][1].split()[1], "r") as f:
                    lines = "".join(f.readlines())
                    contents = lines[offset:]

                list_expected.append(contents)

            log(bcolors.OKBLUE + "Client " + commands[c][0] + " enters:\t\t" + commands[c][1] + bcolors.ENDC)
            if test_compare(commands[c][1], list_actual, list_expected): # Test case passed
                log(bcolors.OKGREEN + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "Client 2 should have received:\n" + actual[a + 2].strip() + bcolors.ENDC + "\n")

                log(bcolors.OKGREEN + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "Client 2 actually received:\n" + actual[a + 2].strip() + bcolors.ENDC + "\n")
            else: # Test case failed
                passed = False
                print(bcolors.FAIL + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "Client 2 should have received:\n" + actual[a + 2].strip() + bcolors.ENDC + "\n")

                print(bcolors.FAIL + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "Client 2 actually received:\n" + actual[a + 2].strip() + bcolors.ENDC + "\n")
                return passed
            if "RETR" in commands[c][1].upper():
                e = e + 2 # Expected contains the extra response and the expected output, skip over 2 extra
            else:
                e = e + 1 # LIST/NLST don't have the prediction in 'expected' so only skip 1 extra
            a = a + 2 # Actual contains the extra response and the output, skip over 2 extra
            global_vars.PASV_ACTIVE = False
        elif commands[c][1].upper().split()[0] in global_vars.PASV_COMMANDS and commands[c][1].upper() not in global_vars.DISABLED and global_vars.LOGGED_IN and not global_vars.PASV_ACTIVE:
            list_actual = [actual[a], actual[a + 1]]
            list_expected = [expected[e], expected[e + 1]]
            log(bcolors.OKBLUE + "Client " + commands[c][0] + " enters:\t\t" + commands[c][1] + bcolors.ENDC)
            if test_compare(commands[c][1], list_actual, list_expected): # Test case passed
                log(bcolors.OKGREEN + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)

                log(bcolors.OKGREEN + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC + "\n")
            else: # Test case failed
                passed = False
                print(bcolors.FAIL + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)

                print(bcolors.FAIL + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC + "\n")
                return passed
            e = e + 1 # Expected contains 2 lines total, skip over to keep aligned
            a = a + 1 # Actual contains 2 lines total, skip over to keep aligned
        elif "STOR" in commands[c][1].upper() and "STOR" not in global_vars.DISABLED and global_vars.LOGGED_IN and global_vars.PASV_ACTIVE:
            list_actual = [actual[a], actual[a + 1]]
            list_expected = [expected[e], expected[e + 1]]
            log(bcolors.OKBLUE + "Client " + commands[c][0] + " enters:\t\t" + commands[c][1] + bcolors.ENDC)
            if test_compare(commands[c][1], list_actual, list_expected): # Test case passed
                log(bcolors.OKGREEN + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)

                log(bcolors.OKGREEN + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC + "\n")
            else: # Test case failed
                passed = False
                print(bcolors.FAIL + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)

                print(bcolors.FAIL + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC + "\n")
                return passed
            e = e + 1 # Expected contains 2 lines total, skip over to keep aligned
            a = a + 1 # Actual contains 2 lines total, skip over to keep aligned
        # Special case STAT
        elif "STAT" in commands[c][1].upper() and commands[c][1].split()[0].upper() not in global_vars.DISABLED and global_vars.LOGGED_IN:
            expected_path = " ".join(expected[e + 1].split()[:2]) + " " + expected[e + 1].split()[-1] + "\n"
            list_expected = expected[e] + expected_path + expected[e + 2]
            actual_path = " ".join(actual[a + 1].split()[:2]) + " " + actual[a + 1].split()[-1] + "\n"
            list_actual = actual[a] + actual_path + actual[a + 2]

            log(bcolors.OKBLUE + "Client " + commands[c][0] + " enters:\t\t" + commands[c][1] + bcolors.ENDC)
            if test_compare(commands[c][1], list_actual, list_expected): # Test case passed
                log(bcolors.OKGREEN + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + expected[e + 2].strip() + bcolors.ENDC)

                log(bcolors.OKGREEN + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC)
                log(bcolors.OKGREEN + "\t\t\t\t" + expected[a + 2].strip() + bcolors.ENDC + "\n")

            else: # Test case failed
                passed = False
                print(bcolors.FAIL + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + expected[e + 1].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + expected[e + 2].strip() + bcolors.ENDC)

                print(bcolors.FAIL + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + actual[a + 1].strip() + bcolors.ENDC)
                print(bcolors.FAIL + "\t\t\t\t" + expected[a + 2].strip() + bcolors.ENDC + "\n")
                return passed

            e = e + 2 # Expected uses 3 lines total, skip over to keep alignment
            a = a + 2 # Actual uses 3 lines total, skip over to keep alignment
        # General testing
        elif test_compare(commands[c][1], actual[a], expected[e]) == False: # Test case has failed
            passed = False
            if c == 0:
                print(bcolors.FAIL + "BFTPD Server started." + bcolors.ENDC)
            else:
                print(bcolors.OKBLUE + "Client " + commands[c][0] + " enters:\t\t" + commands[c][1] + bcolors.ENDC)
            print(bcolors.FAIL + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
            print(bcolors.FAIL + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC + "\n")
            return passed
        else: # Test case has passed
            if c == 0:
                log(bcolors.OKGREEN + "BFTPD Server started." + bcolors.ENDC)
            else:
                log(bcolors.OKBLUE + "Client " + commands[c][0] + " enters:\t\t" + commands[c][1] + bcolors.ENDC)
            log(bcolors.OKGREEN + "Expected server response:\t" + expected[e].strip() + bcolors.ENDC)
            log(bcolors.OKGREEN + "Actual server response:\t\t" + actual[a].strip() + bcolors.ENDC + "\n")
        e = e + 1
        a = a + 1
        c = c + 1
    return passed

#################################################
#                    MAIN                       #
#################################################
"""
Parses the command line arguments, and starts the BFTPD server based off of it.
Executes the requested number of test cases, and compares the expected output to the actual output.
If the verbose flag is specified, detailed logs will show, along with a message saying whether or not the test case failed. 
"""

# Initialize global variables file
global_vars.init()

##### PARSE ARGUMENTS #####
parser = argparse.ArgumentParser(description="test cases for bftpd server")
parser.add_argument('-p', action="store", dest="binary")
parser.add_argument('-v', action="store_true", dest="verbose")
parser.add_argument('-f', action="store", dest="file")

args = parser.parse_args()
 
if args.binary == None:
    print("Usage: python framework.py -p <binary> [-v] [-f testfcase]")
    sys.exit(2) # Exit code 2, can't run the program with the given arguments

test_case = args.file
VERBOSE = args.verbose

##### GET LIST OF ALL TEST CASES TO RUN #####
files = []
if test_case is None: # Get a list of all files in test case directory
    all_files = os.listdir("./testing_environment/testcases")
    for f in all_files:
        if "input" in f:
            i = "./testing_environment/testcases/" + f
            o = "./testing_environment/testcases/" + f.replace("input", "output")
            files.append([i, o])
else: # Only execute the one test case
    files.append([test_case,test_case.replace("input", "output")])

files.sort()

##### RUNS ALL TEST CASES #####
case = 1
for input_file, output_file in files:
    print("="*50)
    print("\t\tTEST CASE " + str(case))
    print("="*50)

    configuration_file = get_configuration(input_file)

    # Start Server
    start_server('./' +  args.binary + ' -c ' +  configuration_file + " -D")

    # Read file and parse commands
    commands = read_input(input_file)

    # Find all banned commands (if any)
    with open(configuration_file, "r") as c:
        for line in c:
            if re.match('^ALLOWCOMMAND_[A-Z]*=\"no\"$', line.strip()):
                cmd = line.strip()[13:-5]
                global_vars.DISABLED.append(cmd)

    # Find current working directory
    original_cwd = os.getcwd() + "/testing_environment"
    global_vars.CWD = original_cwd
    response = []
    for command in commands:
        r = execute_command(command)
        [response.append(cmd) for cmd in r]

    # Kill BFTPD Server
    os.system("ps -u $USER | grep bftpd | grep -v grep | awk '{print $1}' | xargs -r kill -9") # This kills the bftpd process, and all subprocesses that it created. You must run this before you start another server on the same port

    # Check if test case passed
    if check_test(commands, response, output_file):
        print(bcolors.OKGREEN + "TEST CASE " + str(case) + " PASSED" + bcolors.ENDC)
    else:
        print(bcolors.FAIL + "TEST CASE " + str(case) + " FAILED" + bcolors.ENDC)
        global_vars.PASSED = False
    print("\n")

    # Reset global variables
    global_vars.CTRL = None
    global_vars.DATA = None
    global_vars.DATA_PORT = -1
    global_vars.LOGGED_IN = False
    global_vars.PASV_ACTIVE = False
    global_vars.REST_VALUE = 0
    global_vars.CWD = original_cwd
    case = case + 1
    global_vars.DISABLED = []

    # remove any leftover files
    os.remove(os.path.join(original_cwd, "test")
    os.remove(os.path.join(original_cwd, "testd")
    os.remove(os.path.join(original_cwd, "test2d")


sys.exit(0) if global_vars.PASSED else sys.exit(1)