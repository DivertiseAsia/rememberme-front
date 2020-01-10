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

let login = (usename, password) => {
  let payload =
    Json.Encode.(
      object_([("username", usename |> Js.Json.string), ("password", password |> Js.Json.string)])
    );
  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Post, ~body=payload, None),
    ~url=RememberMeApi.URL.login,
    ~successAction=
      json => {
        let token = Json_decode.(field("token", Json_decode.string, json));
        LoginSuccess(token);
      },
    ~failAction=json => LoginFailed((json |> Json.stringify)),
  );
};

let component = ReasonReact.reducerComponent("LoginContainerRe");
let make = (~afterLoginUrl: option(string)=?, _children) => {
  ...component,
  initialState: () => {sendingLogin: false, loginError: None, usename: "", password: "", loginHasSuccess: false},
  reducer: (action, state) =>
    switch (action) {
    | Login =>
      UpdateWithSideEffects(
        {...state, sendingLogin: true, loginError: None},
        ({send, state}) =>
          Js.Promise.(login(state.usename, state.password) |> then_(action => send(action) |> resolve) |> ignore),
      )
    | LoginSuccess(token) =>
      UpdateWithSideEffects(
        {...state, sendingLogin: false},
        _self => {
          Dom.Storage.(localStorage |> setItem("token", token));
          switch (afterLoginUrl) {
          | Some(route) => ReasonReact.Router.push(route)
          | None => ReasonReact.Router.push(Links.home)
          };
        },
      )
    | SetEmail(usename) => Update({...state, usename})
    | SetPassword(password) => Update({...state, password})
    | LoginFailed(error) => Update({...state, loginError: Some(error), sendingLogin: false})
    },
  render: self =>
    
      {self.state.sendingLogin === true ? 
        <Loading /> : 
        <div className={"login-container " ++ (self.state.sendingLogin ? "loading" : "not-loading")}>
          <LoginForm
            loading={self.state.sendingLogin}
            setEmail={input => self.send(SetEmail(input))}
            setPassword={password => self.send(SetPassword(password))}
            onSubmit={e => {
              ReactEvent.Form.preventDefault(e);
              Js.log(e);
              self.send(Login);
            }}
          />
          {self.state.loginHasSuccess ?
            <div className="text-info"> {string("Logged in. Redirecting..")} </div> : ReasonReact.null}
         {switch (self.state.loginError) {
          | Some("")
          | None => ReasonReact.null
          | Some(x) => x |> getErrorMsgFromJson |> array
          }}
       </div>
      }
      ,
};
