import React from "react";
import NavigationBar from "../../components/NavigationBar";
import PageTitle from "../../components/common/PageTitle";
import { blogDivStyle } from "../..";
import BlogPosts from "../../views/BlogPosts";
import { Container, Row, Col } from "react-bootstrap";

export default class AllTopics extends React.Component {
  render() {
    return (
      <div className="AllTopics" style={blogDivStyle}>
        <NavigationBar />
        <Container>
          <Row>
            <Col sm={8}>
              <PageTitle
                sm="4"
                title="Blog Posts"
                subtitle="All Topics"
                className="text-sm-left"
              />
            </Col>
          </Row>
        </Container>
        <BlogPosts topic="all" />
      </div>
    );
  }
}
