import React, { useState, useEffect } from "react";
import { Button } from "react-bootstrap";
import { Link } from "react-router-dom";
import Toolbar from "../Toolbar";
import axios from "axios";
import "./ConversationList.css";

export default function ConversationList() {
  const [conversations, setConversations] = useState([]);

  useEffect(() => {
    getConversations();
  }, []);

  const getConversations = async () => {
    let config = {
      headers: {
        "content-type": "application/json"
      }
    };
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/get-DMList",
      "http://localhost:5000/get-DMList",
      { userId: localStorage.getItem("ValidatedUser") },
      JSON.stringify(config)
    );
    var newConversations = response.data;

    setConversations([...conversations, ...newConversations]);
  };

  return (
    <div className="conversation-list">
      <Toolbar title="MESSENGER" />
      {conversations.map(conversation => (
        <Link
          to={{
            pathname: "/dm",
            state: {
              receiver: conversation.CommonName,
              receiverId: conversation.OtherUser
            }
          }}
        >
          <Button className="unstyled-button" variant="outline-dark">
            <h1 className="conversation-title">
              {conversation.CommonName} (@{conversation.UserName})
            </h1>
            <p className="conversation-snippet">{conversation.Message}</p>
          </Button>
        </Link>
      ))}
    </div>
  );
}
