@react.component
let make = (
  ~loading: bool,
  ~onSubmit: ReactEvent.Form.t => unit,
  ~setOldPassword,
  ~setNewPassword,
  ~setConfPassword,
  ~className: option<string>=?,
) =>
  <form className={"form form-reset " ++ Js.Option.getWithDefault("", className)} onSubmit>
    <div className="section-reset content -text-center">
      <input
        disabled=loading
        className="login-filled"
        placeholder="Old Password"
        onChange={e => setOldPassword(ReactEvent.Form.target(e)["value"])}
        type_="password"
      />
      <input
        disabled=loading
        className="login-filled"
        placeholder="New Password"
        onChange={e => setNewPassword(ReactEvent.Form.target(e)["value"])}
        type_="password"
      />
      <input
        disabled=loading
        className="login-filled"
        placeholder="New Password Again"
        onChange={e => setConfPassword(ReactEvent.Form.target(e)["value"])}
        type_="password"
      />
      <input
        disabled=loading className="btn-filled-blue btn-line" type_="submit" value="Change Password"
      />
    </div>
  </form>
