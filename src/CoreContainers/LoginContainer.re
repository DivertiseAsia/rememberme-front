open ReasonReact;
open RequestUtils;
open Utils;

type state = {
  sendingLogin: bool,
  loginHasSuccess: bool,
  loginError: option(string),
  usename: string,
  password: string,
};

type action =
  | Login
  | SetEmail(string)
  | SetPassword(string)
  | LoginFailed(string)
  | LoginSuccess(string);

let initialState = {
  sendingLogin: false,
  loginError: None,
  usename: "",
  password: "",
  loginHasSuccess: false,
};

let login = (usename, password) => {
  let payload =
    Json.Encode.(
      object_([
        ("username", usename |> Js.Json.string),
        ("password", password |> Js.Json.string),
      ])
    );
  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Post, ~body=payload, None),
    ~url=RememberMeApi.URL.login,
    ~successAction=
      json => {
        let token = Json_decode.(field("token", Json_decode.string, json));
        LoginSuccess(token);
      },
    ~failAction=json => LoginFailed(json |> Json.stringify),
  );
};

[@react.component]
let make = (~afterLoginUrl: option(string)=?) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Login =>
          // TODO: REWRITE
          {...state, sendingLogin: true, loginError: None}
        //        ({send, state}) =>
        //          Js.Promise.(
        //            login(state.usename, state.password)
        //            |> then_(action => send(action) |> resolve)
        //            |> ignore
        //          ),
        | LoginSuccess(token) =>
          // TODO: REWRITE
          {...state, sendingLogin: false}
        //        _self => {
        //          Dom.Storage.(localStorage |> setItem("token", token));
        //          switch (afterLoginUrl) {
        //          | Some(route) => ReasonReact.Router.push(route)
        //          | None => ReasonReact.Router.push(Links.home)
        //          };
        //        },
        | SetEmail(usename) => {...state, usename}
        | SetPassword(password) => {...state, password}
        | LoginFailed(error) => {
            ...state,
            loginError: Some(error),
            sendingLogin: false,
          }
        },
      initialState,
    );
  state.sendingLogin === true
    ? <Loading />
    : <div
        className={
          "login-container "
          ++ (state.sendingLogin ? "loading" : "not-loading")
        }>
        <LoginForm
          loading={state.sendingLogin}
          setEmail={input => dispatch(SetEmail(input))}
          setPassword={password => dispatch(SetPassword(password))}
          onSubmit={e => {
            ReactEvent.Form.preventDefault(e);
            Js.log(e);
            dispatch(Login);
          }}
        />
        {state.loginHasSuccess
           ? <div className="text-info">
               {string("Logged in. Redirecting..")}
             </div>
           : ReasonReact.null}
        {switch (state.loginError) {
         | Some("")
         | None => ReasonReact.null
         | Some(x) => x |> getErrorMsgFromJson |> array
         }}
      </div>;
};