import React from "react";
import TwistterNav from "../components/TwistterNav";

export default class ProfileSettings extends React.Component {
  render() {
    return (
      <div className="ProfileSettings">
        <TwistterNav />
        <h1>Profile Settings!</h1>
        <h4>
          This page will display the profile/website settings of the current
          user.
        </h4>
        <h4>>TODO: Use Hooks to be able to change information.</h4>
      </div>
    );
  }
}
