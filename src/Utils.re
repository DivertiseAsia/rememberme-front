let getErrorMsgFromJson = json =>
  Js.String.replaceByRe(Js.Re.fromStringWithFlags("[{}\"[\\]]", ~flags="g"), " ", json)
  |> Js.String.split(",")
  |> Js.Array.mapi((s, idx) =>
       Js.String.split(":", s)
       |> (
         arrErrMsg => {
           let field = arrErrMsg[Array.length(arrErrMsg) - 2];
           let msg = arrErrMsg[Array.length(arrErrMsg) - 1];
           <p key={j|msg-$idx|j}> {ReasonReact.string(Js.String.replace("This field", field, msg))} </p>;
         }
       )
     );

let getValueOfForm = e => ReactEvent.Form.target(e)##value

let formPreventDefault = e => ReactEvent.Form.preventDefault(e);

let valueFromEvent = event => ReactEvent.Form.target(event)##value;

let valueFromEventOnBlur = event => ReactEvent.Focus.target(event)##value;

let boolFromCheckbox = (event): bool => ReactEvent.Form.target(event)##checked;

let isStringEmpty = str => str |> Js.String.length <= 0 || str === "";

let isAnyStringEmpty = listStr => listStr |> List.exists(str => str |> isStringEmpty);

let getPasswordWithLimit = (~limit=16, password) =>
  switch (Js.String.match(Js.Re.fromStringWithFlags("\\S", ~flags="g"), password)) {
  | None => ""
  | Some(arr) => Js.String.slice(~from=0, ~to_=limit, Js.Array.joinWith("", arr))
  };

let checkPassword = password => getPasswordWithLimit(password, ~limit=16) |> Js.String.length >= 8;

let checkIsSamePassword = (~password, ~confirmPassword) =>
  checkPassword(password) && checkPassword(confirmPassword) && confirmPassword === password;

let getStrWithoutSpace = (~text, ~limit) => {
  switch (Js.String.match(Js.Re.fromStringWithFlags("\\S", ~flags="g"), text)) {
  | None => ""
  | Some(arr) => Js.String.slice(~from=0, ~to_=limit, Js.Array.joinWith("", arr))
  };
};

let getNumberWithoutSpace = (~limit=?, text) => {
  switch (Js.String.match(Js.Re.fromStringWithFlags("\\d", ~flags="g"), text)) {
  | None => ""
  | Some(arr) =>
    switch(limit){
     | None => arr |> Array.length
     | Some(l) => l
    }
    |> (realLimit => Js.String.slice(~from=0, ~to_=realLimit, Js.Array.joinWith("", arr)))
  };
};

let getCaseId = (~limit=?, text) => {
  switch (Js.String.match(Js.Re.fromStringWithFlags("[a-zA-z0-9-_\/]", ~flags="g"), text)) {
  | None => ""
  | Some(arr) =>
    switch(limit){
     | None => arr |> Array.length
     | Some(l) => l
    }
    |> (realLimit => Js.String.slice(~from=0, ~to_=realLimit, Js.Array.joinWith("", arr)))
  };
};

let getPhoneNumber = text =>
  switch (Js.String.match(Js.Re.fromStringWithFlags("\\d+", ~flags="g"), text)) {
  | None => ""
  | Some(arr) => Js.String.slice(~from=0, ~to_=10, Js.Array.joinWith("", arr))
  };

let checkPhoneNumber = text => getPhoneNumber(text) |> Js.String.length === 10;

let checkEmail = text =>
  switch (Js.String.match(Js.Re.fromStringWithFlags("\\S+@\\S+\\.\\S+$", ~flags="g"), text)) {
  | None => false
  | Some(_) => true
  };
