[@bs.module] external loading: string = "../../../../public/images/loading.gif";
let component = ReasonReact.statelessComponent("Loading");

let make = (~className: option(string)=?, ~onClick=?, _children) => {
  ...component,
  render: _self =>
    <div className={"loading-wrapper " ++ Js.Option.getWithDefault("", className)}>
      <img className={"loading " ++ Js.Option.getWithDefault("loading-default", className)} ?onClick src=loading />
    </div>,
};