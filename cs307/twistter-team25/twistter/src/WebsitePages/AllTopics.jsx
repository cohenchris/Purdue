import React from "react";
import TwistterNav from "../components/TwistterNav";

export default class AllTopics extends React.Component {
  render() {
    return (
      <div className="AllTopics">
        <TwistterNav />
        <h1>#all</h1>
        <h4>
          This page will show all topics, regardless of what the user is
          following.
        </h4>
      </div>
    );
  }
}
