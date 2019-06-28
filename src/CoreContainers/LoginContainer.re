open ReasonReact;
open RequestUtils;
open Utils;

type state = {
  sendingLogin: bool,
  loginHasSuccess: bool,
  loginError: option(string),
  email: string,
  password: string,
};

type action =
  | Login
  | SetEmail(string)
  | SetPassword(string)
  | LoginFailed(string)
  | LoginSuccess(string);

let login = (email, password) => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "email", Js.Json.string(email));
  Js.Dict.set(payload, "password", Js.Json.string(password));

  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Post, ~body=payload, None),
    ~url="", /* TODO: Login URL */
    ~successAction=
      json => {
        let token = Json_decode.(field("token", Json_decode.string, json));
        LoginSuccess(token);
      },
    ~failAction=json => LoginFailed(getErrorMsgFromJson(json)),
  );
};

let component = ReasonReact.reducerComponent("LoginContainerRe");
let make = (~afterLoginUrl: option(string)=?, _children) => {
  ...component,
  initialState: () => {sendingLogin: false, loginError: None, email: "", password: "", loginHasSuccess: false},
  reducer: (action, state) =>
    switch (action) {
    | Login =>
      UpdateWithSideEffects(
        {...state, sendingLogin: true, loginError: None},
        (
          ({send, state}) =>
            Js.Promise.(login(state.email, state.password) |> then_(action => send(action) |> resolve) |> ignore)
        ),
      )
    | LoginSuccess(token) =>
      UpdateWithSideEffects(
        {...state, sendingLogin: false},
        (
          _self => {
            Dom.Storage.(localStorage |> setItem("token", token));
            switch (afterLoginUrl) {
            | Some(route) => ReasonReact.Router.push(route)
            | None => ReasonReact.Router.push(Links.home)
            };
          }
        ),
      )
    | SetEmail(email) => Update({...state, email})
    | SetPassword(password) => Update({...state, password})
    | LoginFailed(error) => Update({...state, loginError: Some(error), sendingLogin: false})
    },
  render: self =>
    <div className={"login-container " ++ (self.state.sendingLogin ? "loading" : "not-loading")}>
      <LoginForm
        loading={self.state.sendingLogin}
        setEmail={input => self.send(SetEmail(input))}
        setPassword={password => self.send(SetPassword(password))}
        onSubmit={
          e => {
            ReactEvent.Form.preventDefault(e);
            Js.log(e);
            self.send(Login);
          }
        }
      />
      {self.state.sendingLogin === true ? <Loading /> : ReasonReact.null}
      {
        self.state.loginHasSuccess ?
          <div className="text-info"> {string("Logged in. Redirecting..")} </div> : ReasonReact.null
      }
      {
        switch (self.state.loginError) {
        | Some("")
        | None => ReasonReact.null
        | Some(x) => <div className="text-error"> {string(x)} </div>
        }
      }
    </div>,
};
