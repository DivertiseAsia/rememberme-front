open ServiceWorker;
open ServiceWorkerGlobalScope;

self->set_oninstall(event => {Js.log2("service worker is installed", event)});

self->set_onactivate(event => {
  Js.log2("service worker is being activate.", event)
});

self->set_onfetch(event => {Js.log2("sw onfetch", event)});
