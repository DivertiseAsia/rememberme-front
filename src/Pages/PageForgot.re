open ReasonReact;
let component = ReasonReact.statelessComponent("PageForgot");

let make = _children => {
  ...component,
  render: _self =>
    <div className="auth-page forgot-page">
      <Logo className="auth-page_logo" />
      <h2 className="-text-space"> {string("Forgot Password")} </h2>
      <p> {string("Please enter your email, and we'll email you instructions")} </p>
      <p> {string("on how to reset your password")} </p>
      <ForgotContainer />
      <h5 className="line-decor"> {string("OR")} </h5>
      <Link href=Links.login className="btn-signup btn btn-line"> {string("Login")} </Link>
    </div>,
};
