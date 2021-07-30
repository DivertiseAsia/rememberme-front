open ReasonReact
open RememberMeApi
open Utils

let str = ReasonReact.string

type email = string
type password = string
type confirmPassword = password
type firstName = string
type lastName = string
type birthDate = string

type changePassState = RememberMeType.apiState<string>
type updateProfileState = RememberMeType.apiState<string>

type state = {
  profile: profile,
  email: email,
  oldPassword: string,
  password: password,
  confirmPassword: confirmPassword,
  changePassState: changePassState,
  updateProfileState: updateProfileState,
}

type action =
  | SetProfile(profile)
  | SetEmail(string)
  | SetOldPassword(string)
  | SetPassword(string)
  | SetConfirmPassword(string)
  | SetChangePassState(changePassState)
  | SetUpdateProfileState(updateProfileState)

let reducer = (state, action) =>
  switch action {
  | SetProfile(profile) => {...state, profile: profile}
  | SetEmail(email) => {...state, email: email}
  | SetOldPassword(oldPassword) => {...state, oldPassword: oldPassword}
  | SetPassword(password) => {...state, password: password}
  | SetConfirmPassword(confirmPassword) => {...state, confirmPassword: confirmPassword}
  | SetChangePassState(changePassState) => {...state, changePassState: changePassState}
  | SetUpdateProfileState(updateProfileState) => {...state, updateProfileState: updateProfileState}
  }

let getClassName = (~extraStyle="", ~invalid=false, ()) => {
  let invalidStyle = invalid ? "invalid" : ""
  j`form-control $extraStyle $invalidStyle`
}

@react.component
let make = (~profile: profile) => {
  let (state, dispatch) = React.useReducer(
    reducer,
    {
      profile: profile,
      email: profile.email,
      oldPassword: "",
      password: "",
      confirmPassword: "",
      changePassState: NotLoaded,
      updateProfileState: NotLoaded,
    },
  )

  let changePassword = () => {
    Loading->SetChangePassState->dispatch

    RequestUtils.requestJsonResponseToAction(
      ~headers=RequestUtils.buildHeader(
        ~verb=Post,
        ~body=RememberMeType.Encode.changePassword(
          ~oldPassword=state.oldPassword,
          ~password=state.password,
          ~confirmPassword=state.confirmPassword,
        ),
        Utils.getToken(),
      ),
      ~url=URL.password,
      ~successAction=_json => Loaded("Password Updated !")->SetChangePassState->dispatch,
      ~failAction=json => json->Utils.getResponseMsgFromJson->Failed->SetChangePassState->dispatch,
    ) |> ignore
  }

  let updateProfile = () => {
    Loading->SetUpdateProfileState->dispatch
    RememberMeApi.updateProfile(
      ~profile=state.profile,
      ~successAction=_ => Loaded("Profile Updated !")->SetUpdateProfileState->dispatch,
      ~failAction=msg => msg->Failed->SetUpdateProfileState->dispatch,
    )
  }

  let isCanChangePassword =
    state.changePassState !== Loading &&
      (state.oldPassword !== "" &&
      (checkPassword(state.password) &&
      checkIsSamePassword(~confirmPassword=state.confirmPassword, ~password=state.password)))

  let isCanUpdateProfile = state.profile != profile
  let buttonDisabled = !(isCanUpdateProfile || isCanChangePassword)

  let handleSubmit = () => {
    if isCanChangePassword {
      changePassword()
    }
    if isCanUpdateProfile {
      updateProfile()
    }
  }

  <div className="row">
    <div className="col-sm-9 col-md-9 col-lg-9 mx-auto signup-container">
      {state.changePassState === Loading ? <Loading /> : null}
      <form id="signup-form">
        <div className="row justify-content-between">
          <div className="col-12 pl-0"> <h6> {string("Profile Detail")} </h6> </div>
          <div className="col-6 pl-0">
            <input
              type_="text"
              id="firstname"
              className={getClassName(
                ~extraStyle="form-control-smaller mr-2",
                ~invalid=state.profile.firstName |> isStringEmpty,
                (),
              )}
              placeholder="First Name"
              value=state.profile.firstName
              onChange={e => {...state.profile, firstName: e->valueFromEvent}->SetProfile->dispatch}
            />
          </div>
          <div className="col-6 pr-0">
            <input
              type_="text"
              id="lastname"
              className={getClassName(
                ~extraStyle="form-control-smaller mr-2",
                ~invalid=state.profile.lastName |> isStringEmpty,
                (),
              )}
              placeholder="Last Name"
              value=state.profile.lastName
              onChange={e => {...state.profile, lastName: e->valueFromEvent}->SetProfile->dispatch}
            />
          </div>
          <input
            type_="email"
            id="inputEmail"
            className={getClassName(~invalid=!checkEmail(state.profile.email), ())}
            placeholder="Your Email"
            value=state.profile.email
            onChange={e => {...state.profile, email: e->valueFromEvent}->SetProfile->dispatch}
          />
          <small> {string("Birthday")} </small>
          <input
            type_="date"
            id="birthday"
            className={getClassName(
              ~extraStyle="form-control-smaller",
              ~invalid=state.profile.birthDate->Utils.Date.dateInputValue->isStringEmpty,
              (),
            )}
            placeholder="Birthday"
            value={state.profile.birthDate->Utils.Date.dateInputValue}
            onChange={e =>
              {...state.profile, birthDate: e->valueFromEvent->Js.Date.fromString}
              ->SetProfile
              ->dispatch}
          />
          <div className="col-12 py-4 px-0">
            <h6> {string("Change Password")} </h6>
            <input
              type_="password"
              className={getClassName(
                ~extraStyle="form-control-small",
                ~invalid=!checkPassword(state.password),
                (),
              )}
              placeholder="Old Password"
              value=state.oldPassword
              onChange={e => dispatch(SetOldPassword(e |> valueFromEvent))}
            />
            <input
              type_="password"
              className={getClassName(
                ~extraStyle="form-control-small",
                ~invalid=!checkPassword(state.password),
                (),
              )}
              placeholder="New Password"
              value=state.password
              onChange={e => dispatch(SetPassword(e |> valueFromEvent))}
            />
            <input
              type_="password"
              className={getClassName(
                ~extraStyle="form-control-small",
                ~invalid=!checkIsSamePassword(
                  ~password=state.password,
                  ~confirmPassword=state.confirmPassword,
                ),
                (),
              )}
              placeholder="Confirm New Password"
              value=state.confirmPassword
              onChange={e => dispatch(SetConfirmPassword(e |> valueFromEvent))}
            />
          </div>
        </div>
        <div className="error-message">
          {switch state.changePassState {
          | Loaded(msg) => <p> {msg->str} </p>
          | Failed(messages) => messages->Utils.errorMessagesEl->array
          | _ => null
          }}
          {switch state.updateProfileState {
          | Loaded(msg) => <p> {msg->str} </p>
          | Failed(messages) => messages->Utils.errorMessagesEl->array
          | _ => null
          }}

          {state.confirmPassword->Js.String.length > 0 &&
            !(state.confirmPassword === state.password)
            ? <p id="error_confirm_password">
                {"The password and confirmation password are mismatch." |> str}
              </p>
            : null}
          {state.password->Js.String.length > 0 && !checkPassword(state.password)
            ? <p id="error_password">
                {"The password must be at least 8 characters long." |> str}
              </p>
            : null}
        </div>
        <button
          id="profile_submit_btn"
          className="btn btn-blue btn-submit mb-5"
          disabled=buttonDisabled
          onClick={e => {
            ReactEvent.Mouse.preventDefault(e) //TODO: Remove this line?
            handleSubmit()
          }}>
          {"Confirm" |> str}
        </button>
      </form>
    </div>
  </div>
}
