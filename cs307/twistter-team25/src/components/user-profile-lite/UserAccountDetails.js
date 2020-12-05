import React, { useState } from "react";
import {
  Card,
  CardHeader,
  ListGroup,
  ListGroupItem,
  Row,
  Col,
  Form,
  FormInput,
  FormTextarea,
  Button
} from "shards-react";
import { Link } from "react-router-dom";
import Alert from "react-bootstrap/Alert";
const axios = require("axios");

let config = {
  headers: {
    "content-type": "application/json"
  }
};

export default class UserAccountDetails extends React.Component {
  constructor() {
    super();

    this.state = {};
    this.handleName = this.handleName.bind(this);
    this.handleFirstPassword = this.handleFirstPassword.bind(this);
    this.handleSecondPassword = this.handleSecondPassword.bind(this);
    this.handleDescription = this.handleDescription.bind(this);
    this.submitForm = this.submitForm.bind(this);
    this.updateUserDetails = this.updateUserDetails.bind(this);
  }

  async updateUserDetails(post_data) {
    // user-update-common-name
    let newName = JSON.stringify({
      userId: localStorage.getItem("ValidatedUser"),
      newCommonName: post_data.newCommonName
    });
    console.log("common name submit:");
    console.log(newName);
    const common_name_response = axios.post(
      //"http://twistter-API.azurewebsites.net/user-update-common-name",
      "http://localhost:5000/user-update-common-name",
      newName,
      config
    );
    console.log("common name");
    console.log(common_name_response);

    // user-update-description
    let newDesc = JSON.stringify({
      userId: localStorage.getItem("ValidatedUser"),
      newDescription: post_data.newDescription
    });
    const description_response = axios.post(
      //"http://twistter-API.azurewebsites.net/user-update-description",
      "http://localhost:5000/user-update-description",
      newDesc,
      config
    );
    console.log("description");
    console.log(description_response);

    // update-password if applicable
    if (post_data.newPassword !== undefined) {
      let newPass = JSON.stringify({
        userId: localStorage.getItem("ValidatedUser"),
        newPassword: post_data.newPassword
      });
      const password_response = axios.post(
        //"http://twistter-API.azurewebsites.net/update-password",
        "http://localhost:5000/update-password",
        newPass,
        config
      );
      console.log("pass");
      console.log(password_response);
    }
  }

  async componentDidMount() {
    let data = JSON.stringify({
      userId: localStorage.getItem("ValidatedUser")
    });
    if (localStorage.getItem("ValidatedUser") !== -1) {
      const response = await axios.post(
        //"http://twistter-API.azurewebsites.net/get-user",
        "http://localhost:5000/get-user",
        data,
        config
      );
      console.log(response.data);
      this.setState(response.data);
    }
  }

  submitForm() {
    if (
      !this.state.FirstPassword === undefined &&
      this.state.SecondPassword === undefined
    ) {
      this.setState({ passwordInvalid: true });
    } else if (
      this.state.FirstPassword === undefined &&
      !this.state.SecondPassword === undefined
    ) {
      this.setState({ passwordInvalid: true });
    } else if (
      this.state.FirstPassword === undefined &&
      this.state.SecondPassword === undefined
    ) {
      this.setState({ passwordInvalid: false });
    } else if (
      this.state.FirstPassword.localeCompare(this.state.SecondPassword) != 0
    ) {
      this.setState({ passwordInvalid: true });
    } else if (
      this.state.FirstPassword.localeCompare(this.state.SecondPassword) == 0 &&
      validate_password(this.state.FirstPassword) == true
    ) {
      this.setState({ passwordInvalid: false });
    } else {
      this.setState({ passwordInvalid: true });
    }

    var userSubmission = {};
    if (this.state.passwordInvalid == false) {
      /* Random lowercase shit is to communicate with the api better */
      userSubmission = {
        UserName: this.state.UserName,
        newCommonName: this.state.CommonName,
        //Email: this.state.Email,
        newDescription: this.state.Description
      };

      if (this.state.FirstPassword === undefined) {
      } else {
        userSubmission.newPassword = this.state.FirstPassword;
      }
      //TODO: COMMUNICATE WITH API
      this.updateUserDetails(userSubmission);
      console.log(userSubmission);
      window.alert("Updated profile information!");
    }
  }

  handleName(e) {
    this.setState({ CommonName: e.target.value });
  }

  handleFirstPassword(e) {
    this.setState({ FirstPassword: e.target.value });
  }

  handleSecondPassword(e) {
    this.setState({ SecondPassword: e.target.value });
  }

  handleDescription(e) {
    this.setState({ Description: e.target.value });
  }

  render() {
    return (
      <Card small bg="dark" text="white" style={{ background: "#353A40" }}>
        <CardHeader className="border-bottom">
          <h6 className="m-0" style={{ color: "white" }}>
            Account Details
          </h6>
        </CardHeader>
        <ListGroup flush>
          <ListGroupItem
            className="p-3"
            style={{ background: "#353A40", color: "white" }}
          >
            <Col>
              <Form>
                <Row form>
                  {/* Name */}
                  <Col md="6" className="form-group">
                    <label htmlFor="DisplayName">Display Name</label>
                    <FormInput
                      id="DisplayName"
                      name="DisplayName"
                      defaultValue={this.state.CommonName}
                      onChange={this.handleName}
                    />
                  </Col>
                </Row>
                <Row form>
                  {/* Password */}
                  <Col md="6" className="form-group">
                    <label htmlFor="Password">Change Password</label>
                    <FormInput
                      type="password"
                      id="Password"
                      name="Password"
                      onChange={this.handleFirstPassword}
                      autoComplete="current-password"
                    />
                    <p htmlFor="PasswordConfirm" color="black">
                      Confirm Password Change
                    </p>
                    <FormInput
                      type="password"
                      id="PasswordConfirm"
                      name="PasswordConfirm"
                      onChange={this.handleSecondPassword}
                      autoComplete="current-password"
                    />
                    {this.state.passwordInvalid && (
                      <div>
                        <p>Passwords must match and be valid strings!</p>
                        <p>
                          Password must be 8-20 characters and contain at least
                          one uppercase, lowercase, number, and special
                          character.
                        </p>
                      </div>
                    )}
                  </Col>
                </Row>
                <Row form>
                  {/* Description */}
                  <Col md="12" className="form-group">
                    <label htmlFor="Description">Description</label>
                    <FormTextarea
                      id="Description"
                      name="Description"
                      defaultValue={this.state.Description}
                      rows="5"
                      onChange={this.handleDescription}
                    />
                  </Col>
                </Row>
                <Row>
                  <Col>
                    <Button theme="outline-light" onClick={this.submitForm}>
                      Update Account
                    </Button>
                  </Col>
                  <Col>
                    <DeleteAccountButton deleteUser={this.deleteUser} />
                  </Col>
                </Row>
              </Form>
            </Col>
          </ListGroupItem>
        </ListGroup>
      </Card>
    );
  }
}
async function deleteUser() {
  let userToDelete = JSON.stringify({
    userId: localStorage.getItem("ValidatedUser")
  });
  console.log("DELETE");
  console.log(userToDelete);
  const userDeleteResponse = axios.post(
    //"http://twistter-API.azurewebsites.net/user-delete",
    "http://localhost:5000/user-delete",
    userToDelete,
    config
  );
  console.log(userDeleteResponse);
}

function DeleteAccountButton() {
  const [show, setShow] = useState(false);

  return (
    <div>
      <Alert variant="danger" show={show}>
        <Alert.Heading>WARNING</Alert.Heading>
        <p>CONTINUING WILL PERMANENTLY DELETE YOUR ACCOUNT!</p>
        <p>This will route you back to the login page.</p>
        <hr />
        <Link to="/login">
          <Button theme="danger" onClick={deleteUser}>
            Yes, I would like to permanently delete my account!
          </Button>
        </Link>
      </Alert>

      {!show && (
        <Button
          theme="outline-danger"
          className="float-right"
          onClick={() => setShow(true)}
        >
          Delete Account
        </Button>
      )}
    </div>
  );
}

/* Validates a password that is passed into the function with the following parameters:
 * At least one uppercase letter, one lowercase letter, one number, one special
 * character, with a total length between 8 and 20 characters, inclusive.
 * TODO: clean password inputs and make sure that no SQL injection is possible.
 *
 * Returns true if password is valid, false otherwise.
 */

function validate_password(pass) {
  if (typeof pass === "undefined" || pass === "") {
    return true;
  }
  return /^(?=.*[a-z])+(?=.*[A-Z])+(?=.*\d)+(?=.*[~`!@#$%^&*()_\-+=:?])+[A-Za-z\d~`!@#$%^&*()_\-+=:?]{8,20}$/.test(
    pass
  );
}
