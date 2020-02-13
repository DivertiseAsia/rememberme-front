let _gaLoad = () => [%bs.raw
  {|
    (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
    (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
    m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
    })(window,document,'script','https://www.google-analytics.com/analytics.js','ga')
  |}
];

let ga: (string, string, string) => unit = [%bs.raw
  {|
    function(param1, param2, param3) {
      if (!window.ga) {
        console.warn("no ga detected");
        return;
      }
      if (param3) {
        console.log('ga('+param1+','+param2+','+param3+')');
        window.ga(param1, param2, param3);
      } else {
        console.log('ga('+param1+','+param2+')');
        window.ga(param1, param2);
      }

    }
  |}
];

let gaInit = (gaId: string) =>
  switch (gaId) {
  | "" => Js.log("No ga id")
  | id =>
    let _ = _gaLoad();
    ga("create", id, "auto");
  };

let _fbqLoad = () => [%bs.raw
  {|
    !function(f,b,e,v,n,t,s)
    {if(f.fbq)return;n=f.fbq=function(){n.callMethod?
    n.callMethod.apply(n,arguments):n.queue.push(arguments)};
    if(!f._fbq)f._fbq=n;n.push=n;n.loaded=!0;n.version='2.0';
    n.queue=[];t=b.createElement(e);t.async=!0;
    t.src=v;s=b.getElementsByTagName(e)[0];
    s.parentNode.insertBefore(t,s)}(window, document,'script',
    "https://connect.facebook.net/en_US/fbevents.js")
  |}
];

let fbq: (string, string) => unit = [%bs.raw
  {|
    function(param1, param2) {
      if (!window.fbq) {
        console.warn("no fb pixel detected");
        return;
      }
      console.log('fbq('+param1+','+param2+')');
      window.fbq(param1, param2);
    }
  |}
];

let fbqInit = (pixel: string) =>
  switch (pixel) {
  | "" => Js.log("No pixel")
  | p =>
    let _ = _fbqLoad();
    fbq("init", p);
  };