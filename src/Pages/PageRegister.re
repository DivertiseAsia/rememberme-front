open ReasonReact;

let component = ReasonReact.statelessComponent("PageRegister");

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g");

let make = _children => {
  ...component,
  render: _self => {
    <div className="auth-page login-page">
      <h1> {"Remember Me" |> string} </h1>
      <RegisterContainer />
      <div className="to-login">
        <p id="signup_logintext"> {"Already have an account? " |> string} </p>
        <a id="signup_login" href=Links.login className="text-blue"> {"Go to Log In." |> string} </a>
      </div>
    </div>;
  },
};
