import React from "react";
import TwistterNav from "../components/TwistterNav";

export default class ProfilePage extends React.Component {
  render() {
    return (
      <div className="ProfilePage">
        <TwistterNav />
        <h1>Profile Page!</h1>
        <h2>
          This page will display the profile information of the current user.
        </h2>
      </div>
    );
  }
}
