open ReasonReact;
open RememberMeType;
open RememberMeApi;

let str = ReasonReact.string;

let months = [0,1,2,3,4,5,6,7,8,9,10,11];

let years = [
  (Js.Date.make() |> Js.Date.getFullYear) -. 2.,
  (Js.Date.make() |> Js.Date.getFullYear) -. 1.,
  (Js.Date.make() |> Js.Date.getFullYear),
  (Js.Date.make() |> Js.Date.getFullYear) +. 1.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 2.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 3.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 4.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 5.,
];

type state = {
  loadState,
  openDropdown: bool,
  targetYear: float,
  holidayList: list(holiday),
  listBirthDay: list(birthDay),
  leaveList: list(leaveDetail),
};

type action =
  | ToggleDropdown(bool)
  | ChangeYear(float)
  | FetchHolidayList
  | FetchHolidayListSuccess(list(holiday))
  | FetchHolidayListFail
  | FetchBirthDayList
  | FetchBirthDayListSuccess(list(birthDay))
  | FetchBirthDayListFail
  | FetchLeaveList
  | FetchLeaveListSuccess(list(leaveDetail))
  | FetchLeaveListFail;

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

let fetchAllRequestLeave = ({send}) => {
  fetchAllLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaveList => send(FetchLeaveListSuccess(leaveList)),
    ~failAction=_ => send(FetchLeaveListFail),
  );
};

let component = ReasonReact.reducerComponent("PageAllMonth");

let make = (_children) => {
  ...component,
  initialState: () => {
    loadState: Idle,
    openDropdown: false, 
    targetYear: (Js.Date.make() |> Js.Date.getFullYear),
    holidayList: [],
    listBirthDay: [],
    leaveList: [],
  },
  reducer: (action, state) => {
    switch (action) {
    | ToggleDropdown(openDropdown) => Update({...state, openDropdown})
    | ChangeYear(targetYear) => Update({...state, targetYear})
    | FetchHolidayList => UpdateWithSideEffects({...state, loadState: Loading}, fetchHolidayList)
    | FetchHolidayListSuccess(holidayList) => Update({...state, loadState: Succeed, holidayList})
    | FetchHolidayListFail => Update({...state, loadState: Failed, holidayList: []})
    | FetchBirthDayList => UpdateWithSideEffects({...state, loadState: Loading}, fetchBirthDay)
    | FetchBirthDayListSuccess(listBirthDay) => Update({...state, loadState: Succeed, listBirthDay})
    | FetchBirthDayListFail => Update({...state, loadState: Failed, listBirthDay: []})
    | FetchLeaveList => UpdateWithSideEffects({...state, loadState: Loading}, fetchAllRequestLeave)
    | FetchLeaveListSuccess(leaveList) => Update({...state, loadState: Succeed, leaveList})
    | FetchLeaveListFail => Update({...state, loadState: Failed, leaveList: []})
    };
  },
  didMount: ({send}) => {
    send(FetchHolidayList);
    send(FetchBirthDayList);
    send(FetchLeaveList);
  },
  render: ({state, send}) =>
    <div className="allmonth-page container-fluid">
      <div className="row">
        <div className="col mb-5 mt-4">
          <button 
            type_="button" 
            className="btn btn-rounded btn-main-color active-menu pl-4 pr-4 dropwdown-year"
            onClick=(_ => send(ToggleDropdown(!state.openDropdown)))
          >
            <img src="/images/calendar.svg" style=(ReactDOMRe.Style.make(~width="35px", ~height="35px", ())) />
            {string(" " ++ (state.targetYear |> Js.Float.toString))}
          </button>
          (state.openDropdown ?
            <>
            <div className="dropdown-items-bg" onClick=(_ => send(ToggleDropdown(false))) />
            <div className="dropdown-items">
              {
                years |> List.mapi((i, year) => {
                  <div className="dropdown-item" 
                    onClick=(_ => {
                      send(ChangeYear(year));
                      send(ToggleDropdown(false));
                    })
                  >
                    {string(year |> int_of_float |> string_of_int)}
                  </div>
                }) |> Array.of_list |> array
              }
            </div>
            </> : null
          )
        </div>
      </div>
      <div className="row mt-2">
        {
          months |> List.mapi((i, month) => {
            <div key=("month-" ++ (i |> string_of_int)) className="col-12 col-sm-12 col-md-6 col-lg-4 col-xl-4 mb-3"> 
              <Calendar 
                isMini=true 
                month=month 
                year=state.targetYear 
                holidayList=state.holidayList 
                listBirthDay=state.listBirthDay 
                leaveList=state.leaveList 
              />
            </div>
          }) |> Array.of_list |> array
        }
      </div>
    </div>,
};
