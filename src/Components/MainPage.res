open Page
open Belt

@react.component
let make = (~isLoggedIn: bool, ~className: option<string>=?, ~children: React.element) =>
  <Page
    pageEnds={
      top: list{Children(List.toArray(list{<Header isLoggedIn />}))},
      bottom: list{},
    }
    className={className->Belt.Option.getWithDefault("mainpage-default")}>
    <div className="main-content"> children </div>
  </Page>
