open ReasonReact;
open RememberMeApi;
open Utils;

let str = ReasonReact.string;

type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed;

type state = {
  loadState,
  startDate: string,
  endDate: string,
  reason: string,
  leaveType: string,
};

type action =
  | RequestLeave
  | RequestLeaveSuccess
  | RequestLeaveFail
  | SetStartDate(string)
  | SetEndDate(string)
  | SetReason(string)
  | OnTypeLeaveChange(string);

let requestLeave = ({state, send}) => {
  let payload =
    Json.Encode.(
      object_([
        ("type", state.leaveType |> int_of_string |> int),
        ("from_date", state.startDate |> Js.Json.string),
        ("to_date", state.endDate |> Js.Json.string),
        ("reason", state.reason |> Js.Json.string),
      ])
    );
  postLeave(
    ~token=Utils.getToken(),
    ~payload,
    ~successAction=_ => send(RequestLeaveSuccess),
    ~failAction=_ => send(RequestLeaveFail),
  );
};

let component = ReasonReact.reducerComponent("RequestLeave");

let make = (~onClose, _children) => {
  ...component,
  initialState: () => {loadState: Idle, startDate: "", endDate: "", reason: "", leaveType:"0"},
  reducer: (action, state) => {
    switch (action) {
    | RequestLeave => UpdateWithSideEffects({...state, loadState: Loading}, requestLeave)
    | RequestLeaveSuccess => Update({...state, loadState: Succeed})
    | RequestLeaveFail => Update({...state, loadState: Failed})
    | SetReason(reason) => Update({...state, reason})
    | SetStartDate(startDate) => Update({...state, startDate})
    | SetEndDate(endDate) => Update({...state, endDate})
    | OnTypeLeaveChange(leaveType) => Update({...state, leaveType})
    };
  },
  render: ({state, send}) => { Js.log2(" state.endDate",  state.endDate);
     let disabled =
      switch (state.loadState) {
      | Loading => true
      | _ => false
      };
    <div className="modal-wrapper">
      <div className="modal-inner">
        <div onClick=onClose className="close-icon"> {"X" |> str}</div>
        <div className="container">
          <div className="row">
            <div className="col-3"> {"Start: " |> str} </div>
            <div className="col-3">
              <input
                type_="date"
                disabled
                className="form-control"
                onChange={e => send(SetStartDate(e |> valueFromEvent))}
              />
            </div>
            <div className="col-3"> {"End: " |> str} </div>
            <div className="col-3">
              <input
                type_="date"
                disabled
                className="form-control"
                onChange={e => { send(SetEndDate(e |> valueFromEvent))}}
              />
            </div>
          </div>
          <div className="row">
            <select  onChange=(e => send(OnTypeLeaveChange(e |> valueFromEvent)))>
              <option value="0">{"Sick Leave" |> str}</option>
              <option value="1">{"Personal Leave"|> str}</option>
            </select>
          </div>
          <div className="row">
            <textarea
              disabled
              className={j| form-control|j}
              placeholder="Detail about your leave"
              value={state.reason}
              onChange={e => send(SetReason(e |> valueFromEvent))}
            />
          </div>
          <div className="row justify-content-center">
            <button disabled onClick={_ => send(RequestLeave)}> {"Submit" |> str} </button>
          </div>
          {switch (state.loadState) {
           | Succeed => <div> {"Send request success" |> str} </div>
           | Failed => <div> {Utils.errorMessage |> str} </div>
           | Loading => <Loading />
           | _ => null
           }}
        </div>
      </div>
    </div>;
  },
};
