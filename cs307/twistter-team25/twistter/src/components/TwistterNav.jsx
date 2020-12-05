import React from "react";
import {
  Nav,
  Navbar,
  NavDropdown,
  Form,
  FormControl,
  Button,
  Container
} from "react-bootstrap";

export default class TwistterNav extends React.Component {
  handleClick(page) {
    /* Redirect to appropriate page here */
  }
  render() {
    return (
      <div>
        <Container>
          <Navbar bg="dark" variant="dark" fixed="top">
            <Navbar.Brand>Twistter</Navbar.Brand>
            {/* Website Title, #all, and Profile */}
            <Nav className="mr-auto">
              <Nav.Link>#all</Nav.Link>
              <NavDropdown title="Profile">
                <NavDropdown.Item>My Profile</NavDropdown.Item>
                <NavDropdown.Item>Settings</NavDropdown.Item>
                <NavDropdown.Divider />
                <NavDropdown.Item>Log Out</NavDropdown.Item>
              </NavDropdown>
            </Nav>
            {/* Create a New Microblog */}
            <Nav className="center">
              <Button variant="outline-light">+ Create A New Microblog</Button>
            </Nav>
            {/* Search Bar */}
            <Nav>
              <Form inline>
                <FormControl
                  type="text"
                  placeholder="Search"
                  className="mr-sm-2"
                />
                <Button variant="outline-light" type="submit">
                  Go!
                </Button>
              </Form>
            </Nav>
          </Navbar>
        </Container>
        {/*THIS H1 IS TO MAKE SURE THAT TEXT ISN'T ACCIDENTALLY RENDERED UNDER THE NAVBAR */}
        <h1>navbar</h1>{" "}
      </div>
    );
  }
}
