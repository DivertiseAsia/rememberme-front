open Utils;
let component = ReasonReact.statelessComponent("LoginForm");

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
          placeholder="Username"
          onChange={e => e |> getValueOfForm |> setEmail}
          type_="text"
        />
        <input
          disabled=loading
          className="login-filled"
          placeholder="Password"
          onChange={e => e |> getValueOfForm |> setPassword}
          type_="password"
          required=true
        />
        <input disabled=loading className="btn-filled-blue btn-line" type_="submit" value="Login" />
      </div>
    </form>,
};
