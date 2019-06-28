let getErrorMsgFromJson = json => {
  let jsonString = Json.stringify(json);
  let re = Js.Re.fromStringWithFlags("[\\[\\]\\{\\}\"]", ~flags="g");
  let splitMsg = Js.String.split(":", Js.String.replaceByRe(re, "", jsonString));
  splitMsg[Js.Array.length(splitMsg) - 1];
};
