let buildHeader =
    (~verb=Fetch.Get, ~body: option(Js.Json.t)=?, token: option(string)) => {
  let headers =
    switch (token) {
    | None => Fetch.HeadersInit.make({"Content-Type": "application/json"})
    | Some(token) =>
      Fetch.HeadersInit.make({
        "Content-Type": "application/json",
        "Authorization": "Token " ++ token,
      })
    };
  switch (body) {
  | None => Fetch.RequestInit.make(~method_=verb, ~headers, ())
  | Some(body) =>
    Fetch.RequestInit.make(
      ~method_=verb,
      ~body=Fetch.BodyInit.make(Js.Json.stringify(body)),
      ~headers,
      (),
    )
  };
};

let buildFileHeader = (~body, token: option(string)) => {
  let headers =
    switch (token) {
    | None => Fetch.HeadersInit.makeWithArray([||])
    | Some(token) =>
      Fetch.HeadersInit.make({"Authorization": "Token " ++ token})
    };

  Fetch.RequestInit.make(
    ~method_=Post,
    ~body=Fetch.BodyInit.make(Js.Json.stringify(body)),
    ~headers,
    (),
  );
};

let internalUrl = endPoint => {
  let server = [%bs.raw "`${API_SERVER}`"];
  server ++ endPoint;
};

let request = (~headers, ~url, ~decoder=?, ()) =>
  Js.Promise.(
    Fetch.fetchWithInit(url, headers)
    |> then_(Fetch.Response.json)
    |> then_(json =>
         switch (decoder) {
         | Some(decoder) =>
           json |> decoder |> (response => resolve(Some(response)))
         | None => resolve(None)
         }
       )
    |> catch(err => {
         Js.log2("Request failure -> " ++ url, err);
         resolve(None);
       })
  );

let requestWithoutHeader = (~url, ~decoder=?, ()) =>
  Js.Promise.(
    Fetch.fetch(url)
    |> then_(Fetch.Response.json)
    |> then_(json =>
         switch (decoder) {
         | Some(decoder) =>
           json |> decoder |> (response => resolve(Some(response)))
         | None => resolve(None)
         }
       )
    |> catch(err => {
         Js.log2("Request failure -> " ++ url, err);
         resolve(None);
       })
  );

let requestJsonResponseToAction =
    (~headers, ~url, ~successAction, ~failAction) =>
  Js.Promise.(
    Fetch.fetchWithInit(url, headers)
    |> then_(response =>
         response
         |> Fetch.Response.status
         |> (
           fun
           | status when status / 100 == 2 =>
             Fetch.Response.json(response)
             |> then_(json => successAction(json) |> resolve)
             |> catch(_err => successAction(Js.Json.null) |> resolve)
           | _ =>
             Fetch.Response.json(response)
             |> then_(json => failAction(json) |> resolve)
             |> catch(_err => failAction(Js.Json.null) |> resolve)
         )
       )
    |> catch(_err => failAction(Js.Json.null) |> resolve)
  );