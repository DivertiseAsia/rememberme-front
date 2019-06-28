open ReasonReact;
open RequestUtils;
open Utils;

type state = {
  sending: bool,
  success: bool,
  error: option(string),
  old_password: string,
  new_password: string,
  confirm_password: string,
};

type action =
  | ChangePassword
  | SetOldPassword(string)
  | SetNewPassword(string)
  | SetConfPassword(string)
  | Failed(string)
  | Success;

let checkThenChange = self =>
  if (!self.state.sending) {
    if (self.state.old_password === "" || self.state.new_password === "" || self.state.confirm_password === "") {
      self.send(Failed("Please fill in all fields"));
    } else if (self.state.new_password !== self.state.confirm_password) {
      self.send(Failed("Passwords must match"));
    } else {
      self.send(ChangePassword);
    };
  };

let changePassword = (old_password, new_password, confirm_password, token) => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "old_password", Js.Json.string(old_password));
  Js.Dict.set(payload, "new_password", Js.Json.string(new_password));
  Js.Dict.set(payload, "confirm_password", Js.Json.string(confirm_password));

  requestJsonResponseToAction(
    ~headers=buildHeader(~verb=Post, ~body=payload, Some(token)),
    ~url="", /* TODO: Change password URL */
    ~successAction=_ => Success,
    ~failAction=json => Failed(getErrorMsgFromJson(json)),
  );
};

let component = ReasonReact.reducerComponent("PageChange");
let make = (~token: string, _children) => {
  ...component,
  initialState: () => {
    sending: false,
    error: None,
    old_password: "",
    new_password: "",
    confirm_password: "",
    success: false,
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangePassword =>
      UpdateWithSideEffects(
        {...state, sending: true, error: None},
        (
          ({send, state}) =>
            Js.Promise.(
              changePassword(state.old_password, state.new_password, state.confirm_password, token)
              |> then_(action => send(action) |> resolve)
              |> ignore
            )
        ),
      )
    | Success => Update({...state, sending: false, success: true})
    | SetOldPassword(old_password) => Update({...state, old_password})
    | SetNewPassword(new_password) => Update({...state, new_password})
    | SetConfPassword(confirm_password) => Update({...state, confirm_password})
    | Failed(error) => Update({...state, error: Some(error), sending: false})
    },
  render: self =>
    <div className={"forgot-container " ++ (self.state.sending ? "loading" : "not-loading")}>
      <ChangeForm
        loading={self.state.sending || self.state.success}
        setOldPassword={input => self.send(SetOldPassword(input))}
        setNewPassword={input => self.send(SetNewPassword(input))}
        setConfPassword={input => self.send(SetConfPassword(input))}
        onSubmit={
          e => {
            ReactEvent.Form.preventDefault(e);
            Js.log(e);
            self.send(ChangePassword);
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
