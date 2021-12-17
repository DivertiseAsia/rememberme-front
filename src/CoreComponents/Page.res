open Belt

type pageEnd =
  | Text(string)
  | Children(array<React.element>)

type pageEnds = {
  top: list<pageEnd>,
  bottom: list<pageEnd>,
}

let mapPageEnd = (pageEndArray: list<pageEnd>, extraClass: string) =>
  switch List.length(pageEndArray) {
  | 0 => React.null
  | _ =>
    <div className={"page-end-container page-end-" ++ extraClass} key=extraClass>
      {React.array(
        List.toArray(
          List.mapWithIndex(pageEndArray, (i, info: pageEnd) =>
            <div className={"page-end page-end-" ++ string_of_int(i)} key={string_of_int(i)}>
              {switch (info: pageEnd) {
              | Text(s) => <div className="page-end-text"> {React.string(s)} </div>
              | Children(children) =>
                <div className="page-end-children"> {children->React.array} </div>
              }}
            </div>
          ),
        ),
      )}
    </div>
  }

let extraClasses = (pageEnds: pageEnds) => {
  let topClass = switch List.length(pageEnds.top) {
  | 0 => ""
  | _ => "page-has-top"
  }
  let bottomClass = switch List.length(pageEnds.bottom) {
  | 0 => ""
  | _ => "page-has-bottom"
  }
  topClass ++ (" " ++ bottomClass)
}

@react.component
let make = (
  ~pageEnds: pageEnds,
  ~className: option<string>=?,
  ~children: React.element,
) => {
  let extraClasses = extraClasses(pageEnds)
  <div
    className={"page " ++
    (extraClasses ++
    (" " ++ (Js.Option.getWithDefault("page-default", className))))}>
    {mapPageEnd(pageEnds.top, "top")}
    <div className="page-content-container" key="children">
      <div className="page-content"> children </div>
    </div>
    {mapPageEnd(pageEnds.bottom, "bottom")}
  </div>
}
