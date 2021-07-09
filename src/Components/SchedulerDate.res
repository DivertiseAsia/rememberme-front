open ReasonReact

@react.component
let make = (
  ~isToday=false,
  ~scheduleList=list{},
  ~datetime="",
  ~schedules: list<RememberMeType.schedule>,
) =>
  <div className="container mt-3">
    <div className="row">
      <div className="col">
        <p className="mb-2"> {string(datetime)} </p>
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
      <div className="row" key={"schedule-date-" ++ (i |> string_of_int)}>
        <div className="col-2 p-0 text-right">
          <div className={"scheduler-status" ++ (backgroundColor ++ border)} />
        </div>
        <div className="col-10">
          <p> {string(schedule.title)} </p>
          {switch schedule.scheduleMenu {
          | Event =>
            let details = Js.String.split("\n", schedule.detail)
            details
            |> Array.to_list
            |> List.map(detail => <div> <p> {string(detail)} </p> </div>)
            |> Array.of_list
            |> array
          | _ => null
          }}
        </div>
      </div>
    })
    |> Array.of_list
    |> array}
  </div>
