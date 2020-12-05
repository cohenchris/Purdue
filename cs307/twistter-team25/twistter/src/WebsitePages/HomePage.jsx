import React from "react";
import TwistterNav from "../components/TwistterNav.jsx";
import "bootstrap/dist/css/bootstrap.min.css";

export default class HomePage extends React.Component {
  render() {
    return (
      <div className="HomePage">
        <TwistterNav />
        <h1>Home Page!</h1>
        <h4>All followed topics will be displayed here...</h4>
      </div>
    );
  }
}
