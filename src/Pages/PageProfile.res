open RememberMeType

@react.component
let make = () => {
  let {data: profileApiState, fetchData: loadProfile} = ProfileContext.useProfileResults()

  React.useEffect0(_ => {
    loadProfile()
    None
  })

  <div className="auth-page profile-page container-fluid">
    <div className="row row-title">
      <div className="col-6 text-left">
        <div className="cursor-pointer" onClick={_ => RescriptReactRouter.push("/")}>
          <img className="form-icon mr-2" src="/images/arrow_back.svg" />
          <span> {React.string("Back")} </span>
        </div>
      </div>
      <div className="col-6 text-right">
        <span className="cursor-pointer">
          {React.string(
            switch profileApiState {
            | Loaded(p) => p.firstName
            | _ => ""
            },
          )}
        </span>
      </div>
      <div className="col-12 text-center">
        <h2 className="m-auto"> {React.string("My Profile")} </h2>
      </div>
    </div>
    <div className="row row-profile-container justify-content-center">
      <div className="col-12 col-sm-12 col-md-5 col-lg-5 col-xl-4">
        {switch profileApiState {
        | Loaded(profile) => <ProfileContainer profile />
        | Failed(_) => React.string("Can't get profile. Please try again")
        | _ => <Loading />
        }}
      </div>
    </div>
  </div>
}
