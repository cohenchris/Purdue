import React from "react";
import logo from "../images/Logo.png";
import { Form, Button, Col } from "react-bootstrap";
import { Link } from "react-router-dom";
import { FormTextarea } from "shards-react";
import { otherDivStyle } from "..";
const axios = require("axios");

export default class CreateNewAccount extends React.Component {
  render() {
    return (
      <div className="CreateNewAccount" style={otherDivStyle}>
        <img src={logo} className="App-logo" alt="logo" />
        <br />
        <br />
        <br />

        <h1>CREATE A NEW ACCOUNT</h1>
        <NewUserForm />
      </div>
    );
  }
}

class NewUserForm extends React.Component {
  constructor() {
    super();
    this.state = {
      fields: {},
      errors: {}
    };
    this.handleChange = this.handleChange.bind(this);
    this.submitUserRegistrationForm = this.submitUserRegistrationForm.bind(
      this
    );
    this.validate_form = this.validate_form.bind(this);
  }

  validate_form() {
    let fields = this.state.fields;
    let errors = {};
    let isValid = true;
    if (
      !fields["Password"].match(
        /^(?=.*[a-z])+(?=.*[A-Z])+(?=.*\d)+(?=.*[~`!@#$%^&*()_\-+=:?])+[A-Za-z\d~`!@#$%^&*()_\-+=:?]{8,20}$/
      )
    ) {
      errors["Password"] =
        "Password must be between 8-20 characters and contain at least one uppercase letter, lowercase letter, number, and special character.";
      isValid = false;
    } else if (fields["Password"].localeCompare(fields["Password_Duplicate"])) {
      errors["Password_Duplicate"] = "Passwords must match";
      isValid = false;
    }
    this.setState({ errors: errors });
    return isValid;
  }

  handleChange(evt) {
    let fields = this.state.fields;
    fields[evt.target.name] = evt.target.value;
    this.setState({ fields });
  }

  async submitUserRegistrationForm(e) {
    e.preventDefault();
    if (this.validate_form()) {
      let fields = {};
      let errors = {};
      fields["CommonName"] = "";
      errors["CommonName"] = "";
      fields["UserName"] = "";
      errors["UserName"] = "";
      fields["Email"] = "";
      errors["Email"] = "";
      fields["Description"] = "";
      errors["Description"] = "";
      fields["Password"] = "";
      errors["Password"] = "";
      fields["Password_Duplicate"] = "";
      errors["Password_Duplicate"] = "";

      /* Random lowercase fields are to communicate with the api better */
      let submitData = {
        username: this.state.fields.UserName,
        password: this.state.fields.Password,
        commonName: this.state.fields.CommonName,
        email: this.state.fields.Email,
        description: this.state.fields.Description
      };

      this.setState({ fields: fields, errors: errors });

      let config = {
        headers: {
          "content-type": "application/json"
        }
      };

      console.log(submitData);
      // user-create-new-user
      const createUserResponse = await axios.post(
        //"http://twistter-API.azurewebsites.net/user-create-new-user",
        "http://localhost:5000/user-create-new-user",
        submitData,
        JSON.stringify(config)
      );
      console.log(createUserResponse);
      window.alert("User Created!");
      window.location.href = "/login";
    }
  }

  render() {
    return (
      <Form>
        <Form.Row>
          {/* CommonName */}
          <Col>
            <Form.Group controlId="CommonName" required>
              <Form.Label>
                <strong>Display Name</strong>
              </Form.Label>
              <Form.Control
                type="text"
                name="CommonName"
                onChange={this.handleChange}
                placeholder="Display Name"
                required
              />
            </Form.Group>
          </Col>
          {/* UserName */}
          <Col>
            <Form.Group controlId="UserName" required>
              <Form.Label>
                <strong>Username</strong>
              </Form.Label>
              <Form.Control
                type="text"
                name="UserName"
                onChange={this.handleChange}
                placeholder="Username"
                required
              />
            </Form.Group>
          </Col>
        </Form.Row>
        <Form.Row>
          <Col>
            <Form.Group controlId="Email">
              <Form.Label>
                <strong>Email</strong>
              </Form.Label>
              <Form.Control
                type="email"
                name="Email"
                onChange={this.handleChange}
                placeholder="Email"
                required
              />
            </Form.Group>
          </Col>
          <Col />
        </Form.Row>
        <Form.Row>
          {/* Description */}
          <Col md="12" className="Description">
            <label htmlFor="Description">
              <strong>Description</strong>
            </label>
            <FormTextarea
              id="Description"
              name="Description"
              placeholder="Description"
              rows="5"
              onChange={this.handleChange}
            />
          </Col>
        </Form.Row>
        <br />
        <Form.Row>
          {/* Password & Password Duplicate */}
          <Col>
            <Form.Group>
              <Form.Label>
                <strong>Password</strong>
                <p>
                  <h8>
                    Password must contain at least one uppercase letter, one
                    lowercase letter, one number, one special character, with a
                    total length between 8 and 20 characters, inclusive.
                  </h8>
                </p>
              </Form.Label>
              <Form.Control
                type="password"
                name="Password"
                id="Password"
                placeholder="Password"
                onChange={this.handleChange}
                isInvalid={!validate_password(this.state.fields.Password)}
                required
              />
              <p className="errMsg">{this.state.errors.password}</p>
              <Form.Control
                type="password"
                name="Password_Duplicate"
                id="Password_Duplicate"
                placeholder="Enter Password Again"
                onChange={this.handleChange}
                required
              />
              <p className="errMsg">{this.state.errors.Password_Duplicate}</p>
            </Form.Group>
          </Col>
          <Col />
        </Form.Row>
        <br></br>
        {/* Agree to Terms and Conditions */}
        <Form.Check
          id="agree"
          label={"I Agree To The Terms & Conditions"}
          required
        />
        <br></br>
        <Button onClick={this.submitUserRegistrationForm} variant="dark">
          Create Account
        </Button>
        <br />
        <br />
      </Form>
    );
  }
}

/* Validates a password that is passed into the function with the following parameters:
 * At least one uppercase letter, one lowercase letter, one number, one special
 * character, with a total length between 8 and 20 characters, inclusive.
 * TODO: clean password inputs and make sure that no SQL injection is possible.
 */

function validate_password(pass) {
  if (typeof pass === "undefined" || pass === "") {
    return true;
  }
  return /^(?=.*[a-z])+(?=.*[A-Z])+(?=.*\d)+(?=.*[~`!@#$%^&*()_\-+=:?])+[A-Za-z\d~`!@#$%^&*()_\-+=:?]{8,20}$/.test(
    pass
  );
}
