open ReasonReact;

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g");

[@react.component]
let make = () => {
  <div className="auth-page register-page container-fluid">
    <div className="row row-logo">
      <img className="logo-" src="/images/logo.png" />
    </div>
    <div className="row row-register-container justify-content-center">
      <div className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-3">
        <RegisterContainer />
      </div>
      <div className="col-12 text-center link-login">
        <a id="signup_login" href=Links.login className="text-blue">
          {"Log In" |> string}
        </a>
      </div>
    </div>
  </div>;
};