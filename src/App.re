open ReasonReact;
open RememberMeType;
open RememberMeApi;

[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";
[@bs.get] external location: Dom.window => Dom.location = "location";
[@bs.get] external href: Dom.location => string = "href";

type eventsApiState = apiState(list(event));

type state = {
  route: ReasonReact.Router.url,
  loadState,
  eventsApiState,
};

type action =
  | RouteTo(Router.url)
  | SetEventsApiState(eventsApiState);

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

let fetchEvents = dispatch => {
  Loading->SetEventsApiState->dispatch;
  fetchEvent(
    ~token=Utils.getToken(),
    ~successAction=eventList => eventList->Loaded->SetEventsApiState->dispatch,
    ~failAction=
      json => json->Json.stringify->Failed->SetEventsApiState->dispatch,
  );
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | RouteTo(route) => {...state, route}
        | SetEventsApiState(eventsApiState) => {...state, eventsApiState}
        },
      {
        route: Router.dangerouslyGetInitialUrl(),
        loadState: Loading,
        eventsApiState: NotLoaded,
      },
    );

  let token = loadToken();
  let isLoggedIn = token !== "";

  let fetchEventsIfNone = () => {
    switch (state.eventsApiState) {
    | Loading
    | Loaded(_) => Js.log("events already loaded")
    | _ => fetchEvents(dispatch)
    };
  };

  React.useEffect0(_ => {
    let watcherID = Router.watchUrl(url => dispatch(RouteTo(url)));

    Some(_ => Router.unwatchUrl(watcherID));
  });

  React.useEffect1(
    _ => {
      if (isLoggedIn) {
        fetchEventsIfNone();
      };
      None;
    },
    [|isLoggedIn|],
  );

  <EventsContext.Provider value=(state.eventsApiState, fetchEventsIfNone)>
    {switch (state.route.path, isLoggedIn) {
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
       <PageLogin queryString={state.route.search} />
     | ([x], _) when routeMatches(x, Links.logout) =>
       let _ = clearToken();
       let _ = ReasonReact.Router.push("/?logout=true");
       <PageLogin queryString={state.route.search} />;
     | ([x], false) when routeMatches(x, Links.register) => <PageRegister />
     | (_, false) =>
       let queryParams = "next=" ++ encodeURIComponent(path());
       let _ = ReasonReact.Router.push("/login?" ++ queryParams);
       <PageLogin queryString=queryParams />;
     | _ => <Page404 isLoggedIn />
     }}
  </EventsContext.Provider>;
};