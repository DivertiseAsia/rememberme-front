open ReasonReact;
[@bs.val] external decodeURIComponent: string => string = "";

let component = ReasonReact.statelessComponent("PageLogin");

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g");

let make = (~queryString="", _children) => {
  ...component,
  render: _self => {
    let afterLoginUrl = {
      let nextParameter = Js.String.match(regex, queryString);
      switch (nextParameter) {
      | None => None
      | Some(arr) =>
        switch (Js.String.replace("next=", "", Js.Array.joinWith("", arr))) {
        | "" => None
        | x => Some(decodeURIComponent(x))
        }
      };
    };
    <div className="auth-page login-page container-fluid">
      <div className="row row-logo"> 
        <img className="logo-" src="/images/logo.png" />
      </div>
      <div className="row row-login-container justify-content-center"> 
        <div className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-3"> 
          <LoginContainer ?afterLoginUrl />
        </div>
        <div className="col-12 text-center link-signup"> 
          <u><Link href=Links.register> {string("Sign up")} </Link></u>
        </div>
      </div>
    </div>;
  },
};
