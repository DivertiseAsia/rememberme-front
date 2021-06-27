open ReasonReact;
open RememberMeApi;
open Utils;

let str = ReasonReact.string;

type email = string;
type password = string;
type confirmPassword = password;
type firstName = string;
type lastName = string;
type birthDate = string;

type changePassState = RememberMeType.apiState(string);

type state = {
  email,
  oldPassword: string,
  password,
  confirmPassword,
  firstName,
  lastName,
  birthDate,
  changePassState,
};

type action =
  | SetEmail(string)
  | SetOldPassword(string)
  | SetPassword(string)
  | SetConfirmPassword(string)
  | SetFirstName(firstName)
  | SetLastName(lastName)
  | SetBirthDate(birthDate)
  | SetChangePassState(changePassState);

let getClassName = (~extraStyle="", ~invalid=false, ()) => {
  let invalidStyle = invalid ? "invalid" : "";
  {j|form-control $extraStyle $invalidStyle|j};
};

[@react.component]
let make = (~profile: profile) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | SetEmail(email) => {...state, email}
        | SetOldPassword(oldPassword) => {...state, oldPassword}
        | SetPassword(password) => {...state, password}
        | SetConfirmPassword(confirmPassword) => {...state, confirmPassword}
        | SetBirthDate(birthDate) => {...state, birthDate}
        | SetFirstName(firstName) => {...state, firstName}
        | SetLastName(lastName) => {...state, lastName}
        | SetChangePassState(changePassState) => {...state, changePassState}
        }
      },
      {
        email: profile.email,
        oldPassword: "",
        password: "",
        confirmPassword: "",
        firstName: profile.firstName,
        lastName: profile.lastName,
        birthDate:
          profile.birthDate
          ->Js.Date.valueOf
          ->RememberMeType.Encode.getDateStrRequestLeave,
        changePassState: NotLoaded,
      },
    );

  let changePassword = () => {
    Loading->SetChangePassState->dispatch;

    RequestUtils.requestJsonResponseToAction(
      ~headers=
        RequestUtils.buildHeader(
          ~verb=Post,
          ~body=
            RememberMeType.Encode.changePassword(
              ~oldPassword=state.oldPassword,
              ~password=state.password,
              ~confirmPassword=state.confirmPassword,
            ),
          Utils.getToken(),
        ),
      ~url=URL.password,
      ~successAction=
        _json => {Loaded("Password Updated !")->SetChangePassState->dispatch},
      ~failAction=
        json => {json->Json.stringify->Failed->SetChangePassState->dispatch},
    )
    |> ignore;
  };

  let buttonDisabled =
    state.changePassState === Loading
    || state.oldPassword === ""
    || !checkPassword(state.password)
    || !
         checkIsSamePassword(
           ~confirmPassword=state.confirmPassword,
           ~password=state.confirmPassword,
         );

  <div className="row">
    <div className="col-sm-9 col-md-9 col-lg-9 mx-auto signup-container">
      {state.changePassState === Loading ? <Loading /> : null}
      <form id="signup-form">
        <div className="row justify-content-between">
          <div className="col-6 pl-0">
            <input
              disabled=true
              type_="text"
              id="firstname"
              className={getClassName(
                ~extraStyle="form-control-smaller mr-2",
                ~invalid={state.firstName |> isStringEmpty},
                (),
              )}
              placeholder="First Name"
              value={state.firstName}
              onChange={e => dispatch(SetFirstName(valueFromEvent(e)))}
            />
          </div>
          <div className="col-6 pr-0">
            <input
              disabled=true
              type_="text"
              id="lastname"
              className={getClassName(
                ~extraStyle="form-control-smaller mr-2",
                ~invalid={state.lastName |> isStringEmpty},
                (),
              )}
              placeholder="Last Name"
              value={state.lastName}
              onChange={e => dispatch(SetLastName(valueFromEvent(e)))}
            />
          </div>
          <input
            disabled=true
            type_="email"
            id="inputEmail"
            className={getClassName(~invalid={!checkEmail(state.email)}, ())}
            placeholder="Your Email"
            value={state.email}
            onChange={e => dispatch(SetEmail(e |> valueFromEvent))}
          />
          <small> {string("Birthday")} </small>
          <input
            type_="date"
            id="birthday"
            className={getClassName(
              ~extraStyle="form-control-smaller",
              ~invalid={state.birthDate |> isStringEmpty},
              (),
            )}
            placeholder="Birthday"
            value={state.birthDate}
            onChange={e => dispatch(SetBirthDate(valueFromEvent(e)))}
          />
          <div className="col-12 p-0">
            <h6> {string("Change Password")} </h6>
            <input
              type_="password"
              className={getClassName(
                ~extraStyle="form-control-small",
                ~invalid={!checkPassword(state.password)},
                (),
              )}
              placeholder="Old Password"
              value={state.oldPassword}
              onChange={e => dispatch(SetOldPassword(e |> valueFromEvent))}
            />
            <input
              type_="password"
              className={getClassName(
                ~extraStyle="form-control-small",
                ~invalid={!checkPassword(state.password)},
                (),
              )}
              placeholder="New Password"
              value={state.password}
              onChange={e => dispatch(SetPassword(e |> valueFromEvent))}
            />
            <input
              type_="password"
              className={getClassName(
                ~extraStyle="form-control-small",
                ~invalid=
                  !{
                    checkIsSamePassword(
                      ~password=state.password,
                      ~confirmPassword=state.confirmPassword,
                    );
                  },
                (),
              )}
              placeholder="Confirm New Password"
              value={state.confirmPassword}
              onChange={e =>
                dispatch(SetConfirmPassword(e |> valueFromEvent))
              }
            />
          </div>
        </div>
        <div className="error-message">
          {switch (state.changePassState) {
           | Loaded(msg) => <p> msg->str </p>
           | Failed(messages) => messages->getErrorMsgFromJson->array
           | _ => null
           }}
          {state.confirmPassword->Js.String.length > 0
           && !(state.confirmPassword === state.password)
             ? <p id="error_confirm_password">
                 {"The password and confirmation password are mismatch." |> str}
               </p>
             : null}
          {state.password->Js.String.length > 0
           && !checkPassword(state.password)
             ? <p id="error_password">
                 {"The password must be at least 8 characters long." |> str}
               </p>
             : null}
        </div>
        <button
          id="signup_btn"
          className="btn btn-blue btn-signup mb-5"
          disabled=buttonDisabled
          onClick={_ => changePassword()}>
          {"Confirm" |> str}
        </button>
      </form>
    </div>
  </div>;
};