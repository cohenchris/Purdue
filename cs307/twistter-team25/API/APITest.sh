#! /bin/bash

# This is a script that contains curl calls to test the Web Api
echo Create New User
curl -X POST \
-H "Content-Type: application/json" \
-d '{"username":"cornettn", "password":"test","commonName":"Noah Cornett", "email":"blah", "description":"This is a desc"}' \
localhost:5000/user-create-new-user
printf "\n\n"

curl -X POST \
-H "Content-Type: application/json" \
-d '{"username":"cornettn", "password":"1234pass"}' \
localhost:5000/validate-login

echo Update Common Name
curl -X POST \
-H "Content-Type: application/json" \
-d '{"userId":3, "newCommonName":"New Name"}' \
localhost:5000/user-update-common-name
printf "\n\n"

#echo Update Description
#curl -X POST \
#-H "Content-Type: application/json" \
#-d '{"userId":3, "newDescription":"New Desc"}' \
#localhost:5000/user-update-description
#printf "\n\n"

echo Update Password
curl -X POST \
-H "Content-Type: application/json" \
-d '{"userId":3, "newPassword":"NewPass"}' \
localhost:5000/update-password
printf "\n\n"

echo Get User
curl -X POST \
-H "Content-Type: application/json" \
-d '{"userId":1}' \
localhost:5000/get-user
printf "\n\n"

echo Validate User
curl -X Post \
-H "Content-Type: application/json" \
-d '{"username":"kbuzza", "password":"password"}' \
localhost:5000/validate-login
printf "\n\n"

echo Add User Topic
curl -X Post \
-H "Content-Type: application/json" \
-d '{"userId":1, "newTopic":"Computers"}' \
localhost:5000/user-add-new-user-topic
printf "\n\n"

echo Delete User
curl -X Post \
-H "Content-Type: application/json" \
-d '{"userId":4}' \
localhost:5000/user-delete
printf "\n\n"

echo Follow User Topics
curl -X Post \
-H "Content-Type: application/json" \
-d '{"userId":1, "followingId":2, "topicsSelected":"New,Fishing"}' \
localhost:5000/follow-users-topics
printf "\n\n"

echo Unfollow User
curl -X Post \
-H "Content-Type: application/json" \
-d '{"userId":1, "followingId":2}' \
localhost:5000/unfollow
printf "\n\n"

echo DM User
curl -X Post \
-H "Content-Type: application/json" \
-d '{"senderId":1, "receiverId":2, "Message":"new DM"}' \
localhost:5000/dm-user
printf "\n\n"

echo Delete DM
curl -X Post \
-H "Content-Type: application/json" \
-d '{"senderId":2, "receiverId":1}' \
localhost:5000/delete-dm
printf "\n\n"

echo Get DM Convo
curl -X Post \
-H "Content-Type: application/json" \
-d '{"userId":1, "receiverId":2}' \
localhost:5000/get-DMConvo
printf "\n\n"

echo Get DM List
curl -X Post \
-H "Content-Type: application/json" \
-d '{"userId":1}' \
localhost:5000/get-DMList
printf "\n\n"

echo Invalid Json Test
curl -X POST \
-H "Content-Type: application/json" \
-d '{"invalid":3, "invalid":"New Name"}' \
localhost:5000/user-update-common-name
printf "\n\n"
