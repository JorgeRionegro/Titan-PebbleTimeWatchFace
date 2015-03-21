Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    Pebble.openURL("http://www.ukuyu.com/p/doctype-html-html-head-title.html");
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
 
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {"Key_UseSeconds": configuration.Showseconds,
       "Key_UseShadows": configuration.Showshadows,
       "Key_Radio": configuration.Radio,
       "Key_ClockType": configuration.ClockType,
       "Key_HandType": configuration.HandType,
       "Key_Bluetooth": configuration.ShowBluetooth,
       "Key_Numbers": configuration.NumbersType},
      function(e) {
        console.log("Sending settings data...");
      },
      function(e) {
        console.log("Settings feedback failed!");
      }
    );
  }
);

