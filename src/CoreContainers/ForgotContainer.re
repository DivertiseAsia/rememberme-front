open ReasonReact;
open RequestUtils;
open Utils;

type state = {
  sending: bool,
  success: bool,
  error: option(string),
  email: string,
};

type action =
  | ResetPassword
  | SetEmail(string)
  | Failed(string)
  | Success;

let resetPassword = email => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "email", Js.Json.string(email));

  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Post, ~body=payload, None),
    ~url="", /* TODO: Reset password URL */
    ~successAction=_ => Success,
    ~failAction=json => Failed(getErrorMsgFromJson(json)),
  );
};

let component = ReasonReact.reducerComponent("ForgotContainerRe");
let make = _children => {
  ...component,
  initialState: () => {sending: false, error: None, email: "", success: false},
  reducer: (action, state) =>
    switch (action) {
    | ResetPassword =>
      UpdateWithSideEffects(
        {...state, sending: true, error: None},
        (
          ({send, state}) =>
            Js.Promise.(resetPassword(state.email) |> then_(action => send(action) |> resolve) |> ignore)
        ),
      )
    | Success => Update({...state, sending: false, success: true})
    | SetEmail(email) => Update({...state, email})
    | Failed(error) => Update({...state, error: Some(error), sending: false})
    },
  render: self =>
    <div className={"forgot-container " ++ (self.state.sending ? "loading" : "not-loading")}>
      <ForgotForm
        loading={self.state.sending || self.state.success}
        setEmail={input => self.send(SetEmail(input))}
        onSubmit={
          e => {
            ReactEvent.Form.preventDefault(e);
            Js.log(e);
            self.send(ResetPassword);
          }
        }
      />
      {self.state.sending === true ? <Loading /> : ReasonReact.null}
      {
        self.state.success ?
          <div className="text-info"> {string("Your password has been reset please check your email")} </div> :
          ReasonReact.null
      }
      {
        switch (self.state.error) {
        | Some("")
        | None => ReasonReact.null
        | Some(x) => <div className="text-error"> {string(x)} </div>
        }
      }
    </div>,
};
