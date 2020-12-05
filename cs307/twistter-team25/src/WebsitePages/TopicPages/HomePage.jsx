import React from "react";
import NavigationBar from "../../components/NavigationBar";
import "bootstrap/dist/css/bootstrap.min.css";
import UserTimeline from "../../views/UserTimeline";
import PageTitle from "../../components/common/PageTitle";
import { blogDivStyle } from "../..";
import { Container, Row, Col } from "react-bootstrap";

export default class HomePage extends React.Component {
  render() {
    return (
      <div className="HomePage" style={blogDivStyle}>
        <NavigationBar />
        <Container>
          <Row>
            <Col sm={8}>
              <PageTitle
                sm="4"
                title="Blog Posts"
                subtitle="HOME"
                className="text-sm-left"
              />
            </Col>
          </Row>
        </Container>
        <UserTimeline />
      </div>
    );
  }
}
