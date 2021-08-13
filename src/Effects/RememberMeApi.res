open RequestUtils

type profile = {
  username: string,
  email: string,
  firstName: string,
  lastName: string,
  birthDate: Js.Date.t,
}

type holiday = {
  name: string,
  date: float,
  isVacation: bool,
}

type event = {
  name: string,
  date: Js.Date.t,
  details: string,
}
type leaveType =
  | Sick
  | Personal

type leaveDetail = {
  id: string,
  user: string,
  leaveType: leaveType,
  fromDate: Js.Date.t,
  toDate: Js.Date.t,
  reason: string,
  isRemote: bool,
  status: RememberMeType.requestStatus,
}

type birthDay = {
  name: string,
  birthDate: Js.Date.t,
}

let baseApiUrl = "https://rememberme-server.herokuapp.com"

module URL = {
  let login = j`$baseApiUrl/account/login/`
  let signUp = j`$baseApiUrl/account/register/`
  let password = j`$baseApiUrl/password/change/`
  let resetPassword = j`$baseApiUrl/password/reset/`
  let profile = j`$baseApiUrl/account/profile/`
  let birthday = j`$baseApiUrl/account/birthday/`
  let leave = j`$baseApiUrl/leave/`
  let allLeave = j`$baseApiUrl/leave/all/`
  let holiday = j`$baseApiUrl/holiday/`
  let event = j`$baseApiUrl/event/`
}

let mapLeaveType = typeInt =>
  switch typeInt {
  | 1 => Sick
  | 0
  | _ =>
    Personal
  }
let getDateOnlyDate = date =>
  Js.Date.makeWithYMD(
    ~year=date |> Js.Date.getFullYear,
    ~month=date |> Js.Date.getMonth,
    ~date=date |> Js.Date.getDate,
    (),
  )

let mapLeaveTypeStr = typeInt =>
  switch typeInt {
  | Sick => "Sick"
  | Personal => "Personal"
  }
let mapRequestStatus = (requestStatus: int) => {
  open RememberMeType
  switch requestStatus {
  | 2 => Pending
  | 0 => Fail
  | _ => Approve
  }
}

module Decode = {
  open Json.Decode

  let birthDay = json => {
    name: json |> field("name", optional(string)) |> Utils.mapOptStr,
    birthDate: json
    |> field("birth_date", optional(string))
    |> Utils.mapOptStr
    |> Js.Date.fromString
    |> getDateOnlyDate,
  }
  let birthDayList = json => json |> list(birthDay)

  let holiday = json => {
    name: json |> field("name", string),
    date: json |> field("date", string) |> Js.Date.fromString |> getDateOnlyDate |> Js.Date.valueOf,
    isVacation: json |> field("is_vacation", bool),
  }
  let holidayList = json => json |> list(holiday)
  let event = json => {
    name: json |> field("name", string),
    date: json |> field("date", string) |> Js.Date.fromString |> getDateOnlyDate,
    details: json |> field("details", string),
  }
  /* json |> field("date", withDefault(true, nullAs(0. |> ))), */
  let eventList = json => json |> list(event)

  let leaveDetail = json => {
    id: json |> field("rid", string),
    user: json |> field("user", string),
    leaveType: json |> field("type", int) |> mapLeaveType,
    fromDate: json |> field("from_date", string) |> Js.Date.fromString |> getDateOnlyDate,
    toDate: json |> field("to_date", string) |> Js.Date.fromString |> getDateOnlyDate,
    reason: json |> field("reason", string),
    isRemote: json |> field("is_remote", bool),
    status: json |> field("status", int) |> mapRequestStatus,
  }
  let leaveList = json => json |> list(leaveDetail)

  let profile = json => {
    username: json |> field("username", string),
    email: json |> field("email", string),
    firstName: json |> field("first_name", string),
    lastName: json |> field("last_name", string),
    birthDate: json |> field("birth_date", string) |> Js.Date.fromString |> getDateOnlyDate,
  }
}

let fetchProfile = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.profile,
    ~successAction=json => json |> Decode.profile |> successAction,
    ~failAction,
  ) |> ignore

let postLeave = (~token, ~payload, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Post, ~body=payload, token),
    ~url=URL.leave,
    ~successAction,
    ~failAction,
  ) |> ignore

let removeRequestLeave = (~token, ~id, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Delete, token),
    ~url=URL.leave ++ (id ++ "/"),
    ~successAction,
    ~failAction,
  ) |> ignore

let fetchUserLeaves = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.leave,
    ~successAction=json => json |> Decode.leaveList |> successAction,
    ~failAction,
  ) |> ignore

let fetchAllLeaves = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.allLeave,
    ~successAction=json => json |> Decode.leaveList |> successAction,
    ~failAction,
  ) |> ignore

let fetchHoliday = (~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(None),
    ~url=URL.holiday,
    ~successAction=json => json |> Decode.holidayList |> successAction,
    ~failAction,
  ) |> ignore

let fetchBirthDay = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.birthday,
    ~successAction=json => json |> Decode.birthDayList |> successAction,
    ~failAction,
  ) |> ignore

let fetchEvent = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.event,
    ~successAction=json => json |> Decode.eventList |> successAction,
    ~failAction,
  ) |> ignore

let updateProfile = (~profile, ~successAction, ~failAction) => {

  let body = {
    open Json.Encode
    object_(list{
      ("first_name", profile.firstName |> Js.Json.string),
      ("last_name", profile.lastName |> Js.Json.string),
      ("birth_date", profile.birthDate |> Utils.Date.dateInputValue |> Js.Json.string),
    })
  }

  RequestUtils.requestJsonResponseToAction(
      ~headers=RequestUtils.buildHeader(
        ~verb=Patch,
        ~body,
        Utils.getToken(),
      ),
      ~url=URL.profile,
      ~successAction=_json => successAction(),
      ~failAction=json => json->Utils.getResponseMsgFromJson->failAction,
    ) |> ignore
}

let resetPassword = (~email, ~successAction, ~failAction) => {

  let body = {
    open Json.Encode
    object_(list{
      ("email", email |> Js.Json.string),
    })
  }

  RequestUtils.requestJsonResponseToAction(
      ~headers=RequestUtils.buildHeader(
        ~verb=Post,
        ~body,
        Utils.getToken(),
      ),
      ~url=URL.resetPassword,
      ~successAction=_json => successAction(),
      ~failAction=json => json->Utils.getResponseMsgFromJson->failAction,
    ) |> ignore
}