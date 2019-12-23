open ReasonReact;

let component = ReasonReact.statelessComponent("SchedulerDate");

let make = (~isToday=false, ~datetimeStr="Day 00 Month Year", ~scheduleList=[], _children) => {
  ...component,
  render: _self => {
    let styleStatus = 
      ReactDOMRe.Style.make(
        ~border="2px solid blue", 
        ~backgroundColor=(isToday ? "blue" : "none"),
        ~marginLeft="80%",
      ());
    <div className="container mt-3">
      <div className="row">
        <div className="col">
          <p className="mb-2">
            {string(datetimeStr)}
          </p>
          <hr 
            className="mt-0"  
            style=(ReactDOMRe.Style.make(
              ~borderTop=(isToday ? "1px solid #2D506D" : "1px solid rgba(0,0,0,.1)"), ()))
          />
        </div>
      </div>
      <div className="row">
        <div className="col-2 p-0 text-right">
          <div className="scheduler-status" style=styleStatus /> 
        </div>
        <div className="col-10">
          {string("Event")}
        </div>
      </div>
    </div>;
  },
};
