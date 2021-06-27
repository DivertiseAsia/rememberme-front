open ReasonReact;
open RememberMeType;

[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";
[@bs.get] external location: Dom.window => Dom.location = "location";
[@bs.get] external href: Dom.location => string = "href";

type state = {
  route: ReasonReact.Router.url,
  loadState,
};

type action =
  | RouteTo(Router.url);

let loadToken = () =>
  switch (Dom.Storage.(localStorage |> getItem("token"))) {
  | None => ""
  | Some(token) => token
  };

let clearToken = () => Dom.Storage.(localStorage |> removeItem("token"));

let path = () =>
  switch ([%external window]) {
  | None => ""
  | Some((window: Dom.window)) => window |> location |> href
  };

let routeMatches = (x, link) => "/" ++ x == link;

[@react.component]
let make = () => {
  let ({route}, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | RouteTo(route) => {...state, route}
        },
      {route: Router.dangerouslyGetInitialUrl(), loadState: Loading},
    );
  React.useEffect0(_ => {
    let watcherID = Router.watchUrl(url => dispatch(RouteTo(url)));
    Some(_ => Router.unwatchUrl(watcherID));
  });
  let token = loadToken();
  let isLoggedIn = token !== "";
  switch (route.path, isLoggedIn) {
  | ([], true)
  | ([""], true) => <PageHome />
  | ([x, monthYear], true) when routeMatches(x, Links.dashboard) =>
    let datetime = Js.String.split("-", monthYear);
    <PageHome
      year={datetime[1] |> float_of_string}
      month={(datetime[0] |> int_of_string) - 1}
    />;
  | ([x], true) when routeMatches(x, Links.profile) => <PageProfile />
  | ([x], true) when routeMatches(x, Links.allMonth) => <PageAllMonth />
  | ([x], _) when routeMatches(x, Links.login) =>
    <PageLogin queryString={route.search} />
  | ([x], _) when routeMatches(x, Links.logout) =>
    let _ = clearToken();
    let _ = ReasonReact.Router.push("/?logout=true");
    <PageLogin queryString={route.search} />;
  | ([x], false) when routeMatches(x, Links.register) => <PageRegister />
  | (_, false) =>
    let queryParams = "next=" ++ encodeURIComponent(path());
    let _ = ReasonReact.Router.push("/login?" ++ queryParams);
    <PageLogin queryString=queryParams />;
  | _ => <Page404 isLoggedIn />
  };
};