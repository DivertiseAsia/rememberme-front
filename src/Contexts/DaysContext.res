open RememberMeType

type birthDay = RememberMeType.birthDay
type event = RememberMeType.event
type holiday = RememberMeType.holiday


type dayContext<'a> = {
  data: 'a,
  fetchData: unit => unit,
  forceFetchData: unit => unit,
}

type days = {
  events: dayContext<apiState<list<event>>>,
  holidayList: dayContext<apiState<list<holiday>>>,
  birthDayList: dayContext<apiState<list<birthDay>>>,
  allLeaveList: dayContext<apiState<list<RememberMeApi.leaveDetail>>>,
  userLeaveList: dayContext<apiState<list<RememberMeApi.leaveDetail>>>,
}

let initDayContext = {
  data: NotLoaded,
  fetchData: _ => ignore(),
  forceFetchData: _ => ignore(),
}

let initValue: days = {
  events: initDayContext,
  holidayList: initDayContext,
  birthDayList: initDayContext,
  allLeaveList: initDayContext,
  userLeaveList: initDayContext,
}

let context = React.createContext(initValue)

let useDaysResults = () => React.useContext(context)

module Provider = {
  let make = React.Context.provider(context)

  @ocaml.doc(" Tell bucklescript how to translate props into JS ")
  let makeProps = (~value, ~children, ()) =>
    {
      "value": value,
      "children": children,
    }
}
