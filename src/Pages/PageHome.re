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

let fetchEvent = ({send}) => {
  fetchEvent(
    ~token=Utils.getToken(),
    ~successAction=eventList => send(FetchEventListSuccess(eventList)),
    ~failAction=_ => send(FetchEventListFail),
  );
};

let fetchAllRequestLeave = ({send}) => {
  fetchAllLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaveList => send(FetchLeaveListSuccess(leaveList)),
    ~failAction=_ => send(FetchLeaveListFail),
  );
};

let fetchUserRequestLeave = ({send}) => {
  fetchUserLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaveList => send(FetchUserLeaveListSuccess(leaveList)),
    ~failAction=_ => send(FetchUserLeaveListFail),
  );
};

let fetchProfile = ({state, send}) => {
  fetchProfile(
    ~token=Utils.getToken(),
    ~successAction=profile => send(FetchProfileSuccess(Some(profile))),
    ~failAction=_ => send(FetchProfileFail),
  );
};

let component = ReasonReact.reducerComponent("PageHome");

let make =
    (
      ~isLoggedIn: bool,
      ~month=Js.Date.make() |> Js.Date.getMonth |> int_of_float,
      ~year=Js.Date.make() |> Js.Date.getFullYear,
      _children,
    ) => {
  ...component,
  initialState: () => {
    loadState: Idle,
    profile: None,
    showRequestLeave: false,
    showRequestUserLeave: false,
    holidayList: [],
    listBirthDay: [],
    leaveList: [],
    eventList: [],
    userLeaveList: [],
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
    | FetchLeaveList => UpdateWithSideEffects({...state, loadState: Loading}, fetchAllRequestLeave)
    | FetchLeaveListSuccess(leaveList) => Update({...state, loadState: Succeed, leaveList})
    | FetchLeaveListFail => Update({...state, loadState: Failed, leaveList: []})
    | FetchEventList => UpdateWithSideEffects({...state, loadState: Loading}, fetchEvent)
    | FetchEventListSuccess(eventList) => Update({...state, loadState: Succeed, eventList})
    | FetchEventListFail => Update({...state, loadState: Failed, eventList: []})
    | FetchUserLeaveList => UpdateWithSideEffects({...state, loadState: Loading}, fetchUserRequestLeave)
    | FetchUserLeaveListSuccess(userLeaveList) => Update({...state, loadState: Succeed, userLeaveList})
    | FetchUserLeaveListFail => Update({...state, loadState: Failed, userLeaveList: []})
    | FetchProfile => UpdateWithSideEffects({...state, loadState: Loading}, fetchProfile)
    | FetchProfileSuccess(profile) => Update({...state, loadState: Succeed, profile})
    | FetchProfileFail => Update({...state, loadState: Failed, profile: None})
    };
  },
  didMount: ({send}) => {
    send(FetchProfile);
    send(FetchHolidayList);
    send(FetchBirthDayList);
    send(FetchLeaveList);
    send(FetchEventList);
    send(FetchUserLeaveList);
  },
  render: ({state, send}) =>
    <div className="home-page container-fluid">
      <div className="row row-main" style={ReactDOMRe.Style.make(~height="100%", ())}>
        <div className="col-12 col-sm-12 col-md-7 col-lg-8 col-xl-8 col-calendar">
          <Calendar
            month
            year
            holidayList={state.holidayList}
            listBirthDay={state.listBirthDay}
            leaveList={state.leaveList}
            eventList={state.eventList}
          />
        </div>
        <div className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-4 p-0  col-schedule">
          <Schedule
            profile={state.profile}
            holidayList={state.holidayList}
            listBirthDay={state.listBirthDay}
            leaveList={state.leaveList}
            userLeaveList={state.userLeaveList}
            eventList={state.eventList}
            onRefresh={_ => {
              send(FetchLeaveList);
              send(FetchUserLeaveList);
            }}
          />
        </div>
      </div>
    </div>,
};