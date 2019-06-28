let component = ReasonReact.statelessComponent("PopupRe");

let make =
    (
      ~loading: bool,
      ~onSubmit: ReactEvent.Form.t => unit,
      ~setEmail,
      ~setPassword,
      ~className: option(string)=?,
      _children,
    ) => {
  ...component,
  render: _self =>
    <form className={"form form-login " ++ Js.Option.getWithDefault("", className)} onSubmit>
      <div className="section-login content -text-center">
        <input
          disabled=loading
          className="login-filled"
          placeholder="Email"
          onChange={e => setEmail(ReactEvent.Form.target(e)##value)}
          type_="email"
        />
        <input
          disabled=loading
          className="login-filled"
          placeholder="Password"
          onChange={e => setPassword(ReactEvent.Form.target(e)##value)}
          type_="password"
          required=true
        />
        <input disabled=loading className="btn-filled-blue btn-line" type_="submit" value="Login" />
      </div>
    </form>,
};
