open ReasonReact;

let str = ReasonReact.string;

type month =
  | Jan
  | Feb
  | Mar
  | Apr
  | May
  | Jun
  | Jul
  | Aug
  | Sep
  | Oct
  | Nov
  | Dec;

type day =
  | Sun
  | Mon
  | Tue
  | Wed
  | Thu
  | Fri
  | Sat;

let months = [Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec];
let days = [Sun, Mon, Tue, Wed, Thu, Fri, Sat];

let mapDayStr = day =>
  switch (day) {
  | Sun => "Sun"
  | Mon => "Mon"
  | Tue => "Tue"
  | Wed => "Wed"
  | Thu => "Thu"
  | Fri => "Fri"
  | Sat => "Sat"
  };
let mapMonthStr = month =>
  switch (month) {
  | Jan => "Jan"
  | Feb => "Feb"
  | Mar => "Mar"
  | Apr => "Apr"
  | May => "May"
  | Jun => "Jun"
  | Jul => "Jul"
  | Aug => "Aug"
  | Sep => "Sep"
  | Oct => "Oct"
  | Nov => "Nov"
  | Dec => "Dec"
  };
type loadState =
  | Idle
  | Loading
  | Succeed
  | Failed;

type state = {
  loadState,
  month,
  year: float,
};

type action =
  | NextMonth
  | PreviousMonth
  | OnChangeMonth(month);

let getMonthFloat = month => months |> Array.of_list |> Js.Array.indexOf(month) |> float_of_int;

let getMonthType = idx =>
  switch (Js.List.nth(months, idx |> int_of_float)) {
  | None => Jan
  | Some(month) => month
  };

let getDates = () => {
  Js.Date.makeWithYM(~year=2019.0, ~month=6.0) |> Js.log2("date");
};

let getLastDateObj = [%bs.raw {|
function f (year, month){
  var date = new Date(year, month, 0);
  return date
}
|}];

let getLastDate = (~year, ~realMonth) => getLastDateObj(year, realMonth) |> Js.Date.getDate;

let dates = (month, year) => {
  let col = 7;
  let row = 6;
  let boxs = col * row;
  let lastDate =
    getLastDate(
      ~year,
      ~realMonth={
        month +. 1.;
      },
    )
    |> int_of_float;
  let startDayInWeek = Js.Date.makeWithYM(~year, ~month, ()) |> Js.Date.getDay |> int_of_float;
  let emptyEl = key => <td key />;

  Array.make(boxs, null)
  |> Array.mapi((idx, _) =>
       if (idx < startDayInWeek || idx - startDayInWeek + 1 > lastDate) {
         idx |> string_of_int |> emptyEl;
       } else {
         <td> {idx - startDayInWeek + 1 |> string_of_int |> str} </td>;
       }
     )
  |> (
    arr => {
      Array.make(row, null)
      |> Array.mapi((idx, _) => {
           let end_ = (idx + 1) * col;
           let start = idx > 0 ? idx * col : idx;
           <tr> {arr |> Js.Array.slice(~start, ~end_) |> array} </tr>;
         });
    }
  )
  |> array;
};

let component = ReasonReact.reducerComponent("Calendar");

let make = _children => {
  ...component,
  initialState: () => {
    loadState: Idle,
    month: Js.Date.make() |> Js.Date.getMonth |> getMonthType,
    year: Js.Date.make() |> Js.Date.getFullYear,
  },
  reducer: (action, state) => {
    switch (action) {
    | NextMonth =>
      switch (state.month) {
      | Dec => Update({...state, month: Jan, year: state.year +. 1.})
      | _ =>
        Update({
          ...state,
          month: {
            state.month |> getMonthFloat |> (idx => idx +. 1.) |> getMonthType;
          },
        })
      }
    | PreviousMonth =>
      switch (state.month) {
      | Jan => Update({...state, month: Dec, year: state.year -. 1.})
      | _ =>
        Update({
          ...state,
          month: {
            state.month |> getMonthFloat |> (idx => idx -. 1.) |> getMonthType;
          },
        })
      }
    | OnChangeMonth(month) => Update({...state, month})
    };
  },
  render: ({state, send}) =>
    <div className="container">
      <div className="row">
        <h1>
          <span className="icon-click" onClick=(_ => send(PreviousMonth))> {{j|<|j} |> str} </span>
          {state.month |> mapMonthStr |> str}
          <span className="icon-click" onClick=(_ => send(NextMonth))> {{j|>|j} |> str} </span>
        </h1>
        <h1 className="col">{state.year |> Js.Float.toString |> str}</h1>
      </div>
      <table className="table">
        <thead>
          <tr> {days |> List.map(day => <th> {day |> mapDayStr |> str} </th>) |> Array.of_list |> array} </tr>
        </thead>
        <tbody> {dates(state.month |> getMonthFloat, state.year)} </tbody>
      </table>
    </div>,
};
