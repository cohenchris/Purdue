#!/bin/bash
directory="./Scripting"
#if statement to check if directory exists
if [ -d "${directory}" ]; then
	echo "Directory Exists"
else
	echo "Directory does not exist"
fi
