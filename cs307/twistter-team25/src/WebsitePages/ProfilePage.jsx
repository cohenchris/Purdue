import React from "react";
import NavigationBar from "../components/NavigationBar";
import UserDetails from "../components/user-profile-lite/UserDetails";
import { otherDivStyle } from "..";

export default class ProfilePage extends React.Component {
  constructor(props) {
    super(props);
    let currId;
    if (this.props.location.id === undefined) {
      currId = localStorage.getItem("ValidatedUser");
      console.log("currId = " + currId);
    } else {
      currId = this.props.location.id;
    }
    this.state = { id: currId };
    console.log("PROFILE STATE ID = " + this.state.id);
  }

  render() {
    return (
      <div className="ProfilePage" style={otherDivStyle}>
        <NavigationBar />
        <UserDetails id={this.state.id} />
      </div>
    );
  }
}
