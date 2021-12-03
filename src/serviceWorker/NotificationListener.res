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
    | "granted" => Js.log("Permission granted")
    | _ => Js.log("Permission is not granted")
    }->Js.Promise.resolve
  , _)->ignore
}

let dailyInfoText = (_leaveInfo, _holidayInfo) => ()

let showNotification = %raw(`
  function showNotification (infos) {
      navigator.serviceWorker.ready.then(function(registration) {
      registration.showNotification(infos);
    })
  }
`)

let showDailyNoti = (dailyInfoText, dailyType) => {
  switch self->navigator->ServiceWorkerLoader.swOpt {
  | Some(_serviceWorker) =>
    switch Dom.Storage.getItem(Config.LocalStorageInfo.dailyReportKey, Dom.Storage.localStorage) {
    | Some(dailyReport) when Js.Date.fromString(dailyReport)->Utils.Date.isToday => ()
    | _ => {
        dailyInfoText->showNotification->ignore
        let today = Js.Date.now()->Js.Date.fromFloat
        let dateStr =
          (today->Js.Date.getMonth->int_of_float + 1)->string_of_int ++
          "-" ++
          today->Js.Date.getDate->int_of_float->string_of_int ++
          "-" ++
          today->Js.Date.getFullYear->int_of_float->string_of_int

        Dom.Storage.setItem(
          Config.LocalStorageInfo.dailyReportKey,
          Config.LocalStorageInfo.dailyReportValueFormate(~dateStr, ~dailyType),
          Dom.Storage.localStorage,
        )
      }
    }
  | None => Js.log("service worker cannot be found.")
  }
}
