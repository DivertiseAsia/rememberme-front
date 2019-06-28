open ReasonReact;
type iconType =
  | Home
  | Location;

type icon = {
  iconType,
  className: string,
  content: ReasonReact.reactElement,
};

let mapStringToIconType = (icon: string) =>
  switch (Js.String.toLowerCase(icon)) {
  | "home" => Some(Home)
  | "location" => Some(Location)
  | _ => None
  };

let mapIcon = icon =>
  switch (icon) {
  | Home => {
      iconType: Home,
      className: "house",
      content:
        <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 15.78 12.66">
          <title> {string("House")} </title>
          <g id="Layer_2">
            <g id="Layer_1-2">
              <path
                d="M.92,12.66a1,1,0,0,1-.65-.27A.91.91,0,0,1,0,11.74V5.29a.88.88,0,0,1,.48-.8L7.39.15a.89.89,0,0,1,1,0l6.94,4.35a.92.92,0,0,1,.44.75v6.45a.94.94,0,0,1-.89.95H.92Zm13.86-.93ZM1,11.66H14.78V5.29L7.88,1,1,5.34Zm0-6.3Zm0-.08ZM7.84,1Zm.1,0h0Z"
              />
            </g>
          </g>
        </svg>,
    }
  | Location => {
      iconType: Location,
      className: "location",
      content:
        <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 16 16">
          <defs> <style> {string(".a{fill:none;}.b{fill:#838383;fill-rule:evenodd;}")} </style> </defs>
          <g transform="translate(-207 -596)">
            <rect className="a" width="16" height="16" transform="translate(207 596)" />
            <path
              className="b"
              d="M6.58,9.47A2.786,2.786,0,0,0,9.371,6.679,2.872,2.872,0,0,0,6.58,3.788,2.786,2.786,0,0,0,3.788,6.579,2.942,2.942,0,0,0,6.58,9.47ZM1.894,1.894a6.626,6.626,0,0,1,9.371,9.371L6.58,15.95,1.894,11.265A6.807,6.807,0,0,1,1.894,1.894Z"
              transform="translate(207.975 596.05)"
            />
          </g>
        </svg>,
    }
  };

let component = ReasonReact.statelessComponent("IconRe");

let make = (~iconType, ~className=?, _children) => {
  ...component,
  render: _self => {
    let icon = mapIcon(iconType);
    <span className={"icon icon-" ++ icon.className ++ " " ++ Js.Option.getWithDefault("", className)}>
      {icon.content}
    </span>;
  },
};
