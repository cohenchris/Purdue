from flask import Flask, url_for, redirect, render_template, request
import DatabaseLibrary as db

app = Flask(__name__)


@app.route("/")
@app.route("/home")
def display_home_page():
  return render_template('sample.html', **locals())


# TODO: Un-comment all the calls to the database


@app.route("/user-create-new-user", methods=['POST'])
def create_user():
  """
  This function is used to create a new user and push it to the database. In
    order to use this function, make a POST request to function url above and
    send a json file with the appropriate information.
  :return: Display information about the newly created user.
  """

  #  Get the from the json file sent with the request
  data = request.get_json()

  username = data['username']
  commonName = data['commonName']
  email = data['email']
  description = data['description']

  #  Push the info to the database
  # db.newUser(username, commonName, email, phone, birthday, description)

  return '''
Created User!
  username: {}
  commonName: {}
  email: {}
  description: {}
'''.format(username, commonName, email, description)


@app.route("/validate-login", methods=['POST'])
def validate_login():
  """
  This function is used to validate login information from the
    database.
  :return: -1 if no user exists, if the user exists it returns the userId
  """

  # Get the info from the json file sent with POST request
  data = request.get_json()

  username = data['username']
  password = data['password']

  # return db.validateLogin(username, password)
  return "Validated"


@app.route("/user-update-common-name", methods=['POST'])
def update_common_name():
  """
  This function is used to update a user's common name in the database.
  :return: Display information about about the call.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']
  newCommonName = data['newCommonName']

  #  Push the info to the database
  # db.updateCommonName(userId, newCommonName)

  return """
Updated Users Common Name!
  userID: {}
  newCommonName: {}
""".format(userId, newCommonName)


@app.route("/user-update-description", methods=['POST'])
def update_description():
  """
  This function is called to update a user's description.
  :return: Display information about the updated information.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data["userId"]
  newDescription = ["newDescription"]

  # Send the info to the database
  # db.updateDescription(userId, newDescription)

  return """
Updated Description!
  userId: {}
  newDescription: {}
""".format(userId, newDescription)


@app.route("/update-password", methods=['POST'])
def update_password():
  """
  This function is used to update the database with new password info.
  :return: Information about the update
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']
  newPassword = data['newPassword']

  # db.updatePassword(userId, newPassword)

  return """
Updated Password!
  userId: {}
  newPassword: {}
""".format(userId, newPassword)


@app.route("/user-add-new-user-topic", methods=['POST'])
def add_user_topic():
  """
  This function is used to add a new topic to the user's profile.
  :return: Display information about the function call.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']
  newTopic = data['newTopic']

  # db.newUserTopic(userId, newTopic)

  return """
Added a new Topic!
  userId: {}
  newTopic: {}
""".format(userId, newTopic)


@app.route("/user-get-user-timeline", methods=['POST'])
def get_user_timeline():
  """
  This function is used to get the timeline of a specific user
  :return: Display information about the timeline
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']

  # return db.getUserTimeline(userId)

  return """
Retireved Timeline!
  userId: {}
""".format(userId)


@app.route("/user-get-user-posts", methods=['POST'])
def get_user_posts():
  """
  This function is used to get all of the posts of the specific user.
  :return: Display information about the posts.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']

  # return db.getUserPosts(userId)

  return """
Retrieved User Posts!
  userId: {}
""".format(userId)


@app.route("/get-user-topics", methods=['POST'])
def get_user_topics():
  """
  This function is used to retrieve the user posts from the database.
  :return: List of user topics
  """
  data = request.get_json()

  userId = data['userId']

  # return db.getUserTopics(userId)
  return "Got Posts"


@app.route("/user-delete", methods=['POST'])
def delete_user():
  """
  This function is called in order to delete a user.
  :return: Display information about deleted user
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']

  # db.deleteUser(userId)

  return """
Deleted User!
  userId: {}
""".format(userId)


@app.route("/get-user", methods=['POST'])
def get_user():
  """
  This function is used to get a user's profile information.
  :return:
  """

  data = request.get_json()
  userId = data['userId']

  # return db.getUser(userId)

  return "Got User Info"


@app.route("/follow-user", methods=['POST'])
def follow_new_user():
  """
  This function is used to follow a new user.
  :return: Display information about the update.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']
  followingId = data['followingId']

  # db.newFollow(userId, followingId)

  return """
Followed a new user!
  userId: {}
  followingId: {}
""".format(userId, followingId)


@app.route("/unfollow", methods=['POST'])
def unfollow_user():
  """
  This function is used to unfollow a user.
  :return: Display information about the action.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']
  followingId = data['followingId']

  # db.unfollowUser(userId, followingId)

  return """
Unfollowed User!
  userId: {}
  followingId: {}
""".format(userId, followingId)


@app.route("/follow-users-topics", methods=['POST'])
def follow_user_topics():
  """
  This function is used to update the database to include following a user's
    topics.
  :return: Display information about the action.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']
  followingId = data['followingId']
  topicsSelected = data['topicsSelected']

  # db.updateFollow(userId, followingId, topics=topicsSelected)

  return """
Followed a user's topics!
  userId: {}
  followingId: {}
  topicsSelected: {}
""".format(userId, followingId, topicsSelected)


@app.route("/validate-email", methods=['POST'])
def validate_email():
  """
      This function is used to validate a given user's email.
      :return: Return a boolean that determines validation
      """
  # Get the info from the json file sent with the request
  data = request.get_json()

  email = data['email']  # The user ID requesting a new follow

  # db.validateEmail(email)

  return """
  Email successfully validated!
    email: {}
  """.format(email)


@app.route("/validate-username", methods=['POST'])
def validate_username():
  """
  This function is used to validate a given user's username.
  :return: Return a boolean that determines validation
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  username = ['user']
  # db.validateUsername(username)

  return """
  Username successfully validated!
  username: {}
  """.format(username)


@app.route("/get-user-id-from-email", methods=['POST'])
def get_id_from_email():
  """
  This function is used to determine a user's Id based on
  a given email.
  :return: Return's the userId of the given email user.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  email = ['email']
  userId = db.getUserId(email)

#  return """
 # Retrieved UserId from given email!
 # userId: {}
 # """.format(userId)

  return "UserId found"


@app.route("/create-post", methods=['POST'])
def post():
  """
  This function is used to add a created post to the database.
  :return: Return a boolean that determines successful creation.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  userId = data['userId']
  postText = data['postText']
  topics = data['topics']

  # db.newPost(userId, postText, topics)

  return "Post successfully created!"


@app.route("/get-all-posts", methods=['POST'])
def get_all_posts():
  """
  This function is used to get all of the posts from all of the topics from
    the database.
  :return: Display Information about the action.
  """
  # return db.getAllPosts()
  return "Success"


@app.route("/get-topic-posts", methods=['POST'])
def get_all_topic_posts():
  """
  This function is used to retrieve all of the posts of a certain topic
    from the database.
  :return: Display information about the action.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  topic = data['topic']
  # return db.getAllTopicPosts(topic)
  return """
Retrieved posts from topic!
  topic: {}
""".format(topic)


@app.route("/delete-post", methods=['POST'])
def delete_post():
  """
  This function is used to delete a post from the database.
  :return: Display information about the action.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  postId = data['postId']
  # db.deletePost(postId)
  return """
Deleted Post!
  postId: {}
""".format(postId)


@app.route("/like-post", methods=['POST'])
def like_post():
  """
  This function takes user and post Id and adds to the like
  counter in the database
  :return: Returns that the post was successfully liked
  """
  data = request.get_json()

  userId = data['userId']
  postId = data['postId']

  db.like(userId, postId)

  return """
Liked!
  userId: {}
  postId: {}
""".format(userId, postId)


@app.route("/dm-user", methods=['POST'])
def dm_user():
  """
  This function is used to send a DM to the database
  :return: Display information message about the action.
  """
  # Get the info from the json file sent with the request
  data = request.get_json()

  senderId = data['senderId']
  receiverId = data['receiverId']
  message = data['Message']

  # db.newDM(senderId, receiverId, message)

  return """
DM Sent!
  senderId: {}
  receiverId: {}
  message: {}
""".format(senderId, receiverId, message)


@app.route("/unlike-post", methods=['POST'])
def unlike_post():
  """
  This function takes user and post Id and subtracts to the like
  counter in the database
  :return: Returns that the post was successfully unliked
  """
  data = request.get_json()

  userId = data['userId']
  postId = data['postId']

  db.unlike(userId, postId)

  return """
Unliked!
  userId: {}
  postId: {}
""".format(userId, postId)


@app.route("/retweet-post", methods=['POST'])
def retweet():
  """
  This function retweets a post and adds it to the database
  :return: Returns that the post was successfully retweeted
  """

  data = request.get_json()

  userId = data['userId']
  postId = data['postId']

  db.retweet(userId, postId)

  return """
  Retweeded!
  """


@app.route("/unretweet-post", methods=['POST'])
def unretweet():
  """
   This function unretweets a post and removes it from the database
   :return: Returns that the post was successfully unretweeted
   """

  data = request.get_json()

  userId = data['userId']
  postId = data['postId']

  db.retweet(userId, postId)

  return """
  Unretweeded!
  """


@app.route("/delete-dm", methods=['POST'])
def deleteDMs():
  """
  This function deletes the dms for a particular user
  :return: Returns that the dms were successfully deleted.
  """
  data = request.get_json()

  userId = data['userId']
  receiverId = data['receiverId']

  db.deleteDMs(userId, receiverId)

  return """
  DMs successfully deleted!
  """


@app.route("/clear-dm", methods=['POST'])
def clearDMs():
  """
  This function wipes the database of any unused dms
  :return: Returns that the dms were successfully wiped
  """

  db.clearDMs()

  return "DMs successfully cleared"


@app.route("/get-DMConvo", methods=['POST'])
def get_DMConvo():
  """
  This function gets a dm conversation between two users
  :return: Returns the conversation between the two users
  """
  data = request.get_json()

  userId = data['userId']
  receiverId = data['receiverId']

  convo = db.getDMConvo(userId, receiverId)

  return """
  convo: {}
  """.format(convo)


@app.route("/get-DMList", methods=['POST'])
def get_DMList():
  """
  This function is used to get a list of dms from the database
  :return: Returns the associated userId and a list of DMs
  """
  # Get info from json file
  data = request.get_json()

  userID = data['userId']

  dmList = db.getDMLIST(userID)

  return """
userID: {}
DMList: {}
""".format(userID, dmList)


if __name__ == '__main__':
  app.run(debug=True)
