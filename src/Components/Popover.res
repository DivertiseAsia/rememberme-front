open ReasonReact

@react.component
let make = (~onClose) => <>
  <div className="popover-bg" onClick=onClose />
  <div className="container popover">
    <div className="popover-arrow" />
    <div className="row">
      <div
        className="col-12 text-center mt-2 mb-2 cursor-pointer"
        onClick={_ => Router.push("profile")}>
        {string("My Profile")}
      </div>
      <div
        className="col-12 text-center mt-2 mb-2 cursor-pointer"
        onClick={_ => {
          {
            open Dom.Storage
            localStorage |> removeItem("token")
          }
          Router.push("")
        }}>
        {string("Sign Out")}
      </div>
    </div>
  </div>
</>
