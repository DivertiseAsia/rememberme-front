//open Css;

let revealAfter = ""
//  style([ TODO:
//    display(`block),
//    maxHeight(`zero),
//    overflow(`hidden),
//    transition("max-height", ~duration=500),
//  ]);

let revealAfterOpen = ""
//style([maxHeight(`vw(1000.))]); TODO:

let changeText = (status: bool) =>
  status ? "" /* TODO: After reveal text */ : ">" /* TODO: Before reveal text */

@react.component
let make = (~before: string, ~after: string, ~className: option<string>=?) => {
  let (show, setShow) = React.useState(_ => false)
  let extraClass = show ? revealAfterOpen : ""

  <div className={"reveal " ++ className->Belt.Option.getWithDefault("")}>
    <div key="before" className="reveal-before" dangerouslySetInnerHTML={"__html": before} />
    <div
      key="after"
      className={"reveal-after " ++ (revealAfter ++ (" " ++ extraClass))}
      dangerouslySetInnerHTML={"__html": after}
    />
    <button onClick={_e => setShow(_ => !show)}> {ReasonReact.string(changeText(show))} </button>
  </div>
}
