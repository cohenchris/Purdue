def init():
    global CTRL
    global DATA
    global DATA_PORT
    global VERBOSE
    global PASSED
    global DISABLED
    global CWD
    global LOGGED_IN
    global PASV_ACTIVE
    global REST_VALUE
    global PASV_COMMANDS

    ###### GLOBAL VARIABLES #####
    CTRL = None                 # Telnet Control Connection
    DATA = None                 # Telnet Data Connection
    DATA_PORT = -1              # Data Port for Data Client
    VERBOSE = True              # Determines whether or not to print information
    PASSED = True               # Global variable to denote whether or not the test suite passes
    DISABLED = []               # Keep track of disabled commands
    CWD = ""                    # Keep track of current working directory
    LOGGED_IN = False           # Keep track of whether or not the 1st control connection is logged in
    PASV_ACTIVE = False         # Keep track of whether or not the passive connection is readily accepting connections
    REST_VALUE = 0              # Keep track of last entered REST value
    PASV_COMMANDS = ["LIST", "NLST", "RETR", "STOR"]
    #############################