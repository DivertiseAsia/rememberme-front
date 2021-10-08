open RememberMeApi

type birthDay = RememberMeType.birthDay
type event = RememberMeType.event
type holiday = RememberMeType.holiday

type date = int
type eventData = {
  holidays: array<holiday>,
  birthdays: array<birthDay>,
  leaves: array<RememberMeType.schedule>,
  events: array<event>,
}

type cell = Today(date, eventData) | CurrentMonth(date, eventData) | NotCurrentMonth(date)

@react.component
let make = (~extraClassName="", ~cell: cell, ~children=?) => {
  let className =
    "day" ++
    {
      switch cell {
      | NotCurrentMonth(_) => ` not-current-month ${extraClassName}`
      | Today(_) => ` today ${extraClassName}`
      | _ => ` ${extraClassName}`
      }
    }

  let holidayElement = idx =>
    <div key={"holiday-point-" ++ (idx |> string_of_int)} className="point point-holiday" />


  let eventElements = (
    ~holidayList,
    ~birthDayList,
    ~schedulesLeave,
    ~eventList,
    ~jsDate,
    ~idx,
    ~month,
    ~date,
  ) => {
    <div className="points">
      {holidayList
      ->Belt.List.keep((holiday: holiday) => holiday.date === jsDate)
      ->Belt.List.map(_holiday =>
        <div key={"holiday-point-" ++ (idx |> string_of_int)} className="point point-holiday" />
      )
      ->Array.of_list
      ->React.array}
      {birthDayList
      ->Belt.List.keep((birthDay: birthDay) =>
        birthDay.name !== "" &&
          RememberMeUtils.validateBirthday(birthDay.birthDate, month, date |> float_of_int)
      )
      ->Belt.List.map(_birthDay =>
        <div key={"birthday-point-" ++ (idx |> string_of_int)} className="point point-birthday" />
      )
      ->Array.of_list
      ->React.array}
      {schedulesLeave
      ->Belt.List.keep((schedule: RememberMeType.schedule) => schedule.date === jsDate)
      ->Belt.List.map((schedule: RememberMeType.schedule) =>
        <div
          key={`schedule-point-${idx->Belt.Int.toString}-${schedule.title}`}
          className="point point-leave"
        />
      )
      ->Array.of_list
      ->React.array}
      {eventList
      ->Belt.List.keep((event: event) => event.date |> Js.Date.valueOf === jsDate)
      ->Belt.List.map((event: event) =>
        <div
          key={`event-point-${idx->Belt.Int.toString}-${event.name}`} className="point point-event"
        />
      )
      ->Array.of_list
      ->React.array}
    </div>
  }
  <td className style={ReactDOM.Style.make(~paddingTop="20px", ())}>
    {switch cell {
    | NotCurrentMonth(date) => <span> {date->Belt.Int.toString->React.string} </span>
    | CurrentMonth(date, events) | Today(date, events) => <>
        <div className="circle-today" />
        <span className={String.length(date |> string_of_int) === 1 ? "single-char" : ""}>
          {date->Belt.Int.toString->React.string}
        </span>
      </>
    }}
    {children->Belt.Option.getWithDefault(React.null)}
  </td>
}
