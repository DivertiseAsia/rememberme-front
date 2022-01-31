@react.component
let make = (~onClose) => <>
  <div className="popover-bg" onClick=onClose />
  <div className="container popover">
    <div className="popover-arrow" />
    <div className="row">
      <div
        className="col-12 text-center mt-2 mb-2 cursor-pointer"
        onClick={_ => RescriptReactRouter.push("profile")}>
        {React.string("My Profile")}
      </div>
      <div
        className="col-12 text-center mt-2 mb-2 cursor-pointer"
        onClick={_ => {
          {
            open Dom.Storage
            localStorage->clear
          }

          RescriptReactRouter.push("")
        }}>
        {React.string("Sign Out")}
      </div>
    </div>
  </div>
</>
