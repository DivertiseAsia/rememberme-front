open ReasonReact;
open Belt;
//open Css_Js_Core;
//open Css_Legacy_Core

type pageEnd =
  | Text(string)
  | Children(array(ReasonReact.reactElement));

type pageEnds = {
  top: list(pageEnd),
  bottom: list(pageEnd),
};

let pageStyle = "";
//  style([ TODO:
//    display(flexBox),
//    flexDirection(column),
//    maxHeight(vh(100.0)),
//    height(`percent(100.0)),
//  ]);

let pageContentsStyle = "";
//style([overflow(auto), flex(1)]); TODO:

let mapPageEnd = (pageEndArray: list(pageEnd), extraClass: string) =>
  switch (List.length(pageEndArray)) {
  | 0 => ReasonReact.null
  | _ =>
    <div
      className={"page-end-container page-end-" ++ extraClass} key=extraClass>
      {array(
         List.toArray(
           List.mapWithIndex(pageEndArray, (i, info: pageEnd) =>
             <div
               className={"page-end page-end-" ++ string_of_int(i)}
               key={string_of_int(i)}>
               {switch ((info: pageEnd)) {
                | Text(s) =>
                  <div className="page-end-text">
                    {ReasonReact.string(s)}
                  </div>
                | Children(children) =>
                  <div className="page-end-children">
                    children->React.array
                  </div>
                }}
             </div>
           ),
         ),
       )}
    </div>
  };

let extraClasses = (pageEnds: pageEnds) => {
  let topClass =
    switch (List.length(pageEnds.top)) {
    | 0 => ""
    | _ => "page-has-top"
    };
  let bottomClass =
    switch (List.length(pageEnds.bottom)) {
    | 0 => ""
    | _ => "page-has-bottom"
    };
  topClass ++ " " ++ bottomClass;
};

[@react.component]
let make =
    (
      ~pageEnds: pageEnds,
      ~className: option(string)=?,
      ~children: array(ReasonReact.reactElement),
    ) => {
  let extraClasses = extraClasses(pageEnds);
  <div
    className={
      "page "
      ++ extraClasses
      ++ " "
      ++ Js.Option.getWithDefault("page-default", className)
      ++ " "
      ++ pageStyle
    }>
    {mapPageEnd(pageEnds.top, "top")}
    <div
      className={"page-content-container " ++ pageContentsStyle}
      key="children">
      <div className="page-content"> children->React.array </div>
    </div>
    {mapPageEnd(pageEnds.bottom, "bottom")}
  </div>;
};