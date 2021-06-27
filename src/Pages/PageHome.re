open RememberMeType;
open RememberMeApi;

let str = ReasonReact.string;
type birthDay = RememberMeApi.birthDay;
type holidayApiState = apiState(list(holiday));
type birthDayApiState = apiState(list(birthDay));
type leaveListApiState = apiState(list(leaveDetail));
type userLeaveListApiState = apiState(list(leaveDetail));
type profileApiState = apiState(profile);

type state = {
  profileApiState,
  showRequestLeave: bool,
  showRequestUserLeave: bool,
  userLeaveListApiState,
};

type action =
  | ToggleRequestLeave
  | ToggleRequestUserLeave
  | SetProfileApiState(profileApiState)
  | SetUserLeaveListApiState(userLeaveListApiState);

let initialState = {
  profileApiState: NotLoaded,
  showRequestLeave: false,
  showRequestUserLeave: false,
  userLeaveListApiState: NotLoaded,
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
  let {
    events: {data: eventsApiState, fetchData: loadEvents},
    holidayList: {data: holidayListApiState, fetchData: loadHolidayList},
    birthDayList: {data: birthDayListApiState, fetchData: loadBirthdayList},
    allLeaveList: {
      data: leaveListApiState,
      fetchData: loadLeaveList,
      forceFetchData: forceLoadLeaveList,
    },
  } =
    DaysContext.useDaysResults();

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
        | SetUserLeaveListApiState(userLeaveListApiState) => {
            ...state,
            userLeaveListApiState,
          }
        }
      },
      initialState,
    );

  React.useEffect0(() => {
    loadEvents();
    loadHolidayList();
    loadBirthdayList();
    loadLeaveList();
    fetchProfile(dispatch);
    //    fetchAllRequestLeave(dispatch);
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
          holidayList={holidayListApiState->RememberMeUtils.getListFromState}
          listBirthDay={birthDayListApiState->RememberMeUtils.getListFromState}
          leaveList={leaveListApiState->RememberMeUtils.getListFromState}
          eventList={eventsApiState->RememberMeUtils.getListFromState}
        />
      </div>
      <div
        className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-4 p-0  col-schedule">
        <Schedule
          profile={
            state.profileApiState->RememberMeUtils.getOptionDataFromState
          }
          holidayList={holidayListApiState->RememberMeUtils.getListFromState}
          listBirthDay={birthDayListApiState->RememberMeUtils.getListFromState}
          leaveList={leaveListApiState->RememberMeUtils.getListFromState}
          userLeaveList={
            state.userLeaveListApiState->RememberMeUtils.getListFromState
          }
          eventList={eventsApiState->RememberMeUtils.getListFromState}
          onRefresh={_ => {
            forceLoadLeaveList();
            fetchUserRequestLeaveList(dispatch);
          }}
        />
      </div>
    </div>
    <div className="error-status">
      eventsApiState->RememberMeUtils.getErrorElFromState
      state.profileApiState->RememberMeUtils.getErrorElFromState
      holidayListApiState->RememberMeUtils.getErrorElFromState
      birthDayListApiState->RememberMeUtils.getErrorElFromState
      leaveListApiState->RememberMeUtils.getErrorElFromState
      state.userLeaveListApiState->RememberMeUtils.getErrorElFromState
    </div>
  </div>;
};