open ServiceWorker
open ServiceWorkerGlobalScope
open Notifications

type notificationDetail = {
  body: string,
  icon: string,
}

let requestPermission = () => {
  Notification.requestPermission()->Js.Promise.then_(permission =>
    switch permission {
    | "granted" => {
        %raw(`
          navigator.serviceWorker.ready.then(function(registration) {
            registration.showNotification('Hello RememberME');
          })
        `)
        Js.log("Permission granted")
      }
    | _ => Js.log("Permission is not granted")
    }->Js.Promise.resolve
  , //TODO: convert to rescript syntax and move to showNotification()

  _)->ignore
}

let showNotification = () => {
  switch self->navigator->ServiceWorkerLoader.swOpt {
  | Some(
      serviceWorker,
    ) => /* serviceWorker
    ->ServiceWorkerContainer.GetRegistration.withoutScope
    ->Js.Promise.then_(sw =>
      sw->ServiceWorkerRegistration.ShowNotification.withoutOptions->Js.Promise.resolve
    )
    ->ignore*/
    ()
  | None => Js.log("service worker cannot be found.")
  }
}
