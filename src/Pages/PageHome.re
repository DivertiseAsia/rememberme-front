open RememberMeType;
open RememberMeApi;

let str = ReasonReact.string;
type birthDay = RememberMeApi.birthDay;
type holidayApiState = apiState(list(holiday));
type birthDayApiState = apiState(list(birthDay));
type leaveListApiState = apiState(list(leaveDetail));
type userLeaveListApiState = apiState(list(leaveDetail));
type eventListApiState = apiState(list(event));
type profileApiState = apiState(profile);

type state = {
  profileApiState,
  showRequestLeave: bool,
  showRequestUserLeave: bool,
  holidayApiState,
  birthDayApiState,
  leaveListApiState,
  eventListApiState,
  userLeaveListApiState,
};

type action =
  | ToggleRequestLeave
  | ToggleRequestUserLeave
  | SetProfileApiState(profileApiState)
  | SetHolidayApiState(holidayApiState)
  | SetBirthDayApiState(birthDayApiState)
  | SetLeaveListApiState(leaveListApiState)
  | SetEventListApiState(eventListApiState)
  | SetUserLeaveListApiState(userLeaveListApiState);

let initialState = {
  profileApiState: NotLoaded,
  showRequestLeave: false,
  showRequestUserLeave: false,
  holidayApiState: NotLoaded,
  birthDayApiState: NotLoaded,
  leaveListApiState: NotLoaded,
  eventListApiState: NotLoaded,
  userLeaveListApiState: NotLoaded,
};

let fetchHoliday = dispatch => {
  Loading->SetHolidayApiState->dispatch;
  fetchHoliday(
    ~successAction=
      holidayList => holidayList->Loaded->SetHolidayApiState->dispatch,
    ~failAction=
      json => json->Json.stringify->Failed->SetHolidayApiState->dispatch,
  );
};

let fetchBirthDay = dispatch => {
  Loading->SetBirthDayApiState->dispatch;
  fetchBirthDay(
    ~token=Utils.getToken(),
    ~successAction=
      birthdayList => birthdayList->Loaded->SetBirthDayApiState->dispatch,
    ~failAction=
      json => json->Json.stringify->Failed->SetBirthDayApiState->dispatch,
  );
};

let fetchEvent = dispatch => {
  Loading->SetEventListApiState->dispatch;
  fetchEvent(
    ~token=Utils.getToken(),
    ~successAction=
      eventList => eventList->Loaded->SetEventListApiState->dispatch,
    ~failAction=
      json => json->Json.stringify->Failed->SetEventListApiState->dispatch,
  );
};

let fetchAllRequestLeave = dispatch => {
  Loading->SetLeaveListApiState->dispatch;
  fetchAllLeaves(
    ~token=Utils.getToken(),
    ~successAction=
      leaveList => leaveList->Loaded->SetLeaveListApiState->dispatch,
    ~failAction=
      json => json->Json.stringify->Failed->SetLeaveListApiState->dispatch,
  );
};

let fetchUserRequestLeaveList = dispatch => {
  Loading->SetUserLeaveListApiState->dispatch;
  fetchUserLeaves(
    ~token=Utils.getToken(),
    ~successAction=
      leaveList => leaveList->Loaded->SetUserLeaveListApiState->dispatch,
    ~failAction=
      json => json->Json.stringify->Failed->SetUserLeaveListApiState->dispatch,
  );
};

let fetchProfile = dispatch => {
  Loading->SetProfileApiState->dispatch;
  fetchProfile(
    ~token=Utils.getToken(),
    ~successAction=profile => profile->Loaded->SetProfileApiState->dispatch,
    ~failAction=
      json => json->Json.stringify->Failed->SetProfileApiState->dispatch,
  );
};

[@react.component]
let make =
    (
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
        | SetProfileApiState(profileApiState) => {...state, profileApiState}
        | SetHolidayApiState(holidayApiState) => {...state, holidayApiState}
        | SetBirthDayApiState(birthDayApiState) => {
            ...state,
            birthDayApiState,
          }
        | SetLeaveListApiState(leaveListApiState) => {
            ...state,
            leaveListApiState,
          }
        | SetEventListApiState(eventListApiState) => {
            ...state,
            eventListApiState,
          }
        | SetUserLeaveListApiState(userLeaveListApiState) => {
            ...state,
            userLeaveListApiState,
          }
        }
      },
      initialState,
    );

  React.useEffect0(() => {
    fetchProfile(dispatch);
    fetchHoliday(dispatch);
    fetchBirthDay(dispatch);
    fetchAllRequestLeave(dispatch);
    fetchEvent(dispatch);
    fetchUserRequestLeaveList(dispatch);
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
          holidayList={state.holidayApiState->RememberMeUtils.getListFromState}
          listBirthDay={
            state.birthDayApiState->RememberMeUtils.getListFromState
          }
          leaveList={state.leaveListApiState->RememberMeUtils.getListFromState}
          eventList={state.eventListApiState->RememberMeUtils.getListFromState}
        />
      </div>
      <div
        className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-4 p-0  col-schedule">
        <Schedule
          profile={
            state.profileApiState->RememberMeUtils.getOptionDataFromState
          }
          holidayList={state.holidayApiState->RememberMeUtils.getListFromState}
          listBirthDay={
            state.birthDayApiState->RememberMeUtils.getListFromState
          }
          leaveList={state.leaveListApiState->RememberMeUtils.getListFromState}
          userLeaveList={
            state.userLeaveListApiState->RememberMeUtils.getListFromState
          }
          eventList={state.eventListApiState->RememberMeUtils.getListFromState}
          onRefresh={_ => {
            fetchAllRequestLeave(dispatch);
            fetchUserRequestLeaveList(dispatch);
          }}
        />
      </div>
    </div>
    <div className="error-status">
      state.profileApiState->RememberMeUtils.getErrorElFromState
      state.holidayApiState->RememberMeUtils.getErrorElFromState
      state.birthDayApiState->RememberMeUtils.getErrorElFromState
      state.eventListApiState->RememberMeUtils.getErrorElFromState
      state.leaveListApiState->RememberMeUtils.getErrorElFromState
      state.userLeaveListApiState->RememberMeUtils.getErrorElFromState
    </div>
  </div>;
};