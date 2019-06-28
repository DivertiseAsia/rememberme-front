open ReasonReact;
[@bs.val] external encodeURIComponent: string => string = "";
[@bs.get] external location: Dom.window => Dom.location = "";
[@bs.get] external href: Dom.location => string = "";

type state = {route: ReasonReact.Router.url};

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

let component = reducerComponent("App");

let routeMatches = (x, link) => "/" ++ x == link;

let make = _children => {
  ...component,
  initialState: () => {route: Router.dangerouslyGetInitialUrl()},
  reducer: (action, _) =>
    switch (action) {
    | RouteTo(route) => Update({route: route})
    },
  didMount: ({send, onUnmount}) => {
    let watcherID = Router.watchUrl(url => send(RouteTo(url)));
    onUnmount(() => Router.unwatchUrl(watcherID));
  },
  render: ({state: {route}}) => {
    let token = loadToken();
    let isLoggedIn = token !== "";
    switch (route.path, isLoggedIn) {
    | ([], true)
    | ([""], true) => <PageHome isLoggedIn />
    | ([x], _) when routeMatches(x, Links.login) => <PageLogin queryString={route.search} />
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
  },
};
