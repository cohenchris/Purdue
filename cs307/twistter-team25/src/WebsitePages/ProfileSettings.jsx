import React from "react";
import NavigationBar from "../components/NavigationBar";
import UserAccountDetails from "../components/user-profile-lite/UserAccountDetails";
import { otherDivStyle } from "..";

export default class ProfileSettings extends React.Component {
  render() {
    return (
      <div className="ProfileSettings" style={otherDivStyle}>
        <NavigationBar />
        <br />
        <br />
        <UserAccountDetails />
      </div>
    );
  }
}
