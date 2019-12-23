open ReasonReact;
open RememberMeType;

let requestMenus = [Sick, Vacation];

type state = {
  loadState,
  formType,
  startDate: float,
  endDate: float,
  note: string,
};

type action =
  | ChangeFormType(formType);

let component = ReasonReact.reducerComponent("RequestForm");

let make = (_children) => {
  ...component,
  initialState: () => {
    loadState: Idle, 
    formType: Sick,
    startDate: Js.Date.now(),
    endDate: 0.,
    note: "",
  },
  reducer: (action, state) => {
    switch (action) {
    | ChangeFormType(formType) => Update({...state, formType})
    };
  },
  render: ({state, send}) =>
  <div className="container mt-4 request-form-container">
    <div className="row"> 
      <p>{string("Create Form")}</p>
    </div>
    <div 
      className="row justify-content-center pt-5 pb-5" 
      style=(ReactDOMRe.Style.make(
        ~backgroundColor="white", 
        ~borderTop="2px solid #FFA227",
      ()))
    >
      {
        requestMenus |> List.mapi((i, menu:formType) => {
          <div key=("form-type-" ++ (menu |> RememberMeUtils.requestMenuToStr)) className="col-6">
            <button 
              type_="button" 
              className=("btn btn-rounded btn-form-" ++ (state.formType === menu ? " btn-form-active" : ""))
              onClick=(_ => send(ChangeFormType(menu)))
            >
              <img 
                className="form-icon mr-2" 
                src={
                  switch (menu, state.formType === menu) {
                  | (Sick, true) => "/images/sick.svg"
                  | (Sick, false) => "/images/sick_blue.svg"
                  | (Vacation, true) =>"/images/vacation.svg"
                  | _ => "/images/vacation_blue.svg"
                  }
                } 
              />
              {string(menu |> RememberMeUtils.requestMenuToStr)}
            </button>
          </div>
        }) |> Array.of_list |> array
      }
      <div className="col-12"> 
        <div className="row"> 
          <div className="timeline-point timeline-start" />
        </div>
        <div className="row"> 
          <div className="timeline-point timeline-end" />
        </div>
        
        <div className="row mt-4">
          <div className="col-12 col-md-4">  
            {string("Note:")}
          </div> 
          <div className="col-12 col-md-8">  
            <textarea 
              className="form-control" 
              rows=2
            >
            </textarea>
          </div> 
        </div>
      </div>
    </div>
  </div>,
};
