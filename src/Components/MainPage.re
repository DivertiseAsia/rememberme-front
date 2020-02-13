open ReasonReact;
open Page;
open Belt;

let component = ReasonReact.statelessComponent("MainPageRe");

let make = (~isLoggedIn: bool, ~className: option(string)=?, children: array(ReasonReact.reactElement)) => {
  ...component,
  render: _self =>
    <Page
      pageEnds={top: [Children(List.toArray([<Header isLoggedIn />]))], bottom: []}
      className={Js.Option.getWithDefault("mainpage-default", className)}>
      <div className="main-content"> ...children </div>
    </Page>,
};