open ReasonReact;
open Page;
open Belt;

[@react.component]
let make =
    (
      ~isLoggedIn: bool,
      ~className: option(string)=?,
      ~children: ReasonReact.reactElement,
    ) => {
  <Page
    pageEnds={
      top: [Children(List.toArray([<Header isLoggedIn />]))],
      bottom: [],
    }
    className={className->Belt.Option.getWithDefault("mainpage-default")}>
    <div className="main-content"> children </div>
  </Page>;
};