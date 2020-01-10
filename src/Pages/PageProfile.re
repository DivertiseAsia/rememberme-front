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

let fetchProfile = ({state, send}) => {
  fetchProfile(
    ~token=Utils.getToken(),
    ~successAction=profile => send(FetchProfileSuccess(Some(profile))),
    ~failAction=_ => send(FetchProfileFail),
  );
};
let component = ReasonReact.reducerComponent("PageProfile");

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g");

let make = (_children) => {
  ...component,
  initialState: () => {
    loadState: Loading,
    profile: None,
  },
  reducer: (action, state) => {
    switch (action) {
    | FetchProfile => UpdateWithSideEffects({...state, loadState: Loading}, fetchProfile)
    | FetchProfileSuccess(profile) => Update({...state, loadState: Succeed, profile})
    | FetchProfileFail => Update({...state, loadState: Failed, profile: None})
    };
  },
  didMount: ({state, send}) => {
    send(FetchProfile)
  },
  render: ({state}) => {
    <div className="auth-page profile-page container-fluid">
      <div className="row row-title"> 
        <div className="col-6 text-left">
          <div 
            className="cursor-pointer" 
            onClick=(_ => Router.push(""))
          >
            <img 
              className="form-icon mr-2" 
              src="/images/arrow_back.svg" 
            />
            <span>
              {string("Back")}
            </span>
          </div>
        </div>
        <div className="col-6 text-right">
          <span className="cursor-pointer">
          {string(switch state.profile {
          | Some(p) => p.firstName
          | None => ""
          })}
          </span>
        </div>
        <div className="col-12 text-center">
          <h2 className="m-auto">{string("My Profile")}</h2>
        </div>
      </div>
      <div className="row row-profile-container justify-content-center"> 
        <div className="col-12 col-sm-12 col-md-5 col-lg-5 col-xl-4"> 
          {
            switch (state.loadState, state.profile) {
            | (Succeed, Some(profile)) => <ProfileContainer profile />
            | (Failed, _) => null 
            | _ => {string("Loading")}
            }
          }
        </div>
      </div>
    </div>;
  },
};
