open ReasonReact;

let component = ReasonReact.statelessComponent("PageProfile");

let regex = Js.Re.fromStringWithFlags("next=([^&#]*)", ~flags="g");

let make = (~profile, _children) => {
  ...component,
  render: _self => {
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
          <span className="cursor-pointer">{string("Nameeeeee  e ")}</span>
        </div>
        <div className="col-12 text-center">
          <h2 className="m-auto">{string("My Profile")}</h2>
        </div>
      </div>
      <div className="row row-profile-container justify-content-center"> 
        <div className="col-12 col-sm-12 col-md-5 col-lg-5 col-xl-4"> 
          <ProfileContainer />
        </div>
        /*<div className="col-12 text-center link-login"> 
          <a id="signup_login" href=Links.home className="text-blue"> {"Log In" |> string} </a>
        </div>*/
      </div>
    </div>;
  },
};
