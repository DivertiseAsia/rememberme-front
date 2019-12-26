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

let make = (
    ~isLoggedIn: bool, 
    ~month=(Js.Date.make() |> Js.Date.getMonth |> int_of_float),
    ~year=(Js.Date.make() |> Js.Date.getFullYear), 
    _children
  ) => {
  ...component,
  initialState: () => {showRequestLeave: false, showRequestUserLeave: false},
  reducer: (action, state) => {
    switch (action) {
    | ToggleRequestLeave => Update({...state, showRequestLeave: !state.showRequestLeave})
    | ToggleRequestUserLeave => Update({...state, showRequestUserLeave: !state.showRequestUserLeave})
    };
  },
  render: ({state: {showRequestLeave, showRequestUserLeave}, send}) =>
    <div className="home-page container-fluid">
      <div className="row row-main" style=(ReactDOMRe.Style.make(~height="100%", ()))>
        <div className="col-12 col-sm-12 col-md-7 col-lg-8 col-xl-8 col-calendar"> 
          <Calendar month year />
        </div>
        <div className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-4 p-0  col-schedule">
          /*<div className="row">
            <button onClick={_ => send(ToggleRequestLeave)}> {"RequestLeave" |> str} </button>
            <button onClick={_ => send(ToggleRequestUserLeave)}> {"UserLeave" |> str} </button>
          </div>*/
          <Schedule />
        </div>
      </div>
      /*{showRequestLeave ? <RequestLeave onClose={_ => send(ToggleRequestLeave)} /> : null}
      {showRequestUserLeave ? <UserLeave onClose={_ => send(ToggleRequestUserLeave)} /> : null}
      */
    </div>,
};
