type month =
  | Jan
  | Feb
  | Mar
  | Apr
  | May
  | Jun
  | Jul
  | Aug
  | Sep
  | Oct
  | Nov
  | Dec

type day =
  | Sun
  | Mon
  | Tue
  | Wed
  | Thu
  | Fri
  | Sat

type scheduleMenu =
  | Leave
  | Holiday
  | Event
  | Birthday
  | All

type formType =
  | Sick
  | Vacation

type requestStatus =
  | Pending
  | Fail
  | Approve

type formMenu =
  | MyForm
  | History

type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed(string)

type apiState<'a> =
  | NotLoaded
  | Loading
  | Loaded('a)
  | Failed(string)

type schedule = {
  scheduleMenu: scheduleMenu,
  title: string,
  date: float,
  detail: string,
}

type event = {
  name: string,
  date: Js.Date.t,
  details: string,
}

type holiday = {
  name: string,
  date: float,
  date2: Js.Date.t,
  isVacation: bool,
}

module Encode = {
  let changePassword = (~oldPassword, ~password, ~confirmPassword) => {
    open Json.Encode
    object_(list{
      ("old_password", oldPassword |> Js.Json.string),
      ("new_password", password |> Js.Json.string),
      ("confirm_password", confirmPassword |> Js.Json.string),
    })
  }

  let getDateStrRequestLeave = (datetime: float) => {
    let getTwoPositionStr = (month: string) => String.length(month) === 1 ? "0" ++ month : month

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
  }

  let leaveRequest = (~formType, ~fromDate, ~toDate, ~reason, ~isRemote) => {
    open Json.Encode
    object_(list{
      ("type", (formType === Sick ? 1 : 0)->int),
      ("from_date", fromDate |> getDateStrRequestLeave |> Js.Json.string),
      ("to_date", toDate |> getDateStrRequestLeave |> Js.Json.string),
      ("reason", reason |> Js.Json.string),
      ("is_remote", isRemote |> Js.Json.boolean),
    })
  }
}
