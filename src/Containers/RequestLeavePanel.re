open ReasonReact;
open RememberMeApi;
open RememberMeType;
open Utils;

type state = {
  loadState,
  formMenu,
  requestLeaves: list(string),
};

type action =
  | ChangeFormMenu(formMenu);

let component = ReasonReact.reducerComponent("RequestLeavePanel");

let make = (_children) => {
  ...component,
  initialState: () => {
    loadState: Idle, 
    formMenu: MyForm,
    requestLeaves: ["", ""],
  },
  reducer: (action, state) => {
    switch (action) {
    | ChangeFormMenu(formMenu) => Update({...state, formMenu})
    };
  },
  render: ({state, send}) => { 
    <div className="container request-form-container">
      <div className="row mt-4 mb-2"> 
        <div className="col-7 col-md-6 col-lg-7 col-xl-9"> 
          <p 
            className="cursor-pointer" 
            onClick=(_ => send(ChangeFormMenu(MyForm)))
            style=(ReactDOMRe.Style.make(~marginTop="8px", ~marginBottom="0px", ()))
          >
            {string("My Form")}
          </p>
        </div>
        <div className="col-5 col-md-6 col-lg-5 col-xl-3 pl-0"> 
          <button 
            type_="button" 
            className=("btn btn-rounded " ++ (state.formMenu === History ? "btn-main-color active-menu" : "btn-outline"))
            onClick=(_ => send(ChangeFormMenu(History)))
          >
            (state.formMenu === History ? 
              <img src="/images/history.svg" />
              : 
              <img src="/images/history_blue.svg" />
            )
            {string(" History")}
          </button>
        </div>
      </div>
      <div 
        className="row justify-content-center pt-5 pb-5" 
        style=(ReactDOMRe.Style.make(
          ~backgroundColor="white", 
          ~borderTop="2px solid #52C4BB",
        ()))
      >
        {
          state.requestLeaves |> List.mapi((i, requestLeave) => {
            <div key=("RequestLeave-" ++ (i |> string_of_int)) className="container " style=(ReactDOMRe.Style.make(~width="100%", ()))>
              (i === 0 ? <hr /> : null)
              <RequestLeave onCancel=(_ => ()) />
              <hr />
            </div>
          }) |> Array.of_list |> array
        }
      </div>
    </div>;
  },
};
