[@bs.module]
external loading: string = "../../../../public/images/loading.gif";

[@react.component]
let make = (~className: option(string)=?, ~onClick=?) => {
  <div
    className={
      "loading-wrapper " ++ Belt.Option.getWithDefault(className, "")
    }>
    <img
      className={
        "loading " ++ Belt.Option.getWithDefault(className, "loading-default")
      }
      ?onClick
      src=loading
    />
  </div>;
};