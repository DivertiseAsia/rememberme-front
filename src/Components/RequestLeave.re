open ReasonReact;
open RememberMeApi;
open RememberMeType;
open RememberMeUtils;

let component = ReasonReact.statelessComponent("RequestLeave");

let make = (~requestLeave:leaveDetail, ~onCancel, _children) => {
  ...component,
  render: _self => {
    let isOneDay = (requestLeave.fromDate |> Js.Date.valueOf) === (requestLeave.toDate |> Js.Date.valueOf);
    let datetimeStr = (isOneDay ? 
      (requestLeave.fromDate |> Js.Date.valueOf |> getDatetimeStr) : 
      (requestLeave.fromDate |> Js.Date.valueOf |> getDatetimeStr) ++ " - " ++ (requestLeave.toDate |> Js.Date.valueOf |> getDatetimeStr));
    <div className="request-leave-container">
      <div className="row"> 
        <div className="col-2 pr-0 text-center"> 
          <img src="/images/sick_blue.svg" />
        </div>
        <div className="col-6"> 
          <p className="mb-1"><b>{string(datetimeStr)}</b></p>
          <p className="mb-1">{string(requestLeave.reason)}</p>
          {
            switch requestLeave.status {
            | Pending => 
              <button 
                type_="button" 
                className="btn btn-rounded btn-outline btn-cancel-request"
                onClick=onCancel
              >
                {string("Cancel")}
              </button>
            | _ => null
            }
          }
        </div>
        <div className="col-4 pl-0"> 
          {
            switch requestLeave.status {
            | Pending => 
              <div className="request-status btn-rounded btn-pending" onClick=onCancel>
                <img src="/images/pending.svg" style=(ReactDOMRe.Style.make(~width="13px", ~height="13px", ~marginBottom="2px", ())) />
                {string(" Pending")}
              </div>
            | Fail => 
              <div className="request-status btn-rounded btn-fail">
                <img src="/images/pending.svg" style=(ReactDOMRe.Style.make(~width="13px", ~height="13px", ~marginBottom="2px", ())) />
                {string(" Rejected")}
              </div>
            | Approve => 
              <div className="request-status btn-rounded btn-approve">
                <img src="/images/pending.svg" style=(ReactDOMRe.Style.make(~width="13px", ~height="13px", ~marginBottom="2px", ())) />
                {string(" Arrpoved")}
              </div>
            };
          }
          
        </div>
      </div>
    </div>;
  },
};
