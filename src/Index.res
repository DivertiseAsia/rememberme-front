/*
     This is an entry point file.
     Do not add anything here.
 */

let appName = Config.Info.name

%%raw(`
  console.log("App Version", BUILD.VERSION)
`)

switch ReactDOM.querySelector("#container") {
| Some(root) => ReactDOM.render(<App />, root)
| None => ()
}
