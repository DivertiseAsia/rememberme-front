@react.component
let make = (~isLoggedIn: bool) =>
  <MainPage isLoggedIn className="-text-center">
    {<>
      <h2> {React.string("404")} </h2> <p> {React.string("Maybe you were looking for something else")} </p>
    </>}
  </MainPage>
