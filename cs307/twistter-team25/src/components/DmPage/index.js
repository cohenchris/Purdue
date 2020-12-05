import React from "react";
import Messenger from "../Messenger";
import NavigationBar from "../NavigationBar";
import { DMPageStyle } from "../..";

export default class DmPage extends React.Component {
  constructor(props) {
    super(props);
    let receive;
    let receiveId;
    if (
      this.props.location !== undefined &&
      this.props.location.state !== undefined &&
      this.props.location.state.receiver !== undefined &&
      this.props.location.state.receiverId !== undefined
    ) {
      receive = this.props.location.state.receiver;
      receiveId = this.props.location.state.receiverId;
    }

    this.state = {
      sender: localStorage.getItem("ValidatedUser"),
      receiver: receive,
      receiverId: receiveId
    };
  }
  render() {
    return (
      <div className="DmPage" style={DMPageStyle}>
        <NavigationBar />
        <Messenger
          sender={this.state.sender}
          receiver={this.state.receiver}
          receiverId={this.state.receiverId}
        />
      </div>
    );
  }
}
