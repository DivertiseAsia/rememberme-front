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
      <div className="row mt-4"> 
        <div className="col-7 col-md-8 col-xl-9"> 
          <p className="cursor-pointer" onClick=(_ => send(ChangeFormMenu(MyForm)))>{string("My Form")}</p>
        </div>
        <div className="col-5 col-md-4 col-xl-3"> 
          <button 
            type_="button" 
            className=("btn btn-rounded " ++ (state.formMenu === History ? " btn-main-color active-menu" : ""))
            onClick=(_ => send(ChangeFormMenu(History)))
          >
            {string("History")}
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
          state.requestLeaves |> List.map((requestLeave) => {
            <RequestLeave />
          }) |> Array.of_list |> array
        }
      </div>
    </div>;
  },
};
