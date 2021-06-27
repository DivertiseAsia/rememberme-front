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
  | Dec;

type day =
  | Sun
  | Mon
  | Tue
  | Wed
  | Thu
  | Fri
  | Sat;

type scheduleMenu =
  | Leave
  | Holiday
  | Event
  | Birthday
  | All;

type formType =
  | Sick
  | Vacation;

type requestStatus =
  | Pending
  | Fail
  | Approve;

type formMenu =
  | MyForm
  | History;

type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed(string);

type apiState('a) =
  | NotLoaded
  | Loading
  | Loaded('a)
  | Failed(string);

type schedule = {
  scheduleMenu,
  title: string,
  date: float,
  detail: string,
};

module Encode = {
  let changePassword = (~oldPassword, ~password, ~confirmPassword) => {
    Json.Encode.(
      object_([
        ("old_password", oldPassword |> Js.Json.string),
        ("new_password", password |> Js.Json.string),
        ("confirm_password", confirmPassword |> Js.Json.string),
      ])
    );
  };
};