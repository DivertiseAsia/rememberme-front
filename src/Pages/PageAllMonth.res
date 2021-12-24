open RememberMeType

let str = React.string

let months = list{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}

let years = list{
  (Js.Date.make() |> Js.Date.getFullYear) -. 2.,
  (Js.Date.make() |> Js.Date.getFullYear) -. 1.,
  Js.Date.make() |> Js.Date.getFullYear,
  (Js.Date.make() |> Js.Date.getFullYear) +. 1.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 2.,
}

type state = {
  loadState: loadState,
  openDropdown: bool,
  targetYear: float,
}

type action =
  | ToggleDropdown(bool)
  | ChangeYear(float)

let initialState = {
  loadState: Idle,
  openDropdown: false,
  targetYear: Js.Date.make() |> Js.Date.getFullYear,
}

@react.component
let make = () => {
  let {
    events: {data: eventsApiState, fetchData: loadEvents},
    holidayList: {data: holidayListApiState, fetchData: loadHolidayList},
    birthDayList: {data: birthDayListApiState, fetchData: loadBirthdayList},
    allLeaveList: {data: leaveListApiState, fetchData: loadLeaveList},
  } = DaysContext.useDaysResults()

  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | ToggleDropdown(openDropdown) => {...state, openDropdown: openDropdown}
    | ChangeYear(targetYear) => {...state, targetYear: targetYear}
    }
  , initialState)

  React.useEffect0(_ => {
    loadBirthdayList()
    loadEvents()
    loadHolidayList()
    loadLeaveList()
    None
  })

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
          {React.string(" " ++ (state.targetYear |> Js.Float.toString))}
        </button>
        {state.openDropdown
          ? <>
              <div className="dropdown-items-bg" onClick={_ => dispatch(ToggleDropdown(false))} />
              <div className="dropdown-items">
                {years
                |> List.mapi((i, year) =>
                  <div
                    key=j`drowndown-item-$i-$year`
                    className="dropdown-item"
                    onClick={_ => {
                      dispatch(ChangeYear(year))
                      dispatch(ToggleDropdown(false))
                    }}>
                    {React.string(year |> int_of_float |> string_of_int)}
                  </div>
                )
                |> Array.of_list
                |> React.array}
              </div>
            </>
          : React.null}
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
            year=state.targetYear
            listBirthDay={birthDayListApiState->RememberMeUtils.getListFromState}
            leaveList={leaveListApiState->RememberMeUtils.getListFromState}
            eventList={eventsApiState->RememberMeUtils.getListFromState}
            holidayList={holidayListApiState->RememberMeUtils.getListFromState}
          />
        </div>
      )
      |> Array.of_list
      |> React.array}
    </div>
  </div>
}
