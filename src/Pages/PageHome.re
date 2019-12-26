open ReasonReact;
open RememberMeType;
open RememberMeApi;

let str = ReasonReact.string;
type birthDay = RememberMeApi.birthDay;

type state = {
  loadState,
  showRequestLeave: bool,
  showRequestUserLeave: bool,
  holidayList: list(holiday),
  listBirthDay: list(birthDay),
};

type action =
  | ToggleRequestLeave
  | ToggleRequestUserLeave
  | FetchHolidayList
  | FetchHolidayListSuccess(list(holiday))
  | FetchHolidayListFail
  | FetchBirthDayList
  | FetchBirthDayListSuccess(list(birthDay))
  | FetchBirthDayListFail;

let fetchHolidayList = ({send}) => {
  fetchHoliday(
    ~successAction=holidayList => send(FetchHolidayListSuccess(holidayList)),
    ~failAction=_ => send(FetchHolidayListFail),
  );
};

let fetchBirthDay = ({send}) => {
  fetchBirthDay(
    ~token=Utils.getToken(),
    ~successAction=birthdayList => send(FetchBirthDayListSuccess(birthdayList)),
    ~failAction=_ => send(FetchBirthDayListFail),
  );
};

let component = ReasonReact.reducerComponent("PageHome");

let make = (
    ~isLoggedIn: bool, 
    ~month=(Js.Date.make() |> Js.Date.getMonth |> int_of_float),
    ~year=(Js.Date.make() |> Js.Date.getFullYear), 
    _children
  ) => {
  ...component,
  initialState: () => {
    loadState: Idle,
    showRequestLeave: false, 
    showRequestUserLeave: false,
    holidayList: [],
    listBirthDay: [],
  },
  reducer: (action, state) => {
    switch (action) {
    | ToggleRequestLeave => Update({...state, showRequestLeave: !state.showRequestLeave})
    | ToggleRequestUserLeave => Update({...state, showRequestUserLeave: !state.showRequestUserLeave})
    | FetchHolidayList => UpdateWithSideEffects({...state, loadState: Loading}, fetchHolidayList)
    | FetchHolidayListSuccess(holidayList) => Update({...state, loadState: Succeed, holidayList})
    | FetchHolidayListFail => Update({...state, loadState: Failed, holidayList: []})
    | FetchBirthDayList => UpdateWithSideEffects({...state, loadState: Loading}, fetchBirthDay)
    | FetchBirthDayListSuccess(listBirthDay) => Update({...state, loadState: Succeed, listBirthDay})
    | FetchBirthDayListFail => Update({...state, loadState: Failed, listBirthDay: []})
    };
  },
  didMount: ({send}) => {send(FetchHolidayList)send(FetchBirthDayList)},
  render: ({state, send}) =>
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
          <Schedule holidayList=state.holidayList listBirthDay=state.listBirthDay />
        </div>
      </div>
      /*{showRequestLeave ? <RequestLeave onClose={_ => send(ToggleRequestLeave)} /> : null}
      {showRequestUserLeave ? <UserLeave onClose={_ => send(ToggleRequestUserLeave)} /> : null}
      */
    </div>,
};
