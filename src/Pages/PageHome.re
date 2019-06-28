open ReasonReact;

let str = ReasonReact.string;

type state = {
  showRequestLeave: bool,
  showRequestUserLeave: bool,
};

type action =
  | ToggleRequestLeave
  | ToggleRequestUserLeave;

let component = ReasonReact.reducerComponent("PageHome");

let make = (~isLoggedIn: bool, _children) => {
  ...component,
  initialState: () => {showRequestLeave: false, showRequestUserLeave: false},
  reducer: (action, state) => {
    switch (action) {
    | ToggleRequestLeave => Update({...state, showRequestLeave: !state.showRequestLeave})
    | ToggleRequestUserLeave => Update({...state, showRequestUserLeave: !state.showRequestUserLeave})
    };
  },
  render: ({state: {showRequestLeave, showRequestUserLeave}, send}) =>
    <div className="container">
      <h1> {string("Remember Me")} </h1>
      <div className="row">
        <div className="col-8"> <Calendar /> </div>
        <div className="col-4">
          <div> {"June" |> str} </div>
          <div className="row">
            <button onClick={_ => send(ToggleRequestLeave)}> {"RequestLeave" |> str} </button>
            <button onClick={_ => send(ToggleRequestUserLeave)}> {"UserLeave" |> str} </button>
          </div>
        </div>
      </div>
      {showRequestLeave ? <RequestLeave onClose={_ => send(ToggleRequestLeave)} /> : null}
      {showRequestUserLeave ? <UserLeave onClose={_ => send(ToggleRequestUserLeave)} /> : null}
    </div>,
};
