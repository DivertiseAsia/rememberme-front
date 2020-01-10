open ReasonReact;

let component = ReasonReact.statelessComponent("SchedulerDate");

let make = (~isSick=true, ~onConfirm, _children) => {
  ...component,
  render: _self => {
    <>
      <div className="popup-bg" onClick=onConfirm>
      </div>
      <div className="container popup-container">
        <div className="row row-icon"> 
          <div className="col-12 text-center pt-4"> 
            <img src="/images/smiley.svg" />
          </div>
        </div>
        <div className="row row-text"> 
          <div className="col-12 text-center" style=(ReactDOMRe.Style.make(~display="contents", ()))>
            {(isSick ? 
              <div className="m-auto">
                <h5>{string("Get well soon !")}</h5>
                /*<h5>{string("change following the event..")}</h5>*/
                <p className="mb-0">{string("waiting for..")}</p>
              </div>
              : 
              <div className="m-auto">
                <h5>{string("Waiting for approve")}</h5>
              </div>
            )}
            
          </div>
        </div>
        <div className="row row-btn"> 
          <div className="col-12 text-center"> 
            <button 
              type_="button" 
              className="btn btn-rounded btn-form- btn-form-active m-auto"
              onClick=onConfirm
              style=(ReactDOMRe.Style.make(~maxWidth="120px", ()))
            >
              {string("OK")}
            </button>
          </div>
        </div>
      </div>
    </>;
  },
};
