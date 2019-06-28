let component = ReasonReact.statelessComponent("PopupRe");

let make = (~loading: bool, ~onSubmit: ReactEvent.Form.t => unit, ~setEmail, ~className: option(string)=?, _children) => {
  ...component,
  render: _self =>
    <form className={"form form-forgot " ++ Js.Option.getWithDefault("", className)} onSubmit>
      <div className="section-forgot content -text-center">
        <input
          disabled=loading
          className="login-filled"
          placeholder="Email"
          onChange={e => setEmail(ReactEvent.Form.target(e)##value)}
          type_="email"
        />
        <input disabled=loading className="btn-filled-blue btn-line" type_="submit" value="Reset Password" />
      </div>
    </form>,
};
