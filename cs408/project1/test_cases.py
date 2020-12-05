import os 
import subprocess

import global_vars

# TODO
# STOR

def test_compare(command, actual, expected):
    try:
        cmd = command.strip().upper()
    except:
        cmd = "PLACEHOLDER"

    if cmd.split()[0] in global_vars.PASV_COMMANDS and cmd not in global_vars.DISABLED and global_vars.LOGGED_IN:
        actual = [a.strip().replace("\r\n", "\n") for a in actual]
        expected = [e.strip().replace("\r\n", "\n") for e in expected]
    else:
        actual = actual.strip()
        expected = expected.strip()

    if cmd.split()[0] in global_vars.DISABLED or not global_vars.LOGGED_IN and cmd.split()[0] not in ["USER", "PASS"]:
        return actual == expected
    elif cmd.split()[0] in global_vars.PASV_COMMANDS and not global_vars.PASV_ACTIVE:
        # Command requires PASV, but PASV is not active. Should return error
        return actual == expected
    elif "USER" in cmd:
        return actual == expected
    elif "PASS" in cmd:
        return actual == expected
    elif "PWD" in cmd:
        expected = expected.replace("/<<cwd>>", global_vars.CWD)
        return actual == expected
    elif "CWD" in cmd:
        if "250" in actual:
            global_vars.CWD = global_vars.CWD + "/" + cmd.split()[1].lower()
        return actual == expected
    elif "CDUP" in cmd:
        if "250" in actual:
            global_vars.CWD = "/".join(global_vars.CWD.split("/")[:-1])
        return actual == expected
    elif "MKD" in cmd:
        return actual == expected
    elif "RMD" in cmd:
        return actual == expected
    elif "QUIT" in cmd:
        return actual == expected
    elif "PASV" in cmd:
        # Compare based on everything execpt for port numbers (last 2 values in parentheses)
        a = ",".join(actual.split(",")[:-2]) + ")"
        e = ",".join(expected.split(",")[:-2]) + ")"
        if a == e:
            global_vars.PASV_ACTIVE = True
        return a == e
    elif "NLST" in cmd:
        expected_list = expected[-1].split("\n")
        expected_list.sort()
        actual_list = actual[-1].split("\n")
        actual_list.sort()
        global_vars.PASV_ACTIVE = False
        return actual[:-1] == expected[:-1] and actual_list == expected_list
    elif "LIST" in cmd:
        actual_listing = actual[-1].split("\n")
        new_listing = []
        for entry in actual_listing:
            entry = entry.split()
            new_entry = entry[:2]
            new_entry.append(entry[4])
            new_entry.append(entry[-1])
            new_listing.append(" ".join(new_entry))
        actual_listing = new_listing
        expected_listing = expected[-1].split("\n")
        new_listing = []
        for entry in expected_listing:
            entry = entry.split()
            new_entry = entry[:2]
            new_entry.append(entry[4])
            new_entry.append(entry[-1])
            new_listing.append(" ".join(new_entry))
        expected_listing = new_listing
        expected_listing.sort()
        actual_listing.sort()
        global_vars.PASV_ACTIVE = False
        return actual[:-1] == expected[:-1] and actual_listing == expected_listing
    elif "STAT" in cmd:
        f = cmd.split()[1].lower()
        expected = expected.replace("<<file>>", global_vars.CWD + "/" + f)
        expected = expected.replace("\r\n", "\n")
        actual = actual.replace("\r\n", "\n")
        return actual == expected
    elif "SIZE" in cmd:
        return actual == expected
    elif "STOR" in cmd:
        global_vars.PASV_ACTIVE = False
        return actual == expected
    elif "RETR" in cmd:
        global_vars.PASV_ACTIVE = False
        return actual == expected
    elif "REST" in cmd:
        return actual == expected
    else:
        # Command not recognized
        pass