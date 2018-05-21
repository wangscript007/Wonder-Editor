open Sinon;

let stubGetElementByIdReturnFakeDom = [%bs.raw
  {|
    function(fakeDom){
        document.getElementById = (id) => {
          return fakeDom;
        };

        return fakeDom;
    }
|}
];

let buildFakeDom = [%bs.raw
  {|
  function(){
    return {
      eventDataMap: {},
      addEventListener: function(eventName, handleFunc) {
        var eventDataMap = this.eventDataMap;
        if(!!!eventDataMap[eventName]){
          eventDataMap[eventName] = [handleFunc];
          return;
        }

        eventDataMap[eventName].push(handleFunc);
      },
      removeEventListener: function(eventName){
      }
    }
  }
  |}
];

let triggerEvent = [%bs.raw
  {|
    function (fakeDom, eventName, fakeEvent){
      fakeDom.eventDataMap[eventName].forEach((handleFunc) => {
        handleFunc(fakeEvent);
      })
    }
    |}
];