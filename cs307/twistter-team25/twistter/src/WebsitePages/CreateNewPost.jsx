import React from "react";
import TwistterNav from "../components/TwistterNav";

export default class CreateANewPost extends React.Component {
  render() {
    return (
      <div className="CreateNewPost">
        <TwistterNav />
        <h1>Create A New Post!</h1>
        <h4>
          This page will display an editable text box and topic selection
          drop-down (with search) to post a microblog.
        </h4>
        <h4>TODO: Use hooks to make editable fields.</h4>
      </div>
    );
  }
}
