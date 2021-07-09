[@react.component]
let make = (~src: string, ~className: option(string)=?, ~children) => {
  <div
    className={
      "image-background "
      ++ Js.Option.getWithDefault("image-background-default", className)
    }
    style={ReactDOM.Style.make(~backgroundImage="url('" ++ src ++ "')", ())}>
    children
  </div>;
};