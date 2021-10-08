open Notifications

let requestPermission = () => {
  Notification.requestPermission()->Js.Promise.then_(permission =>
    switch permission {
    | "granted" => Js.log("Permission granted")
    | _ => Js.log("Permission is not granted")
    }->Js.Promise.resolve
  , _)->ignore
}
