open ReasonReact
open Utils

@react.component
let make = (
  ~loading: bool,
  ~onSubmit: ReactEvent.Form.t => unit,
  ~setEmail,
  ~setPassword,
  ~className: option<string>=?,
) =>
  <form className={"form-group form-login " ++ Belt.Option.getWithDefault(className, "")} onSubmit>
    <div className="section-login content -text-center">
      <small> {string("Username")} </small>
      <input
        disabled=loading
        className="form-control"
        placeholder="Username"
        onChange={e => e |> getValueOfForm |> setEmail}
        type_="text"
      />
      <input
        disabled=loading
        className="form-control"
        placeholder="Password"
        onChange={e => e |> getValueOfForm |> setPassword}
        type_="password"
        required=true
      />
      <input
        disabled=loading
        className="btn-filled-blue btn-line btn-login btn-main-color"
        type_="submit"
        value="Login"
      />
    </div>
  </form>
