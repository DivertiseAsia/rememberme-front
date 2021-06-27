open ReasonReact;
open RememberMeType;
open RememberMeApi;

let str = ReasonReact.string;

let months = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11];

let years = [
  (Js.Date.make() |> Js.Date.getFullYear) -. 2.,
  (Js.Date.make() |> Js.Date.getFullYear) -. 1.,
  Js.Date.make() |> Js.Date.getFullYear,
  (Js.Date.make() |> Js.Date.getFullYear) +. 1.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 2.,
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

let initialState = {
  loadState: Idle,
  openDropdown: false,
  targetYear: Js.Date.make() |> Js.Date.getFullYear,
  holidayList: [],
  listBirthDay: [],
  leaveList: [],
};

let fetchHolidayList = dispatch => {
  fetchHoliday(
    ~successAction=
      holidayList => dispatch(FetchHolidayListSuccess(holidayList)),
    ~failAction=_ => dispatch(FetchHolidayListFail),
  );
};

let fetchBirthDay = dispatch => {
  fetchBirthDay(
    ~token=Utils.getToken(),
    ~successAction=
      birthdayList => dispatch(FetchBirthDayListSuccess(birthdayList)),
    ~failAction=_ => dispatch(FetchBirthDayListFail),
  );
};

let fetchAllRequestLeave = dispatch => {
  fetchAllLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaveList => dispatch(FetchLeaveListSuccess(leaveList)),
    ~failAction=_ => dispatch(FetchLeaveListFail),
  );
};

[@react.component]
let make = () => {
  let (eventsApiState, loadEvents) = EventsContext.useEventsResults();

  let (state, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | ToggleDropdown(openDropdown) => {...state, openDropdown}
        | ChangeYear(targetYear) => {...state, targetYear}
        | FetchHolidayList =>
          // TODO REWRITE
          {...state, loadState: Loading}
        //      , fetchHolidayList)
        | FetchHolidayListSuccess(holidayList) => {
            ...state,
            loadState: Succeed,
            holidayList,
          }
        | FetchHolidayListFail => {
            ...state,
            loadState: Failed(""),
            holidayList: [],
          }
        | FetchBirthDayList =>
          // TODO REWRITE
          {...state, loadState: Loading}
        //      , fetchBirthDay)
        | FetchBirthDayListSuccess(listBirthDay) => {
            ...state,
            loadState: Succeed,
            listBirthDay,
          }
        | FetchBirthDayListFail => {
            ...state,
            loadState: Failed(""),
            listBirthDay: [],
          }
        | FetchLeaveList =>
          // TODO REWRITE
          {...state, loadState: Loading}
        //        fetchAllRequestLeave,
        | FetchLeaveListSuccess(leaveList) => {
            ...state,
            loadState: Succeed,
            leaveList,
          }
        | FetchLeaveListFail => {
            ...state,
            loadState: Failed(""),
            leaveList: [],
          }
        }
      },
      initialState,
    );

  React.useEffect0(_ => {
    dispatch(FetchHolidayList);
    dispatch(FetchBirthDayList);
    dispatch(FetchLeaveList);
    //    dispatch(FetchEventList);
    loadEvents();
    None;
  });

  <div className="allmonth-page container-fluid">
    <div className="row">
      <div className="col mb-5 mt-4">
        <button
          type_="button"
          className="btn btn-rounded btn-main-color active-menu pl-4 pr-4 dropwdown-year"
          onClick={_ => dispatch(ToggleDropdown(!state.openDropdown))}>
          <img
            src="/images/calendar.svg"
            style={ReactDOM.Style.make(~width="35px", ~height="35px", ())}
          />
          {string(" " ++ (state.targetYear |> Js.Float.toString))}
        </button>
        {state.openDropdown
           ? <>
               <div
                 className="dropdown-items-bg"
                 onClick={_ => dispatch(ToggleDropdown(false))}
               />
               <div className="dropdown-items">
                 {years
                  |> List.mapi((i, year) =>
                       <div
                         key={j|drowndown-item-$i-$year|j}
                         className="dropdown-item"
                         onClick={_ => {
                           dispatch(ChangeYear(year));
                           dispatch(ToggleDropdown(false));
                         }}>
                         {string(year |> int_of_float |> string_of_int)}
                       </div>
                     )
                  |> Array.of_list
                  |> array}
               </div>
             </>
           : null}
      </div>
    </div>
    <div className="row mt-2">
      {months
       |> List.mapi((i, month) =>
            <div
              key={"month-" ++ (i |> string_of_int)}
              className="col-12 col-sm-12 col-md-6 col-lg-4 col-xl-4 mb-3">
              <Calendar
                isMini=true
                month
                year={state.targetYear}
                holidayList={state.holidayList}
                listBirthDay={state.listBirthDay}
                leaveList={state.leaveList}
                eventList={eventsApiState->RememberMeUtils.getListFromState}
              />
            </div>
          )
       |> Array.of_list
       |> array}
    </div>
  </div>;
};