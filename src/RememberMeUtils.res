open RememberMeType

let scheduleMenuToStr = scheduleMenu =>
  switch scheduleMenu {
  | Leave => "Leave"
  | Holiday => "Holiday"
  | Event => "Event"
  | _ => "All"
  }
let strToscheduleMenu = (scheduleMenu: string) =>
  switch scheduleMenu {
  | "Leave" => Leave
  | "Holiday" => Holiday
  | "Event" => Event
  | _ => All
  }

let requestMenuToStr = formType =>
  switch formType {
  | Sick => "Sick"
  | _ => "Vacation"
  }
let strToRequestMenu = (formType: string) =>
  switch formType {
  | "Sick" => Sick
  | _ => Vacation
  }

let formStatusToStr = requestStatus =>
  switch requestStatus {
  | Pending => "Pending"
  | Fail => "Rejected"
  | _ => "Approved"
  }
let strToFormStatus = (requestStatus: string) =>
  switch requestStatus {
  | "Approved" => Pending
  | "Rejected" => Fail
  | _ => Approve
  }
let mapRequestStatus = (requestStatus: int) =>
  switch requestStatus {
  | 2 => Pending
  | 0 => Fail
  | _ => Approve
  }

let formMenuToStr = formMenu =>
  switch formMenu {
  | MyForm => "My Form"
  | _ => "History"
  }
let strToFormMenu = (formMenu: string) =>
  switch formMenu {
  | "My Form" => MyForm
  | _ => History
  }

let mapDayStr = day =>
  switch day {
  | Sun => "Sun"
  | Mon => "Mon"
  | Tue => "Tue"
  | Wed => "Wed"
  | Thu => "Thu"
  | Fri => "Fri"
  | Sat => "Sat"
  }
let mapMonthStr = month =>
  switch month {
  | Jan => "Jan"
  | Feb => "Feb"
  | Mar => "Mar"
  | Apr => "Apr"
  | May => "May"
  | Jun => "Jun"
  | Jul => "Jul"
  | Aug => "Aug"
  | Sep => "Sep"
  | Oct => "Oct"
  | Nov => "Nov"
  | Dec => "Dec"
  }

let mapFullMonthStr = month =>
  switch month {
  | Jan => "January"
  | Feb => "February"
  | Mar => "March"
  | Apr => "April"
  | May => "May"
  | Jun => "June"
  | Jul => "July"
  | Aug => "August"
  | Sep => "September"
  | Oct => "October"
  | Nov => "November"
  | Dec => "December"
  }

let mapDayInt = (day: int) =>
  switch day {
  | 0 => "Sun"
  | 1 => "Mon"
  | 2 => "Tue"
  | 3 => "Wed"
  | 4 => "Thu"
  | 5 => "Fri"
  | 6 => "Sat"
  | _ => "N/A"
  }
let mapMonthInt = (month: int) =>
  switch month {
  | 0 => "Jan"
  | 1 => "Feb"
  | 2 => "Mar"
  | 3 => "Apr"
  | 4 => "May"
  | 5 => "Jun"
  | 6 => "Jul"
  | 7 => "Aug"
  | 8 => "Sep"
  | 9 => "Oct"
  | 10 => "Nov"
  | 11 => "Dec"
  | _ => "N/A"
  }

let mapFullMonthInt = (month: int) =>
  switch month {
  | 0 => "January"
  | 1 => "February"
  | 2 => "March"
  | 3 => "April"
  | 4 => "May"
  | 5 => "June"
  | 6 => "July"
  | 7 => "August"
  | 8 => "September"
  | 9 => "October"
  | 10 => "November"
  | 11 => "December"
  | _ => "N/A"
  }

let getDatetimeStr = (~formCurrentYear: bool=false, datetime: float) =>
  (datetime |> Js.Date.fromFloat |> Js.Date.getDay |> int_of_float |> mapDayInt) ++
    (" " ++
    ((datetime |> Js.Date.fromFloat |> Js.Date.getDate |> int_of_float |> string_of_int) ++
      (" " ++
      ((datetime |> Js.Date.fromFloat |> Js.Date.getMonth |> int_of_float |> mapFullMonthInt) ++
        (" " ++
        ((formCurrentYear ? Js.Date.now() : datetime)
        |> Js.Date.fromFloat
        |> Js.Date.getFullYear
        |> int_of_float
        |> string_of_int))))))

let getTwoPositionStr = (month: string) => String.length(month) === 1 ? "0" ++ month : month
let getDateStrRequestLeave = (datetime: float) =>
  (datetime |> Js.Date.fromFloat |> Js.Date.getFullYear |> int_of_float |> string_of_int) ++
    ("-" ++
    (((datetime |> Js.Date.fromFloat |> Js.Date.getMonth |> int_of_float) + 1
    |> string_of_int
    |> getTwoPositionStr) ++
      ("-" ++
      (datetime
      |> Js.Date.fromFloat
      |> Js.Date.getDate
      |> int_of_float
      |> string_of_int
      |> getTwoPositionStr))))

let mapHolidayToSchedule = (holiday: RememberMeApi.holiday) => {
  let schedule = {
    scheduleMenu: Holiday,
    title: holiday.name,
    date: holiday.date,
    detail: "",
  }
  schedule
}

let mapEventToSchedule = (event: RememberMeApi.event) => {
  let schedule = {
    scheduleMenu: Event,
    title: event.name,
    date: event.date |> Js.Date.valueOf,
    detail: event.details,
  }
  schedule
}
let mapLeaveToSchedule = (leaveDetail: RememberMeApi.leaveDetail) => {
  let schedule = {
    scheduleMenu: Leave,
    title: (leaveDetail.isRemote ? "[Remote] " : "") ++
    (leaveDetail.user ++
    (leaveDetail.leaveType === Sick ? " Sick" : " Vacation")),
    date: leaveDetail.fromDate |> Js.Date.valueOf,
    detail: "",
  }
  schedule
}

let splitRequestLeave = (leaveDetail: RememberMeApi.leaveDetail) => {
  let dateFloat =
    (leaveDetail.toDate |> Js.Date.valueOf) -. (leaveDetail.fromDate |> Js.Date.valueOf)
  switch dateFloat {
  | x when x === 0. => list{leaveDetail |> mapLeaveToSchedule}
  | _ =>
    Array.make(dateFloat /. (1000. *. 60. *. 60. *. 24.) +. 1. |> int_of_float, ReasonReact.null)
    |> Array.mapi((idx, _) => {
      let date =
        (leaveDetail.fromDate |> Js.Date.valueOf) +.
          1000. *. 60. *. 60. *. 24. *. (idx |> float_of_int)
      let schedule = {
        scheduleMenu: Leave,
        title: leaveDetail.user ++ (leaveDetail.leaveType === Sick ? " Sick" : " Vacation"),
        date: date,
        detail: "",
      }
      schedule
    })
    |> Array.to_list
  }
}

let mapBirthDayToSchedule = (birthday: RememberMeApi.birthDay) => {
  let schedule = {
    scheduleMenu: Birthday,
    title: birthday.name ++ "'s birthday",
    date: Js.Date.makeWithYMD(
      ~year=Js.Date.now() |> Js.Date.fromFloat |> Js.Date.getFullYear,
      ~month=birthday.birthDate |> Js.Date.getMonth,
      ~date=birthday.birthDate |> Js.Date.getDate,
      (),
    ) |> Js.Date.valueOf,
    detail: "",
  }
  schedule
}

let validateBirthday = (birthday: Js.Date.t, month, date) =>
  birthday |> Js.Date.getMonth === month && birthday |> Js.Date.getDate === date

let setStrToJsDate = (datetime: string) => {
  /* xxxx-xx-xx */
  let datetimeSplited = Js.String.split("-", datetime)
  Js.Date.makeWithYMD(
    ~year=datetimeSplited[0] |> float_of_string,
    ~month=datetimeSplited[1] |> float_of_string,
    ~date=datetimeSplited[2] |> float_of_string,
    (),
  )
}

let validateWeekend = (datetime: Js.Date.t) => {
  let day = datetime |> Js.Date.getDay
  day === 0. || day === 6.
}

let getListFromState = apiState =>
  switch apiState {
  | Loaded(listData) => listData
  | _ => list{}
  }

let getOptionDataFromState = apiState =>
  switch apiState {
  | Loaded(data) => Some(data)
  | _ => None
  }

let getErrorElFromState = apiState =>
  switch apiState {
  | Failed(jsonStr) => jsonStr->Utils.getErrorMsgFromJson->React.array
  | _ => React.null
  }

let doActionIfNotLoaded = (apiState, action) =>
  switch apiState {
  | Loading
  | Loaded(_) => ()
  | _ => action()
  }
