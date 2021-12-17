
@react.component
let make = () =>
  <div className="auth-page login-page container-fluid">
    <div className="row row-logo"> <img className="logo-" src="/images/logo.png" /> </div>
    <div className="row row-login-container justify-content-center">
      <div className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-3">
        <ForgotPasswordContainer />
      </div>
      <div className="col-12 text-center link-login">
        <a id="signup_login" href=Links.login className="text-blue">
          {"Return to login" -> React.string}
        </a>
      </div>
    </div>
  </div>
