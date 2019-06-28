open ReasonReact;
let component = ReasonReact.statelessComponent("Page404");

let make = (~isLoggedIn: bool, _children) => {
  ...component,
  render: _self =>
    <MainPage isLoggedIn className="-text-center">
      <h2> {string("404")} </h2>
      <p> {string("Maybe you were looking for something else")} </p>
    </MainPage>,
};
