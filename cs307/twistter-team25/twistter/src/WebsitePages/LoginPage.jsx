import React from "react";
import { Form, Button, Nav } from "react-bootstrap";
import TwistterNav from "../components/TwistterNav";

export default class LoginPage extends React.Component {
  render() {
    return (
      <div className="LoginPage">
        <TwistterNav />
        <h1>LOGIN PAGE</h1>
        <LoginBoxes />
      </div>
    );
  }
}

class LoginBoxes extends React.Component {
  render() {
    return (
      <Form>
        <Form.Group controlId="formBasicEmail">
          <Form.Label>Email address</Form.Label>
          <Form.Control type="email" placeholder="Enter email" />
        </Form.Group>
        <Form.Group controlId="formBasicPassword">
          <Form.Label>Password</Form.Label>
          <Form.Control type="password" placeholder="Password" />
        </Form.Group>
        <Form.Group controlId="formBasicCheckbox"></Form.Group>
        <Button variant="dark" type="submit">
          Submit
        </Button>
        <Nav className="mr-auto">
          {/* RE-ROUTE TO CREATE NEW ACCOUNT PAGE */}
          <Nav.Link>New User? Click Here!</Nav.Link>
        </Nav>
      </Form>
    );
  }
}
