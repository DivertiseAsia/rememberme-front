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

let getEventsOfTargetDaye = (~targerDate: Js.Date.t, {holidays, birthdays, events, leaves}) => {
  {holidays: holidays->Belt.Array.keep((holiday: holiday) => ReDate.isSameDay(holiday.date2, targerDate)),
  birthdays: birthdays->Belt.Array.keep((birthDay:birthDay) => birthDay.name !== "" && RememberMeUtils.validateBirthday2(birthDay.birthDate, targerDate)),
  events: events->Belt.Array.keep((event: event) => ReDate.isSameDay(event.date, targerDate)),
  leaves: leaves->Belt.Array.keep((schedule: schedule) => ReDate.isSameDay(schedule.date2, targerDate)),
  }
}
let eventElements = (~targetDate: Js.Date.t, {holidays, birthdays, events, leaves}) => {
  <div className="points">
    {holidays
    ->Belt.Array.map(_=>
          <div
            key={`holiday-point-${targetDate->Js.Date.toDateString}`}
            className="point point-holiday"
          />
    )
    ->React.array}
    {birthdays
    ->Belt.Array.map(_ =>
          <div
            key={`birthday-point-${targetDate->Js.Date.toDateString}`}
            className="point point-birthday"
          />
    )
    ->React.array}
    {leaves
    ->Belt.Array.map((schedule: RememberMeType.schedule) =>
          <div
            key={`schedule-point-${schedule.title}-${targetDate->Js.Date.toDateString}`}
            className="point point-leave"
          />
    )
    ->React.array}
    {events
    ->Belt.Array.map((event: event) =>
          <div
            key={`event-point-${targetDate->Js.Date.toDateString}-${event.name}`}
            className="point point-event"
          />
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

    <td className style={ReactDOM.Style.make(~paddingTop="20px", ())} onClick={_ => setShowEventDetails(isShow => !isShow)}>
      {switch cell {
      | NotCurrentMonth(date) => <span> {date->Belt.Int.toString->React.string} </span>
      | CurrentMonth(date, events) | Today(date, events) =>
        let dateNumber = date->Js.Date.getDate->Belt.Int.fromFloat
        let eventsOfDate = getEventsOfTargetDaye(~targerDate=date, events)

        <>
          <div className="circle-today" />
          <span className={String.length(dateNumber->string_of_int) === 1 ? "single-char" : ""}>
            {dateNumber->Belt.Int.toString->React.string}
          </span>
          {eventElements(~targetDate=date, eventsOfDate)}
        </>
      }}
      {children->Belt.Option.getWithDefault(React.null)}
    </td>
}
