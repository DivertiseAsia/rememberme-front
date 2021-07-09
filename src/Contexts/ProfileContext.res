type contextValue<'a> = {
  data: 'a,
  fetchData: unit => unit,
}

let initValue: contextValue<RememberMeType.apiState<RememberMeApi.profile>> = {
  data: RememberMeType.NotLoaded,
  fetchData: _ => ignore(),
}

let context = React.createContext(initValue)

let useProfileResults = () => React.useContext(context)

module Provider = {
  let make = React.Context.provider(context)

  @ocaml.doc(" Tell bucklescript how to translate props into JS ")
  let makeProps = (~value, ~children, ()) =>
    {
      "value": value,
      "children": children,
    }
}
