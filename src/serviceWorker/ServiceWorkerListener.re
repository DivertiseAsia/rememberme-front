open ServiceWorker;
open ServiceWorkerGlobalScope;

self->set_oninstall(event => {Js.log2("service worker is installed", event)});

self->set_onfetch(event => {
  let _req = event->FetchEvent.request;
  ();
});
