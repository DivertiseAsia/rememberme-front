open ReasonReact;
open RememberMeApi;
open RememberMeType;

[@bs.val] external encodeURIComponent: string => string = "";
[@bs.get] external location: Dom.window => Dom.location = "";
[@bs.get] external href: Dom.location => string = "";

type state = {
  route: ReasonReact.Router.url,
  profile: option(profile),
  loadState,
};

type action =
  | RouteTo(Router.url)
  | FetchProfile
  | FetchProfileSuccess(option(profile))
  | FetchProfileFail;

let fetchProfile = ({state, send}) => {
  fetchProfile(
    ~token=Utils.getToken(),
    ~successAction=profile => send(FetchProfileSuccess(Some(profile))),
    ~failAction=_ => send(FetchProfileFail),
  );
};

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
    profile: None,
    loadState: Idle,
  },
  reducer: (action, state) =>
    switch (action) {
    | RouteTo(route) => Update({...state, route})
    | FetchProfile => UpdateWithSideEffects({...state, loadState: Loading}, fetchProfile)
    | FetchProfileSuccess(profile) => Update({...state, loadState: Succeed, profile})
    | FetchProfileFail => Update({...state, loadState: Failed, profile: None})
    },
  didMount: ({send, onUnmount}) => {
    send(FetchProfile);
    let watcherID = Router.watchUrl(url => send(RouteTo(url)));
    onUnmount(() => Router.unwatchUrl(watcherID));
  },
  render: ({state: {route, profile}}) => {
    
    let token = loadToken();
    let isLoggedIn = token !== "";
    Js.log2("!! route.path", route.path);
    switch (route.path, isLoggedIn, profile) {
    | ([], true, Some(p))
    | ([""], true, Some(p)) => <PageHome isLoggedIn profile=p />
    | ([x, monthYear], true, Some(p)) when routeMatches(x, Links.dashboard) => {
        let datetime = Js.String.split("-", monthYear);
        <PageHome isLoggedIn year=(datetime[1] |> float_of_string) month=((datetime[0] |> int_of_string) - 1) profile=p />
      }
    | ([x], true, Some(p)) when routeMatches(x, Links.profile) => <PageProfile profile=p />
    | ([x], true, Some(p)) when routeMatches(x, Links.allMonth) => <PageAllMonth />
    | ([x], _, None) when routeMatches(x, Links.login) => <PageLogin queryString={route.search} />
    | ([x], _, Some(p)) when routeMatches(x, Links.logout) =>
      let _ = clearToken();
      let _ = ReasonReact.Router.push("/?logout=true");
      <PageLogin queryString={route.search} />;
    | ([x], false, None) when routeMatches(x, Links.register) => <PageRegister />
    | (_, false, None) =>
      let queryParams = "next=" ++ encodeURIComponent(path());
      let _ = ReasonReact.Router.push("/login?" ++ queryParams);
      <PageLogin queryString=queryParams />;
    | _ => <Page404 isLoggedIn />
    };
  },
};
