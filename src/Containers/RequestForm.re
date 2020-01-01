open ReasonReact;
open RememberMeType;

let requestMenus = [Sick, Vacation];

type state = {
  loadState,
  formType,
  startDate: float,
  endDate: float,
  note: string,
  showPopup: bool,
};

type action =
  | ChangeFormType(formType)
  | TogglePopup(bool);

let dateForm = (~title="Start") => {
  <>
    <div className="col-1 p-0" />
    <div className="col-2 pt-3">
      {string(title)}
    </div>
    <div className="col-3 p-1">
      <input 
        type_="text" 
        className="form-control input-request" 
        placeholder="Day" 
      />
    </div>
    <div className="col-3 p-1">
      <input 
        type_="text" 
        className="form-control input-request" 
        placeholder="Month" 
      />
    </div>
    <div className="col-3 p-1">
      <input 
        type_="text" 
        className="form-control input-request" 
        placeholder="Year" 
      />
    </div>
  </>
};

let component = ReasonReact.reducerComponent("RequestForm");

let make = (_children) => {
  ...component,
  initialState: () => {
    loadState: Idle, 
    formType: Sick,
    startDate: Js.Date.now(),
    endDate: 0.,
    note: "",
    showPopup: false,
  },
  reducer: (action, state) => {
    switch (action) {
    | ChangeFormType(formType) => Update({...state, formType})
    | TogglePopup(showPopup) => Update({...state, showPopup})
    };
  },
  render: ({state, send}) =>
  <div className="container mt-4 request-form-container">
    <div className="row"> 
      <p>{string("Create Form")}</p>
    </div>
    <div 
      className="row justify-content-center pt-5 pb-2" 
      style=(ReactDOMRe.Style.make(
        ~backgroundColor="white", 
        ~borderTop="2px solid #FFA227",
      ()))
    >
      {
        requestMenus |> List.mapi((i, menu:formType) => {
          <div 
            key=("form-type-" ++ (menu |> RememberMeUtils.requestMenuToStr)) 
            className="col-6 col-sm-6 col-md-6 col-lg-5 col-xl-4"
          >
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
        <div className="row mt-5 pl-2 pr-2"> 
          <div className="timeline-point timeline-start" />
          {dateForm(~title="Start")}
        </div>
        <div className="row mt-4 mb-5 pl-2 pr-2"> 
          <div className="timeline-point timeline-end" />
          {dateForm(~title="End")}
        </div>
        
        <div className="row mt-4">
          <div className="col-12 col-md-4 col-note">  
            {string("Note:")}
          </div> 
          <div className="col-12 col-md-8">  
            <textarea 
              rows=2
              style=(ReactDOMRe.Style.make(~width="100%", ()))
            >
            </textarea>
          </div> 
        </div>
        {
          switch state.formType {
          | Sick => 
            <div className="row mt-4"> 
              <div className="col-12">
                <input 
                  type_="file" 
                  name="file" 
                  id="file" 
                  className="inputfile"
                  style=(ReactDOMRe.Style.make(~display="none", ()))
                />
                <label 
                  htmlFor="file"
                  style=(ReactDOMRe.Style.make(
                    ~width="100%",
                    ~padding="5px",
                    ~border="1px solid rgba(0,0,0,0.3)",
                    ()))
                >
                  <div className="row"> 
                    <div className="col-4 text-center"> 
                      <img className="m-auto" src="/images/upload.svg" style=(ReactDOMRe.Style.make(~width="80%", ())) />
                    </div>
                    <div className="col-8 p-0"> 
                      <p className="mb-1">{string("Please, Upload the Medical certificate.")}</p>
                      <p className="mb-1">{string("Description..")}</p>
                    </div>
                  </div>
                </label>
              </div>
            </div>
          | _ => null
          }
        }
        <div className="row mt-5"> 
          <button 
            type_="button" 
            className="btn btn-rounded btn-form- btn-form-active m-auto"
            onClick=(_ => send(TogglePopup(true)))
            style=(ReactDOMRe.Style.make(~maxWidth="120px", ()))
          >
            {string("Submit")}
          </button>
        </div>
        {
          switch state.formType {
          | Vacation =>  
            <div className="row mt-3"> 
              <div className="col"> 
                <img src="/images/coconut.svg" />
              </div>
            </div>
          | _ => null
          }
        }
      </div>
    </div>
    (state.showPopup ? 
      <PopupRequestForm onConfirm=(_ => send(TogglePopup(false))) /> : null
    )
  </div>,
};
