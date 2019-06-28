open ReasonReact;

let str = ReasonReact.string;


let component = ReasonReact.statelessComponent("PageHome");

let make = (~isLoggedIn: bool, _children) => {
  ...component,
  render: _self =>
    <div className="container">
      <h1> {string("Remember Me")} </h1>
      <div className="row">
        <div className="col-8"> <Calendar /> </div>
        <div className="col-4"> <div> {"June" |> str} </div> </div>
      </div>
    </div>,
};
