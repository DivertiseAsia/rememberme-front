/*
     This is an entry point file.
     Do not add anything here.
 */

let appName = Config.Info.name
//let _ = [%bs.raw "console.log(`${appName}: ${BUILD.VERSION}`)"];

switch ReactDOM.querySelector("#container") {
| Some(root) => ReactDOM.render(<App />, root)
| None => ()
}
