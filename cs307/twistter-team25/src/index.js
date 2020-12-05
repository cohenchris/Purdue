import React from "react";
import ReactDOM from "react-dom";
import "./index.css";
import * as serviceWorker from "./serviceWorker";
import "bootstrap/dist/css/bootstrap.min.css";
import "../node_modules/bootstrap/dist/css/bootstrap.min.css";
import { BrowserRouter as Router, Switch, Route } from "react-router-dom";
// PAGES
import StartPage from "./WebsitePages/StartPage/StartPage";
import CreateMicroblog from "./WebsitePages/CreateMicroblog";
import CreateNewAccount from "./WebsitePages/CreateNewAccount";
import DownForMaintenance from "./WebsitePages/DownForMaintenance";
import LoginPage from "./WebsitePages/LoginPage";
import ProfilePage from "./WebsitePages/ProfilePage";
import ProfileSettings from "./WebsitePages/ProfileSettings";
import DmPage from "./components/DmPage";
// Topics
import AllTopics from "./WebsitePages/TopicPages/AllTopics";
import HomePage from "./WebsitePages/TopicPages/HomePage";
import Topic_Sports from "./WebsitePages/TopicPages/Topic_Sports.jsx";
import Topic_Animals from "./WebsitePages/TopicPages/Topic_Animals";
import Topic_Art from "./WebsitePages/TopicPages/Topic_Art";
import Topic_Beauty from "./WebsitePages/TopicPages/Topic_Beauty";
import Topic_CS from "./WebsitePages/TopicPages/Topic_CS";
import Topic_Memes from "./WebsitePages/TopicPages/Topic_Memes";
import Topic_Music from "./WebsitePages/TopicPages/Topic_Music";
import Topic_Gaming from "./WebsitePages/TopicPages/Topic_Gaming";

document.body.style = "background: lightgrey;";

export const blogDivStyle = {
  marginLeft: "200px",
  marginRight: "200px",
  marginTop: "30px"
};

export const otherDivStyle = {
  marginLeft: "50px",
  marginRight: "50px",
  marginTop: "30px"
};

export const blackLink = {
  color: "black"
};

export const whiteLink = {
  color: "white"
};

export const DMPageStyle = {
  marginTop: "15px"
};

const ValidatedUserContext = React.createContext(-1);
const routing = (
  <Router>
    <Switch>
      <Route path="/" exact component={StartPage} />
      <Route path="/create-microblog" component={CreateMicroblog} />
      <Route path="/create-account" component={CreateNewAccount} />
      <Route path="/maintenance" component={DownForMaintenance} />
      <Route path="/login" component={LoginPage} />
      <Route
        path="/profile"
        render={props => (
          <ValidatedUserContext.Provider value={4}>
            <ProfilePage {...props} />
          </ValidatedUserContext.Provider>
        )}
      />
      <Route path="/settings" component={ProfileSettings} />
      {/*<Route path="/dm" component={DmPage} />*/}
      <Route
        path="/dm"
        render={props => <DmPage {...props} key={Math.random()} />}
      />
      {/* TOPICS */}
      <Route path="/home" component={HomePage} />
      <Route path="/all" component={AllTopics} />
      <Route path="/music" component={Topic_Music} />
      <Route path="/computerscience" component={Topic_CS} />
      <Route path="/gaming" component={Topic_Gaming} />
      <Route path="/beauty" component={Topic_Beauty} />
      <Route path="/animals" component={Topic_Animals} />
      <Route path="/memes" component={Topic_Memes} />
      <Route path="/art" component={Topic_Art} />
      <Route path="/sports" component={Topic_Sports} />
    </Switch>
  </Router>
);

ReactDOM.render(routing, document.getElementById("root"));

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: https://bit.ly/CRA-PWA
serviceWorker.unregister();
