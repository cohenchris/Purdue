/* eslint jsx-a11y/anchor-is-valid: 0 */

import React, { useState } from "react";
import { Container, Row, Col, Card, CardBody, Badge } from "shards-react";
import { ToggleButton, ToggleButtonGroup, Button } from "react-bootstrap";
import { Link } from "react-router-dom";
import { whiteLink } from "../index.js";

const axios = require("axios");

export default class BlogPosts extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      PostsList: [],
      id: this.props.userId
    };
    this.handleDelete = this.handleDelete.bind(this);
    this.componentDidMount = this.componentDidMount.bind(this);
  }

  async componentDidMount() {
    let config = {
      headers: {
        "content-type": "application/json"
      }
    };
    console.log("getting posts for userId " + this.state.id);
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/user-get-user-posts"
      "http://localhost:5000/user-get-user-posts",
      { userId: this.state.id },
      config
    );
    console.log(response.data);
    this.setState({ PostsList: response.data });
  }

  async handleDelete(id) {
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/delete-post",
      "http://localhost:5000/delete-post",
      { postId: id }
    );
    console.log(response);
    window.location.reload();
    window.alert("Deleted post");
  }

  render() {
    const PostsList = this.state.PostsList;
    return (
      <Container fluid className="main-content-container px-4">
        {/* Page Header */}
        <Row noGutters className="page-header py-4"></Row>
        <Row>
          {PostsList.map((post, idx) => (
            <Col lg="3" md="6" sm="12" className="mb-4" key={idx}>
              <Card small className="bg-dark text-white">
                <div className="card-post__image">
                  <Col>
                    <Badge
                      pill
                      className={`card-post__category bg-light text-dark`}
                    >
                      {post.Topics.substr(0, post.Topics.length - 4)}
                    </Badge>
                    {post.UserId == localStorage.getItem("ValidatedUser") && (
                      <Button
                        size="sm"
                        variant="outline-danger"
                        className="float-right mt-sm-2"
                        onClick={this.handleDelete.bind(this, post.PostId)}
                      >
                        X
                      </Button>
                    )}
                  </Col>
                  <Col>
                    <div className="card-post__author d-flex">
                      <Link
                        to={{
                          pathname: "/profile",
                          id: post.UserId
                        }}
                        style={whiteLink}
                      >
                        {post.CommonName + " (@" + post.UserName + ")"}
                      </Link>
                    </div>
                  </Col>
                </div>
                <Col>
                  <div>
                    <span>
                      {post.Timestamp.substr(0, 10) +
                        " at " +
                        post.Timestamp.substr(11)}
                    </span>
                  </div>
                </Col>
                <CardBody>
                  <h5 className="card-title">{post.PostTitle}</h5>
                  <p className="card-text d-inline-block mb-3">
                    {post.PostText}
                  </p>
                  <LikeAndQuoteButtons
                    like_count={post.Likes}
                    quote_count={post.Retweets}
                    postId={post.PostId}
                  />
                  {post.RetweetUserName !== null && (
                    <center>
                      <br />
                      <strong>
                        ---- quoted by {post.RetweetUserName} ----
                      </strong>
                    </center>
                  )}
                </CardBody>
              </Card>
            </Col>
          ))}
        </Row>
      </Container>
    );
  }
}

const LikeAndQuoteButtons = props => {
  const [button, setButton] = useState();
  const [likes, setLikes] = useState(props.like_count);
  const [quotes, setQuotes] = useState(props.quote_count);

  const handleLike = async num_likes => {
    setButton(num_likes);
    if (num_likes == 1) {
      setLikes(likes + 1);
      // like-post
      const response = await axios.post(
        //"http://twistter-API.azurewebsites.net/like-post",
        "http://localhost:5000/like-post",
        { userId: localStorage.getItem("ValidatedUser"), postId: props.postId }
      );
      console.log(response);
    } else {
      setLikes(likes - 1);
      // unlike-post
      const response = await axios.post(
        //"http://twistter-API.azurewebsites.net/unlike-post",
        "http://localhost:5000/unlike-post",
        { userId: localStorage.getItem("ValidatedUser"), postId: props.postId }
      );
      console.log(response);
    }
  };
  const handleQuote = async num_quotes => {
    setButton(num_quotes);
    if (num_quotes == 2) {
      setQuotes(quotes + 1);
      const response = await axios.post(
        //"http://twistter-API.azurewebsites.net/retweet-post",
        "http://localhost:5000/retweet-post",
        { userId: localStorage.getItem("ValidatedUser"), postId: props.postId }
      );
      console.log(response);
    } else {
      setQuotes(quotes - 1);
      const response = await axios.post(
        //"http://twistter-API.azurewebsites.net/unretweet-post",
        "http://localhost:5000/unretweet-post",
        { userId: localStorage.getItem("ValidatedUser"), postId: props.postId }
      );
      console.log(response);
    }
  };

  return (
    <Row>
      <Col>
        <ToggleButtonGroup type="checkbox" size="sm" onChange={handleLike}>
          <ToggleButton variant="outline-light" value={1}>
            Like
          </ToggleButton>
          <Button variant="light" disabled>
            {likes}
          </Button>
        </ToggleButtonGroup>
      </Col>

      <Col>
        <ToggleButtonGroup type="checkbox" size="sm" onChange={handleQuote}>
          <ToggleButton variant="outline-light" value={2}>
            Quote
          </ToggleButton>
          <Button variant="light" disabled>
            {quotes}
          </Button>
        </ToggleButtonGroup>
      </Col>
    </Row>
  );
};
