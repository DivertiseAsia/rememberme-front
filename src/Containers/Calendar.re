open ReasonReact;
open RememberMeApi;

let str = ReasonReact.string;

type birthDay = RememberMeApi.birthDay;
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

let months = [Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec];
let days = [Mon, Tue, Wed, Thu, Fri, Sat, Sun];

let mapDayStr = day =>
  switch (day) {
  | Sun => "Sun"
  | Mon => "Mon"
  | Tue => "Tue"
  | Wed => "Wed"
  | Thu => "Thu"
  | Fri => "Fri"
  | Sat => "Sat"
  };
let mapMonthStr = month =>
  switch (month) {
  | Jan => "Jan"
  | Feb => "Feb"
  | Mar => "Mar"
  | Apr => "Apr"
  | May => "May"
  | Jun => "Jun"
  | Jul => "Jul"
  | Aug => "Aug"
  | Sep => "Sep"
  | Oct => "Oct"
  | Nov => "Nov"
  | Dec => "Dec"
  };
type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed;

type state = {
  loadState,
  month,
  year: float,
  holidayList: list(holiday),
  listBirthDay: list(birthDay),
};

type action =
  | FetchHolidayList
  | FetchHolidayListSuccess(list(holiday))
  | FetchHolidayListFail
  | FetchBirthDayList
  | FetchBirthDayListSuccess(list(birthDay))
  | FetchBirthDayListFail
  | NextMonth
  | PreviousMonth
  | OnChangeMonth(month);

let getMonthFloat = month => months |> Array.of_list |> Js.Array.indexOf(month) |> float_of_int;

let getMonthType = idx =>
  switch (Js.List.nth(months, idx |> int_of_float)) {
  | None => Jan
  | Some(month) => month
  };

let getLastDateObj = [%bs.raw {|
function f (year, month){
  var date = new Date(year, month, 0);
  return date
}
|}];

let getLastDate = (~year, ~realMonth) => getLastDateObj(year, realMonth) |> Js.Date.getDate;

let getDateOnlyDate = date => {
  Js.Date.makeWithYMD(
    ~year={
      date |> Js.Date.getFullYear;
    },
    ~month={
      date |> Js.Date.getMonth;
    },
    ~date={
      date |> Js.Date.getDate;
    },
    (),
  );
};

let mocks: list(holiday) = [
  {name: "DAY 1 ", date: "2019-06-01" |> Js.Date.fromString |> getDateOnlyDate |> Js.Date.valueOf, isVacation: true},
  {name: "DAY 2 ", date: "2019-06-27" |> Js.Date.fromString |> getDateOnlyDate |> Js.Date.valueOf, isVacation: true},
];

let getHolidayData = (holidayList, date) => {
  switch (
    holidayList
    |> List.find(vacation => {
         Js.log2("vacationDate", vacation.date |> Js.Date.fromFloat);
         vacation.date === date;
       })
  ) {
  | holiday => {Js.log2("holiday",holiday); Some(holiday)}
  | exception Not_found => None
  };
};

let getBirthDayElement = (birthList, date) => {
  switch (
    birthList
    |> List.find((data:birthDay) => {
          (data.birthDate |> getDateOnlyDate |> Js.Date.valueOf)  === date;
//false
       })
  ) {
  | data => Some(<p>{data.name ++ " BirthDay" |> str }</p>)
  | exception Not_found => None
  };
};

let dates = (month, year, holidayList, birthDayList) => {
  let col = 7;
  let row = 6;
  let boxs = col * row;
  let lastDate =
    getLastDate(
      ~year,
      ~realMonth={
        month +. 1.;
      },
    )
    |> int_of_float;
  let startDayInWeek = Js.Date.makeWithYM(~year, ~month, ()) |> Js.Date.getDay |> int_of_float;
  let emptyEl = key => <td key />;
  let today =
    Js.Date.make()
    |> (
      date =>
        Js.Date.makeWithYMD(
          ~year={
            date |> Js.Date.getFullYear;
          },
          ~month={
            date |> Js.Date.getMonth;
          },
          ~date={
            date |> Js.Date.getDate;
          },
          (),
        )
    )
    |> Js.Date.valueOf;
  Array.make(boxs, null)
  |> Array.mapi((idx, _) =>
       if (idx < startDayInWeek || idx - startDayInWeek + 1 > lastDate) {
         idx |> string_of_int |> emptyEl;
       } else {
         let date = idx - startDayInWeek + 1;
         let jsDate =
           Js.Date.makeWithYMD(
             ~year,
             ~month,
             ~date={
               date |> float_of_int;
             },
             (),
           )
           |> Js.Date.valueOf;
         let classThisDay =
           switch (jsDate) {
           | date when today === date => "today"
           | _ => ""
           };
         let (classHoliday, holidayName) =
           switch (getHolidayData(holidayList, jsDate)) {
           | None => ("", "")
           | Some(holiday) => (holiday.isVacation ? "vacation" : "not-vacation", holiday.name)
           };
           let (classBirthDay, birthDayEl) =
           switch (getBirthDayElement(birthDayList, jsDate)) {
           | None => ("", null)
           | Some(el) => ("birth-day", el)
           };


         <td key={idx |> string_of_int} className={j|day $classThisDay $classHoliday $classBirthDay|j}>
           {holidayName |> str}
           {birthDayEl}
           {date |> string_of_int |> str}
         </td>;
       }
     )
  |> (
    arr => {
      Array.make(row, null)
      |> Array.mapi((idx, _) => {
           let end_ = (idx + 1) * col;
           let start = idx > 0 ? idx * col : idx;
           <tr key={j|row-$idx|j}> {arr |> Js.Array.slice(~start, ~end_) |> array} </tr>;
         });
    }
  )
  |> array;
};

let fetchHolidayList = ({send}) => {
  fetchHoliday(
    ~successAction=holidayList => send(FetchHolidayListSuccess(holidayList)),
    ~failAction=_ => send(FetchHolidayListFail),
  );
};

let fetchBirthDay = ({send}) => {
  fetchBirthDay(
    ~successAction=holidayList => send(FetchBirthDayListSuccess(holidayList)),
    ~failAction=_ => send(FetchBirthDayListFail),
  );
};

let component = ReasonReact.reducerComponent("Calendar");

let make = _children => {
  ...component,
  initialState: () => {
    loadState: Idle,
    month: Js.Date.make() |> Js.Date.getMonth |> getMonthType,
    year: Js.Date.make() |> Js.Date.getFullYear,
    holidayList: [],
    listBirthDay: [],
  },
  didMount: ({send}) => {send(FetchHolidayList)send(FetchBirthDayList)},
  reducer: (action, state) => {
    switch (action) {
    | FetchHolidayList => UpdateWithSideEffects({...state, loadState: Loading}, fetchHolidayList)
    | FetchHolidayListSuccess(holidayList) => Update({...state, loadState: Succeed, holidayList})
    | FetchHolidayListFail => Update({...state, loadState: Failed, holidayList: []})
    | FetchBirthDayList => UpdateWithSideEffects({...state, loadState: Loading}, fetchBirthDay)
    | FetchBirthDayListSuccess(listBirthDay) => Update({...state, loadState: Succeed, listBirthDay})
    | FetchBirthDayListFail => Update({...state, loadState: Failed, listBirthDay: []})
    | NextMonth =>
      switch (state.month) {
      | Dec => Update({...state, month: Jan, year: state.year +. 1.})
      | _ =>
        Update({
          ...state,
          month: {
            state.month |> getMonthFloat |> (idx => idx +. 1.) |> getMonthType;
          },
        })
      }
    | PreviousMonth =>
      switch (state.month) {
      | Jan => Update({...state, month: Dec, year: state.year -. 1.})
      | _ =>
        Update({
          ...state,
          month: {
            state.month |> getMonthFloat |> (idx => idx -. 1.) |> getMonthType;
          },
        })
      }
    | OnChangeMonth(month) => Update({...state, month})
    };
  },
  render: ({state, send}) =>
    <div className="container">
      <div className="row">
        <button 
          type_="button" 
          className="btn btn-rounded btn-main-color active-menu pl-4 pr-4"
          onClick=(_ => ())
          
        >
          <img src="/images/calendar.svg" style=(ReactDOMRe.Style.make(~width="35px", ~height="35px", ())) />
          {string(" 2019")}
        </button>
      </div>
      <div className="row">
        <h1>
          <span className="icon-click" onClick={_ => send(PreviousMonth)}> {{j|<|j} |> str} </span>
          {state.month |> mapMonthStr |> str}
          <span className="icon-click" onClick={_ => send(NextMonth)}> {{j|>|j} |> str} </span>
        </h1>
        <h1 className="col"> {state.year |> Js.Float.toString |> str} </h1>
      </div>
      <table className="table">
        <thead>
          <tr> {days |> List.map(day => <th> {day |> mapDayStr |> str} </th>) |> Array.of_list |> array} </tr>
        </thead>
        <tbody> 
        {state.loadState === Loading ? 
          null:
          dates(state.month |> getMonthFloat, state.year, state.holidayList, state.listBirthDay)
        } 
        </tbody>
      </table>
    </div>,
};
