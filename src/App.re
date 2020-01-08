open ReasonReact;
open RememberMeApi;
open RememberMeType;

[@bs.val] external encodeURIComponent: string => string = "";
[@bs.get] external location: Dom.window => Dom.location = "";
[@bs.get] external href: Dom.location => string = "";

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

let component = reducerComponent("App");

let routeMatches = (x, link) => "/" ++ x == link;

let make = _children => {
  ...component,
  initialState: () => {
    route: Router.dangerouslyGetInitialUrl(), 
    loadState: Loading,
  },
  reducer: (action, state) =>
    switch (action) {
    | RouteTo(route) => Update({...state, route})
    },
  didMount: ({send, onUnmount}) => {
    let watcherID = Router.watchUrl(url => send(RouteTo(url)));
    onUnmount(() => Router.unwatchUrl(watcherID));
  },
  render: ({state: {route, loadState}}) => {
    
    let token = loadToken();
    let isLoggedIn = token !== "";
    switch (route.path, isLoggedIn) {
    | ([], true) => <PageHome isLoggedIn />
    | ([""], true) => <PageHome isLoggedIn />
    | ([x, monthYear], true) when routeMatches(x, Links.dashboard) => {
        let datetime = Js.String.split("-", monthYear);
        <PageHome isLoggedIn year=(datetime[1] |> float_of_string) month=((datetime[0] |> int_of_string) - 1) />
      }
    | ([x], true) when routeMatches(x, Links.profile) => <PageProfile />
    | ([x], true) when routeMatches(x, Links.allMonth) => <PageAllMonth />
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
    }
  },
};
