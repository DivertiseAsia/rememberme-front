type birthDay = RememberMeType.birthDay
type event = RememberMeType.event
type holiday = RememberMeType.holiday
type schedule = RememberMeType.schedule

type date = int
type datObject = Js.Date.t

type eventData = {
  holidays: array<holiday>,
  birthdays: array<birthDay>,
  leaves: array<schedule>,
  events: array<event>,
}

type cell = Today(datObject, eventData) | CurrentMonth(datObject, eventData) | NotCurrentMonth(date)

let eventElements = (~targetDate: Js.Date.t, {holidays, birthdays, events, leaves}) => {
  <div className="points">
    {holidays
    ->Belt.Array.keepMap((holiday: holiday) =>
      if ReDate.isSameDay(holiday.date2, targetDate) {
        Some(
          <div
            key={`holiday-point-${targetDate->Js.Date.toDateString}`}
            className="point point-holiday"
          />,
        )
      } else {
        None
      }
    )
    ->React.array}
    {birthdays
    ->Belt.Array.keepMap((birthDay: birthDay) =>
      if birthDay.name !== "" && RememberMeUtils.validateBirthday2(birthDay.birthDate, targetDate) {
        Some(
          <div
            key={`birthday-point-${targetDate->Js.Date.toDateString}`}
            className="point point-birthday"
          />,
        )
      } else {
        None
      }
    )
    ->React.array}
    {leaves
    ->Belt.Array.keepMap((schedule: RememberMeType.schedule) =>
      if ReDate.isSameDay(schedule.date2, targetDate) {
        Some(
          <div
            key={`schedule-point-${schedule.title}-${targetDate->Js.Date.toDateString}`}
            className="point point-leave"
          />,
        )
      } else {
        None
      }
    )
    ->React.array}
    {events
    ->Belt.Array.keepMap((event: event) =>
      if ReDate.isSameDay(event.date, targetDate) {
        Some(
          <div
            key={`event-point-${targetDate->Js.Date.toDateString}-${event.name}`}
            className="point point-event"
          />,
        )
      } else {
        None
      }
    )
    ->React.array}
  </div>
}

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

  <td className style={ReactDOM.Style.make(~paddingTop="20px", ())} >
    {switch cell {
    | NotCurrentMonth(date) => <span> {date->Belt.Int.toString->React.string} </span>
    | CurrentMonth(date, events) | Today(date, events) =>
      let dateNumber = date->Js.Date.getDate->Belt.Int.fromFloat
      <>
        <div className="circle-today" />
        <span className={String.length(dateNumber->string_of_int) === 1 ? "single-char" : ""}>
          {dateNumber->Belt.Int.toString->React.string}
        </span>
        {eventElements(~targetDate=date, events)}
      </>
    }}
    {children->Belt.Option.getWithDefault(React.null)}
  </td>
}
