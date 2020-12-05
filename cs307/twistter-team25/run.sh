#!/bin/bash

# Traps background processes so that they terminate when the script ends
trap "exit" INT TERM ERR
trap "kill 0" EXIT

echo "Installing submodule React-ShardsDashboard..."
echo
git submodule init
git submodule update

echo "Updating npm..."
echo
npm install

echo
echo "Starting API..."
echo
cd ./API
python3 ./app.py &
cd ..

echo
echo "Starting app..."
echo
npm start &

wait
echo
echo "Success!"
echo
