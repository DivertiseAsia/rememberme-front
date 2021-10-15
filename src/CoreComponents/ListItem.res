
type itemEnd =
  | Text(string)
  | Image(string)
  | Button(string, ReactEvent.Mouse.t => unit)
  | Children(array<React.element>)

type itemBody = {
  title: string,
  subtitle: option<string>,
  topTitle: option<string>,
  children: option<array<React.element>>,
}

type item = {
  body: itemBody,
  left: option<itemEnd>,
  right: option<itemEnd>,
  onClick: option<ReactEvent.Mouse.t => unit>,
}

let renderItemEnd = (itemEnd: option<itemEnd>, extraClass: string) =>
  switch itemEnd {
  | None => React.null
  | Some(value) =>
    <span className={"item-end item-end-" ++ extraClass}>
      {switch value {
      | Text(s) => <span> {React.string(s)} </span>
      | Image(s) =>
        <span
          className="list-time-image"
          style={ReactDOM.Style.make(~backgroundImage="url('" ++ (s ++ "')"), ())}
        />
      | Button(s, onClick) => <button onClick> {React.string(s)} </button>
      | _ => <span />
      }}
    </span>
  }

let renderToDiv = (text: option<string>, extraClass: string) =>
  switch text {
  | None
  | Some("") => React.null
  | Some(s) => <div className=extraClass> {React.string(s)} </div>
  }

let renderItemBody = (itemBody: itemBody) =>
  <span className="item-body">
    {renderToDiv(itemBody.topTitle, "item-top")}
    <div className="list-item-title"> {React.string(itemBody.title)} </div>
    {renderToDiv(itemBody.subtitle, "item-sub")}
    {switch itemBody.children {
    | None => React.null
    | Some(children) => <div className="list-item-subchildren"> {children->React.array} </div>
    }}
  </span>

@react.component
let make = (~item: item, ~className: option<string>=?) =>
  <div
    className={"list-item " ++ Js.Option.getWithDefault("list-item-default", className)}
    onClick=?item.onClick>
    {renderItemEnd(item.left, "left")}
    {renderItemBody(item.body)}
    {renderItemEnd(item.right, "right")}
  </div>
