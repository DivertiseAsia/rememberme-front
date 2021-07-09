open ReasonReact

@react.component
let make = (~isLoggedIn: bool) =>
  <MainPage isLoggedIn className="-text-center">
    {<>
      <h2> {string("404")} </h2> <p> {string("Maybe you were looking for something else")} </p>
    </>}
  </MainPage>
