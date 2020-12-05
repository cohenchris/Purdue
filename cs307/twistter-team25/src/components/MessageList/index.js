import React, { useEffect, useState } from "react";
import Compose from "../Compose";
import Toolbar from "../Toolbar";
import Message from "../Message";
import moment from "moment";
import "./MessageList.css";

const axios = require("axios");

const MY_USER_ID = localStorage.getItem("ValidatedUser");
let convoTitle = "Conversation with ...";
let SenderId = -1;
let ReceiverId = -1;

export default function MessageList(props) {
  let { sender, receiver, receiverId } = props;

  const [messages, setMessages] = useState([]);
  useEffect(() => {
    if (
      receiver !== -1 &&
      sender !== -1 &&
      receiver !== undefined &&
      sender !== undefined
    ) {
      convoTitle = "Conversation with " + receiver;
      getMessages();
    }
  }, []);

  const getMessages = async () => {
    var tempMessages = [];
    let config = {
      headers: {
        "content-type": "application/json"
      }
    };
    let send = {
      userId: parseInt(localStorage.getItem("ValidatedUser")),
      receiverId: receiverId
    };
    console.log(send);
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/get-DMConvo",
      "http://localhost:5000/get-DMConvo",
      send,
      JSON.stringify(config)
    );
    console.log(response.data);
    tempMessages = response.data;

    setMessages([...messages, ...tempMessages]);
  };

  const renderMessages = () => {
    let i = 0;
    let messageCount = messages.length;
    let tempMessages = [];

    while (i < messageCount) {
      console.log(messages[i]);
      ReceiverId = messages[i].ReceiverId;
      let previous = messages[i - 1];
      let current = messages[i];
      let next = messages[i + 1];
      let isMine = current.SenderId == MY_USER_ID;
      let currentMoment = moment(current.TimeStamp);
      let prevBySameAuthor = false;
      let nextBySameAuthor = false;
      let startsSequence = true;
      let endsSequence = true;
      let showTimestamp = true;

      if (previous) {
        let previousMoment = moment(previous.TimeStamp);
        let previousDuration = moment.duration(
          currentMoment.diff(previousMoment)
        );
        prevBySameAuthor = previous.SenderId === current.SenderId;

        if (prevBySameAuthor && previousDuration.as("hours") < 1) {
          startsSequence = false;
        }

        if (previousDuration.as("hours") < 1) {
          showTimestamp = false;
        }
      }

      if (next) {
        let nextMoment = moment(next.TimeStamp);
        let nextDuration = moment.duration(nextMoment.diff(currentMoment));
        nextBySameAuthor = next.SenderId === current.SenderId;

        if (nextBySameAuthor && nextDuration.as("hours") < 1) {
          endsSequence = false;
        }
      }

      tempMessages.push(
        <Message
          key={i}
          isMine={isMine}
          startsSequence={startsSequence}
          endsSequence={endsSequence}
          showTimestamp={showTimestamp}
          data={current}
        />
      );

      // Proceed to the next message.
      i += 1;
    }

    return tempMessages;
  };

  return (
    <div className="message-list">
      <Toolbar title={convoTitle} />

      <div className="message-list-container">{renderMessages()}</div>
      <Compose
        sender={localStorage.getItem("ValidatedUser")}
        receiver={receiverId}
        validConvo={receiverId !== -1}
      />
    </div>
  );
}
