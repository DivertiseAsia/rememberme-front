let home = "/"
let dashboard = "/dashboard"
let login = "/login"
let logout = "/logout"
let forgot = "/forgot"
let register = "/signup"
let profile = "/profile"
let allMonth = "/all-month"


let getValueFromQuerystring = (~key: string, querystring: string): option<string> =>
  Js.Re.fromStringWithFlags(`${key}=([^&#]*)`, ~flags="g")
  ->Js.String.match_(querystring)
  ->Belt.Option.mapWithDefault(None, values => {
    switch Js.String.replace(`${key}=`, "", Js.Array.joinWith("", values)) {
    | "" => None
    | value => value->Js.Global.decodeURIComponent->Some
    }
  })