
let changeText = (status: bool) =>
  status ? "" /* TODO: After reveal text */ : ">" /* TODO: Before reveal text */

@react.component
let make = (~before: string, ~after: string, ~className: option<string>=?) => {
  let (show, setShow) = React.useState(_ => false)

  <div className={"reveal " ++ className->Belt.Option.getWithDefault("")}>
    <div key="before" className="reveal-before" dangerouslySetInnerHTML={"__html": before} />
    <div
      key="after"
      className=`reveal-after ${show ? "reveal-after-open" : ""}`
      dangerouslySetInnerHTML={"__html": after}
    />
    <button onClick={_e => setShow(_ => !show)}> {React.string(changeText(show))} </button>
  </div>
}
