import React from "react";
import ProfilePosts from "../../views/ProfilePosts";
import { Card, CardHeader, ListGroup, Row, Col, Button } from "shards-react";
import { DropdownButton, Dropdown } from "react-bootstrap";
import { FormSelect } from "shards-react";
const axios = require("axios");

export default class UserDetails extends React.Component {
  constructor(props) {
    super(props);
    this.state = { Posts: 0, UserId: this.props.id };
    this.componentDidMount = this.componentDidMount.bind(this);
    this.followUser = this.followUser.bind(this);
  }

  async componentDidMount() {
    let config = {
      headers: {
        "content-type": "application/json"
      }
    };

    let userData = JSON.stringify({ userId: this.state.UserId });
    if (this.state.UserId !== -1) {
      const response = await axios.post(
        //"http://twistter-API.azurewebsites.net/get-user",
        "http://localhost:5000/get-user",
        userData,
        config
      );
      this.setState(response.data);
    }
  }

  async followUser() {
    let config = {
      headers: {
        "content-type": "application/json"
      }
    };
    let userData = JSON.stringify({
      userId: localStorage.getItem("ValidatedUser"),
      followingId: this.state.UserId,
      topicsSelected:
        "All,Sports,Art,Animals,Art,Beauty,ComputerScience,Memes,Music,Gaming"
    });
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/follow-users-topics",
      "http://localhost:5000/follow-users-topics",
      userData,
      config
    );
    console.log(response);
    window.alert("Followed User!");
  }

  render() {
    return (
      <div>
        <Card
          small
          text="white"
          style={{ background: "#353A40" }}
          className="mb-4 pt-3"
        >
          <CardHeader className="border-bottom text-center">
            <h2 className="mb-0" style={{ color: "white" }}>
              {this.state.CommonName}
            </h2>
            <h5 style={{ color: "white" }}>{this.state.UserName}</h5>
            <br />
            {this.props.id !== localStorage.getItem("ValidatedUser") && (
              <Button theme="outline-light" onClick={this.followUser}>
                + Follow
              </Button>
            )}

            <br />
            <br />
            <p style={{ color: "white" }}>{this.state.Followers} Followers</p>
            <p style={{ color: "white" }}>{this.state.Following} Following</p>
          </CardHeader>
          <ListGroup flush>
            <Col>
              <br />
              <Row form>
                {/* Description */}
                <Col md="12" className="form-group">
                  <label style={{ color: "white" }} htmlFor="Description">
                    <strong>Description</strong>
                  </label>
                  <p style={{ color: "white" }}>{this.state.Description}</p>
                </Col>
              </Row>
            </Col>
          </ListGroup>
        </Card>
        <br></br>

        <h1>{this.state.Posts} POSTS BY THIS USER</h1>

        {/* TODO: pass userId into BlogPosts */}
        <ProfilePosts userId={this.state.UserId} />
      </div>
    );
  }
}
