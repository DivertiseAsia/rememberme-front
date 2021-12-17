@react.component
let make = (~percent: int, ~className: option<string>=?, ~onClick=?, ~title: option<string>=?) =>
  <div
    ?title
    ?onClick
    className={"progress " ++ Js.Option.getWithDefault("progress-default", className)}>
    <div
      className="fillbar" style={ReactDOM.Style.make(~width=string_of_int(percent) ++ "%", ())}
    />
  </div>
