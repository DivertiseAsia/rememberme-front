open ReasonReact
open RememberMeApi

let str = ReasonReact.string

type birthDay = RememberMeType.birthDay
type event = RememberMeType.event
type holiday = RememberMeType.holiday

let months: list<RememberMeType.month> = list{
  Jan,
  Feb,
  Mar,
  Apr,
  May,
  Jun,
  Jul,
  Aug,
  Sep,
  Oct,
  Nov,
  Dec,
}
let days: list<RememberMeType.day> = list{Mon, Tue, Wed, Thu, Fri, Sat, Sun}

type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed

type state = {
  loadState: loadState,
  month: RememberMeType.month,
  year: float,
}

type action =
  | NextMonth
  | PreviousMonth
  | OnChangeMonth(RememberMeType.month)
  | OnChangeYear(float)

let getMonthFloat = month => months->Array.of_list->Js.Array2.indexOf(month)->float_of_int

let getMonthType = idx => {
  open RememberMeType
  switch Js.List.nth(months, idx->int_of_float) {
  | None => Jan
  | Some(month) => month
  }
}

let getLastDateObj = %bs.raw(`
function f (year, month){
  var date = new Date(year, month, 0);
  return date
}
`)

let getLastDate = (~year, ~realMonth) => getLastDateObj(year, realMonth)->Js.Date.getDate

let getDateOnlyDate = date =>
  Js.Date.makeWithYMD(
    ~year=date->Js.Date.getFullYear,
    ~month=date->Js.Date.getMonth,
    ~date=date->Js.Date.getDate,
    (),
  )

let mocks: list<holiday> = list{
  {
    name: "DAY 1 ",
    date: "2019-06-01"->Js.Date.fromString->getDateOnlyDate->Js.Date.valueOf,
    date2: "2019-06-01"->Js.Date.fromString,
    isVacation: true,
  },
  {
    name: "DAY 2 ",
    date: "2019-06-27"->Js.Date.fromString->getDateOnlyDate->Js.Date.valueOf,
    date2: "2019-06-27"->Js.Date.fromString,
    isVacation: true,
  },
}

let getHolidayData = (holidayList, date) =>
  holidayList->Belt.List.getBy((vacation: holiday) => vacation.date === date)

let getBirthDayElement = (birthList, date) =>
  birthList
  ->Belt.List.getBy(({birthDate}: birthDay) => birthDate->getDateOnlyDate->Js.Date.valueOf === date)
  ->Belt.Option.map(({name}: birthDay) => <p> {`${name} BirthDay`->React.string} </p>)

let getDayByStartOnMonday = (dayInWeek: int) => dayInWeek === 0 ? 6 : dayInWeek - 1

let dates = (month, year, holidayList, birthDayList, leaveList, eventList) => {
  let col = 7
  let row = 6
  let boxs = col * row
  let lastDatePreviousMonth = getLastDate(~year, ~realMonth=month)->int_of_float
  let lastDate = getLastDate(~year, ~realMonth=month +. 1.)->int_of_float

  let startDayInWeek =
    Js.Date.makeWithYM(~year, ~month, ())->Js.Date.getDay->int_of_float->getDayByStartOnMonday

  let today =
    Js.Date.make()
    ->(date =>
      Js.Date.makeWithYMD(
        ~year=date->Js.Date.getFullYear,
        ~month=date->Js.Date.getMonth,
        ~date=date->Js.Date.getDate,
        (),
      ))
    ->Js.Date.valueOf

  let monthEventsData: CalendarDateCell.eventData = {
    holidays: holidayList->Array.of_list,
    birthdays: birthDayList->Array.of_list,
    leaves: list{}->Array.of_list,
    events: eventList->Array.of_list,
  }
  Belt.Array.range(0, boxs)
  ->Belt.Array.mapWithIndex((idx, _) =>
    switch idx {
    | idx when idx < startDayInWeek =>
      <CalendarDateCell
        key={`date-cell-${year->Belt.Float.toString}-${month->Belt.Float.toString}-${idx->Belt.Int.toString}`}
        extraClassName=`day not-current-month`
        cell=NotCurrentMonth({lastDatePreviousMonth - (startDayInWeek - (idx + 1))})
      />
    | idx when idx - startDayInWeek + 1 > lastDate =>
      <CalendarDateCell
        key={`date-cell-${year->Belt.Float.toString}-${month->Belt.Float.toString}-${idx->Belt.Int.toString}`}
        extraClassName=j`day not-current-month`
        cell=NotCurrentMonth({idx - startDayInWeek + 1 - lastDate})
      />
    | _ =>
      let date = idx - startDayInWeek + 1
      let jsDate =
        Js.Date.makeWithYMD(~year, ~month, ~date=date |> float_of_int, ()) |> Js.Date.valueOf
      let schedulesLeave =
        leaveList
        |> List.map((requestLeave: leaveDetail) =>
          requestLeave |> RememberMeUtils.splitRequestLeave
        )
        |> List.concat
      <CalendarDateCell
        key={`date-cell-${year->Belt.Float.toString}-${month->Belt.Float.toString}-${idx->Belt.Int.toString}`}
        extraClassName="day"
        cell={today === jsDate
          ? Today(date, monthEventsData)
          : CurrentMonth(date, monthEventsData)}>
        {List.length(
          holidayList |> List.find_all((holiday: holiday) => holiday.date === jsDate),
        ) > 0 ||
          (List.length(
            eventList |> List.find_all((event: event) => event.date |> Js.Date.valueOf === jsDate),
          ) > 0 ||
          (List.length(
            schedulesLeave |> List.find_all((schedule: RememberMeType.schedule) =>
              schedule.date === jsDate
            ),
          ) > 0 ||
            List.length(
              birthDayList |> List.find_all((birthDay: birthDay) =>
                RememberMeUtils.validateBirthday(birthDay.birthDate, month, date |> float_of_int)
              ),
            ) > 0))
          ? <div className="points">
              {holidayList
              |> List.find_all((holiday: holiday) => holiday.date === jsDate)
              |> List.map(_holiday =>
                <div
                  key={"holiday-point-" ++ (idx |> string_of_int)} className="point point-holiday"
                />
              )
              |> Array.of_list
              |> array}
              {birthDayList
              |> List.filter((birthDay: birthDay) => birthDay.name !== "")
              |> List.find_all((birthDay: birthDay) =>
                RememberMeUtils.validateBirthday(birthDay.birthDate, month, date |> float_of_int)
              )
              |> List.map(_birthDay =>
                <div
                  key={"birthday-point-" ++ (idx |> string_of_int)} className="point point-birthday"
                />
              )
              |> Array.of_list
              |> array}
              {schedulesLeave
              |> List.filter((schedule: RememberMeType.schedule) => schedule.date === jsDate)
              |> List.map((schedule: RememberMeType.schedule) =>
                <div
                  key={`schedule-point-${idx->Belt.Int.toString}-${schedule.title}`}
                  className="point point-leave"
                />
              )
              |> Array.of_list
              |> array}
              {eventList
              |> List.find_all((event: event) => event.date |> Js.Date.valueOf === jsDate)
              |> List.map((event: event) =>
                <div
                  key={`event-point-${idx->Belt.Int.toString}-${event.name}`}
                  className="point point-event"
                />
              )
              |> Array.of_list
              |> array}
            </div>
          : null}
      </CalendarDateCell>
    }
  )
  ->(arr =>
    Array.make(row, null) |> Array.mapi((idx, _) => {
      let end_ = (idx + 1) * col
      let start = idx > 0 ? idx * col : idx
      <tr key=j`row-$idx`> {arr |> Js.Array.slice(~start, ~end_) |> array} </tr>
    }))
  ->React.array
}

@react.component
let make = (
  ~isMini=false,
  ~month=Js.Date.make() |> Js.Date.getMonth |> int_of_float,
  ~year=Js.Date.make() |> Js.Date.getFullYear,
  ~holidayList=list{},
  ~listBirthDay=list{},
  ~leaveList=list{},
  ~eventList=list{},
) => {
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | NextMonth =>
      switch state.month {
      | Dec => {...state, month: Jan, year: state.year +. 1.}
      | _ => {
          ...state,
          month: state.month |> getMonthFloat |> (idx => idx +. 1.) |> getMonthType,
        }
      }
    | PreviousMonth =>
      switch state.month {
      | Jan => {...state, month: Dec, year: state.year -. 1.}
      | _ => {
          ...state,
          month: state.month |> getMonthFloat |> (idx => idx -. 1.) |> getMonthType,
        }
      }
    | OnChangeMonth(month) => {...state, month: month}
    | OnChangeYear(year) => {...state, year: year}
    }
  , {loadState: Idle, month: month |> float_of_int |> getMonthType, year: year})
  let isCurrentMonth =
    isMini &&
    (Js.Date.make()
    |> Js.Date.getMonth
    |> int_of_float
    |> float_of_int
    |> getMonthType === state.month &&
    Js.Date.make() |> Js.Date.getFullYear |> int_of_float === (year |> int_of_float))
      ? " current-month"
      : ""
  let targetYear = isMini ? year : state.year
  <div
    className={"container calendar-container" ++
    (isCurrentMonth ++
    (isMini ? " mini-calendar" : ""))}
    onClick={_ =>
      isMini
        ? Router.push(
            "dashboard/" ++
            ((month + 1 |> string_of_int) ++
            ("-" ++ (targetYear |> int_of_float |> string_of_int))),
          )
        : ()}>
    <div className="row">
      <div className="col">
        {isMini
          ? null
          : <button
              type_="button"
              className="btn btn-rounded btn-main-color active-menu pl-4 pr-4"
              onClick={_ => Router.push("all-month")}>
              <img
                src="/images/calendar.svg"
                style={ReactDOM.Style.make(~width="35px", ~height="35px", ())}
              />
              {string(" " ++ (targetYear |> Js.Float.toString))}
            </button>}
      </div>
    </div>
    <div className={"row" ++ (isMini ? " mt-1 mb-1" : " mt-5 mb-5")}>
      {isMini
        ? <div className="col-12 text-center" style={ReactDOM.Style.make(~fontSize="14px", ())}>
            <b>
              {(month |> float_of_int |> getMonthType |> RememberMeUtils.mapFullMonthStr) ++
                (" " ++
                (targetYear |> Js.Float.toString)) |> str}
            </b>
          </div>
        : <>
            <div className="col-2 pr-0">
              <img
                className="cursor-pointer"
                src="/images/arrow_left.svg"
                onClick={_ => dispatch(PreviousMonth)}
              />
            </div>
            <div className="col-8 text-center" style={ReactDOM.Style.make(~fontSize="20px", ())}>
              <b>
                {(state.month |> RememberMeUtils.mapFullMonthStr) ++
                  ("   " ++
                  (targetYear |> Js.Float.toString)) |> str}
              </b>
            </div>
            <div className="col-2 pl-0 text-right">
              <img
                className="cursor-pointer"
                src="/images/arrow_right.svg"
                onClick={_ => dispatch(NextMonth)}
              />
            </div>
          </>}
    </div>
    <table className="table calendar-table">
      <thead>
        <tr>
          {days
          |> List.map(day =>
            <th key={"day-" ++ (day |> RememberMeUtils.mapDayStr)}>
              {day |> RememberMeUtils.mapDayStr |> str}
            </th>
          )
          |> Array.of_list
          |> array}
        </tr>
      </thead>
      <tbody>
        {state.loadState === Loading
          ? null
          : dates(
              (isMini ? month |> float_of_int |> getMonthType : state.month) |> getMonthFloat,
              targetYear,
              holidayList,
              listBirthDay,
              leaveList,
              eventList,
            )}
      </tbody>
    </table>
  </div>
}
