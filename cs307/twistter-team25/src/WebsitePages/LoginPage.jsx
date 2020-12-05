import React from "react";
import { Form, Button, Nav, Row } from "react-bootstrap";
import logo from "../images/Logo.png";
import { Link } from "react-router-dom";
import { otherDivStyle } from "../index.js";
import { blackLink } from "../index.js";
const axios = require("axios");

export default class LoginPage extends React.Component {
  render() {
    return (
      <div className="LoginPage" style={otherDivStyle}>
        <img src={logo} className="App-logo" alt="logo" />
        <br />
        <br />
        <br />

        <LoginBoxes />
      </div>
    );
  }
}

class LoginBoxes extends React.Component {
  constructor() {
    super();

    this.state = {
      username: "",
      password: ""
    };
    this.handleUserName = this.handleUserName.bind(this);
    this.handlePassword = this.handlePassword.bind(this);
    this.submitLoginRequest = this.submitLoginRequest.bind(this);
  }

  handleUserName(e) {
    this.setState({ username: e.target.value });
  }

  handlePassword(e) {
    this.setState({ password: e.target.value });
  }

  async submitLoginRequest() {
    let config = {
      headers: {
        "content-type": "application/json"
      }
    };
    console.log(this.state);
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/validate-login",
      "http://localhost:5000/validate-login",
      this.state,
      JSON.stringify(config)
    );
    console.log(response.data);
    if (response.data == -1) {
      window.alert("Login Failed.");
    } else {
      window.alert("Login Success!");
      window.location.href = "/home";
    }
    localStorage.setItem("ValidatedUser", response.data);
    console.log("GLOBAL = " + localStorage.getItem("ValidatedUser"));
  }

  render() {
    return (
      <Form>
        <Row>
          <Form.Group controlId="UserName">
            <Form.Label>Username</Form.Label>
            <Form.Control
              type="text"
              placeholder="Enter Username"
              onChange={this.handleUserName}
              required
            />
          </Form.Group>
        </Row>
        <Row>
          <Form.Group controlId="Password">
            <Form.Label>Password</Form.Label>
            <Form.Control
              type="password"
              placeholder="Password"
              onChange={this.handlePassword}
              required
            />
          </Form.Group>
        </Row>
        <Row>
          <Button variant="dark" onClick={this.submitLoginRequest}>
            Submit
          </Button>
        </Row>
        <Row>
          <Nav className="mr-auto">
            {/* RE-ROUTE TO CREATE NEW ACCOUNT PAGE */}
            <Nav.Link>
              <Link to="/create-account" style={blackLink}>
                New User? Click Here!
              </Link>
            </Nav.Link>
          </Nav>
        </Row>
      </Form>
    );
  }
}
