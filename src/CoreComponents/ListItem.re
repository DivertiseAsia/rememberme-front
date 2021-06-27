open ReasonReact;
//open Css;
open Css_Js_Core;

type itemEnd =
  | Text(string)
  | Image(string)
  | Button(string, ReactEvent.Mouse.t => unit)
  | Children(array(ReasonReact.reactElement));

type itemBody = {
  title: string,
  subtitle: option(string),
  topTitle: option(string),
  children: option(array(ReasonReact.reactElement)),
};

type item = {
  body: itemBody,
  left: option(itemEnd),
  right: option(itemEnd),
  onClick: option(ReactEvent.Mouse.t => unit),
};

let imageStyle = "";
//  style([display(inlineBlock), height(`px(20)), width(`px(20))]); //TODO:

//let component = ReasonReact.statelessComponent("ListItemRe");

let renderItemEnd = (itemEnd: option(itemEnd), extraClass: string) =>
  switch (itemEnd) {
  | None => ReasonReact.null
  | Some(value) =>
    <span className={"item-end item-end-" ++ extraClass}>
      {switch (value) {
       | Text(s) => <span> {ReasonReact.string(s)} </span>
       | Image(s) =>
         <span
           className=imageStyle
           style={ReactDOM.Style.make(
             ~backgroundImage="url('" ++ s ++ "')",
             (),
           )}
         />
       | Button(s, onClick) =>
         <button onClick> {ReasonReact.string(s)} </button>
       | _ => <span />
       }}
    </span>
  };

let renderToDiv = (text: option(string), extraClass: string) =>
  switch (text) {
  | None
  | Some("") => ReasonReact.null
  | Some(s) => <div className=extraClass> {ReasonReact.string(s)} </div>
  };

let renderItemBody = (itemBody: itemBody) =>
  <span className="item-body">
    {renderToDiv(itemBody.topTitle, "item-top")}
    <div className="list-item-title">
      {ReasonReact.string(itemBody.title)}
    </div>
    {renderToDiv(itemBody.subtitle, "item-sub")}
    {switch (itemBody.children) {
     | None => ReasonReact.null
     | Some(children) =>
       <div className="list-item-subchildren"> children->React.array </div>
     }}
  </span>;

[@react.component]
let make = (~item: item, ~className: option(string)=?) => {
  <div
    className={
      "list-item " ++ Js.Option.getWithDefault("list-item-default", className)
    }
    onClick=?{item.onClick}>
    {renderItemEnd(item.left, "left")}
    {renderItemBody(item.body)}
    {renderItemEnd(item.right, "right")}
  </div>;
};