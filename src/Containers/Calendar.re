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

let mapFullMonthStr = month =>
  switch (month) {
  | Jan => "January"
  | Feb => "February"
  | Mar => "March"
  | Apr => "April"
  | May => "May"
  | Jun => "June"
  | Jul => "July"
  | Aug => "August"
  | Sep => "September"
  | Oct => "October"
  | Nov => "November"
  | Dec => "December"
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
  | OnChangeMonth(month)
  | OnChangeYear(float);

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
         /*Js.log2("vacationDate", vacation.date |> Js.Date.fromFloat);*/
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

let getDayByStartOnMonday = (dayInWeek:int) => {
  (dayInWeek === 0 ? 6 : dayInWeek - 1)
};

let dates = (month, year, holidayList, birthDayList) => {
  let col = 7;
  let row = 6;
  let boxs = col * row;
  let lastDatePreviousMonth = getLastDate(~year, ~realMonth=month) |> int_of_float;
  let lastDate = getLastDate(~year, ~realMonth={month +. 1.;}) |> int_of_float;
  let startDayInWeek = Js.Date.makeWithYM(~year, ~month, ()) |> Js.Date.getDay |> int_of_float |> getDayByStartOnMonday;
  let emptyEl = key => <td key />;
  let today =
    Js.Date.make()
    |> (date =>
        Js.Date.makeWithYMD(
          ~year=date |> Js.Date.getFullYear,
          ~month=date |> Js.Date.getMonth,
          ~date=date |> Js.Date.getDate,
          (),
        )
      ) |> Js.Date.valueOf;

  Array.make(boxs, null)
  |> Array.mapi((idx, _) =>
       if (idx < startDayInWeek) {
          <td key={idx |> string_of_int} className={j|day not-current-month|j}>
            {(lastDatePreviousMonth - (startDayInWeek - (idx + 1))) |> string_of_int |> str}
          </td>
       } else if (idx - startDayInWeek + 1 > lastDate) {
          <td key={idx |> string_of_int} className={j|day not-current-month|j}>
            {(idx - startDayInWeek + 1 - lastDate) |> string_of_int |> str}
          </td>
       } else {
         let date = idx - startDayInWeek + 1;
         let jsDate = Js.Date.makeWithYMD(~year,~month,~date={date |> float_of_int}, ()) |> Js.Date.valueOf;
         let classThisDay =
           switch (jsDate) {
           | date when today === date => "today"
           | _ => ""
           };
          
         <td key={idx |> string_of_int} className={j|day $classThisDay|j}>
           <div className="circle-today" />
           <span className=(String.length(date |> string_of_int) === 1 ? "single-char" : "")>{date |> string_of_int |> str}</span>
           (List.length(holidayList |> List.find_all(holiday => holiday.date === jsDate)) > 0 ||
            List.length(birthDayList |> List.find_all(birthDay => (birthDay.birthDate |> getDateOnlyDate |> Js.Date.valueOf) === jsDate)) > 0 ?
              <div className="points">
                {holidayList 
                  |> List.find_all(holiday => holiday.date === jsDate) 
                  |> List.map(holiday => <div className="point point-holiday" />) |> Array.of_list |> array
                }
                {birthDayList 
                  |> List.find_all(birthDay => (birthDay.birthDate |> getDateOnlyDate |> Js.Date.valueOf) === jsDate) 
                  |> List.map(birthDay => <div className="point point-birthday" />) |> Array.of_list |> array
                }
              </div> : null
           )
         </td>;
       }
     )
  |> (arr => {
      Array.make(row, null)
      |> Array.mapi((idx, _) => {
           let end_ = (idx + 1) * col;
           let start = idx > 0 ? idx * col : idx;
           <tr key={j|row-$idx|j}> {arr |> Js.Array.slice(~start, ~end_) |> array} </tr>;
         });
    })
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

let make = (
    ~isMini=false, 
    ~month=(Js.Date.make() |> Js.Date.getMonth |> int_of_float), 
    ~year=(Js.Date.make() |> Js.Date.getFullYear),
    _children
  ) => {
  ...component,
  initialState: () => {
    loadState: Idle,
    month: month |> float_of_int |> getMonthType,
    year: year,
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
    | OnChangeYear(year) => Update({...state, year})
    };
  },
  render: ({state, send}) =>
    {
      Js.log(state.year);
      let isCurrentMonth = (isMini && 
        ((Js.Date.make() |> Js.Date.getMonth |> int_of_float |> float_of_int |> getMonthType) === state.month &&
        (Js.Date.make() |> Js.Date.getFullYear |> int_of_float) === (year |> int_of_float)) ? " current-month" : "");
      let targetYear = (isMini ? year : state.year);
      <div 
        className=("container calendar-container" ++ isCurrentMonth ++ (isMini ? " mini-calendar" : ""))
        onClick=(_ => (isMini ? 
          Router.push("dashboard/" ++ 
          ((month + 1) |> string_of_int) ++ "-" ++ 
          (targetYear |> int_of_float |> string_of_int)) : ()))
      >
        <div className="row">
          <div className="col">
            (isMini ? 
              null:
              <button 
                type_="button" 
                className="btn btn-rounded btn-main-color active-menu pl-4 pr-4"
                onClick=(_ => {
                  Router.push("all-month");
                })
              >
                <img src="/images/calendar.svg" style=(ReactDOMRe.Style.make(~width="35px", ~height="35px", ())) />
                {string(" " ++ (targetYear |> Js.Float.toString))}
              </button>
            )
          </div>
        </div>
        <div className=("row" ++ (isMini? " mt-1 mb-1" : " mt-5 mb-5"))>
          (isMini ?
            <div className="col-12 text-center" style=(ReactDOMRe.Style.make(~fontSize="14px", ()))>
              <b>{((month |> float_of_int |> getMonthType |> mapFullMonthStr) ++ " " ++ (targetYear |> Js.Float.toString)) |> str}</b>
            </div>
            : 
            <> 
              <div className="col-2 pr-0">
                <img className="cursor-pointer" src="/images/arrow_left.svg" onClick={_ => send(PreviousMonth)} />
              </div>
              <div className="col-8 text-center" style=(ReactDOMRe.Style.make(~fontSize="20px", ()))>
                <b>{((state.month |> mapFullMonthStr) ++ "   " ++ (targetYear |> Js.Float.toString)) |> str}</b>
              </div>
              <div className="col-2 pl-0 text-right">
                <img className="cursor-pointer" src="/images/arrow_right.svg" onClick={_ => send(NextMonth)} />
              </div>
            </>
          )
          
        </div>
        <table className="table calendar-table">
          <thead>
            <tr> {days |> List.map(day => <th> {day |> mapDayStr |> str} </th>) |> Array.of_list |> array} </tr>
          </thead>
          <tbody> 
          /*{state.loadState === Loading ? 
            null:
            dates((isMini ? (month |> float_of_int |> getMonthType) : state.month) |> getMonthFloat, targetYear, state.holidayList, state.listBirthDay)
          } */
          {dates((isMini ? (month |> float_of_int |> getMonthType) : state.month) |> getMonthFloat, targetYear, state.holidayList, state.listBirthDay)}
          </tbody>
        </table>
      </div>
    },
};
