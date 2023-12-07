open ServiceWorker
open ServiceWorkerGlobalScope

self->set_oninstall(event => {
  Js.log2("service worker is installed", event)
})

self->set_onactivate(_event => {
  ()
})

self->set_onfetch(_event => {
  ()
})
