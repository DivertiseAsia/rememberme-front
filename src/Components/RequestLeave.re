open ReasonReact;

let component = ReasonReact.statelessComponent("RequestLeave");

let make = (~onCancel, _children) => {
  ...component,
  render: _self => {
    <div className="request-leave-container">
      <div className="row"> 
        <div className="col-2 pr-0 text-center"> 
          <img src="/images/sick_blue.svg" />
        </div>
        <div className="col-6"> 
          <p className="mb-1"><b>{string("11 Sep 2019")}</b></p>
          <p className="mb-1">{string("description...")}</p>
          <button 
            type_="button" 
            className="btn btn-rounded btn-outline btn-cancel-request"
            onClick=(_ => ())
          >
            {string("Cancel")}
          </button>
        </div>
        <div className="col-4 pl-0"> 
          <div className="request-status btn-rounded btn-pending" onClick=onCancel>
            <img src="/images/pending.svg" style=(ReactDOMRe.Style.make(~width="13px", ~height="13px", ~marginBottom="2px", ())) />
            {string(" Pending")}
          </div>
        </div>
      </div>
    </div>;
  },
};
