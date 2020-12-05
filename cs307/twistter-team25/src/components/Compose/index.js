import React from "react";
import { InputGroup, FormControl, Button, Form } from "react-bootstrap";
import "./Compose.css";
const axios = require("axios");

export default class Compose extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      Message: "",
      senderId: this.props.sender,
      receiverId: this.props.receiver
    };
    this.handleMessage = this.handleMessage.bind(this);
    this.submitMessage = this.submitMessage.bind(this);
  }

  handleMessage(e) {
    this.setState({ Message: e.target.value });
  }
  async submitMessage() {
    console.log("state = " + this.state);

    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/dm-user",
      "http://localhost:5000/dm-user",
      this.state
    );
    console.log(response);
    window.location.reload();
  }

  render() {
    return (
      <InputGroup className="compose">
        <FormControl
          placeholder="Enter a message..."
          aria-label="Enter a message..."
          aria-describedby="basic-addon2"
          onChange={this.handleMessage}
        />
        <InputGroup.Append>
          {!this.props.validConvo && (
            <Button
              variant="outline-secondary"
              onClick={this.submitMessage}
              disabled
            >
              Send
            </Button>
          )}
          {this.props.validConvo && (
            <Button variant="outline-secondary" onClick={this.submitMessage}>
              Send
            </Button>
          )}
        </InputGroup.Append>
      </InputGroup>
    );
  }
}
