open Belt;
open ReasonReact;

type navItem = {
  href: string,
  match: string,
  matchIfNone: bool,
  caption: string,
};

let isSelected =
    (currentItem: navItem, items: list(navItem), current: string) => {
  let matchesNotMe =
    ListLabels.exists(
      ~f=x => x.match === current && x.match !== currentItem.match,
      items,
    );
  switch (currentItem.matchIfNone) {
  | false => currentItem.match === current
  | true => !matchesNotMe
  };
};

[@react.component]
let make =
    (~items: list(navItem), ~current: string, ~className: option(string)=?) => {
  <nav
    className={
      "submenu " ++ Js.Option.getWithDefault("submenu-default", className)
    }>
    {items
     ->Belt.List.map(item =>
         <li
           key={item.match}
           className={isSelected(item, items, current) ? "selected" : ""}>
           <Link href={item.href}> {string(item.caption)} </Link>
         </li>
       )
     ->List.toArray
     ->React.array}
  </nav>;
};