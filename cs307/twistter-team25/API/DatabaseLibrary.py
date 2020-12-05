# -*- coding: utf-8 -*-
"""
Created on Thu Aug 29 22:45:59 2019

@author: kbuzz
"""
import pyodbc
import datetime
import json

connectionString = "Driver={ODBC Driver 17 for SQL Server};Server=tcp:twistter-dns3.eastus.cloudapp.azure.com,1401;Database=master;Uid=kbuzza;Pwd=TestTwistter1;Encrypt=no;TrustServerCertificate=no;Connection Timeout=60;"

############################ USER METHODS #####################################

# INSERTS A NEW USER INTO THE SYSTEM
def newUser(userName, password, commonName, email, description):
    cnxn = pyodbc.connect(connectionString)
    
    cursor = cnxn.cursor()
    cursor.execute("SELECT MAX(UserId) FROM UserTable")
    
    userId = cursor.fetchone()[0]
    if userId is None: 
        userId = 1
    else:
        userId = userId + 1
        
    cursor = cnxn.cursor()
    cursor.execute("INSERT INTO UserTable (UserID, UserName, Password, CommonName, Email, Description)" + 
    " VALUES (" + str(userId) + ",'" + userName + "',ENCRYPTBYPASSPHRASE('team25','" + password + "'),'" + commonName + 
    "','" + email + "','" + description + "')")
    cnxn.commit()
    
    newUserTopic(userId, "All")


#VALIDATES A USERNAME AND PASSWORD
def validateLogin(username, password):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT UserId FROM UserTable WHERE UserName='" + username + "' AND " +
    "CONVERT(varchar(50), DECRYPTBYPASSPHRASE('team25', Password))='" + password + "'")
    
    ret = cursor.fetchone()
    if ret is None:
        return -1
    else:
        return ret[0]
    
   
# UPDATES A USER PROFILE COMMON NAME
def updateCommonName(userId, newCommonName):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("UPDATE UserTable SET CommonName = '" + newCommonName +
                   "' WHERE UserId = '" + str(userId) + "'")
    cnxn.commit()
    

# UPDATES A USER PROFILE DESCRIPTION
def updateDescription(userId, newDescription):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("UPDATE UserTable SET Description = '" + newDescription +
                   "' WHERE UserId = " + str(userId))
    cnxn.commit()


# UPDATES A USER PASSWORD
def updatePassword(userId, newPassword):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("UPDATE UserTable SET Password=ENCRYPTBYPASSPHRASE('team25','" + newPassword + "') " +
    "WHERE UserId=" + str(userId))
    cnxn.commit()

    
# ADDS A TOPIC TO THE USERS PROFILE
def newUserTopic(userId, topic):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("INSERT INTO TopicTable (UserId, Topic)" +
                   " VALUES ("  + str(userId) + ",'" + topic + "')")
    cnxn.commit()
    

# SHOWS THE TIMELINE FOR A USER
def getUserTimeline(userId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT * FROM (SELECT TOP 10000 COALESCE(y.PostId, x.PostId) as PostId,COALESCE(z.UserId, x.UserId) as UserId,COALESCE(z.UserName, x.UserName) as UserName,COALESCE(z.CommonName, x.CommonName) as CommonName,x.PostTitle,x.PostText,x.Topics,x.Timestamp,x.Likes,x.Retweets," +
        "CASE WHEN x.RetweetId IS NULL THEN NULL ELSE x.CommonName END AS RetweetUserName,x.TimelineTimestamp FROM (SELECT DISTINCT TOP 10000 a.PostId,b.UserId,b.UserName,b.CommonName,a.PostTitle,a.PostText,a.Topics,a.Timestamp,COALESCE(a.RetweetTimestamp, a.Timestamp) as TimelineTimestamp,(SELECT COUNT(*) FROM LikeTable AS c WHERE c.PostId = a.PostId OR c.PostId = a.RetweetId) AS Likes,(SELECT COUNT(*) FROM PostTable AS d WHERE d.RetweetId = a.PostId OR d.RetweetId = a.RetweetId) AS Retweets,a.RetweetId, a.RetweetTimestamp FROM PostTable AS a LEFT JOIN UserTable AS b ON a.UserId = b.UserId LEFT JOIN FollowerTable AS e ON a.UserId = e.FollowingId AND (a.Topics LIKE CONCAT(e.Topic, ',%') OR  a.Topics LIKE CONCAT('%,', e.Topic) OR a.Topics LIKE CONCAT('%,', e.Topic, ',%') OR   a.Topics LIKE e.Topic) " +
        "WHERE e.UserId=" + str(userId) + " ORDER BY COALESCE(a.RetweetTimestamp, a.Timestamp) DESC ) AS x LEFT JOIN PostTable as y ON x.RetweetId = y.PostId LEFT JOIN UserTable as z ON y.UserId = z.UserId ORDER BY x.TimelineTimestamp DESC) " +
        "as n WHERE NOT (n.UserId = " + str(userId) + " AND n.RetweetUserName IS NOT NULL AND n.RetweetUserName != n.CommonName) " +
        "ORDER BY n.TimelineTimestamp DESC")
    
    test = [{"PostId": x[0], "UserId": x[1], "UserName": x[2], "CommonName": x[3], "PostTitle": x[4], "PostText": x[5], "Topics": x[6], "Timestamp": str(x[7]), "Likes": x[8], "Retweets": x[9], "RetweetUserName": x[10]} for x in cursor.fetchall()]
    return json.dumps(test)


# GETS ALL POSTS MADE BY A PARTICULAR USER
def getUserPosts(userId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT * FROM (SELECT TOP 10000 COALESCE(y.postid, x.postid) AS PostId," + 
       "COALESCE(z.userid, x.userid)         AS UserId, " +
       "COALESCE(z.username, x.username)     AS UserName, " +
       "COALESCE(z.commonname, x.commonname) AS CommonName, " +
       "x.posttitle, " +
       "x.posttext, " +
       "x.topics, " +
       "x.timestamp, " +
       "x.likes, " +
       "x.retweets, " +
       "CASE WHEN x.RetweetId IS NULL THEN NULL " +
	   "ELSE x.CommonName " +
	   "END AS RetweetUserName,  " +
       "x.timelinetimestamp  " +
       "FROM   (SELECT DISTINCT TOP 10000 a.postid, " +
       "                           b.userid, " +
       "                           b.username, " +
       "                           b.commonname, " +
       "                           a.posttitle, " +
       "                           a.posttext, " +
       "                           a.topics, " +
       "                           a.timestamp, " +
       "                           COALESCE(a.retweettimestamp, a.timestamp) AS " +
       "                                  TimelineTimestamp, " +
       "                           (SELECT Count(*) " +
       "                            FROM   liketable AS c " +
       "                            WHERE  c.postid = a.postid " +
       "                                    OR c.postid = a.retweetid)       AS " +
       "                           Likes, " +
       "                           (SELECT Count(*) " +
       "                            FROM   posttable AS d " +
       "                            WHERE  d.retweetid = a.postid " +
       "                                    OR d.retweetid = a.retweetid)    AS " +
       "                           Retweets " +
       "                                  , " +
       "                           a.retweetid, " +
       "                           a.retweettimestamp " +
       " FROM   posttable AS a " +
       "        LEFT JOIN usertable AS b " +
       "               ON a.userid = b.userid " +
       " WHERE  a.userid = " + str(userId) +
       #"         OR a.retweetid = " + str(userId) +
       " ORDER  BY COALESCE(a.retweettimestamp, a.timestamp) DESC) AS x " +
       " LEFT JOIN posttable AS y " +
       "       ON x.retweetid = y.postid " +
       " LEFT JOIN usertable AS z " +
       "       ON y.userid = z.userid " +
       " ORDER  BY x.timelinetimestamp DESC) as n " +
       " WHERE NOT (n.UserId = " + str(userId) + " AND n.RetweetUserName IS NOT NULL AND n.RetweetUserName != n.CommonName) " +
       " ORDER BY n.TimelineTimestamp DESC")
    
    test = [{"PostId": x[0], "UserId": x[1], "UserName": x[2], "CommonName": x[3], "PostTitle": x[4], "PostText": x[5], "Topics": x[6], "Timestamp": str(x[7]), "Likes": x[8], "Retweets": x[9], "RetweetUserName": x[10]} for x in cursor.fetchall()]
    return json.dumps(test)


# GETS ALL TOPICS FOR A PARTICULAR USER
def getUserTopics(userId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT Topic FROM TopicTable WHERE UserId=" + str(userId))
    
    topics = []
    for topic in cursor.fetchall():
        topics.append(topic[0])

    return topics


# REMOVES A USER AND ALL THEIR DATA FROM THE DATABASE
def deleteUser(userId):
    cnxn = pyodbc.connect(connectionString)
    
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM UserTable WHERE UserId=" + str(userId))
    
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM PostTable WHERE UserId=" + str(userId))
    
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM DMTable WHERE SenderId=" + str(userId) + " OR RecieverId=" + str(userId))
    
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM FollowerTable WHERE UserId=" + str(userId) + " OR FollowingId=" + str(userId))

    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM TopicTable WHERE UserId=" + str(userId))
    
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM LikeTable WHERE UserId=" + str(userId))    

    cnxn.commit()


# GETS USER PROFILE INFORMATION FROM THE DATABASE
def getUser(userId):
    cnxn = pyodbc.connect(connectionString)
    
    cursor = cnxn.cursor()
    cursor.execute("SELECT UserId,UserName,CommonName," +
	   "(SELECT COUNT(*) FROM FollowerTable WHERE UserId=x.UserId) as Following," +
	   "(SELECT COUNT(*) FROM FollowerTable WHERE FollowingId=x.UserId) as Followers," +
	   "(SELECT COUNT(*) FROM PostTable WHERE UserId=x.UserId) as Posts," +
	   "Description " +
       "FROM UserTable as x WHERE UserId=" + str(userId) + " FOR JSON AUTO")

    return json.dumps(eval(cursor.fetchone()[0])[0])
###############################################################################


############################ FOLLOW METHODS ###################################

# ALLOWS A USER TO FOLLOW ANOTHER USER-TOPIC COMBINATION
def newFollow(userId, followingId, topic):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("INSERT INTO FollowerTable (UserId, FollowingId, Topic)" +
                   " VALUES ("  + str(userId) + "," + str(followingId) + ",'" + topic + "')")
    cnxn.commit()
    

# ALLOWS A USER TO COMPLETELY UNFOLLOW ANOTHER USER
def unfollowUser(userId, followingId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM FollowerTable WHERE UserId=" + str(userId) +
                   " AND FollowingId=" + str(followingId))
    cnxn.commit()
    

# ALLOWS A USER TO UPDATE TOPICS THEY FOLLOW FOR A PARTICULAR USER
def updateFollow(userId, followingId, topics):
    test = ""
    for letter in topics:
        test += letter
    topics = test.split(",")
    unfollowUser(userId, followingId)
    
    for topic in topics:
        newFollow(userId, followingId, topic)

###############################################################################


############################ GENERAL METHODS ##################################

# VALIDATES NEW EMAILS FOR NEW USERS; RETURNS TRUE IF NOT IN THE DATABASE    
def validateEmail(email):
    cnxn = pyodbc.connect(connectionString)
    
    cursor = cnxn.cursor()
    cursor.execute("SELECT TOP 1 UserId FROM UserTable WHERE Email = '" + email + "'")
    
    if cursor.fetchone() is None:
        return True
    else:
        return False
    
    
def validateUsername(username):
    cnxn = pyodbc.connect(connectionString)
    
    cursor = cnxn.cursor()
    cursor.execute("SELECT TOP 1 UserId FROM UserTable WHERE UserName = '" + username + "'")
    
    if cursor.fetchone() is None:
        return True
    else:
        return False
    

# RETURNS USER ID FROM ACCOUNT EMAIL
def getUserId(email):
    cnxn = pyodbc.connect(connectionString)
    
    cursor = cnxn.cursor()
    cursor.execute("SELECT TOP 1 UserId FROM UserTable WHERE Email = '" + email + "'")
    
    ret = cursor.fetchone()
    if ret is None:
        return -1
    else:
        return ret[0]

###############################################################################


############################ POST METHODS #####################################

# CREATES A NEW POST
def newPost(userId, postTitle, postText, topics):    
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT MAX(PostId) FROM PostTable")
    
    postId = cursor.fetchone()[0]
    if postId is None:
        postId = 1
    else:
        postId = postId + 1
    
    cursor = cnxn.cursor()
    cursor.execute("INSERT INTO PostTable (PostId, UserId, PostTitle, PostText, Topics, Timestamp)" +
                   " VALUES (" + str(postId) + "," + str(userId) + ",'" + postTitle + "','" + postText + "','" +
                   topics + ",All','" + str(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')) + "')")
    cnxn.commit()


# GETS ALL POSTS ON THE PLATFORM
def getAllPosts():
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT a.PostId,a.PostTitle, a.RetweetId,b.UserId,b.UserName,b.CommonName,a.PostText,a.Topics,a.Timestamp," +
	"(SELECT COUNT(*) FROM LikeTable AS c WHERE c.PostId = a.PostId OR c.PostId = a.RetweetId) AS Likes," +
	"(SELECT COUNT(*) FROM PostTable AS d WHERE d.RetweetId = a.PostId OR d.RetweetId = a.RetweetId) AS Retweets " +
    "FROM PostTable AS a LEFT JOIN UserTable AS b ON a.UserId = b.UserId " +
    "WHERE a.RetweetId IS NULL " +
    "ORDER BY a.Timestamp DESC")

    test = [{"PostId": x[0], "PostTitle": x[1], "PostText": x[6], "Topics": x[7], "Timestamp": str(x[8]), "UserId": x[3], "UserName": x[4], "CommonName": x[5], "Likes": x[9], "Retweets": x[10]} for x in cursor.fetchall()]
    return json.dumps(test)
    


# GETS ALL POST ON THE PLATFORM WITH A SPECIFIC TOPIC
def getAllTopicPosts(topic):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT a.PostId,PostTitle,a.RetweetId,b.UserId,b.UserName,b.CommonName,a.PostText,a.Topics,a.Timestamp," +
	"(SELECT COUNT(*) FROM LikeTable AS c WHERE c.PostId = a.PostId OR c.PostId = a.RetweetId) AS Likes," +
	"(SELECT COUNT(*) FROM PostTable AS d WHERE d.RetweetId = a.PostId OR d.RetweetId = a.RetweetId) AS Retweets " +
    "FROM PostTable AS a LEFT JOIN UserTable AS b ON a.UserId = b.UserId " +
    "WHERE a.RetweetId IS NULL AND (Topics LIKE '" + topic + ",%' OR ""Topics LIKE '%," + topic + "' OR ""Topics LIKE '%," + topic + ",%' OR ""Topics LIKE '" + topic + "') " +
    "ORDER BY a.Timestamp DESC")
    
    test = [{"PostId": x[0], "PostTitle": x[1], "PostText": x[6], "Topics": x[7], "Timestamp": str(x[8]), "UserId": x[3], "UserName": x[4], "CommonName": x[5], "Likes": x[9], "Retweets": x[10]} for x in cursor.fetchall()]
    return json.dumps(test)
    


# DELETES A POST ON THE PLATFORM
def deletePost(postId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM PostTable WHERE PostId=" + str(postId) + " OR RetweetId=" + str(postId))
    cnxn.commit()


# ADDS A LIKE TO A POST
def like(userId, postId):
    cnxn = pyodbc.connect(connectionString)

    cursor = cnxn.cursor()
    cursor.execute("INSERT INTO LikeTable (UserId,PostId)" +
                    " VALUES (" + str(userId) + "," + str(postId) + ")")
    cnxn.commit()


# REMOVES A LIKE FROM A POST
def unlike(userId, postId):
    cnxn = pyodbc.connect(connectionString)

    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM LikeTable WHERE UserId=" + str(userId) +
    " AND PostId=" + str(postId))
    cnxn.commit()


# RETWEETS A POST
def retweet(userId, postId):
    cnxn = pyodbc.connect(connectionString)

    cursor = cnxn.cursor()
    cursor.execute("SELECT MAX(PostId) FROM PostTable")
    
    newPostId = cursor.fetchone()[0]
    if newPostId is None:
        newPostId = 1
    else:
        newPostId = newPostId + 1

    cursor = cnxn.cursor()
    oldPost = cursor.execute("SELECT * FROM PostTable WHERE PostId=" + str(postId)).fetchone()
    
    cursor = cnxn.cursor()
    cursor.execute("INSERT INTO PostTable (PostId,UserId,PostTitle,PostText,Topics,Timestamp,RetweetId,RetweetTimestamp) " +
    "VALUES (" + str(newPostId) + "," + str(userId) + ",'" + oldPost[2] + "','" + oldPost[3] + "','" + str(oldPost[4]) + "','" + str(oldPost[5]) + "'," + str(oldPost[0]) + ",'" + str(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')) + "')")
    cnxn.commit()


# REMOVES A RETWEET FROM A POST
def unretweet(userId, postId):
    cnxn = pyodbc.connect(connectionString)

    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM PostTable WHERE UserId=" + str(userId) + " AND RetweetId =" + str(postId))
    cnxn.commit()

###############################################################################  
    

############################## DM METHODS #####################################
  
# CREATES A NEW DM MESSAGE  
def newDM(senderId, recieverId, message):    
    cnxn = pyodbc.connect(connectionString)

    cursor = cnxn.cursor()
    cursor.execute("SELECT Private FROM UserTable WHERE UserId =" + str(recieverId))
    test = cursor.fetchone()[0]
    if not test:
        cursor = cnxn.cursor()
        cursor.execute("INSERT INTO DMTable (SenderId, RecieverId, Message, Timestamp)" +
                   " VALUES (" + str(senderId) + "," + str(recieverId) + ",'" +
                   message + "','" + str(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')) + "')")
        cnxn.commit()
    else:
        cursor = cnxn.cursor()
        cursor.execute("SELECT * FROM FollowerTable WHERE UserId=" + str(recieverId) +
                       "AND FollowingId=" + str(senderId))
        
        if cursor.fetchall() == []:
            return
        else:
            cursor = cnxn.cursor()
            cursor.execute("INSERT INTO DMTable (SenderId, RecieverId, Message, Timestamp)" +
                   " VALUES (" + str(senderId) + "," + str(recieverId) + ",'" +
                   message + "','" + str(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')) + "')")
            cnxn.commit()


# DELETES DMS FOR A PARTICULAR USER
def deleteDMs(userId, recieverId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("UPDATE DMTable SET SenderDeleted=1 WHERE " +
    "SenderId=" + str(userId) + " AND RecieverId=" + str(recieverId))

    cursor = cnxn.cursor()
    cursor.execute("UPDATE DMTable SET RecieverDeleted=1 WHERE " +
    "SenderId=" + str(recieverId) + " AND RecieverId=" + str(userId))

    cnxn.commit()

    clearDMs()


# WIPES THE DATABASE OF ANY UNUSED DMS
def clearDMs():
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("DELETE FROM DMTable WHERE SenderDeleted=1 AND RecieverDeleted=1")
    cnxn.commit()

# GETS A DM CONVERSATION BETWEEN TWO USERS
def getDMConvo(userId, recieverId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT SenderId,RecieverId,Message,TimeStamp FROM DMTable " +
    "WHERE (SenderId=" + str(userId) + " AND RecieverId=" + str(recieverId) + " AND SenderDeleted=0) OR " +
    "(SenderId=" + str(recieverId) + " AND RecieverId=" + str(userId) + " AND RecieverDeleted=0) " +
    "ORDER BY TimeStamp")

    test = [{"SenderId": x[0], "RecieverId": x[1], "Message": x[2], "TimeStamp": str(x[3])} for x in cursor.fetchall()]
    return json.dumps(test)


# GETS LIST OF DM CONVOS FOR A USER
def getDMList(userId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT q.UserName,q.CommonName,x.Message,x.TimeStamp,x.OtherUser FROM (" +
	               "SELECT SenderId,RecieverId," +
		           "CASE WHEN SenderId=" + str(userId) + " THEN RecieverId ELSE SenderId END AS OtherUser," +
		           "Message,TimeStamp," +
                   "ROW_NUMBER() OVER (PARTITION BY CASE WHEN SenderId=" + str(userId) + " THEN RecieverId ELSE SenderId END " +
				   "ORDER BY TimeStamp DESC) as part " +
                   "FROM DMTable) as x LEFT JOIN UserTable as y on x.SenderId=y.UserId " +
                   "LEFT JOIN UserTable as q on q.UserId = x.OtherUser " +
                   "WHERE x.part=1 AND (x.SenderId=" + str(userId) + " OR x.RecieverId=" + str(userId) + ") AND " +
                   "(EXISTS (SELECT 1 FROM DMTable WHERE SenderId=" + str(userId) + " AND SenderDeleted=0) OR " +
                   "EXISTS (SELECT 1 FROM DMTable WHERE RecieverId=" + str(userId) + " AND RecieverDeleted=0)) " +
                   "ORDER BY TimeStamp DESC")
    
    test = [{"UserName": x[0], "CommonName": x[1], "Message": x[2], "TimeStamp": str(x[3]), "OtherUser": x[4]} for x in cursor.fetchall()]
    return json.dumps(test)

###############################################################################

############################## Sprint 2 Stuff #####################################
# SHOWS THE TIMELINE FOR A USER
def getUserTimeline2(userId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT DISTINCT a.PostId,b.UserId,b.UserName,b.CommonName,a.PostText,a.Topics,a.Timestamp," +
          "(SELECT COUNT(*) FROM LikeTable AS c WHERE c.PostId = a.PostId OR c.PostId = a.RetweetId) AS Likes," +
          "(SELECT COUNT(*) FROM PostTable AS d WHERE d.RetweetId = a.PostId OR d.RetweetId = a.RetweetId) AS Retweets " +
          "FROM PostTable AS a " +
          "LEFT JOIN UserTable AS b ON a.UserId = b.UserId LEFT JOIN FollowerTable AS e ON a.UserId = e.FollowingId AND " +
          "(a.Topics LIKE CONCAT(e.Topic, ',%') OR a.Topics LIKE CONCAT('%,', e.Topic) OR a.Topics LIKE CONCAT('%,', e.Topic, ',%') OR a.Topics LIKE e.Topic) " +
          "WHERE e.UserId=1 AND a.RetweetId IS NULL ORDER BY a.Timestamp DESC FOR JSON AUTO")
    
    ret = cursor.fetchall()[0][0]
    return ret[1 : len(ret) - 1]


# GETS ALL POSTS MADE BY A PARTICULAR USER
def getUserPosts2(userId):
    cnxn = pyodbc.connect(connectionString)
        
    cursor = cnxn.cursor()
    cursor.execute("SELECT a.PostId,a.PostTitle, a.RetweetId,b.UserId,b.UserName,b.CommonName,a.PostText,a.Topics,a.Timestamp," +
	"(SELECT COUNT(*) FROM LikeTable AS c WHERE c.PostId = a.PostId OR c.PostId = a.RetweetId) AS Likes," +
	"(SELECT COUNT(*) FROM PostTable AS d WHERE d.RetweetId = a.PostId OR d.RetweetId = a.RetweetId) AS Retweets " +
    "FROM PostTable AS a LEFT JOIN UserTable AS b ON a.UserId = b.UserId " +
    "WHERE a.RetweetId IS NULL AND b.UserId=" + str(userId) +
    " ORDER BY a.Timestamp DESC FOR JSON AUTO")
    
    ret = cursor.fetchall()[0][0]
    return ret[1 : len(ret) - 1]
##################################################################################
