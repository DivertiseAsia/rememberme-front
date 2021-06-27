open RememberMeType;

type dayContext('a) = {
  data: 'a,
  fetchData: unit => unit,
};

type days = {
  events: dayContext(apiState(list(RememberMeApi.event))),
  holidayList: dayContext(apiState(list(RememberMeApi.holiday))),
  birthDayList: dayContext(apiState(list(RememberMeApi.birthDay))),
};

let initDayContext = {data: NotLoaded, fetchData: _ => ignore()};

let initValue: days = {
  events: initDayContext,
  holidayList: initDayContext,
  birthDayList: initDayContext,
};

let context = React.createContext(initValue);

let useDaysResults = () => React.useContext(context);

module Provider = {
  let make = React.Context.provider(context);

  /** Tell bucklescript how to translate props into JS */
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
};