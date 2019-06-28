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
    <div className="auth-page login-page">
        <h1> {"Remember Me" |> string} </h1>
        <h2 className="-text-space"> {string("Login")} </h2>
        <LoginContainer ?afterLoginUrl />
        <Link href=Links.register> {string("Sign up")} </Link>
      </div>;
  },
};
