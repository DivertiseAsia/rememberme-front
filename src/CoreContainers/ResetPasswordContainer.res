open ReasonReact
open Utils

type apiState = RememberMeType.apiState<bool>
type email = string
type password = string

type state = {
  email: email,
  newPassword: password,
  confirmNewPassword: password,
  apiState: apiState,
}
type action =
  | SetEmail(email)
  | SetNewPassword(password)
  | SetConfirmNewPassword(password)
  | SetApiState(apiState)

@react.component
let make = () => {
  let querystring = ReasonReactRouter.useUrl().search
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | SetEmail(email) => {...state, email: email}
    | SetNewPassword(newPassword) => {...state, newPassword: newPassword}
    | SetConfirmNewPassword(confirmNewPassword) => {
        ...state,
        confirmNewPassword: confirmNewPassword,
      }
    | SetApiState(apiState) => {...state, apiState: apiState}
    }
  , {
    email: querystring->Links.getValueFromQuerystring(~key="email")->Belt.Option.getWithDefault(""),
    newPassword: "",
    confirmNewPassword: "",
    apiState: RememberMeType.NotLoaded,
  })

  let onSubmit = e => {
    ReactEvent.Form.preventDefault(e)
    Loading->SetApiState->dispatch
    RememberMeApi.forgotPassword(
      ~email=state.email,
      ~successAction={_ => RememberMeType.Loaded(true)->SetApiState->dispatch},
      ~failAction={err => RememberMeType.Failed(err)->SetApiState->dispatch},
    )
  }

  switch state.apiState {
  | Loading => <Loading />
  | _ =>
    <div className="login-container ">
      <form className="form-group form-login " onSubmit>
        <div className="section-login content -text-center">
          <small> {string("Email")} </small>
          <input
            disabled=true
            required=true
            className="form-control"
            type_="email"
            id="inputEmail"
            placeholder="Your Email"
            autoComplete="email"
            value=state.email
          />
          <input
            required=true
            className="login-filled"
            placeholder="New Password"
            onChange={e => e->Utils.getValueOfForm->SetNewPassword->dispatch}
            type_="password"
            value=state.newPassword
          />
          <input
            required=true
            className="login-filled"
            placeholder="New Password Again"
            onChange={e => e->Utils.getValueOfForm->SetConfirmNewPassword->dispatch}
            type_="password"
            value=state.confirmNewPassword
          />
          <input
            className="btn-filled-blue btn-line btn-login btn-main-color"
            type_="submit"
            value="Reset Password"
          />
        </div>
      </form>
      {switch state.apiState {
      | Loaded(true) =>
        <div className="text-info"> {string("Your password have been change.")} </div>
      | Failed(x) when x != "" => x |> getErrorMsgFromJson |> array
      | _ => ReasonReact.null
      }}
    </div>
  }
}
