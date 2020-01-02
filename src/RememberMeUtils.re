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

let mapDayInt = (day:int) =>
  switch (day) {
  | 0 => "Sun"
  | 1 => "Mon"
  | 2 => "Tue"
  | 3 => "Wed"
  | 4 => "Thu"
  | 5 => "Fri"
  | 6 => "Sat"
  };
let mapMonthInt = (month:int) =>
  switch (month) {
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
  };

let mapFullMonthInt = (month:int) =>
  switch (month) {
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
  };

let getDatetimeStr = (~formCurrentYear=false, datetime:float) => {
  (datetime |> Js.Date.fromFloat |> Js.Date.getDay |> int_of_float |> mapDayInt) ++ " " ++
  (datetime |> Js.Date.fromFloat |> Js.Date.getDate |> int_of_float |> string_of_int) ++ " " ++
  (datetime |> Js.Date.fromFloat |> Js.Date.getMonth |> int_of_float |> mapFullMonthInt) ++ " " ++
  ((formCurrentYear ? Js.Date.now() : datetime) |> Js.Date.fromFloat |> Js.Date.getFullYear |> int_of_float |> string_of_int);
};

let mapHolidayToSchedule = (holiday:RememberMeApi.holiday) => {
  let schedule = {
    scheduleMenu: Holiday,
    title: holiday.name,
    date: holiday.date,
  };
  schedule
};

let mapBirthDayToSchedule = (birthday:RememberMeApi.birthDay) => {
  let schedule = {
    scheduleMenu: Birthday,
    title: birthday.name ++ "'s birthday",
    date: 
      Js.Date.makeWithYMD(
        ~year={
          Js.Date.now() |> Js.Date.fromFloat |> Js.Date.getFullYear;
        },
        ~month={
          birthday.birthDate  |> Js.Date.getMonth;
        },
        ~date={
          birthday.birthDate  |> Js.Date.getDate;
        }, ()) |> Js.Date.valueOf,
  };
  schedule
};

let mapLeaveToSchedule = (leaveDetail:RememberMeApi.leaveDetail, date) => {
  let schedule = {
    scheduleMenu: Leave,
    title: leaveDetail.user ++ (leaveDetail.leaveType === Sick ? " Sick" : " Vacation"),
    date,
  };
  schedule
};

let validateBirthday = (birthday:Js.Date.t, month, date) => {
  (birthday |> Js.Date.getMonth === month && birthday |> Js.Date.getDate === date)
};
