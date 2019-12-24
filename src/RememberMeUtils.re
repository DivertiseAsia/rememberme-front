open RememberMeType;

let scheduleMenuToStr = (scheduleMenu) => {
  switch scheduleMenu {
  | Leave => "Leave"
  | Holiday => "Holiday"
  | Event => "Event"
  | _ => "All"
  }
};
let strToscheduleMenu = (scheduleMenu:string) => {
  switch scheduleMenu {
  | "Leave" => Leave
  | "Holiday" => Holiday
  | "Event" => Event
  | _ => All
  }
};

let requestMenuToStr = (formType) => {
  switch formType {
  | Sick => "Sick"
  | _ => "Vacation"
  }
};
let strToRequestMenu = (formType:string) => {
  switch formType {
  | "Sick" => Sick
  | _ => Vacation
  }
};

let formStatusToStr = (formStatus) => {
  switch formStatus {
  | Pending => "Pending"
  | Fail => "Fail"
  | _ => "Approve"
  }
};
let strToFormStatus = (formStatus:string) => {
  switch formStatus {
  | "Sick" => Pending
  | "Fail" => Fail
  | _ => Approve
  }
};

let formMenuToStr = (formMenu) => {
  switch formMenu {
  | MyForm => "My Form"
  | _ => "History"
  }
};
let strToFormMenu = (formMenu:string) => {
  switch formMenu {
  | "My Form" => MyForm
  | _ => History
  }
};
