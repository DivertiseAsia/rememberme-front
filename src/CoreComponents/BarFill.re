let component = ReasonReact.statelessComponent("Link");

let make = (~percent: int, ~className: option(string)=?, ~onClick=?, ~title: option(string)=?, _children) => {
  ...component,
  render: _self =>
    <div ?title ?onClick className={"progress " ++ Js.Option.getWithDefault("progress-default", className)}>
      <div className="fillbar" style={ReactDOMRe.Style.make(~width=string_of_int(percent) ++ "%", ())} />
    </div>,
};