let errorMessage = "Something went wrong please try again later."

let errorMessagesEl = errorMessages =>
  Js.String.split(":", errorMessages)->Belt.Array.mapWithIndex((idx, s) =>
    <p key=j`msg-$idx`> {React.string(s)} </p>
  )

let getErrorMsgFromJson = json =>
  Js.String.replaceByRe(Js.Re.fromStringWithFlags("[{}\"[\\]]", ~flags="g"), " ", json)
  |> Js.String.split(",")
  |> Js.Array.mapi((s, idx) =>
    Js.String.split(":", s) |> (
      arrErrMsg => {
        let field = arrErrMsg[Array.length(arrErrMsg) - 2]
        let msg = arrErrMsg[Array.length(arrErrMsg) - 1]
        <p key=j`msg-$idx`> {React.string(Js.String.replace("This field", field, msg))} </p>
      }
    )
  )

let getResponseMsgFromJson = json => {
  switch Js.Json.classify(json) {
  | Js.Json.JSONNull => errorMessage
  | _ =>
    let jsonString = Json.stringify(json)
    let re = Js.Re.fromStringWithFlags("[\\[\\]\\{\\}\"]", ~flags="g")
    let splitMsg = Js.String.split(":", Js.String.replaceByRe(re, "", jsonString))
    Js.Array.joinWith(" ", splitMsg)
  }
}

let getValueOfForm = e => ReactEvent.Form.target(e)["value"]

let formPreventDefault = e => ReactEvent.Form.preventDefault(e)

let valueFromEvent = event => ReactEvent.Form.target(event)["value"]

let filesFromEvent = event => ReactEvent.Form.target(event)["files"]

let valueFromEventOnBlur = event => ReactEvent.Focus.target(event)["value"]

let boolFromCheckbox = (event): bool => ReactEvent.Form.target(event)["checked"]

let isStringEmpty = str => str |> Js.String.length <= 0 || str === ""

let isAnyStringEmpty = listStr => listStr |> List.exists(str => str |> isStringEmpty)

let getPasswordWithLimit = (~limit=16, password) =>
  switch Js.String.match_(Js.Re.fromStringWithFlags("\\S", ~flags="g"), password) {
  | None => ""
  | Some(arr) => Js.String.slice(~from=0, ~to_=limit, Js.Array.joinWith("", arr))
  }

let checkPassword = password => getPasswordWithLimit(password, ~limit=16) |> Js.String.length >= 8

let checkIsSamePassword = (~password, ~confirmPassword) =>
  checkPassword(password) && (checkPassword(confirmPassword) && confirmPassword === password)

let getStrWithoutSpace = (~text, ~limit) =>
  switch Js.String.match_(Js.Re.fromStringWithFlags("\\S", ~flags="g"), text) {
  | None => ""
  | Some(arr) => Js.String.slice(~from=0, ~to_=limit, Js.Array.joinWith("", arr))
  }

let getNumberWithoutSpace = (~limit=?, text) =>
  switch Js.String.match_(Js.Re.fromStringWithFlags("\\d", ~flags="g"), text) {
  | None => ""
  | Some(arr) =>
    switch limit {
    | None => arr |> Array.length
    | Some(l) => l
    } |> (realLimit => Js.String.slice(~from=0, ~to_=realLimit, Js.Array.joinWith("", arr)))
  }

let getCaseId = (~limit=?, text) =>
  switch Js.String.match_(Js.Re.fromStringWithFlags("[a-zA-z0-9-_\/]", ~flags="g"), text) {
  | None => ""
  | Some(arr) =>
    switch limit {
    | None => arr |> Array.length
    | Some(l) => l
    } |> (realLimit => Js.String.slice(~from=0, ~to_=realLimit, Js.Array.joinWith("", arr)))
  }

let getPhoneNumber = text =>
  switch Js.String.match_(Js.Re.fromStringWithFlags("\\d+", ~flags="g"), text) {
  | None => ""
  | Some(arr) => Js.String.slice(~from=0, ~to_=10, Js.Array.joinWith("", arr))
  }

let checkPhoneNumber = text => getPhoneNumber(text) |> Js.String.length === 10

let checkEmail = text =>
  switch Js.String.match_(Js.Re.fromStringWithFlags("\\S+@\\S+\\.\\S+$", ~flags="g"), text) {
  | None => false
  | Some(_) => true
  }

let getLocalStorage = key => Dom.Storage.getItem(key, Dom.Storage.localStorage)

let getLocalStorageWithDefault = (~default="", key) =>
  Js.Option.getWithDefault(default, getLocalStorage(key))

let getToken = () => getLocalStorage("token")

let mapOpt = (opt, ~defaultValue) =>
  switch opt {
  | None => defaultValue
  | Some(value) => value
  }

let mapOptStr = optStr => optStr |> mapOpt(~defaultValue="")

module Date = {
  @deriving(abstract)
  type dateOptions = {
    @optional month: string,
    @optional year: string,
    @optional day: string,
    @optional hour: string,
    @optional minute: string,
  }

  @bs.send
  external toLocaleDateString: (Js.Date.t, string, dateOptions) => string = "toLocaleDateString"

  let dateInputValue = (date): string =>
    toLocaleDateString(
      date,
      "en-CA",
      dateOptions(~year="numeric", ~month="numeric", ~day="numeric", ()),
    )

  let isToday = (datetime: Js.Date.t) => {
    let today = Js.Date.now()
    let currentDate = today->Js.Date.fromFloat->Js.Date.getDate
    let currentMonth = today->Js.Date.fromFloat->Js.Date.getMonth
    let currentYear = today->Js.Date.fromFloat->Js.Date.getFullYear

    let date = datetime->Js.Date.getDate
    let month = datetime->Js.Date.getMonth
    let year = datetime->Js.Date.getFullYear
    currentDate === date && currentMonth === month && currentYear === year
  }

  let isSameTimestamp = (firstDate, secondDate) => {
    firstDate->Js.Date.valueOf === secondDate->Js.Date.valueOf
  }

  //dd Mmm YYYY
  let dateWithShortMonthFullYear = date =>
    toLocaleDateString(
      date,
      "en-GB",
      dateOptions(~month="short", ~day="numeric", ~year="numeric", ()),
    )

}
