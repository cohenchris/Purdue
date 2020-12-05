import React from "react";
import {
  Nav,
  Navbar,
  NavDropdown,
  Button,
  Container,
  Col,
  Form,
  FormControl
} from "react-bootstrap";
import { Link } from "react-router-dom";
import FormCheckInput from "react-bootstrap/FormCheckInput";

const navBarStyle = {
  color: "white"
};

export default class NavigationBar extends React.Component {
  logOut() {
    localStorage.setItem("ValidatedUser", -1);
  }

  requestTopic() {
    window.alert("Topic requested!");
  }

  render() {
    return (
      <div>
        <Container>
          <Navbar bg="dark" variant="dark" fixed="top">
            <Navbar.Brand>
              <Link to="/home" style={navBarStyle}>
                Twistter
              </Link>
            </Navbar.Brand>

            {/* Website Title, #all, and Profile */}
            <Nav className="mr-auto">
              <NavDropdown title="Profile">
                <NavDropdown.Item href="/profile">My Profile</NavDropdown.Item>
                <NavDropdown.Item href="/settings">Settings</NavDropdown.Item>
                <NavDropdown.Item href="/dm">Direct Messages</NavDropdown.Item>
                <NavDropdown.Divider />
                <NavDropdown.Item href="/login" onClick={this.logOut}>
                  Log Out
                </NavDropdown.Item>
              </NavDropdown>

              <NavDropdown title="Topics">
                <NavDropdown.Item href="/all">All Topics</NavDropdown.Item>
                <NavDropdown.Item href="/home">Home</NavDropdown.Item>
                <NavDropdown.Divider />
                <NavDropdown.Item href="/sports">Sports</NavDropdown.Item>
                <NavDropdown.Item href="/animals">Animals</NavDropdown.Item>
                <NavDropdown.Item href="/art">Art</NavDropdown.Item>
                <NavDropdown.Item href="/beauty">Beauty</NavDropdown.Item>
                <NavDropdown.Item href="/computerscience">
                  Computer Science
                </NavDropdown.Item>
                <NavDropdown.Item href="/memes">Memes</NavDropdown.Item>
                <NavDropdown.Item href="/music">Music</NavDropdown.Item>
                <NavDropdown.Item href="/gaming">Gaming</NavDropdown.Item>
              </NavDropdown>
            </Nav>
            {/* Create a New Microblog */}
            <Col>
              <Form inline className="float-right">
                <FormControl
                  type="text"
                  placeholder="Topic..."
                  className="mr-sm-2"
                />
                <Button variant="outline-light" onClick={this.requestTopic}>
                  Request Topic
                </Button>
              </Form>
              <Nav className="float-right">
                <Link to="/create-microblog" style={navBarStyle}>
                  <Button variant="outline-light" className="mr-sm-5">
                    + Create A New Microblog
                  </Button>
                </Link>
              </Nav>
            </Col>
          </Navbar>
        </Container>
        <br />
      </div>
    );
  }
}
