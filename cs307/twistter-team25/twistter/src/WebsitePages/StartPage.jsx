import React from "react";
import logo from "../Pictures/Logo.png";
import "../CSSFiles/App.css";
import Button from "react-bootstrap/Button";

/* HOME PAGE WITH LOGO AND LOGIN BUTTON */
export default class StartPage extends React.Component {
  handleClick() {
    // GO TO LOGIN PAGE!
    //ReactDOM.render(<LoginPage />, document.getElementById("root"));
  }
  render() {
    return (
      <div className="StartPage">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <p>
            <h1>Welcome to Twistter</h1>
            <h2>Go Team 25!</h2>
            <br></br>
            <Button variant="dark" size="lg" onClick={this.handleClick}>
              Click Here to Enter The Website!
            </Button>
          </p>
        </header>
      </div>
    );
  }
}
