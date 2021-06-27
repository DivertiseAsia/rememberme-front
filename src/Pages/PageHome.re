open ReasonReact;
open RememberMeType;
open RememberMeApi;

let str = ReasonReact.string;
type birthDay = RememberMeApi.birthDay;

type state = {
  loadState,
  profile: option(profile),
  showRequestLeave: bool,
  showRequestUserLeave: bool,
  holidayList: list(holiday),
  listBirthDay: list(birthDay),
  leaveList: list(leaveDetail),
  eventList: list(event),
  userLeaveList: list(leaveDetail),
};

type action =
  | ToggleRequestLeave
  | ToggleRequestUserLeave
  | FetchHolidayList
  | FetchHolidayListSuccess(list(holiday))
  | FetchHolidayListFail
  | FetchBirthDayList
  | FetchBirthDayListSuccess(list(birthDay))
  | FetchBirthDayListFail
  | FetchLeaveList
  | FetchLeaveListSuccess(list(leaveDetail))
  | FetchLeaveListFail
  | FetchEventList
  | FetchEventListSuccess(list(event))
  | FetchEventListFail
  | FetchUserLeaveList
  | FetchUserLeaveListSuccess(list(leaveDetail))
  | FetchUserLeaveListFail
  | FetchProfile
  | FetchProfileSuccess(option(profile))
  | FetchProfileFail;

let initialState = {
  loadState: Idle,
  profile: None,
  showRequestLeave: false,
  showRequestUserLeave: false,
  holidayList: [],
  listBirthDay: [],
  leaveList: [],
  eventList: [],
  userLeaveList: [],
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

let fetchEvent = dispatch => {
  fetchEvent(
    ~token=Utils.getToken(),
    ~successAction=eventList => dispatch(FetchEventListSuccess(eventList)),
    ~failAction=_ => dispatch(FetchEventListFail),
  );
};

let fetchAllRequestLeave = dispatch => {
  fetchAllLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaveList => dispatch(FetchLeaveListSuccess(leaveList)),
    ~failAction=_ => dispatch(FetchLeaveListFail),
  );
};

let fetchUserRequestLeave = dispatch => {
  fetchUserLeaves(
    ~token=Utils.getToken(),
    ~successAction=
      leaveList => dispatch(FetchUserLeaveListSuccess(leaveList)),
    ~failAction=_ => dispatch(FetchUserLeaveListFail),
  );
};

let fetchProfile = (state, dispatch) => {
  fetchProfile(
    ~token=Utils.getToken(),
    ~successAction=profile => dispatch(FetchProfileSuccess(Some(profile))),
    ~failAction=_ => dispatch(FetchProfileFail),
  );
};

[@react.component]
let make =
    (
      ~isLoggedIn: bool,
      ~month=Js.Date.make() |> Js.Date.getMonth |> int_of_float,
      ~year=Js.Date.make() |> Js.Date.getFullYear,
    ) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | ToggleRequestLeave => {
            ...state,
            showRequestLeave: !state.showRequestLeave,
          }
        | ToggleRequestUserLeave => {
            ...state,
            showRequestUserLeave: !state.showRequestUserLeave,
          }
        | FetchHolidayList => {...state, loadState: Loading}
        //      , fetchHolidayList) TODO
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
        | FetchBirthDayList => {...state, loadState: Loading}
        //      , fetchBirthDay) TODO
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
        | FetchLeaveList => {...state, loadState: Loading}
        //        fetchAllRequestLeave, TODO
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
        | FetchEventList => {...state, loadState: Loading}
        //      , fetchEvent) TODO
        | FetchEventListSuccess(eventList) => {
            ...state,
            loadState: Succeed,
            eventList,
          }
        | FetchEventListFail => {
            ...state,
            loadState: Failed(""),
            eventList: [],
          }
        | FetchUserLeaveList =>
          //TODO
          {...state, loadState: Loading}
        //        fetchUserRequestLeave,
        | FetchUserLeaveListSuccess(userLeaveList) => {
            ...state,
            loadState: Succeed,
            userLeaveList,
          }
        | FetchUserLeaveListFail => {
            ...state,
            loadState: Failed(""),
            userLeaveList: [],
          }
        | FetchProfile =>
          // TODO:
          {...state, loadState: Loading}
        //      , fetchProfile)
        | FetchProfileSuccess(profile) => {
            ...state,
            loadState: Succeed,
            profile,
          }
        | FetchProfileFail => {
            ...state,
            loadState: Failed(""),
            profile: None,
          }
        }
      },
      initialState,
    );

  React.useEffect0(() => {
    dispatch(FetchProfile);
    dispatch(FetchHolidayList);
    dispatch(FetchBirthDayList);
    dispatch(FetchLeaveList);
    dispatch(FetchEventList);
    dispatch(FetchUserLeaveList);

    None;
  });
  <div className="home-page container-fluid">
    <div
      className="row row-main"
      style={ReactDOM.Style.make(~height="100%", ())}>
      <div
        className="col-12 col-sm-12 col-md-7 col-lg-8 col-xl-8 col-calendar">
        <Calendar
          month
          year
          holidayList={state.holidayList}
          listBirthDay={state.listBirthDay}
          leaveList={state.leaveList}
          eventList={state.eventList}
        />
      </div>
      <div
        className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-4 p-0  col-schedule">
        <Schedule
          profile={state.profile}
          holidayList={state.holidayList}
          listBirthDay={state.listBirthDay}
          leaveList={state.leaveList}
          userLeaveList={state.userLeaveList}
          eventList={state.eventList}
          onRefresh={_ => {
            dispatch(FetchLeaveList);
            dispatch(FetchUserLeaveList);
          }}
        />
      </div>
    </div>
  </div>;
};