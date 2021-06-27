open ReasonReact;
open RememberMeApi;
open RememberMeType;
open Utils;

type state = {
  loadState,
  formMenu,
};

type action =
  | ChangeFormMenu(formMenu)
  | RemoveRequestLeave(string)
  | RemoveRequestLeaveSuccess
  | RemoveRequestLeaveFailed;

let removeRequestLeave = (id, dispatch) => {
  removeRequestLeave(
    ~token=Utils.getToken(),
    ~id,
    ~successAction=_ => dispatch(RemoveRequestLeaveSuccess),
    ~failAction=_ => dispatch(RemoveRequestLeaveFailed),
  );
};

[@react.component]
let make = (~requestLeaves, ~onRefresh) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | ChangeFormMenu(formMenu) => {...state, formMenu}
        | RemoveRequestLeave(id) =>
          //        removeRequestLeave(id, dispatch);
          // TODO:

          {...state, loadState: Loading}
        | RemoveRequestLeaveSuccess =>
          onRefresh();
          {...state, loadState: Succeed};
        | RemoveRequestLeaveFailed => {...state, loadState: Failed("")}
        }
      },
      {loadState: Idle, formMenu: MyForm},
    );

  <div className="container request-form-container">
    <div className="row mt-4 mb-2">
      <div className="col-7 col-md-6 col-lg-7 col-xl-9">
        <p
          className="cursor-pointer"
          onClick={_ => dispatch(ChangeFormMenu(MyForm))}
          style={ReactDOM.Style.make(
            ~marginTop="8px",
            ~marginBottom="0px",
            (),
          )}>
          {string("My Form")}
        </p>
      </div>
      <div className="col-5 col-md-6 col-lg-5 col-xl-3 pl-0">
        <button
          type_="button"
          className={
            "btn btn-rounded "
            ++ (
              state.formMenu === History
                ? "btn-main-color active-menu" : "btn-outline"
            )
          }
          onClick={_ => dispatch(ChangeFormMenu(History))}>
          {state.formMenu === History
             ? <img src="/images/history.svg" />
             : <img src="/images/history_blue.svg" />}
          {string(" History")}
        </button>
      </div>
    </div>
    <div
      className="row justify-content-center pt-5 pb-5"
      style={ReactDOM.Style.make(
        ~backgroundColor="white",
        ~borderTop="2px solid #52C4BB",
        (),
      )}>
      {requestLeaves
       |> List.filter((requestLeaves: leaveDetail) =>
            switch (state.formMenu) {
            | MyForm =>
              switch (requestLeaves.status) {
              | Pending => true
              | _ => false
              }
            | _ =>
              switch (requestLeaves.status) {
              | Pending => false
              | _ => true
              }
            }
          )
       |> List.mapi((i, requestLeave) =>
            <div
              key={"RequestLeave-" ++ (i |> string_of_int)}
              className="container "
              style={ReactDOM.Style.make(~width="100%", ())}>
              {i === 0 ? <hr /> : null}
              <RequestLeave
                requestLeave
                onCancel={_ => dispatch(RemoveRequestLeave(requestLeave.id))}
              />
              <hr />
            </div>
          )
       |> Array.of_list
       |> array}
    </div>
  </div>;
};