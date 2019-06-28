open ReasonReact;
[@bs.val] external decodeURIComponent: string => string = "";

let component = ReasonReact.statelessComponent("PageLogin");

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g");

let make = (~queryString: string, _children) => {
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
    <div className="auth-page login-page">
      <Logo className="auth-page_logo" />
      <h2 className="-text-space"> {string("Login")} </h2>
      <LoginContainer ?afterLoginUrl />
      <Link href=Links.forgot> {string("forgot password")} </Link>
    </div>;
  },
};
