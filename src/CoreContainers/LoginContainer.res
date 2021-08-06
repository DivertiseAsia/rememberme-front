open ReasonReact
open RequestUtils
open Utils

type loginState = RememberMeType.apiState<bool>

type state = {
  usename: string,
  password: string,
  loginState: loginState,
}

type action =
  | SetEmail(string)
  | SetPassword(string)
  | SetLoginState(loginState)

let initialState = {usename: "", password: "", loginState: NotLoaded}

@react.component
let make = (~afterLoginUrl: option<string>=?) => {
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    | SetEmail(usename) => {...state, usename: usename}
    | SetPassword(password) => {...state, password: password}
    | SetLoginState(loginState) => {...state, loginState: loginState}
    }
  , initialState)

  let submitLogin = () => {
    let payload = {
      open Json.Encode
      object_(list{
        ("username", state.usename |> Js.Json.string),
        ("password", state.password |> Js.Json.string),
      })
    }
    Loading->SetLoginState->dispatch
    requestJsonResponseToAction(
      ~headers=buildHeader(~verb=Post, ~body=payload, None),
      ~url=RememberMeApi.URL.login,
      ~successAction=json => {
        let token = {
          open Json_decode
          field("token", Json_decode.string, json)
        }
        {
          open Dom.Storage
          localStorage |> setItem("token", token)
        }

        Loaded(true)->SetLoginState->dispatch
        afterLoginUrl->Belt.Option.getWithDefault(Links.home)->ReasonReact.Router.push
      },
      ~failAction=json => json->Json.stringify->Failed->SetLoginState->dispatch,
    )->ignore
  }

  switch state.loginState {
  | Loading => <Loading />
  | _ =>
    <div className="login-container ">
      <LoginForm
        loading=false
        setEmail={input => dispatch(SetEmail(input))}
        setPassword={password => dispatch(SetPassword(password))}
        onSubmit={e => {
          ReactEvent.Form.preventDefault(e)
          Js.log(e)
          submitLogin()
        }}
      />
      {switch state.loginState {
      | Loaded(true) => <div className="text-info"> {string("Logged in. Redirecting..")} </div>
      | Failed(x) when x != "" => x |> getErrorMsgFromJson |> array
      | _ => ReasonReact.null
      }}
    </div>
  }
}
