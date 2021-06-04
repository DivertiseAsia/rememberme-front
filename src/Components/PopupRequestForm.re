open ReasonReact;
open RememberMeType;

let component = ReasonReact.statelessComponent("SchedulerDate");

let make = (~isSick=true, ~loadState, ~onConfirm, _children) => {
  ...component,
  render: _self => {
    <>
      <div className="popup-bg" onClick=onConfirm />
      {switch (loadState) {
       | Failed(errMsg) =>
         <div className="container p-4 popup-container popup-container-fail">
           <div className="row">
             <div className="col-12 text-center">
               <h5> {string("Something went wrong !")} </h5>
             </div>
           </div>
           {if (errMsg != "") {
              <div className="row">
                <div className="col-12 text-center">
                  {errMsg->Utils.getErrorMsgFromJson->React.array}
                </div>
              </div>;
            } else {
              React.null;
            }}
           <div className="row">
             <div className="col-12 text-center">
               <p> {string("Please try again.")} </p>
             </div>
           </div>
           <div className="row row-btn">
             <div className="col-12 text-center">
               <button
                 type_="button"
                 className="btn btn-rounded btn-form- btn-form-active m-auto"
                 onClick=onConfirm
                 style={ReactDOMRe.Style.make(~maxWidth="120px", ())}>
                 {string("OK")}
               </button>
             </div>
           </div>
         </div>
       | _ =>
         <div className="container popup-container">
           <div className="row row-icon">
             <div className="col-12 text-center pt-4">
               <img src="/images/smiley.svg" />
             </div>
           </div>
           <div className="row row-text">
             <div
               className="col-12 text-center"
               style={ReactDOMRe.Style.make(~display="contents", ())}>
               {isSick ?
                  <div className="m-auto">
                    <h5> {string("Get well soon !")} </h5>
                  </div> :
                  <div className="m-auto">
                    <h5> {string("Waiting for approve")} </h5>
                  </div>}
             </div>
           </div>
           <div className="row row-btn">
             <div className="col-12 text-center">
               <button
                 type_="button"
                 className="btn btn-rounded btn-form- btn-form-active m-auto"
                 onClick=onConfirm
                 style={ReactDOMRe.Style.make(~maxWidth="120px", ())}>
                 {string("OK")}
               </button>
             </div>
           </div>
         </div>
       }}
    </>;
  },
};