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

type state = {
  loading: bool,
  email,
  password,
  confirmPassword,
  firstName,
  lastName,
  birthDate,
  err: option(string),
};

type action =
  | SignUp
  | SignUpFailed(string)
  | SignUpSuccess
  | SetEmail(string)
  | SetPassword(string)
  | SetConfirmPassword(string)
  | SetFirstName(firstName)
  | SetLastName(lastName)
  | SetBirthDate(birthDate);

let signup = ({send, state}) => {
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

  RequestUtils.requestJsonResponseToAction(
    ~headers=RequestUtils.buildHeader(~verb=Post, ~body=payload, None),
    ~url=URL.signUp,
    ~successAction=json => send(SignUpSuccess),
    ~failAction=json => send(SignUpFailed(json |> Json.stringify)),
  )
  |> ignore;
};

let getClassName = (~extraStyle="", ~invalid=false, ()) => {
  let invalidStyle = invalid ? "invalid" : "";
  {j|form-control $extraStyle $invalidStyle|j};
};

let component = ReasonReact.reducerComponent("SignupBox");

let make = _children => {
  ...component,
  initialState: () => {
    loading: false,
    email: "",
    password: "",
    confirmPassword: "",
    firstName: "",
    lastName: "",
    birthDate: "",
    err: None,
  },
  reducer: (action, state) => {
    switch (action) {
    | SignUp => UpdateWithSideEffects({...state, loading: true, err: None}, signup)
    | SignUpSuccess => SideEffects(_ => ())
    | SignUpFailed(err) => Update({...state, loading: false, err: Some(err)})
    | SetEmail(email) => Update({...state, email})
    | SetPassword(password) => Update({...state, password: password |> getPasswordWithLimit})
    | SetConfirmPassword(confirmPassword) =>
      Update({...state, confirmPassword: confirmPassword |> getPasswordWithLimit})
    | SetBirthDate(birthDate) => Update({...state, birthDate})
    | SetFirstName(firstName) => Update({...state, firstName})
    | SetLastName(lastName) => Update({...state, lastName})
    };
  },
  render: ({state, send}) => {
    let buttonDisabled =
      state.loading
      || !checkEmail(state.email)
      || !checkPassword(state.password)
      || !checkIsSamePassword(~confirmPassword=state.confirmPassword, ~password=state.password)
      || [state.firstName, state.lastName, state.birthDate]
      |> isAnyStringEmpty;

    <div className="row">
      <div className="col-sm-9 col-md-9 col-lg-9 mx-auto signup-container">
        {state.loading ? <Loading /> : null}
        <form id="signup-form">
          <div className="row justify-content-between">
            <small>{string("Email")}</small>
              <input
                type_="email"
                id="inputEmail"
                className={getClassName(~invalid={!checkEmail(state.email)}, ())}
                placeholder="Your Email"
                value={state.email}
                onChange={e => send(SetEmail(e |> valueFromEvent))}
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
                onChange={e => send(SetPassword(e |> valueFromEvent))}
              />
              <input
                type_="password"
                id="inputConfirmPassword"
                className={getClassName(
                  ~extraStyle="form-control-small",
                  ~invalid=!{checkIsSamePassword(~password=state.password, ~confirmPassword=state.confirmPassword)},
                  (),
                )}
                placeholder="Password Confirmation"
                value={state.confirmPassword}
                onChange={e => send(SetConfirmPassword(e |> valueFromEvent))}
              />
            <div className="line-signup" />
              <input
                type_="name"
                id="name"
                className={getClassName(
                  ~extraStyle="form-control-smaller mr-2",
                  ~invalid={state.firstName |> isStringEmpty},
                  (),
                )}
                placeholder="Your Name"
                value={state.firstName}
                onChange={e => send(SetBirthDate(valueFromEvent(e)))}
              />
              <small>{string("Birthday")}</small>
              <input
                type_="date"
                id="birthday"
                className={getClassName(
                  ~extraStyle="form-control-smaller",
                  ~invalid={state.lastName |> isStringEmpty},
                  (),
                )}
                placeholder="Birthday"
                value={state.lastName}
                onChange={e => send(SetBirthDate(valueFromEvent(e)))}
              />
          </div>
          <div className="error-message">
            {switch (state.err) {
             | None => null
             | Some(jsonStr) => jsonStr |> getErrorMsgFromJson |> array
             }}
            {state.confirmPassword |> Js.String.length > 0 && !(state.confirmPassword === state.password) ?
               <p id="error_confirm_password"> {"The password and confirmation password are mismatch." |> str} </p> :
               null}
            {state.password |> Js.String.length > 0 && !checkPassword(state.password) ?
               <p id="error_password"> {"The password must be at least 8 characters long." |> str} </p> : null}
          </div>
          <button id="signup_btn" className="btn btn-blue btn-signup" disabled=buttonDisabled onClick={_ => send(SignUp)}>
            {"Sign Up" |> str}
          </button>
        </form>

      </div>
    </div>;
  },
};
