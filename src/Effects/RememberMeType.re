
type scheduleMenu = 
  | Leave
  | Holiday
  | Event
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


