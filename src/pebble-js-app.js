Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    Pebble.openURL("http://www.ukuyu.com/p/doctype-html-html-head-title.html");
    console.log("Connected to http configuration page!");
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
    //{"Showseconds":"on","Showshadows":"off","Radio":"110","ClockType":"1","HandType":"0","ShowBluetooth":"on","NumbersType":"1","DateBox":"on"}
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {"Key_UseSeconds": configuration.Showseconds,
       "Key_UseShadows": configuration.Showshadows,
       "Key_Radio": configuration.Radio,
       "Key_ClockType": configuration.ClockType,
       "Key_HandType": configuration.HandType, 
       "Key_Bluetooth": configuration.ShowBluetooth,
       "Key_Numbers": configuration.NumbersType,
       "Key_DateBox": configuration.DateBox
       },
      function(e) {console.log("Settings data received..."); },
      function(e) {
        console.log("Unable to send message. Error is: ");
        console.log( JSON.stringify(e.error.message));
      }
    );
  }
);

