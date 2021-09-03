type date = int
type cell = Today(date) | CurrentMonth(date) | NotCurrentMonth(date)

@react.component
let make = (~extraClassName="", ~cell: cell, ~children=?) => {
  let className =
    "day" ++
    {
      switch cell {
      | NotCurrentMonth(_) => ` not-current-month ${extraClassName}`
      | Today(_) =>` today ${extraClassName}`
      | _ => ` ${extraClassName}`
      }
    }
    
  <td className style={ReactDOM.Style.make(~paddingTop="20px", ())}>
    {switch cell {
    | NotCurrentMonth(date) => <span> {date->Belt.Int.toString->React.string} </span>
    | CurrentMonth(date) | Today(date) => 
    <>
        <div className="circle-today" />
        <span className={String.length(date |> string_of_int) === 1 ? "single-char" : ""}>
          {date->Belt.Int.toString->React.string}
        </span>
      </>
    }}
    {children->Belt.Option.getWithDefault(React.null)}
  </td>
}
