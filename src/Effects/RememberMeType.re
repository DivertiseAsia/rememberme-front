
type scheduleMenu = 
  | Leave
  | Holiday
  | Event
  | Birthday
  | All;

type formType = 
  | Sick 
  | Vacation;

type formStatus = 
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
  title:string,
  date: float,
};


