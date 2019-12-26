open ReasonReact;

let str = ReasonReact.string;

let months = [0,1,2,3,4,5,6,7,8,9,10,11];

let years = [
  (Js.Date.make() |> Js.Date.getFullYear) -. 2.,
  (Js.Date.make() |> Js.Date.getFullYear) -. 1.,
  (Js.Date.make() |> Js.Date.getFullYear),
  (Js.Date.make() |> Js.Date.getFullYear) +. 1.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 2.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 3.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 4.,
  (Js.Date.make() |> Js.Date.getFullYear) +. 5.,
];

type state = {
  openDropdown: bool,
  targetYear: float,
};

type action =
  | ToggleDropdown(bool)
  | ChangeYear(float);

let component = ReasonReact.reducerComponent("PageAllMonth");

let make = (_children) => {
  ...component,
  initialState: () => {openDropdown: false, targetYear: (Js.Date.make() |> Js.Date.getFullYear)},
  reducer: (action, state) => {
    switch (action) {
    | ToggleDropdown(openDropdown) => Update({...state, openDropdown})
    | ChangeYear(targetYear) => Update({...state, targetYear})
    };
  },
  render: ({state, send}) =>
    <div className="allmonth-page container-fluid">
      <div className="row">
        <div className="col mb-5 mt-4">
          <button 
            type_="button" 
            className="btn btn-rounded btn-main-color active-menu pl-4 pr-4 dropwdown-year"
            onClick=(_ => send(ToggleDropdown(!state.openDropdown)))
          >
            <img src="/images/calendar.svg" style=(ReactDOMRe.Style.make(~width="35px", ~height="35px", ())) />
            {string(" " ++ (state.targetYear |> Js.Float.toString))}
          </button>
          (state.openDropdown ?
            <>
            <div className="dropdown-items-bg" onClick=(_ => send(ToggleDropdown(false))) />
            <div className="dropdown-items">
              {
                years |> List.mapi((i, year) => {
                  <div className="dropdown-item" 
                    onClick=(_ => {
                      send(ChangeYear(year));
                      send(ToggleDropdown(false));
                    })
                  >
                    {string(year |> int_of_float |> string_of_int)}
                  </div>
                }) |> Array.of_list |> array
              }
            </div>
            </> : null
          )
        </div>
      </div>
      <div className="row mt-2">
        {
          months |> List.mapi((i, month) => {
            <div key=("month-" ++ (i |> string_of_int)) className="col-12 col-sm-12 col-md-6 col-lg-4 col-xl-4 mb-3"> 
              <Calendar isMini=true month=month year=state.targetYear />
            </div>
          }) |> Array.of_list |> array
        }
      </div>
    </div>,
};
