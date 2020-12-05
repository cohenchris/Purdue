import React from "react";
import logo from "../images/Logo.png";

export default class DownForMaintenance extends React.Component {
  render() {
    return (
      <div className="DownForMaintenance">
        <img src={logo} className="App-logo" alt="logo" />
        <br></br>
        <br></br>
        <br></br>
        <h2>We're Sorry, Twistter is Currently Down For Maintenance.</h2>
        <br></br>
        <h5>Please Try Again Later.</h5>
      </div>
    );
  }
}
