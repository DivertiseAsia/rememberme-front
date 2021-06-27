[@react.component]
let make = (~isLoggedIn: bool) => {
  <nav className="nav -text-right">
    <Logo className="logo-header" />
    <input type_="checkbox" />
    <div className="nav-icon"> <div /> <div /> <div /> </div>
    <div className="nav-container -text-right">
      {!isLoggedIn
         ? <div className="header_btn-container">
             <Link href=Links.login className="btn btn-line">
               {React.string(Locale.link_login)}
             </Link>
           </div>
         : <div className="subnav-container">
             <input type_="checkbox" id="myaccountsub" />
             <div className="subnav">
               <Link href=Links.home className="sublink">
                 {React.string(Locale.link_home)}
               </Link>
               <Link href=Links.logout className="sublink">
                 {React.string(Locale.link_logout)}
               </Link>
             </div>
           </div>}
    </div>
  </nav>;
};