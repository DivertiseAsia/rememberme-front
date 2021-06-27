open ReasonReact;
open RememberMeApi;
open RememberMeType;

type state = {
  loadState,
  profile: option(profile),
};

type action =
  | FetchProfile
  | FetchProfileSuccess(option(profile))
  | FetchProfileFail;

let initialState = {loadState: Loading, profile: None};

let fetchProfile = dispatch => {
  fetchProfile(
    ~token=Utils.getToken(),
    ~successAction=profile => dispatch(FetchProfileSuccess(Some(profile))),
    ~failAction=_ => dispatch(FetchProfileFail),
  );
};

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g");

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | FetchProfile => {...state, loadState: Loading}
        //      , fetchProfile) TODO
        | FetchProfileSuccess(profile) => {loadState: Succeed, profile}
        | FetchProfileFail => {loadState: Failed(""), profile: None}
        }
      },
      initialState,
    );

  React.useEffect0(_ => {
    dispatch(FetchProfile);

    None;
  });

  <div className="auth-page profile-page container-fluid">
    <div className="row row-title">
      <div className="col-6 text-left">
        <div className="cursor-pointer" onClick={_ => Router.push("/")}>
          <img className="form-icon mr-2" src="/images/arrow_back.svg" />
          <span> {string("Back")} </span>
        </div>
      </div>
      <div className="col-6 text-right">
        <span className="cursor-pointer">
          {string(
             switch (state.profile) {
             | Some(p) => p.firstName
             | None => ""
             },
           )}
        </span>
      </div>
      <div className="col-12 text-center">
        <h2 className="m-auto"> {string("My Profile")} </h2>
      </div>
    </div>
    <div className="row row-profile-container justify-content-center">
      <div className="col-12 col-sm-12 col-md-5 col-lg-5 col-xl-4">
        {switch (state.loadState, state.profile) {
         | (Succeed, Some(profile)) => <ProfileContainer profile />
         | (Failed(_), _) => string("Can't get profile. Please try again")
         | _ => <Loading />
         }}
      </div>
    </div>
  </div>;
};