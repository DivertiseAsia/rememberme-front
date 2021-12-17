module Info = {
  let name = "RememberME"
}

module LocalStorageInfo = {
  let dailyReportKey = "noti-daily-report"
  let dailyReportValueSeparateStr = "_"
  let dailyReportValueFormate = (~dateStr, ~dailyType) =>
    `${dateStr}${dailyReportValueSeparateStr}${dailyType}`
}
