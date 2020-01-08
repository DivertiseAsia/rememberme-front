open RequestUtils;

type profile = {
  username: string,
  email: string,
  firstName: string,
  lastName: string,
  birthDate: Js.Date.t
};

type holiday = {
  name: string,
  date: float,
  isVacation: bool,
};
type leaveType =
  | Sick
  | Personal;

type leaveDetail = {
  id: string,
  user:string,
  leaveType,
  fromDate: Js.Date.t,
  toDate: Js.Date.t,
  reason: string,
  status: RememberMeType.requestStatus,
};

type birthDay = {
  name: string,
  birthDate: Js.Date.t,
};
//   "rid": "9094c152-9f70-4c46-88a4-e3edc5443225",
//   "user": {
//     "username": "dev",
//     "email": "dev@divertise.asia",
//     "first_name": "Dev",
//     "last_name": "Divertise",
//     "birth_date": "1997-06-12"
//   },
//   "type": 1,
//   "from_date": "2019-06-27",
//   "to_date": "2019-06-27",
//   "reason": "Trust me, I'm sick.",
//   "is_approved": true
let baseApiUrl = "https://rememberme-server.herokuapp.com";

module URL = {
  let login = {j|$baseApiUrl/account/login/|j};
  let signUp = {j|$baseApiUrl/account/register/|j};
  let password = {j|$baseApiUrl/password/change/|j};
  let profile = {j|$baseApiUrl/account/profile/|j};
  let birthday = {j|$baseApiUrl/account/birthday/|j};
  let leave = {j|$baseApiUrl/leave/|j};
  let allLeave = {j|$baseApiUrl/leave/all/|j};
  let holiday = {j|$baseApiUrl/holiday/|j};
};

let mapLeaveType = typeInt =>
  switch (typeInt) {
  | 1 => Sick
  | 0
  | _ => Personal
  };
let getDateOnlyDate = date => {
  Js.Date.makeWithYMD(
    ~year={date |> Js.Date.getFullYear;},
    ~month={date |> Js.Date.getMonth;},
    ~date={date |> Js.Date.getDate;},
    (),
  );
};

let mapLeaveTypeStr = typeInt =>
  switch (typeInt) {
  | Sick => "Sick"
  | Personal => "Personal"
  };
let mapRequestStatus = (requestStatus:int) => {
  open RememberMeType;
    switch requestStatus {
    | 2 => Pending
    | 0 => Fail
    | _ => Approve
    }
  };

module Decode = {
  open Json.Decode;

  let birthDay = json => {
    name: json |> field("name", optional(string)) |> Utils.mapOptStr,
    birthDate: json |> field("birth_date", optional(string)) |> Utils.mapOptStr |> Js.Date.fromString |> getDateOnlyDate,
  };
  let birthDayList = json => json |> list(birthDay);

  let holiday = json => {
    name: json |> field("name", string),
    date: json |> field("date", string) |> Js.Date.fromString |> getDateOnlyDate |> Js.Date.valueOf,
    isVacation: json |> field("is_vacation", bool),
  };
  /*json |> field("date", withDefault(true, nullAs(0. |> ))), */
  let holidayList = json => json |> list(holiday);

  let leaveDetail = json => {
    id: json |> field("rid", string),
    user: json |> field("user", string),
    leaveType: json |> field("type", int) |> mapLeaveType,
    fromDate: json |> field("from_date", string) |> Js.Date.fromString |> getDateOnlyDate,
    toDate: json |> field("to_date", string) |> Js.Date.fromString |> getDateOnlyDate,
    reason: json |> field("reason", string),
    status: json |> field("status", int) |> mapRequestStatus,
  };
  let leaveList = json => json |> list(leaveDetail);

  let profile = json => {
    username: json |> field("username", string),
    email: json |> field("email", string),
    firstName: json |> field("first_name", string),
    lastName: json |> field("last_name", string),
    birthDate: json |> field("birth_date", string) |> Js.Date.fromString |> getDateOnlyDate,
  };
};

let fetchProfile = (~token,~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.profile,
    ~successAction=json => json |> Decode.profile |> successAction,
    ~failAction,
  )
  |> ignore;

let postLeave = (~token, ~payload, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Post, ~body=payload, token),
    ~url=URL.leave,
    ~successAction,
    ~failAction,
  )
  |> ignore;

let removeRequestLeave = (~token, ~id,  ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Delete, token),
    ~url=(URL.leave ++ id ++ "/"),
    ~successAction,
    ~failAction,
  )
  |> ignore;

let fetchUserLeaves = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.leave,
    ~successAction=json => json |> Decode.leaveList |> successAction,
    ~failAction,
  )
  |> ignore;

let fetchAllLeaves = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.allLeave,
    ~successAction=json => json |> Decode.leaveList |> successAction,
    ~failAction,
  )
  |> ignore;

let fetchHoliday = (~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(None),
    ~url=URL.holiday,
    ~successAction=json => json |> Decode.holidayList |> successAction,
    ~failAction,
  )
  |> ignore;

let fetchBirthDay = (~token, ~successAction, ~failAction) =>
  requestJsonResponseToAction(
    ~headers=buildHeader(token),
    ~url=URL.birthday,
    ~successAction=json => json |> Decode.birthDayList |> successAction,
    ~failAction,
  )
  |> ignore;
