open ServiceWorker
open ServiceWorkerGlobalScope

let path = "/sw.js"

@bs.get
external swOpt: ServiceWorkerNavigator.t => option<ServiceWorkerContainer.t> = "serviceWorker"

let register = _ => {
  switch self->navigator->swOpt {
  | Some(sw) => sw->ServiceWorkerContainer.Register.withoutOptions(path) |> ignore
  | None => Js.log("service worker cannot be found.")
  }
}
