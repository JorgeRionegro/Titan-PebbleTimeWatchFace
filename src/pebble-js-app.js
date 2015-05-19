Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    var settings = localStorage.getItem('settings')? localStorage.getItem('settings') : '';
    var url_args = settings.replace(/[&\/\\#+()$~%.'":*?<>{}]/g,'_');
    url_args = url_args.replace('__','?');
    url_args = url_args.replace(/_,_/g,'&');
    url_args = url_args.replace(/___/g,'=');
    url_args = url_args.replace('__','');
    //console.log("url_args: " + url_args); 
    
    //get current platform
    var platform;
    if(Pebble.getActiveWatchInfo) {
      // Available for use! aplite or basalt
      platform = Pebble.getActiveWatchInfo().platform;
    } else {
      // Not available, handle gracefully
      platform = 'aplite';
    }
    
    if (platform=='aplite') {
            Pebble.openURL("https://dl.dropboxusercontent.com/u/34982193/TitanBW_v6_5.html"+url_args);
            console.log("Connected to http configuration page!");
    } else {
            Pebble.openURL("https://dl.dropboxusercontent.com/u/34982193/Titan_v6_5.html"+url_args);
            console.log("Connected to http configuration page!");
    }
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    localStorage.setItem("settings", JSON.stringify(configuration));
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
       "Key_DateBox": configuration.DateBox,
       "Key_Crown": configuration.UseCrown,
       "Key_Ticks": configuration.HandleTicks
       },
      function(e) {console.log("Settings data received..."); },
      function(e) {
        console.log("Unable to send message. Error is: ");
        console.log( JSON.stringify(e.error.message));
      }
    );
  }
);

