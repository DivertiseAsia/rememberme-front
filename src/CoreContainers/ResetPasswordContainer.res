open ReasonReact
open Utils

type apiState = RememberMeType.apiState<bool>

@react.component
let make = () => {
  let (email, setEmail) = React.useState(_ => "")
  let (apiState, setApiState) = React.useState(_ => RememberMeType.NotLoaded)

  let onSubmit = e => {
    ReactEvent.Form.preventDefault(e)
    setApiState(_ => Loading)
    RememberMeApi.resetPassword(
      ~email,
      ~successAction={_ => setApiState(_ => RememberMeType.Loaded(true))},
      ~failAction={err => setApiState(_ => RememberMeType.Failed(err))},
    )
  }

  switch apiState {
  | Loading => <Loading />
  | _ =>
    <div className="login-container ">
      <form className="form-group form-login " onSubmit>
        <div className="section-login content -text-center">
          <small> {string("Email")} </small>
          <input
            autoFocus=true
            required=true
            className="form-control"
            type_="email"
            id="inputEmail"
            placeholder="Your Email"
            autoComplete="email"
            value=email
            onChange={e => {
              ReactEvent.Form.persist(e)
              setEmail(_ => e |> valueFromEvent)
            }}
          />
          <input
            className="btn-filled-blue btn-line btn-login btn-main-color"
            type_="submit"
            value="Reset Password"
          />
        </div>
      </form>
      {switch apiState {
      | Loaded(true) => <div className="text-info"> {string("Reset password email have been sent")} </div>
      | Failed(x) when x != "" => x |> getErrorMsgFromJson |> array
      | _ => ReasonReact.null
      }}
    </div>
  }
}
