open ReasonReact;

let component = ReasonReact.statelessComponent("RequestLeave");

let make = (_children) => {
  ...component,
  render: _self => {
    <div className="container" style=(ReactDOMRe.Style.make(~borderBottom="1px solid", ()))>
    </div>;
  },
};
