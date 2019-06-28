open ReasonReact;
let component = ReasonReact.statelessComponent("PageHome");

let make = (~isLoggedIn: bool, _children) => {
  ...component,
  render: _self => <MainPage isLoggedIn> <div> {string("Home Page")} </div> </MainPage>,
};
