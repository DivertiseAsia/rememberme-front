open ReasonReact;
open RememberMeType;
open RememberMeApi;
open Utils;

let menus = [All, Leave, Holiday, Event];

type contentForm = 
  | Idle
  | RequestForm
  | MyForm;

type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed;

type state = {
  loadState,
  scheduleMenu,
  contentForm,
  scheduleList: list(string),
  showPopover: bool,
};

type action =
  | RequestSchedule
  | RequestScheduleSuccess(list(string))
  | RequestScheduleFail
  | ChangeScheduleMenu(scheduleMenu)
  | ChangeContentForm(contentForm)
  | TogglePopover(bool);

let component = ReasonReact.reducerComponent("Schedule");

let make = (_children) => {
  ...component,
  initialState: () => {
    loadState: Idle, 
    scheduleMenu: All,
    contentForm: Idle,
    scheduleList: [],
    showPopover: false,
  },
  didMount: ({send}) => send(RequestSchedule),
  reducer: (action, state) => {
    switch (action) {
    | RequestSchedule => UpdateWithSideEffects({...state, loadState: Loading}, (_ => ()))
    | RequestScheduleSuccess(scheduleList) => Update({...state, loadState: Succeed, scheduleList})
    | RequestScheduleFail => Update({...state, loadState: Failed})
    | ChangeScheduleMenu(scheduleMenu) => Update({...state, scheduleMenu})
    | ChangeContentForm(contentForm) => Update({...state, contentForm})
    | TogglePopover(showPopover) => Update({...state, showPopover})
    };
  },
  render: ({state, send}) => {
    <div 
      className="container-fluid schedule-container"
      style=(ReactDOMRe.Style.make(
          ~backgroundColor=(switch state.contentForm {
          | Idle => ""
          | RequestForm => "#E6E6E6"
          | MyForm => "#E6E6E6"
          }), 
        ()))
    >
      <div className="row">
        <div className="col-6">
          <div 
            className="cursor-pointer" 
            onClick=(_ => {
              switch state.contentForm {
              | Idle => send(ChangeContentForm(MyForm))
              | _ => send(ChangeContentForm(Idle))
              };
            })
          >
            <img 
              className="form-icon mr-2" 
              src={
                switch state.contentForm {
                | Idle => "/images/form.svg"
                | _ => "/images/arrow_back.svg"
                }
              } 
            />
            <span>
              {
                switch state.contentForm {
                | Idle => string("My Form")
                | _ => string("Back")
                }
              }
            </span>
          </div>
        </div>
        <div className="col-6 text-right">
          <span className="cursor-pointer" onClick=(_ => send(TogglePopover(!state.showPopover))) >{string("Nameeeeee  e ")}</span>
          (state.showPopover ? <Popover onClose=(_ => send(TogglePopover(false))) /> : null)
        </div>
      </div>
      {
        switch state.contentForm {
        | Idle => 
          <>
            <div className="row pt-4">
              {
                menus |> List.mapi((i, menu:scheduleMenu) => {
                  <div key=("menu-" ++ (menu |> RememberMeUtils.scheduleMenuToStr)) className="col-3 p-2 text-center">
                    <button 
                      type_="button" 
                      className=("btn btn-rounded" ++ (state.scheduleMenu === menu ? " btn-main-color active-menu " : " none-active-menu"))  
                      onClick=(_ => send(ChangeScheduleMenu(menu)))
                    >
                      {string(menu |> RememberMeUtils.scheduleMenuToStr)}
                    </button>
                  </div>
                }) |> Array.of_list |> array
              }
              {
                /* Filter */
                switch state.scheduleList {
                | x when x === [] => {
                  <>
                    <SchedulerDate />
                    <SchedulerDate />
                    <SchedulerDate />
                    <SchedulerDate />
                    <SchedulerDate />
                    </>
                  }
                | _ => 
                  <div className="col-12 text-center pt-5 pb-5">
                    <p>{string("Scheduler not found")}</p>
                  </div>
                };
              }
            </div>
            <button type_="button" className="btn btn-request" onClick=(_ => send(ChangeContentForm(RequestForm)))>
              <img className="requestform-icon" src="/images/request_form.svg" />
              {string("Request Form")}
            </button>
          </>
        | RequestForm => <RequestForm />
        | MyForm => <RequestLeavePanel />
        };
      }
      
    </div>;
  },
};
