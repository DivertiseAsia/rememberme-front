let initValue: (
  RememberMeType.apiState(list(RememberMeApi.event)),
  unit => unit,
) = (
  RememberMeType.NotLoaded,
  () => ignore(),
);

let context = React.createContext(initValue);

let useEventsResults = () => React.useContext(context);

module Provider = {
  let make = React.Context.provider(context);

  /** Tell bucklescript how to translate props into JS */
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
};