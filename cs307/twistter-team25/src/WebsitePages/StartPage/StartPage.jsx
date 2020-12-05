import React from "react";
import logo from "../../images/Logo.png";
import "./StartPage.css";
import Button from "react-bootstrap/Button";
import { Link } from "react-router-dom";

/* HOME PAGE WITH LOGO AND LOGIN BUTTON */
export default class StartPage extends React.Component {
  handleClick() {
    // GO TO LOGIN PAGE!
    //ReactDOM.render(<LoginPage />, document.getElementById("root"));
  }
  render() {
    return (
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          <h1>Welcome to Twistter</h1>
          <h2>Go Team 25!</h2>
          <br></br>
          <Link to="/login">
            <Button variant="dark" size="lg" onClick={this.handleClick}>
              Click Here to Enter The Website!
            </Button>
          </Link>
        </p>
      </header>
    );
  }
}
