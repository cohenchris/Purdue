import React from "react";
import TwistterNav from "../components/TwistterNav";
import { Form, Button, Col } from "react-bootstrap";

export default class CreateNewAccount extends React.Component {
  render() {
    return (
      <div className="CreateNewAccount">
        <TwistterNav />
        <h1>CREATE A NEW ACCOUNT</h1>
        <NewUserForm />
      </div>
    );
  }
}

class NewUserForm extends React.Component {
  render() {
    return (
      <Form>
        <Form.Row>
          <Col>
            <Form.Group controlId="FirstName">
              <Form.Label>First Name</Form.Label>
              <Form.Control type="text" placeholder="First Name" />
            </Form.Group>
          </Col>
          <Col>
            <Form.Group controlId="LastName">
              <Form.Label>Last Name</Form.Label>
              <Form.Control type="text" placeholder="Last Name" />
            </Form.Group>
          </Col>
        </Form.Row>
        <Form.Row>
          <Col>
            <Form.Group controlId="Email">
              <Form.Label>Email</Form.Label>
              <Form.Control type="text" placeholder="example@website.com" />
            </Form.Group>
          </Col>
          <Col>
            <Form.Group controlId="Username">
              <Form.Label>Username</Form.Label>
              <Form.Control type="text" placeholder="Username" />
            </Form.Group>
          </Col>
        </Form.Row>
        <h7>Birthday</h7>
        <Form.Row>
          <Col>
            <Form.Group controlId="birthMonth">
              <Form.Control placeholder="Month" as="select">
                <option>Choose Month...</option>
                <option>January</option>
                <option>February</option>
                <option>March</option>
                <option>April</option>
                <option>May</option>
                <option>June</option>
                <option>July</option>
                <option>August</option>
                <option>September</option>
                <option>October</option>
                <option>November</option>
                <option>December</option>
              </Form.Control>
            </Form.Group>
          </Col>
          <Form.Group controlId="birthDay">
            <Form.Control placeholder="Day"></Form.Control>
          </Form.Group>
          <Col>
            <Form.Group controlId="birthYear">
              <Form.Control placeholder="Year"></Form.Control>
            </Form.Group>
          </Col>
        </Form.Row>
        <Form.Row>
          <Form.Group controlId="password">
            <Form.Label>Password</Form.Label>
            <Form.Control type="password" placeholder="Password" />
            <br></br>
            <Form.Control type="password" placeholder="Enter Password Again" />
          </Form.Group>
        </Form.Row>
        <br></br>
        <Form.Check id="agree" label={"I Agree To The Terms & Conditions"} />
        <br></br>
        <Button type="submit" variant="dark">
          Create Account
        </Button>
      </Form>
    );
  }
}
