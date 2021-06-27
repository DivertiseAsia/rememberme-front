open ReasonReact;
open RememberMeApi;
open Utils;

let str = ReasonReact.string;

type username = string;
type email = string;
type password = string;
type confirmPassword = password;
type firstName = string;
type lastName = string;
type birthDate = string;

type apiState = RememberMeType.apiState(bool);

type state = {
  email,
  password,
  confirmPassword,
  firstName,
  lastName,
  userName: string,
  birthDate,
  apiState,
};

type action =
  | SetEmail(string)
  | SetPassword(string)
  | SetConfirmPassword(string)
  | SetFirstName(firstName)
  | SetLastName(lastName)
  | SetUsername(string)
  | SetBirthDate(birthDate)
  | SetApiState(apiState);

let initialState = {
  email: "",
  password: "",
  confirmPassword: "",
  firstName: "",
  lastName: "",
  userName: "",
  birthDate: "",
  apiState: NotLoaded,
};

let getClassName = (~extraStyle="", ~invalid=false, ()) => {
  let invalidStyle = invalid ? "invalid" : "";
  {j|form-control $extraStyle $invalidStyle|j};
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | SetEmail(email) => {...state, email}
        | SetPassword(password) => {...state, password}
        | SetConfirmPassword(confirmPassword) => {...state, confirmPassword}
        | SetBirthDate(birthDate) => {...state, birthDate}
        | SetFirstName(firstName) => {...state, firstName}
        | SetLastName(lastName) => {...state, lastName}
        | SetUsername(userName) => {...state, userName}
        | SetApiState(apiState) => {...state, apiState}
        }
      },
      initialState,
    );

  let signup = () => {
    let payload =
      Json.Encode.(
        object_([
          ("email", state.email |> Js.Json.string),
          ("password", state.password |> Js.Json.string),
          ("confirm_password", state.confirmPassword |> Js.Json.string),
          ("first_name", state.firstName |> Js.Json.string),
          ("last_name", state.lastName |> Js.Json.string),
          ("birthDate", state.birthDate |> Js.Json.string),
        ])
      );

    Loading->SetApiState->dispatch;

    RequestUtils.requestJsonResponseToAction(
      ~headers=RequestUtils.buildHeader(~verb=Post, ~body=payload, None),
      ~url=URL.signUp,
      ~successAction=_json => Loaded(true)->SetApiState->dispatch,
      ~failAction=json => json->Json.stringify->Failed->SetApiState->dispatch,
    )
    |> ignore;
  };

  let buttonDisabled =
    state.apiState === Loading
    || !checkEmail(state.email)
    || !checkPassword(state.password)
    || !
         checkIsSamePassword(
           ~confirmPassword=state.confirmPassword,
           ~password=state.password,
         )
    || [state.firstName, state.lastName, state.birthDate]
    |> isAnyStringEmpty;

  <div className="row">
    <div className="col-sm-9 col-md-9 col-lg-9 mx-auto signup-container">
      {state.apiState === Loading ? <Loading /> : null}
      <form id="signup-form">
        <div className="row justify-content-between">
          <div className="col-6 pl-0">
            <input
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
            type_="email"
            id="inputEmail"
            className={getClassName(~invalid={!checkEmail(state.email)}, ())}
            placeholder="Your Email"
            value={state.email}
            onChange={e => dispatch(SetEmail(e |> valueFromEvent))}
          />
          <input
            type_="text"
            id="username"
            className={getClassName(
              ~extraStyle="form-control-smaller mr-2",
              ~invalid={state.userName |> isStringEmpty},
              (),
            )}
            placeholder="Username"
            value={state.userName}
            onChange={e => dispatch(SetUsername(valueFromEvent(e)))}
          />
          <input
            type_="password"
            id="inputPassword"
            className={getClassName(
              ~extraStyle="form-control-small",
              ~invalid={!checkPassword(state.password)},
              (),
            )}
            placeholder="Password"
            value={state.password}
            onChange={e => dispatch(SetPassword(e |> valueFromEvent))}
          />
          <input
            type_="password"
            id="inputConfirmPassword"
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
            placeholder="Password Confirmation"
            value={state.confirmPassword}
            onChange={e => dispatch(SetConfirmPassword(e |> valueFromEvent))}
          />
          <div className="line-signup" />
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
        </div>
        <div className="error-message">
          {switch (state.apiState) {
           | Failed(jsonStr) => jsonStr |> getErrorMsgFromJson |> array
           | _ => null
           }}
          {state.confirmPassword
           |> Js.String.length > 0
           && !(state.confirmPassword === state.password)
             ? <p id="error_confirm_password">
                 {"The password and confirmation password are mismatch." |> str}
               </p>
             : null}
          {state.password
           |> Js.String.length > 0
           && !checkPassword(state.password)
             ? <p id="error_password">
                 {"The password must be at least 8 characters long." |> str}
               </p>
             : null}
        </div>
        <button
          id="signup_btn"
          className="btn btn-blue btn-signup"
          disabled=buttonDisabled
          onClick={_ => signup()}>
          {"Sign Up" |> str}
        </button>
      </form>
    </div>
  </div>;
};