#! /bin/bash

echo Create New User
curl -X POST \
-H "Content-Type: application/json" \
-d '{"username":"cornettn", "commonName":"Noah Cornett", "password":"1234pass", "email":"blah", "description":"This is a desc"}' \
localhost:5000/user-create-new-user
printf "\n\n"

echo Getting all posts
curl -X POST \
localhost:5000/get-all-posts
printf "\n\n"

echo Validate password
curl -X POST \
-H "Content-Type: application/json" \
-d '{"username":"cornettn", "password":"1234pass"}' \
localhost:5000/validate-login

echo Get User
curl -X POST \
-H "Content-Type: application/json" \
-d '{"userId":4}' \
localhost:5000/get-user
printf "\n\n"
