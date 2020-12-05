import React from "react";
import ReactQuill from "react-quill";
import {
  Card,
  CardBody,
  Form,
  FormInput,
  FormGroup,
  FormSelect
} from "shards-react";
import { Button, Dropdown, DropdownButton } from "react-bootstrap";

import "react-quill/dist/quill.snow.css";
import "../../assets/quill.css";
import { Link } from "react-router-dom";
const axios = require("axios");

export default class Editor extends React.Component {
  constructor() {
    super();
    this.state = {
      UserId: localStorage.getItem("ValidatedUser"),
      PostTitle: "",
      PostText: "",
      Topics: "All",
      post_characters_remaining: 150,
      valid_post: true
    };
    this.handleChange = this.handleChange.bind(this);
    this.submitForm = this.submitForm.bind(this);
    this.handleTopicSelect = this.handleTopicSelect.bind(this);
    this.handleTitle = this.handleTitle.bind(this);
    this.postData = this.postData.bind(this);
  }

  async postData(post_data) {
    console.log(post_data);
    const response = await axios.post(
      //"http://twistter-API.azurewebsites.net/create-post",
      "http://localhost:5000/create-post",
      post_data
    );
    return response;
  }

  submitForm() {
    var html = this.state.PostText;
    var div = document.createElement("div");
    div.innerHTML = html;
    var stripped_text = div.textContent || div.innerText || "";

    /* Random lowercase variables are to try to communicate better with the api */
    const post_submission = {
      postTitle: this.state.PostTitle,
      topics: this.state.Topics,
      postText: stripped_text,
      userId: this.state.UserId
    };
    console.log(post_submission);
    let response = this.postData(post_submission);
    console.log(response);
  }

  handleTopicSelect(e) {
    this.setState({ Topics: e.target.value });
  }

  handleTitle(e) {
    this.setState({ PostTitle: e.target.value });
  }

  handleChange(text_value) {
    let post_characters_remaining = this.state.post_characters_remaining;
    let valid_post = this.state.valid_post;

    /* strips html text so real length of post body is obtained */
    var html = text_value;
    var div = document.createElement("div");
    div.innerHTML = html;
    var stripped_text = div.textContent || div.innerText || "";

    post_characters_remaining = 150 - stripped_text.length;
    if (post_characters_remaining < 0) {
      valid_post = false;
    } else {
      valid_post = true;
    }
    this.setState({
      post_characters_remaining: post_characters_remaining,
      valid_post: valid_post,
      PostText: text_value
    });
  }

  render() {
    return (
      <Card small className="mb-3" style={{ background: "#353A40" }}>
        <CardBody>
          <Form className="add-new-post">
            <FormInput
              size="lg"
              className="mb-3"
              placeholder="Your Post Title"
              maxLength={45}
              onChange={this.handleTitle}
              required
            />
            <ReactQuill
              className="add-new-post__editor mb-1"
              name="post_text_box"
              value={this.state.PostText}
              onChange={this.handleChange}
              style={{ background: "white" }}
              required
            />
            <p style={{ color: "white" }}>
              {this.state.post_characters_remaining} characters remaining
            </p>
            <FormGroup id="topicSelect">
              <FormSelect as="select" onClick={this.handleTopicSelect}>
                <option>Music</option>
                <option>Computer Science</option>
                <option>Gaming</option>
                <option>Beauty</option>
                <option>Animals</option>
                <option>Memes</option>
                <option>Art</option>
                <option>Sports</option>
              </FormSelect>
            </FormGroup>
          </Form>
          {!this.state.valid_post && (
            <Button type="submit" variant="dark" disabled>
              Post
            </Button>
          )}
          {this.state.valid_post && (
            <Link to={"/" + this.state.Topics.replace(" ", "")}>
              <Button
                type="submit"
                variant="outline-light"
                onClick={this.submitForm}
              >
                Post
              </Button>
            </Link>
          )}
        </CardBody>
      </Card>
    );
  }
}
