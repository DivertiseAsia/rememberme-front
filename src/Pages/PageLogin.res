@bs.val external decodeURIComponent: string => string = "decodeURIComponent"

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g")

@react.component
let make = (~queryString="") => {
  let afterLoginUrl = {
    let nextParameter = Js.String.match_(regex, queryString)
    switch nextParameter {
    | None => None
    | Some(arr) =>
      switch Js.String.replace("next=", "", Js.Array.joinWith("", arr)) {
      | "" => None
      | x => Some(decodeURIComponent(x))
      }
    }
  }
  <div className="auth-page login-page container-fluid">
    <div className="row row-logo"> <img className="logo-" src="/images/logo.png" /> </div>
    <div className="row row-login-container justify-content-center">
      <div className="col-12 col-sm-12 col-md-5 col-lg-4 col-xl-3">
        <LoginContainer ?afterLoginUrl />
      </div>
      <div className="col-12 text-center link-signup">
        <u> <Link href=Links.register> {React.string("Sign up")} </Link> </u>
        <br />
        <br />
        <u> <Link href=Links.forgot> {React.string("Forgot password?")} </Link> </u>
      </div>
    </div>
  </div>
}
