import React from "react";
import NavigationBar from "../../components/NavigationBar";
import PageTitle from "../../components/common/PageTitle";
import { blogDivStyle } from "../..";
import BlogPosts from "../../views/BlogPosts";
import { Button, Container, Row, Col } from "react-bootstrap";
const axios = require("axios");

export default class Topic_Art extends React.Component {
  async handleFollow() {
    let config = {
      headers: {
        "content-type": "application/json"
      }
    };
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/user-add-new-user-topic",
      "http://localhost:5000/user-add-new-user-topic",
      { userId: localStorage.getItem("ValidatedUser"), newTopic: "art" },
      JSON.stringify(config)
    );
    console.log(response);
    window.alert("Sucessfully followed!");
  }
  render() {
    return (
      <div className="MiscTopic" style={blogDivStyle}>
        <NavigationBar />
        <Container>
          <Row>
            <Col sm={8}>
              <PageTitle
                sm="4"
                title="Blog Posts"
                subtitle="Art"
                className="text-sm-left"
              />
            </Col>
            <Col sm={4}>
              <Button type="submit" variant="dark" onClick={this.handleFollow}>
                Follow Topic
              </Button>
            </Col>
          </Row>
        </Container>
        <BlogPosts topic="art" />
      </div>
    );
  }
}
