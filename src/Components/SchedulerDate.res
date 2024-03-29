type dateType = Short | Full

@react.component
let make = (
  ~isToday=false,
  ~datetime="",
  ~schedules: list<RememberMeType.schedule>,
  ~dateType=Full,
) =>
  <div className="container mt-3">
    <div className="row">
      <div className="col">
        <p className="mb-2"> {React.string(datetime)} </p>
        <hr
          className="mt-0"
          style={ReactDOM.Style.make(
            ~borderTop=isToday ? "1px solid #2D506D" : "1px solid rgba(0,0,0,.1)",
            (),
          )}
        />
      </div>
    </div>
    {schedules->Belt.List.mapWithIndex((i, schedule: RememberMeType.schedule) => {
      let (backgroundColor, border) = switch schedule.scheduleMenu {
      | Leave => (isToday ? " bg-schedule-leave" : " ", " border-schedule-leave")
      | Holiday => (isToday ? " bg-schedule-holiday" : " ", " border-schedule-holiday")
      | Event => (isToday ? " bg-schedule-event" : " ", " border-schedule-event")
      | Birthday => (isToday ? " bg-schedule-birthday" : " ", " border-schedule-birthday")
      | _ => (isToday ? " bg-schedule-event" : " ", " border-schedule-event")
      }
      <div className="row" key={`schedule-date-${datetime}-${i->Belt.Int.toString}-${dateType === Short ? "short" : "full"}`}>
        <div className="col-2 p-0 text-right">
          <div className={"scheduler-status" ++ (backgroundColor ++ border)} />
        </div>
        <div className="col-10">
          <p> {React.string(schedule.title)} </p>
          {switch (schedule.scheduleMenu, dateType) {
          | (Event, Full) =>
            let details = Js.String.split("\n", schedule.detail)
            details
            -> Belt.Array.mapWithIndex((idx, detail) =>
                <div className={`event-date-details-${idx->Belt.Int.toString}-${datetime}-${i->Belt.Int.toString}-${dateType === Short ? "short" : "full"}`}>
                  <p> {React.string(detail)} </p>
                </div>)
            -> React.array
          | _ => React.null
          }}
        </div>
      </div>
    })
    |> Array.of_list
    |> React.array}
  </div>
