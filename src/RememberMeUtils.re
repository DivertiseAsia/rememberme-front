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
