open ReasonReact
open RememberMeType

let requestMenus = list{Sick, Vacation}

let months: list<month> = list{Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec}
let years: list<float> = list{
  (Js.Date.now() |> Js.Date.fromFloat |> Js.Date.getFullYear) -. 1.,
  Js.Date.now() |> Js.Date.fromFloat |> Js.Date.getFullYear,
  (Js.Date.now() |> Js.Date.fromFloat |> Js.Date.getFullYear) +. 1.,
}

type state = {
  loadState: loadState,
  formType: formType,
  startDate: float,
  endDate: float,
  note: string,
  isRemote: bool,
  showPopup: bool,
}

type action =
  | ChangeFormType(formType)
  | TogglePopup(bool)
  | ChangeStartDate(float)
  | ChangeEndDate(float)
  | ChangeNote(string)
  | ChangeIsRemote(bool)
  | SetLoadState(loadState)
  | OnSubmitRequestLeaveSuccess
  | OnSubmitRequestLeaveFailed(string)

let dateForm = (
  ~schedules=list{},
  ~title="Start",
  ~targetDate,
  ~targetMonth,
  ~targetYear,
  ~onChangeDate,
) => <>
  <div className="col-12 col-md-2 pt-3"> {string(title)} </div>
  <div className="col-3 col-md-3 p-1">
    <div className="dropdown show dropdown-request-leave">
      {cloneElement(
        <a className="btn dropdown-toggle" role="button" id="dropdown-day">
          {string(targetDate |> int_of_float |> string_of_int)}
        </a>,
        ~props={
          "data-toggle": "dropdown",
          "aria-haspopup": "true",
          "aria-expanded": "false",
        },
        [],
      )}
      <div
        className="dropdown-menu"
        ariaLabelledby="dropdown-day"
        style={ReactDOM.Style.make(~maxHeight="200px", ~overflowY="auto", ())}>
        {Array.make(31, null)
        |> Array.mapi((idx, _) => {
          let today =
            Js.Date.make()
            |> (date =>
              Js.Date.makeWithYMD(
                ~year=date |> Js.Date.getFullYear,
                ~month=date |> Js.Date.getMonth,
                ~date=date |> Js.Date.getDate,
                (),
              ))
            |> Js.Date.valueOf
          let datetime = Js.Date.makeWithYMD(
            ~year=targetYear,
            ~month=targetMonth,
            ~date=idx + 1 |> float_of_int,
            (),
          )
          let isLeaveDay = switch schedules |> List.find(schedule =>
            schedule.date === (datetime |> Js.Date.valueOf)
          ) {
          | _leaveDay => true
          | exception Not_found => false
          }
          let isValidate =
            datetime |> Js.Date.getMonth === targetMonth &&
              (datetime |> Js.Date.valueOf >= today &&
              (!RememberMeUtils.validateWeekend(datetime) && !isLeaveDay))
          isValidate
            ? <a 
                key=`dropdown-item-date-${idx->Belt.Int.toString}-${datetime->Js.Date.toLocaleString}` 
                className="dropdown-item" 
                onClick={_ => onChangeDate(datetime |> Js.Date.valueOf)}>
                  {string(idx + 1 |> string_of_int)}
              </a>
            : null
        })
        |> array}
      </div>
    </div>
  </div>
  <div className="col-6 col-md-4 p-1">
    <div className="dropdown show dropdown-request-leave">
      {cloneElement(
        <a className="btn dropdown-toggle" role="button" id="dropdown-month">
          {string(targetMonth |> int_of_float |> RememberMeUtils.mapFullMonthInt)}
        </a>,
        ~props={
          "data-toggle": "dropdown",
          "aria-haspopup": "true",
          "aria-expanded": "false",
        },
        [],
      )}
      <div className="dropdown-menu" ariaLabelledby="dropdown-month">
        {months
        |> List.mapi((i, month) =>
          <a
            key={"input-month-" ++ (i |> string_of_int)}
            className="dropdown-item"
            onClick={_ => {
              let datetime = Js.Date.makeWithYMD(
                ~year=targetYear,
                ~month=i |> float_of_int,
                ~date=targetDate,
                (),
              )
              onChangeDate(datetime |> Js.Date.valueOf)
            }}>
            {string(month |> RememberMeUtils.mapFullMonthStr)}
          </a>
        )
        |> Array.of_list
        |> array}
      </div>
    </div>
  </div>
  <div className="col-3 col-md-3 p-1">
    <div className="dropdown show dropdown-request-leave">
      {cloneElement(
        <a className="btn dropdown-toggle" role="button" id="dropdown-year">
          {string(targetYear |> int_of_float |> string_of_int)}
        </a>,
        ~props={
          "data-toggle": "dropdown",
          "aria-haspopup": "true",
          "aria-expanded": "false",
        },
        [],
      )}
      <div className="dropdown-menu" ariaLabelledby="dropdown-year">
        {years
        |> List.mapi((i, year) =>
          <a
            key={"input-year-" ++ (i |> string_of_int)}
            className="dropdown-item"
            onClick={_ => {
              let datetime = Js.Date.makeWithYMD(~year, ~month=targetMonth, ~date=targetDate, ())
              onChangeDate(datetime |> Js.Date.valueOf)
            }}>
            {string(year |> int_of_float |> string_of_int)}
          </a>
        )
        |> Array.of_list
        |> array}
      </div>
    </div>
  </div>
</>

@react.component
let make = (~schedules, ~onRefresh) => {
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | ChangeFormType(formType) => {...state, formType: formType}
    | TogglePopup(showPopup) => {...state, showPopup: showPopup}
    | ChangeStartDate(startDate) => {
        ...state,
        startDate: startDate,
        endDate: startDate > state.endDate ? startDate : state.endDate,
      }
    | ChangeEndDate(endDate) => {
        ...state,
        endDate: endDate,
        startDate: state.startDate > endDate ? endDate : state.startDate,
      }
    | ChangeNote(note) => {...state, note: note}
    | ChangeIsRemote(isRemote) => {...state, isRemote: isRemote}
    | SetLoadState(loadState) => {...state, loadState: loadState}
    | OnSubmitRequestLeaveSuccess => {
        ...state,
        loadState: Succeed,
        startDate: Js.Date.now(),
        endDate: Js.Date.now(),
        note: "",
        isRemote: false,
        showPopup: true,
      }
    | OnSubmitRequestLeaveFailed(errMsg) => {
        ...state,
        loadState: Failed(errMsg),
        note: "",
        isRemote: false,
        showPopup: true,
      }
    }
  , {
    loadState: Idle,
    formType: Sick,
    startDate: Js.Date.now(),
    endDate: Js.Date.now(),
    note: "",
    isRemote: false,
    showPopup: false,
  })

  let onSubmit = () => {
    let payload = Encode.leaveRequest(
      ~formType=state.formType,
      ~fromDate=state.startDate,
      ~toDate=state.endDate,
      ~reason=state.note,
      ~isRemote=state.isRemote,
    )

    Loading->SetLoadState->dispatch

    RememberMeApi.postLeave(
      ~token=Utils.getToken(),
      ~payload,
      ~successAction=_ => dispatch(OnSubmitRequestLeaveSuccess),
      ~failAction=json => json->Json.stringify->OnSubmitRequestLeaveFailed->dispatch,
    )
  }
  <div className="container mt-4 request-form-container">
    <div className="row"> <p> {string("Create Form")} </p> </div>
    <form
      className="row justify-content-center pt-5 pb-2"
      style={ReactDOM.Style.make(~backgroundColor="white", ~borderTop="2px solid #FFA227", ())}
      onSubmit={e => {ReactEvent.Form.preventDefault(e); onSubmit()}}
      >
      {requestMenus
      |> List.map((menu: formType) =>
        <div
          key={"form-type-" ++ (menu |> RememberMeUtils.requestMenuToStr)}
          className="col-6 col-sm-6 col-md-6 col-lg-5 col-xl-4">
          <button
            type_="button"
            className={"btn btn-rounded btn-form-" ++ (
              state.formType === menu ? " btn-form-active" : ""
            )}
            onClick={_ => dispatch(ChangeFormType(menu))}>
            <img
              className="form-icon mr-2"
              src={switch (menu, state.formType === menu) {
              | (Sick, true) => "/images/sick.svg"
              | (Sick, false) => "/images/sick_blue.svg"
              | (Vacation, true) => "/images/vacation.svg"
              | _ => "/images/vacation_blue.svg"
              }}
            />
            {string(menu |> RememberMeUtils.requestMenuToStr)}
          </button>
        </div>
      )
      |> Array.of_list
      |> array}
      <div className="col-12">
        <div className="row mt-5 pl-2 pr-2">
          <div className="timeline-point timeline-start" />
          {dateForm(
            ~schedules,
            ~title="Start",
            ~targetDate=state.startDate |> Js.Date.fromFloat |> Js.Date.getDate,
            ~targetMonth=state.startDate |> Js.Date.fromFloat |> Js.Date.getMonth,
            ~targetYear=state.startDate |> Js.Date.fromFloat |> Js.Date.getFullYear,
            ~onChangeDate=startDate => dispatch(ChangeStartDate(startDate)),
          )}
        </div>
        <div className="row mt-4 mb-5 pl-2 pr-2">
          <div className="timeline-point timeline-end" />
          {dateForm(
            ~schedules,
            ~title="End",
            ~targetDate=state.endDate |> Js.Date.fromFloat |> Js.Date.getDate,
            ~targetMonth=state.endDate |> Js.Date.fromFloat |> Js.Date.getMonth,
            ~targetYear=state.endDate |> Js.Date.fromFloat |> Js.Date.getFullYear,
            ~onChangeDate=startDate => dispatch(ChangeEndDate(startDate)),
          )}
        </div>
        <div className="row mt-4">
          <div className="col-12 col-md-4 col-note"> {string("Note:")} </div>
          <div className="col-12 col-md-8">
            <textarea
              rows=2
              style={ReactDOM.Style.make(~width="100%", ())}
              onChange={e => dispatch(ChangeNote(Utils.valueFromEvent(e)))}
            />
          </div>
        </div>
        <div className="row mt-3">
          <div className="col-12 col-md-4 col-note"> {string("Remote:")} </div>
          <div className="col-12 col-md-8">
            <input
              type_="checkbox" onInput={e => dispatch(ChangeIsRemote(Utils.boolFromCheckbox(e)))}
            />
          </div>
        </div>
        /* {
            switch state.formType {
            | Sick =>
              <div className="row mt-4">
                <div className="col-12">
                  <input
                    type_="file"
                    name="file"
                    id="file"
                    className="inputfile"
                    onChange=(e => Js.log(e |> Utils.filesFromEvent))
                    style=(ReactDOM.Style.make(~display="none", ()))
                  />
                  <label
                    htmlFor="file"
                    style=(ReactDOM.Style.make(
                      ~width="100%",
                      ~padding="5px",
                      ~border="1px solid rgba(0,0,0,0.3)",
                      ()))
                  >
                    <div className="row">
                      <div className="col-4 text-center">
                        <img className="m-auto" src="/images/upload.svg" style=(ReactDOM.Style.make(~width="80%", ())) />
                      </div>
                      <div className="col-8 p-0">
                        <p className="mb-1">{string("Please, Upload the Medical certificate.")}</p>
                        <p className="mb-1">{string("Description..")}</p>
                      </div>
                    </div>
                  </label>
                </div>
              </div>
            | _ => null
            }
          }*/
        <div className="row mt-5">
          <button
            type_="submit"
            className="btn btn-rounded btn-form- btn-form-active m-auto"
            // onClick={_ => onSubmit()}
            style={ReactDOM.Style.make(~maxWidth="120px", ())}>
            {string("Submit")}
          </button>
        </div>
        {switch state.formType {
        | Vacation =>
          <div className="row mt-3">
            <div className="col"> <img src="/images/coconut.svg" /> </div>
          </div>
        | _ => null
        }}
      </div>
    </form>
    {state.showPopup
      ? <PopupRequestForm
          isSick={state.formType === Sick}
          loadState=state.loadState
          onConfirm={_ => {
            dispatch(TogglePopup(false))
            switch state.loadState {
            | Failed(_) => ()
            | _ => onRefresh()
            }
          }}
        />
      : null}
  </div>
}
