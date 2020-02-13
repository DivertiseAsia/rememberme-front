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
  | Failed;

type schedule = {
  scheduleMenu,
  title: string,
  date: float,
  detail: string,
};