# -*- coding: utf-8 -*-
"""
Created on Thu Aug 29 23:02:30 2019

@author: kbuzz
"""

import DatabaseLibrary as d
import time

#d.newUser("kbuzza", "password", "Kyle", "kbuzza@purdue.edu", "This is my description.")
#d.newUser("testuser1", "pass1", "User1", "test1@test.net", "desc1")
#d.newUser("testuser2", "pass2", "User2", "test2@test.net", "desc2")

#d.updateDescription(1, "new")

#print(d.validateLogin('kbuzza', 'password'))
#print(d.validateLogin('testuser1', 'pass1'))
#print(d.validateLogin('kbuzza', 'notpassword'))


#d.updatePassword(1, 'newpass')
#print(d.validateLogin('kbuzza', 'password'))
#print(d.validateLogin('kbuzza', 'newpass'))


#print(d.validateEmail("kbuzza@purdue.edu"))
#print(d.validateEmail("email-not-in-database@email.com"))


#print(d.validateUsername("kbuzza"))
#print(d.validateUsername("username-not-in-database"))


#print(d.getUserId("kbuzza@purdue.edu"))


#d.updateCommonName(1, "New Kyle")


#d.updateDescription(1, "new description")


#print(d.getUser(1))


#d.newUserTopic(1, "Sports")
#d.newUserTopic(1, "Gaming")
#d.newUserTopic(1, "News")
#d.newUserTopic(1, "Entertainment")
#d.newUserTopic(2, "Sports")
#d.newUserTopic(2, "Gaming")
#d.newUserTopic(2, "News")
#d.newUserTopic(2, "Entertainment")
#d.newUserTopic(3, "Sports")
#d.newUserTopic(3, "Gaming")
#d.newUserTopic(3, "News")
#d.newUserTopic(3, "Entertainment")


#print(d.getUserTopics(1))


#d.newPost(1, "Title 1", "Post 1", "Gaming")
#time.sleep(1.5)
#d.newPost(1, "Title 2", "Post 2", "Sports,News")
#time.sleep(1.5)
#d.newPost(1, "Title 3", "Post 3", "Gaming,Entertainment")
#time.sleep(1.5)
#d.newPost(2, "Title 4", "Post 4", "Gaming")
#time.sleep(1.5)
#d.newPost(2, "Title 5", "Post 5", "Sports,News")
#time.sleep(1.5)
#d.newPost(2, "Title 6", "Post 6", "Gaming,Entertainment")
#time.sleep(1.5)
#d.newPost(3, "Title 7", "Post 7", "Gaming")
#time.sleep(1.5)
#d.newPost(3, "Title 8", "Post 8", "Sports,News")
#time.sleep(1.5)
#d.newPost(3, "Title 9", "Post 9", "Gaming,Entertainment")
#time.sleep(1.5)
d.newPost(1, "work?", "idk", "Gaming")


#print(d.getAllPosts())


#print(d.getAllTopicPosts("Sports"))


#print(d.getUserPosts(1))


#d.newFollow(1, 2, "Gaming")
#d.newFollow(1, 3, "News")
#print(d.getUserTimeline(1))

#d.newFollow(2, 3, "All")
#print(d.getUserTimeline(2))


#d.unfollowUser(2, 3)


#d.newFollow(2, 3, "All")
#items = ["Sports","Gaming","Entertainment"]
#d.updateFollow(2, 3, items)


#d.deletePost(4)
#d.deleteUser(3)


#d.like(1,1)
#d.unlike(1,1)


#d.retweet(3,5)
#d.unretweet(1,11)
#d.retweet(2,4)


#print(d.getUserPosts(1))
#print(d.getAllPosts())
#print(d.getAllTopicPosts("News"))
#print(d.getUserTimeline(1))


#d.newDM(1,2,'message1')
#time.sleep(1.5)
#d.newDM(2,1,'message2')
#time.sleep(1.5)
#d.newDM(1,2,'message3')
#time.sleep(1.5)
#d.newDM(2,1,'message4')
#time.sleep(1.5)
#d.newDM(1,2,'message5')
#time.sleep(1.5)
#d.newDM(2,1,'message6')
#time.sleep(1.5)
#d.newDM(1,3,'message7')
#time.sleep(1.5)
#d.newDM(1,3,'message8')


#d.deleteDMs(1,2)
#d.deleteDMs(2,1)


#print(d.getDMConvo(1,2))


#print(d.getDMList(1))


#print(d.getAllPosts())

