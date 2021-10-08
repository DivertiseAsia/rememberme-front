type birthDay = RememberMeType.birthDay
type event = RememberMeType.event
type holiday = RememberMeType.holiday
type schedule = RememberMeType.schedule

type date = int

type eventData = {
  holidays: array<holiday>,
  birthdays: array<birthDay>,
  leaves: array<schedule>,
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

  let eventElements = (
    ~holidayList,
    ~birthDayList,
    ~schedulesLeave,
    ~eventList,
    ~jsDate,
    ~targetDate: Js.Date.t,
    ~idx,
  ) => {
    <div className="points">
      {holidayList
      ->Belt.List.keepMap((holiday: holiday) =>
        if ReDate.isSameDay(holiday.date2, targetDate) {
          Some(
            <div
              key={`holiday-point-${idx->string_of_int}-${targetDate->Js.Date.toDateString}`}
              className="point point-holiday"
            />,
          )
        } else {
          None
        }
      )
      ->Array.of_list
      ->React.array}
      {birthDayList
      ->Belt.List.keepMap((birthDay: birthDay) =>
        if (
          birthDay.name !== "" && RememberMeUtils.validateBirthday2(birthDay.birthDate, targetDate)
        ) {
          Some(
            <div
              key={`birthday-point-${idx->string_of_int}-${targetDate->Js.Date.toDateString}`}
              className="point point-birthday"
            />,
          )
        } else {
          None
        }
      )
      ->Array.of_list
      ->React.array}
      {schedulesLeave
      ->Belt.List.keepMap((schedule: RememberMeType.schedule) =>
        if schedule.date === jsDate {
          Some(
            <div
              key={`schedule-point-${idx->Belt.Int.toString}-${schedule.title}-${targetDate->Js.Date.toDateString}`}
              className="point point-leave"
            />,
          )
        } else {
          None
        }
      )
      ->Array.of_list
      ->React.array}
      {eventList
      ->Belt.List.keepMap((event: event) =>
        if ReDate.isSameDay(event.date, targetDate) {
          Some(
            <div
              key={`event-point-${idx->Belt.Int.toString}-${event.name}`}
              className="point point-event"
            />,
          )
        } else {
          None
        }
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
