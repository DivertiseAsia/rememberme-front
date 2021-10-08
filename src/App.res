open ReasonReact
open RememberMeType
open RememberMeType.Event
open RememberMeApi

@bs.val external encodeURIComponent: string => string = "encodeURIComponent"
@bs.get external location: Dom.window => Dom.location = "location"
@bs.get external href: Dom.location => string = "href"

type profileApiState = apiState<profile>
type eventsApiState = apiState<list<event>>
type holidayApiState = apiState<list<holiday>>
type birthDayApiState = apiState<list<birthDay>>
type allLeaveListApiState = apiState<list<leaveDetail>>
type userLeaveListApiState = apiState<list<leaveDetail>>

type state = {
  route: ReasonReact.Router.url,
  loadState: loadState,
  profileApiState: profileApiState,
  eventsApiState: eventsApiState,
  holidayApiState: holidayApiState,
  birthDayApiState: birthDayApiState,
  allLeaveListApiState: allLeaveListApiState,
  userLeaveListApiState: userLeaveListApiState,
}

type action =
  | RouteTo(Router.url)
  | SetProfileApiState(profileApiState)
  | SetAllLeaveListApiState(allLeaveListApiState)
  | SetUserLeaveListApiState(userLeaveListApiState)
  | SetEventsApiState(eventsApiState)
  | SetBirthDayApiState(birthDayApiState)
  | SetHolidayApiState(holidayApiState)

let initialState = {
  route: Router.dangerouslyGetInitialUrl(),
  loadState: Loading,
  profileApiState: NotLoaded,
  eventsApiState: NotLoaded,
  holidayApiState: NotLoaded,
  birthDayApiState: NotLoaded,
  allLeaveListApiState: NotLoaded,
  userLeaveListApiState: NotLoaded,
}

let loadToken = () =>
  switch {
    open Dom.Storage
    localStorage |> getItem("token")
  } {
  | None => ""
  | Some(token) => token
  }

let clearToken = () => {
  open Dom.Storage
  localStorage |> removeItem("token")
}

let path = () =>
  switch %external(window) {
  | None => ""
  | Some(window: Dom.window) => window |> location |> href
  }

let routeMatches = (x, link) => "/" ++ x == link

let fetchProfile = dispatch => {
  Loading->SetProfileApiState->dispatch
  fetchProfile(
    ~token=Utils.getToken(),
    ~successAction=profile => profile->Loaded->SetProfileApiState->dispatch,
    ~failAction=json => json->Json.stringify->Failed->SetProfileApiState->dispatch,
  )
}

let fetchEvents = dispatch => {
  Loading->SetEventsApiState->dispatch
  fetchEvent(
    ~token=Utils.getToken(),
    ~successAction=eventList => eventList->Loaded->SetEventsApiState->dispatch,
    ~failAction=json => json->Json.stringify->Failed->SetEventsApiState->dispatch,
  )
}

let fetchHoliday = dispatch => {
  Loading->SetHolidayApiState->dispatch
  fetchHoliday(
    ~successAction=holidayList => holidayList->Loaded->SetHolidayApiState->dispatch,
    ~failAction=json => json->Json.stringify->Failed->SetHolidayApiState->dispatch,
  )
}

let fetchBirthDay = dispatch => {
  Loading->SetBirthDayApiState->dispatch
  fetchBirthDay(
    ~token=Utils.getToken(),
    ~successAction=birthdayList => birthdayList->Loaded->SetBirthDayApiState->dispatch,
    ~failAction=json => json->Json.stringify->Failed->SetBirthDayApiState->dispatch,
  )
}
let fetchAllLeaveList = dispatch => {
  Loading->SetAllLeaveListApiState->dispatch
  fetchAllLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaveList => leaveList->Loaded->SetAllLeaveListApiState->dispatch,
    ~failAction=json => json->Json.stringify->Failed->SetAllLeaveListApiState->dispatch,
  )
}

let fetchUserRequestLeaveList = dispatch => {
  Loading->SetUserLeaveListApiState->dispatch
  fetchUserLeaves(
    ~token=Utils.getToken(),
    ~successAction=leaveList => leaveList->Loaded->SetUserLeaveListApiState->dispatch,
    ~failAction=json => json->Json.stringify->Failed->SetUserLeaveListApiState->dispatch,
  )
}
@react.component
let make = () => {
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | RouteTo(route) => {...state, route: route}
    | SetProfileApiState(profileApiState) => {...state, profileApiState: profileApiState}
    | SetEventsApiState(eventsApiState) => {...state, eventsApiState: eventsApiState}
    | SetBirthDayApiState(birthDayApiState) => {
        ...state,
        birthDayApiState: birthDayApiState,
      }
    | SetHolidayApiState(holidayApiState) => {...state, holidayApiState: holidayApiState}
    | SetAllLeaveListApiState(allLeaveListApiState) => {
        ...state,
        allLeaveListApiState: allLeaveListApiState,
      }
    | SetUserLeaveListApiState(userLeaveListApiState) => {
        ...state,
        userLeaveListApiState: userLeaveListApiState,
      }
    }
  , initialState)

  let token = loadToken()
  let isLoggedIn = token !== ""

  let fetchUserProfileIfNone = () =>
    state.profileApiState->RememberMeUtils.doActionIfNotLoaded(_ => fetchProfile(dispatch))

  let fetchEventsIfNone = () =>
    state.eventsApiState->RememberMeUtils.doActionIfNotLoaded(_ => fetchEvents(dispatch))

  let fetchHolidayIfNone = () =>
    state.eventsApiState->RememberMeUtils.doActionIfNotLoaded(_ => fetchHoliday(dispatch))

  let fetchBirthDayIfNone = () =>
    state.birthDayApiState->RememberMeUtils.doActionIfNotLoaded(_ => fetchBirthDay(dispatch))

  let fetchAllLeaveListIfNone = () =>
    state.allLeaveListApiState->RememberMeUtils.doActionIfNotLoaded(_ =>
      fetchAllLeaveList(dispatch)
    )

  let fetchUserRequestLeaveListIfNone = () =>
    state.userLeaveListApiState->RememberMeUtils.doActionIfNotLoaded(_ =>
      fetchUserRequestLeaveList(dispatch)
    )

  let daysContextValue: DaysContext.days = {
    events: {
      data: state.eventsApiState,
      fetchData: fetchEventsIfNone,
      forceFetchData: _ => fetchEvents(dispatch),
    },
    holidayList: {
      data: state.holidayApiState,
      fetchData: fetchHolidayIfNone,
      forceFetchData: _ => fetchHoliday(dispatch),
    },
    birthDayList: {
      data: state.birthDayApiState,
      fetchData: fetchBirthDayIfNone,
      forceFetchData: _ => fetchBirthDay(dispatch),
    },
    allLeaveList: {
      data: state.allLeaveListApiState,
      fetchData: fetchAllLeaveListIfNone,
      forceFetchData: _ => fetchAllLeaveList(dispatch),
    },
    userLeaveList: {
      data: state.userLeaveListApiState,
      fetchData: fetchUserRequestLeaveListIfNone,
      forceFetchData: _ => fetchUserRequestLeaveList(dispatch),
    },
  }

  React.useEffect0(_ => {
    let watcherID = Router.watchUrl(url => dispatch(RouteTo(url)))

    Some(_ => Router.unwatchUrl(watcherID))
  })

  React.useEffect1(_ => {
    if isLoggedIn {
      fetchEventsIfNone()
      fetchHolidayIfNone()
      fetchBirthDayIfNone()
      fetchAllLeaveListIfNone()
    }
    None
  }, [isLoggedIn])

  <ProfileContext.Provider
    value={(
      {data: state.profileApiState, fetchData: fetchUserProfileIfNone}: ProfileContext.contextValue<
        profileApiState,
      >
    )}>
    <DaysContext.Provider value=daysContextValue>
      {switch (state.route.path, isLoggedIn) {
      | (list{}, true)
      | (list{""}, true) =>
        <PageHome />
      | (list{x, monthYear}, true) when routeMatches(x, Links.dashboard) =>
        let datetime = Js.String.split("-", monthYear)
        <PageHome
          year={datetime[1] |> float_of_string} month={(datetime[0] |> int_of_string) - 1}
        />
      | (list{x}, true) when routeMatches(x, Links.profile) => <PageProfile />
      | (list{x}, true) when routeMatches(x, Links.allMonth) => <PageAllMonth />
      | (list{x}, _) when routeMatches(x, Links.login) =>
        <PageLogin queryString=state.route.search />
      | (list{x}, _) when routeMatches(x, Links.logout) =>
        let _ = clearToken()
        let _ = ReasonReact.Router.push("/?logout=true")
        <PageLogin queryString=state.route.search />
      | (list{x}, false) when routeMatches(x, Links.register) => <PageRegister />
      | (list{x}, false) when routeMatches(x, Links.forgot) => <PageForgotPassword />
      | (list{x}, false) when routeMatches(x, Links.resetPassword) => <PageResetPassword />
      | (_, false) =>
        let queryParams = "next=" ++ encodeURIComponent(path())
        let _ = ReasonReact.Router.push("/login?" ++ queryParams)
        <PageLogin queryString=queryParams />
      | _ => <Page404 isLoggedIn />
      }}
    </DaysContext.Provider>
  </ProfileContext.Provider>
}
