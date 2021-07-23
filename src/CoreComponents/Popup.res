@react.component
let make = (
  ~visible: bool,
  ~pressClose: ReactEvent.Mouse.t => unit,
  ~className: option<string>=?,
  ~children,
) =>
  <div
    className=`pop-up-background ${visible ? "pop-up-visible" : "pop-up-hidden"} ${className->Belt.Option.getWithDefault("popup-default")}`
    onClick=pressClose>
    <div className="pop-up-container" onClick={e => ReactEvent.Mouse.stopPropagation(e)}>
      <div className="pop-up-body"> children </div>
    </div>
  </div>
