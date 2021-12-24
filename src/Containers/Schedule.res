open ReasonReact
open RememberMeType
open RememberMeApi

let menus = list{All, Leave, Holiday, Event}

type contentForm =
  | Idle
  | RequestForm
  | MyForm

type state = {
  loadState: loadState,
  scheduleMenu: scheduleMenu,
  contentForm: contentForm,
  showPopover: bool,
}

type action =
  | ChangeScheduleMenu(scheduleMenu)
  | ChangeContentForm(contentForm)
  | TogglePopover(bool)

let findAllSchedules = (schedules, key, today) => {
  let lastEndDate = [0.]
  schedules
  |> List.filter((schedule: schedule) => {
    let yearFromSchedule = schedule.date->Js.Date.fromFloat->Js.Date.getFullYear
    let yearFromToday = today->Js.Date.fromFloat->Js.Date.getFullYear
    yearFromSchedule === yearFromToday
  })
  |> List.sort((schedule1: schedule, schedule2: schedule) =>
    compare(schedule1.date, schedule2.date)
  )
  |> List.mapi((i, schedule: schedule) =>
    switch schedule.date {
    | date when date > lastEndDate[0] =>
      let schedules = List.append(
        list{schedule},
        schedules |> List.find_all((s: schedule) =>
          s.title !== schedule.title && s.date === schedule.date
        ),
      )
      lastEndDate[0] = (schedules |> List.rev |> List.hd).date
      <div key={key ++ (i |> string_of_int)}>
        <SchedulerDate
          isToday={schedule.date === today}
          datetime={RememberMeUtils.getDatetimeStr(schedule.date)}
          schedules
        />
      </div>
    | _ => null
    }
  )
  |> Array.of_list
  |> array
}

@react.component
let make = (
  ~holidayList=list{},
  ~listBirthDay=list{},
  ~leaveList=list{},
  ~userLeaveList=list{},
  ~eventList=list{},
  ~onRefresh,
  ~profile,
) => {
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | ChangeScheduleMenu(scheduleMenu) => {...state, scheduleMenu: scheduleMenu}
    | ChangeContentForm(contentForm) => {...state, contentForm: contentForm}
    | TogglePopover(showPopover) => {...state, showPopover: showPopover}
    }
  , {
    loadState: Idle,
    scheduleMenu: All,
    contentForm: Idle,
    showPopover: false,
  })

  let leaveSchedules =
    leaveList
    |> List.map((requestLeave: leaveDetail) => requestLeave |> RememberMeUtils.splitRequestLeave)
    |> List.concat
    |> List.sort((schedule1: schedule, schedule2: schedule) =>
      compare(schedule1.date, schedule2.date)
    )
  <div
    className="container-fluid schedule-container"
    style={ReactDOM.Style.make(
      ~backgroundColor=switch state.contentForm {
      | Idle => ""
      | RequestForm => "#E6E6E6"
      | MyForm => "#E6E6E6"
      },
      (),
    )}>
    <div className="row schedule-row-menu">
      <div className="col-6">
        <div
          className="cursor-pointer"
          onClick={_ =>
            switch state.contentForm {
            | Idle => dispatch(ChangeContentForm(MyForm))
            | _ => dispatch(ChangeContentForm(Idle))
            }}>
          <img
            className="form-icon mr-2"
            src={switch state.contentForm {
            | Idle => "/images/form.svg"
            | _ => "/images/arrow_back.svg"
            }}
          />
          <span>
            {switch state.contentForm {
            | Idle => string("My Form")
            | _ => string("Back")
            }}
          </span>
        </div>
      </div>
      <div className="col-6 text-right">
        <span className="cursor-pointer" onClick={_ => dispatch(TogglePopover(!state.showPopover))}>
          {string(
            switch profile {
            | Some(p) => p.firstName
            | None => "-"
            },
          )}
        </span>
        {state.showPopover ? <Popover onClose={_ => dispatch(TogglePopover(false))} /> : null}
      </div>
    </div>
    {switch state.contentForm {
    | Idle => <>
        <div className="row schedule-row-content pt-4">
          <div className="col-12 menu-schedule-">
            <div className="row">
              {menus
              |> List.map((menu: scheduleMenu) =>
                <div
                  key={"menu-" ++ (menu |> RememberMeUtils.scheduleMenuToStr)}
                  className="col-3 p-2 text-center">
                  <button
                    type_="button"
                    className={"btn btn-rounded" ++ (
                      state.scheduleMenu === menu
                        ? " btn-main-color active-menu "
                        : " none-active-menu"
                    )}
                    onClick={_ => dispatch(ChangeScheduleMenu(menu))}>
                    {string(menu |> RememberMeUtils.scheduleMenuToStr)}
                  </button>
                </div>
              )
              |> Array.of_list
              |> array}
            </div>
          </div>
          <div className="col-12 content-schedule- p-0">
            {
              let today =
                (Js.Date.now() |> Js.Date.fromFloat |> RememberMeApi.getDateOnlyDate)
                  ->Js.Date.valueOf
              switch (state.scheduleMenu, listBirthDay, holidayList, leaveSchedules, eventList) {
              | (All, allBirthDay, allHoliday, allLeave, allEvent)
                when allHoliday !== list{} ||
                  (allBirthDay !== list{} ||
                  (allLeave !== list{} || allEvent !== list{})) =>
                let lastEndDate = [0.]
                let birthdaySchedules =
                  allBirthDay
                  |> List.filter(birthDay => birthDay.name !== "")
                  |> List.map(birthDay => birthDay |> RememberMeUtils.mapBirthDayToSchedule)
                let holidaySchedules =
                  allHoliday |> List.map(holiday => holiday |> RememberMeUtils.mapHolidayToSchedule)
                let eventSchedules =
                  allEvent |> List.map(event => event |> RememberMeUtils.mapEventToSchedule)
                let allSchedules = List.append(
                  List.append(eventSchedules, holidaySchedules),
                  List.append(leaveSchedules, birthdaySchedules),
                )
                allSchedules
                |> List.sort((schedule1: schedule, schedule2: schedule) =>
                  compare(schedule1.date, schedule2.date)
                )
                |> List.filter((schedule: schedule) => schedule.date >= today)
                |> List.mapi((i, schedule: schedule) =>
                  switch schedule.date {
                  | date when date > lastEndDate[0] =>
                    let datetime = switch schedule.scheduleMenu {
                    | Birthday =>
                      schedule.date |> RememberMeUtils.getDatetimeStr(~formCurrentYear=true)
                    | _ => RememberMeUtils.getDatetimeStr(schedule.date)
                    }
                    let schedules = List.append(
                      list{schedule},
                      allSchedules |> List.find_all((s: schedule) =>
                        s.date === schedule.date && s.title !== schedule.title
                      ),
                    )
                    lastEndDate[0] = (schedules |> List.rev |> List.hd).date
                    <div
                      key={"scheduler-all-" ++ (schedule.title ++ ("-" ++ (i |> string_of_int)))}>
                      <SchedulerDate isToday={schedule.date === today} datetime schedules />
                    </div>
                  | _ => null
                  }
                )
                |> Array.of_list
                |> array
              | (Leave, _, _, allLeave, _) when allLeave !== list{} => <>
                  {findAllSchedules(allLeave, "scheduler-leave-", today)}
                </>
              | (Holiday, _, allHoliday, _, _) when allHoliday !== list{} => <>
                  {
                    let holidaySchedules =
                      allHoliday |> List.map((holiday: holiday) =>
                        holiday |> RememberMeUtils.mapHolidayToSchedule
                      )
                    findAllSchedules(holidaySchedules, "scheduler-holiday-", today)
                  }
                </>
              | (Event, _, _, _, allEvent) when allEvent !== list{} => <>
                  {
                    let eventSchedules =
                      allEvent |> List.map((event: event) =>
                        event |> RememberMeUtils.mapEventToSchedule
                      )
                    findAllSchedules(eventSchedules, "scheduler-event-", today)
                  }
                </>
              | _ =>
                <div className="col-12 text-center pt-5 pb-5">
                  <p> {string("Data not found")} </p>
                </div>
              }
            }
          </div>
        </div>
        <button
          type_="button"
          className="btn btn-request"
          onClick={_ => dispatch(ChangeContentForm(RequestForm))}>
          <img className="requestform-icon" src="/images/request_form.svg" />
          {string("Request Form")}
        </button>
      </>
    | RequestForm =>
      let schedulesHoliday =
        holidayList |> List.map(holiday => holiday |> RememberMeUtils.mapHolidayToSchedule)
      let schedulesLeave = userLeaveList
      |> List.filter((requestLeave: leaveDetail) =>
        requestLeave.user ===
          switch profile {
          | Some(p) => p.firstName
          | None => ""
          }
      )
      |> List.map((requestLeave: leaveDetail) => requestLeave |> RememberMeUtils.splitRequestLeave)
      |> List.concat
      let schedules = List.append(schedulesHoliday, schedulesLeave)
      <RequestForm
        onRefresh={_ => {
          dispatch(ChangeContentForm(Idle))
          onRefresh()
        }}
        schedules
      />
    | MyForm => <RequestLeavePanel requestLeaves=userLeaveList onRefresh />
    }}
  </div>
}
