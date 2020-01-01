open ReasonReact;
open RememberMeApi;
open Utils;

let str = ReasonReact.string;
type leaveDetail = RememberMeApi.leaveDetail;

type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed;

type state = {
  loadState,
  leaveDetailList: list(leaveDetail),
};

type action =
  | RequestUserLeave
  | RequestUserLeaveSuccess(list(leaveDetail))
  | RequestUserLeaveFail;

let requestLeave = ({state, send}) => {
  fetchUserLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaves => send(RequestUserLeaveSuccess(leaves)),
    ~failAction=_ => send(RequestUserLeaveFail),
  );
};

let component = ReasonReact.reducerComponent("RequestUserLeave");

let make = (~onClose, _children) => {
  ...component,
  initialState: () => {loadState: Idle, leaveDetailList: []},
  didMount: ({send}) => send(RequestUserLeave),
  reducer: (action, state) => {
    switch (action) {
    | RequestUserLeave => UpdateWithSideEffects({...state, loadState: Loading}, requestLeave)
    | RequestUserLeaveSuccess(leaveDetailList) => Update({...state, loadState: Succeed, leaveDetailList})
    | RequestUserLeaveFail => Update({...state, loadState: Failed})
    };
  },
  render: ({state, send}) => {
    let disabled =
      switch (state.loadState) {
      | Loading => true
      | _ => false
      };
    <div className="modal-wrapper">
      <div className="modal-inner">
        <div onClick=onClose className="close-icon"> {"X" |> str} </div>
        <div className="container">
          <div>
            {state.leaveDetailList
             |> List.map(leave =>
                  <div className="row">
                    <p>
                      {{"Date: "
                        ++ (leave.fromDate |> Js.Date.toLocaleDateString)
                        ++ " - "
                        ++ (leave.toDate |> Js.Date.toLocaleDateString)}
                       |> string}
                    </p>
                    <br />
                    <p> {"LeaveType: " ++ (leave.leaveType |> mapLeaveTypeStr) |> string} </p>
                    <br />
                    <p> {"Reason: " ++ leave.reason |> string} </p>
                    <br />
                    <p> {"Approved: " ++ (leave.approved ? "approve" : "rejected") |> string} </p>
                  </div>
                )
             |> Array.of_list
             |> array}
          </div>
        </div>
      </div>
    </div>;
  },
};
