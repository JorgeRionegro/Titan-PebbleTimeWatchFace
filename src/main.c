#include <pebble.h>
#include "antialiasing.h"

//Fonts
#define dFont fonts_get_system_font(FONT_KEY_GOTHIC_18) 
#define dBFont fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD)
#define sFont fonts_get_system_font(FONT_KEY_GOTHIC_14) 
#define sBFont fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD)
#define nBFont fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD)
  
#define Key_UseSeconds 0
#define Key_UseShadows 1  
#define Key_Radio 2
#define Key_ClockType 3
#define Key_HandType 4
#define Key_Bluetooth 5
#define Key_Numbers 6
 
Window *w;
Layer *dial_layer, *time_layer, *battery, *bluetooth;
TextLayer *date, *BN12, *BN3, *BN6, *BN9;
static GPoint hc, hs;
static GRect rDate, next, rBattery, nextBattery, rBluetooth, nextBluetooth, rBNumbers, nextBN12, nextBN3, nextBN6, nextBN9;
static BatteryChargeState c_state;
static int mTicks = 60, Radio = 100, a = 1, cType = 1, hType = 2, numbType = 2;
bool UseSeconds = true, UseShadows = true, DrawInside =false, viewBluetooth = true;
#ifdef PBL_COLOR
  static GColor8 ColorSphere,BackColor,ColorSeconds,ColorMinutes,ColorHours,ColorMMarks,ColorHMarks,ColorQMarks,ColorInside,ColorShadow;
  static GColor8 ColorFont,ColorBattery,ColorBNumbers,ColorSNumbers;
#else
  static GColor ColorSphere,BackColor,ColorSeconds,ColorMinutes,ColorHours,ColorMMarks,ColorHMarks,ColorQMarks,ColorInside,ColorShadow;
  static GColor ColorFont,ColorBattery,ColorBNumbers,ColorSNumbers;
#endif

static GPathInfo INSIDE_HAND_POINTS = { 5, (GPoint []) { { -1, 3}, { 1, 3}, { 3, -15}, { 0, -23}, { -3, -15} } };
static GPathInfo HOUR_POINTS = { 4, (GPoint []) { { -1, 1 }, { 1, 1}, { 1, -14}, { -1, -14} } };
static GPathInfo MINUTE_POINTS = { 4, (GPoint []) { { -1, 1 }, { 1, 1}, { 1, -5}, { -1, -5} } };
static GPathInfo HOUR_QUARTERS = { 4, (GPoint []) { { -3, 3 }, { 3, 3}, { 3, -14}, { -3, -14} } };
static GPathInfo HAND_POINTS = { 4, (GPoint []) { { -3, 3}, { 3, 3}, { 3, -30}, { -3, -30} } };
static GPathInfo HOUR_HAND = { 5, (GPoint []) { { -3,0}, {3,0}, {7, -18}, {0, -37}, {-7,-18} } };
static GPathInfo MINUTE_HAND = { 5, (GPoint []) { { -3,0}, {3,0}, {7, -20}, {0, -40}, {-6,-20} } };
static GPathInfo HOUR_HAND2 = { 5, (GPoint []) { { -4,0}, {4,0}, {4, -26}, {0, -33}, {-4,-26} } };
static GPathInfo MINUTE_HAND2 = { 5, (GPoint []) { { -4,0}, {4,0}, {4, -43}, {0, -50}, {-4,-43} } };

static GPath *minute_arrow, *minute_square, *hour_arrow, *hour_square, *thour_hand, *hour_quarters, *minute_shadow;
static GPath *hour_shadow, *thour_shadow,*minute_inside, *hour_inside, *tminute_hand, *tminute_shadow;

static void setColors (int clockType){
  switch(clockType){
    case 1: //Sport Man
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorBlueMoon,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorInside = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    break;
    case 2: //Fluor Classic
    DrawInside = true;
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorBlack, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorInchworm,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorRed,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorLightGray,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 3: //PinkPanter lovers
    BackColor = COLOR_FALLBACK(GColorBulgarianRose,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorFolly, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorLightGray,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 4: //Welcome to the Future
    BackColor = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorDarkGray, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorChromeYellow,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorPictonBlue,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorLightGray,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 5: //Green Man
    BackColor = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorPastelYellow, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorDarkGreen,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorDarkGray,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorJaegerGreen,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorPastelYellow,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    break;
    case 6: //Passion o'clock
    DrawInside = true;
    BackColor = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorFolly, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorDarkGreen,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorPastelYellow,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorElectricBlue,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorBabyBlueEyes,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorBabyBlueEyes,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorWhite);
    break;
    case 7: //Wine Taste
    BackColor = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorBulgarianRose, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorLightGray,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 8: //Bottle clock
    BackColor = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorDarkGreen, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorRed,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorLightGray,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 9: //Navy man
    BackColor = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorVividCerulean, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorOrange,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorIslamicGreen,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorOrange,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    break;
    case 10: //Rainbow Warrior
    BackColor = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorDarkGreen, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorChromeYellow,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorElectricBlue,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorCeleste,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorVividCerulean,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorWhite);
    ColorInside = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorBabyBlueEyes,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorMintGreen,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorMintGreen,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorBabyBlueEyes,GColorWhite);
    break;
    case 11: //Papyrus
    DrawInside = true;
    BackColor = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorPastelYellow, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorOxfordBlue,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorIndigo,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorInside = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    break;
    case 12: //Classic Taste
    DrawInside = true;
    BackColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorPastelYellow, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorWindsorTan,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorInside = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    break;
  }
}

static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  //Get Tuple
  Tuple *t = dict_read_first(iterator);
  
  // Process all pairs present
  while(t != NULL) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Iterator %d", (int)t->key);
    APP_LOG(APP_LOG_LEVEL_INFO, "Iterator received with value %s", t->value->cstring);
    // Process this pair's key
    switch(t->key)
    {
    case Key_UseSeconds:
      if(strcmp(t->value->cstring, "on") == 0)
      {
        UseSeconds = true;
        persist_write_bool(Key_UseSeconds, true);
      }
      else if(strcmp(t->value->cstring, "off") == 0)
      {
        UseSeconds = false;
        persist_write_bool(Key_UseSeconds, false);
      };
      break;
    case Key_UseShadows:
      if(strcmp(t->value->cstring, "on") == 1)
      {
        UseShadows = true;
        persist_write_bool(Key_UseShadows, true);
      }
      else if(strcmp(t->value->cstring, "off") == 1)
      {
        UseShadows = false;
        persist_write_bool(Key_UseShadows, false);
      };
      break;
    case Key_Radio:
       Radio = atoi(t->value->cstring);
       persist_write_int(Key_Radio, Radio);
       APP_LOG(APP_LOG_LEVEL_INFO, "Radio %d", Radio);
       break;
    case Key_ClockType:
       cType = atoi(t->value->cstring);
       //seteo colores
       setColors (cType);
       persist_write_int(Key_ClockType, cType);
       APP_LOG(APP_LOG_LEVEL_INFO, "ClockType %d", cType);
       break;
    case Key_HandType:
       hType = atoi(t->value->cstring);
       persist_write_int(Key_HandType, hType);
       APP_LOG(APP_LOG_LEVEL_INFO, "HandType %d", hType);
       break;
    case Key_Bluetooth:
      if(strcmp(t->value->cstring, "on") == 0)
      {
        viewBluetooth = true;
        persist_write_bool(Key_Bluetooth, true);
      }
      else if(strcmp(t->value->cstring, "off") == 0)
      {
        viewBluetooth = false;
        persist_write_bool(Key_Bluetooth, false);
      };
      break;
    case Key_Numbers:
       numbType = atoi(t->value->cstring);
       persist_write_int(Key_Numbers, numbType);
       APP_LOG(APP_LOG_LEVEL_INFO, "NumbersType %d", numbType);
       break;
    }
   // Get next pair, if any
   t = dict_read_next(iterator);
 }
}

static void handle_battery(BatteryChargeState c_state) {
  layer_mark_dirty(battery);
}

static void handle_bluetooth(bool connected) {
  layer_mark_dirty(bluetooth);
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  static BatteryChargeState c_state;
  c_state = battery_state_service_peek();
  
  graphics_context_set_stroke_color(ctx, c_state.is_charging ? ColorBattery : ColorSphere);
  graphics_draw_rect(ctx, GRect(0,0,15,15));
  
  graphics_context_set_stroke_color(ctx, ColorBattery);
  graphics_context_set_fill_color(ctx, ColorBattery);
  switch(c_state.charge_percent/10){
    case 10: graphics_draw_rect(ctx, GRect(3,3,12,12)); graphics_draw_rect(ctx, GRect(1,1,12,12)); break;
    case 9: graphics_draw_rect(ctx, GRect(10,2,3,3));
    case 8: graphics_draw_rect(ctx, GRect(2,2,3,3));
    case 7: graphics_draw_rect(ctx, GRect(10,6,3,3));
    case 6: graphics_draw_rect(ctx, GRect(2,6,3,3));
    case 5: graphics_draw_rect(ctx, GRect(6,2,3,3));
    case 4: graphics_draw_rect(ctx, GRect(6,6,3,3));
    case 3: graphics_draw_rect(ctx, GRect(10,10,3,3));
    case 2: graphics_draw_rect(ctx, GRect(2,10,3,3));
    case 1: graphics_draw_rect(ctx, GRect(6,10,3,3));
  }
}

static void bluetooth_update_proc(Layer *layer, GContext *ctx) {
  if (viewBluetooth == true) {
  graphics_context_set_stroke_color(ctx, bluetooth_connection_service_peek() ?  ColorSphere : ColorBattery);
  GPoint bt;
  bt.x = 15;
  bt.y = 15;
  graphics_draw_circle(ctx, bt, 10);
  }
}

void dial_layer_update(Layer *me, GContext *ctx) {
  //BackColor
  graphics_context_set_fill_color(ctx, BackColor);
  graphics_fill_rect(ctx, layer_get_bounds(me), 0, GCornerNone);
  
  //Clock Sphere
  //#ifdef PBL_COLOR
  //  graphics_fill_circle_antialiased(ctx, hc, Radio, ColorSphere);
  //#else
    graphics_context_set_fill_color(ctx, ColorSphere);
    graphics_fill_circle(ctx, hc, Radio);
  //#endif

  // draw hour dial points
  GPoint ray;
  GPoint sEnd;
  for (int x=1; x <= mTicks; x++) {
      int32_t angle = TRIG_MAX_ANGLE / mTicks * x;
      ray.y = (int16_t)(-cos_lookup(angle) *
	      (int32_t)Radio*0.81 / TRIG_MAX_RATIO) + hc.y;
      ray.x = (int16_t)(sin_lookup(angle) *
	      (int32_t)Radio*0.81 / TRIG_MAX_RATIO) + hc.x;
      sEnd.y = (int16_t)(-cos_lookup(angle) *
	      (int32_t)(Radio-10) / TRIG_MAX_RATIO) + hc.y;
      sEnd.x = (int16_t)(sin_lookup(angle) *
	      (int32_t)(Radio-10) / TRIG_MAX_RATIO) + hc.x;
    
    if (x % 5 != 0) {
      graphics_context_set_stroke_color(ctx, ColorMMarks);
      graphics_context_set_fill_color(ctx, ColorMMarks);
      gpath_move_to(minute_square, ray);
      gpath_rotate_to(minute_square, angle);
      gpath_draw_filled(ctx, minute_square);
      gpath_draw_outline(ctx, minute_square);
    } else if (x % 15 != 0) {
      graphics_context_set_stroke_color(ctx, ColorHMarks);
      graphics_context_set_fill_color(ctx, ColorHMarks);
      gpath_move_to(hour_square, ray);
      gpath_rotate_to(hour_square, angle);
      gpath_draw_filled(ctx, hour_square);
      gpath_draw_outline(ctx, hour_square);
    }
    else{
      graphics_context_set_stroke_color(ctx, ColorQMarks);
      graphics_context_set_fill_color(ctx, ColorQMarks);
      gpath_move_to(hour_quarters, ray);
      gpath_rotate_to(hour_quarters, angle);
      gpath_draw_filled(ctx, hour_quarters);
      gpath_draw_outline(ctx, hour_quarters);
      graphics_context_set_stroke_color(ctx, ColorSphere);
      graphics_draw_line(ctx, ray, sEnd);
    }
  }

}

void time_layer_update(Layer *me, GContext *ctx) {
  struct tm *now;
  time_t t = time(NULL);
  now = localtime(&t);
  
  GPoint rHand;
  GPoint sHand;
  GPoint eHand;
  GPoint esHand;
  GPoint iHand;
  int32_t hh_angle = (TRIG_MAX_ANGLE*(((now->tm_hour%12)*6)+(now->tm_min/10)))/(12*6);
  rHand.x = ((int32_t)(sin_lookup(hh_angle) * (int32_t)Radio*0.20 / TRIG_MAX_RATIO) + hc.x);
  rHand.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)Radio*0.20 / TRIG_MAX_RATIO) + hc.y);
  sHand.x = ((int32_t)(sin_lookup(hh_angle) * (int32_t)Radio*0.20 / TRIG_MAX_RATIO) + hs.x);
  sHand.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)Radio*0.20 / TRIG_MAX_RATIO) + hs.y);
  eHand.x = ((int32_t)(sin_lookup(hh_angle) * (int32_t)(Radio*0.20) / TRIG_MAX_RATIO) + hc.x);
  eHand.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.20) / TRIG_MAX_RATIO) + hc.y);
  esHand.x = ((int32_t)(sin_lookup(hh_angle) * (int32_t)(Radio*0.20) / TRIG_MAX_RATIO) + hs.x);
  esHand.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.20) / TRIG_MAX_RATIO) + hs.y);
  iHand.x = ((int32_t)(sin_lookup(hh_angle) * (int32_t)Radio*0.27 / TRIG_MAX_RATIO) + hc.x);
  iHand.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)Radio*0.27 / TRIG_MAX_RATIO) + hc.y);
  
  GPoint mHand;
  GPoint smHand;
  GPoint emHand;
  GPoint esmHand;
  GPoint imHand;
  int32_t mi_angle = TRIG_MAX_ANGLE * (now->tm_min) / 60;
  mHand.x = ((int32_t)(sin_lookup(mi_angle) * (int32_t)Radio*0.28 / TRIG_MAX_RATIO) + hc.x);
  mHand.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)Radio*0.28 / TRIG_MAX_RATIO) + hc.y);
  smHand.x = ((int32_t)(sin_lookup(mi_angle) * (int32_t)Radio*0.28 / TRIG_MAX_RATIO) + hs.x);
  smHand.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)Radio*0.28 / TRIG_MAX_RATIO) + hs.y);
  emHand.x = ((int32_t)(sin_lookup(mi_angle) * (int32_t)(Radio*0.28) / TRIG_MAX_RATIO) + hc.x);
  emHand.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(Radio*0.28) / TRIG_MAX_RATIO) + hc.y);
  esmHand.x = ((int32_t)(sin_lookup(mi_angle) * (int32_t)(Radio*0.28) / TRIG_MAX_RATIO) + hs.x);
  esmHand.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(Radio*0.28) / TRIG_MAX_RATIO) + hs.y);
  imHand.x = ((int32_t)(sin_lookup(mi_angle) * (int32_t)Radio*0.38 / TRIG_MAX_RATIO) + hc.x);
  imHand.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)Radio*0.38 / TRIG_MAX_RATIO) + hc.y);
  
  int32_t ss_angle = TRIG_MAX_ANGLE / 60 * (now->tm_sec);

  //center shadow
  if (UseShadows==true){
    graphics_context_set_fill_color(ctx, ColorShadow);
    graphics_fill_circle(ctx, hs, 6);
  }
  
  //draw shadows
  if (UseShadows==true){
    graphics_context_set_fill_color(ctx, ColorShadow);
    graphics_context_set_stroke_color(ctx, ColorShadow);
    gpath_move_to(tminute_shadow, smHand);
    gpath_rotate_to(tminute_shadow, mi_angle);
    gpath_move_to(minute_shadow, hs);
    gpath_rotate_to(minute_shadow, mi_angle);
    gpath_draw_filled(ctx, minute_shadow);
    //gpath_draw_outline(ctx, minute_shadow);    
    gpath_draw_filled(ctx, tminute_shadow);
    //gpath_draw_outline(ctx, tminute_shadow);
    if (hType != 3){
      graphics_fill_circle(ctx, esmHand, 3);
    }
    graphics_context_set_fill_color(ctx, ColorShadow);
    graphics_context_set_stroke_color(ctx, ColorShadow);
    gpath_move_to(thour_shadow, sHand);
    gpath_rotate_to(thour_shadow, hh_angle);
    gpath_move_to(hour_shadow, hs);
    gpath_rotate_to(hour_shadow, hh_angle);
    gpath_draw_filled(ctx, hour_shadow);
    //gpath_draw_outline(ctx, hour_shadow);
    gpath_draw_filled(ctx, thour_shadow);
    //gpath_draw_outline(ctx, thour_shadow);
    if (hType != 3){
       graphics_fill_circle(ctx, esHand, 3);
    }   
  }
  if (UseSeconds == true&&UseShadows==true) {
      GPoint ss;
      ss.y = (int16_t)(-cos_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hs.y;
      ss.x = ((int16_t)(sin_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hs.x);
    //#ifdef PBL_COLOR
    //  graphics_draw_line_antialiased(ctx, hc, ss, ColorShadow);
    //#else
      graphics_context_set_stroke_color(ctx, ColorShadow);
      graphics_draw_line(ctx, hc, ss);
    //#endif
  }

  // Draw minute hand
  graphics_context_set_fill_color(ctx, ColorMinutes);
  graphics_context_set_stroke_color(ctx, ColorMinutes);
  gpath_move_to(minute_arrow, hc);
  gpath_rotate_to(minute_arrow, mi_angle);
  gpath_move_to(tminute_hand, mHand);
  gpath_rotate_to(tminute_hand, mi_angle);
  //#ifdef PBL_COLOR
  //  gpath_draw_filled_antialiased(ctx, minute_arrow, ColorMinutes);
  //  gpath_draw_filled_antialiased(ctx, tminute_hand, ColorMinutes);
  //#else
    gpath_draw_filled(ctx, minute_arrow);
    gpath_draw_filled(ctx, tminute_hand);    
  //#endif
  if (hType != 3){
    graphics_fill_circle(ctx, emHand, 3);
  }
  if (DrawInside == true) {
     graphics_context_set_fill_color(ctx, ColorInside);
     graphics_context_set_stroke_color(ctx, ColorInside);
    if (hType==1) {
     gpath_move_to(minute_inside, imHand);
     gpath_rotate_to(minute_inside, mi_angle);
     gpath_draw_filled(ctx, minute_inside);
    }
  }
  if (hType != 3){
      graphics_context_set_fill_color(ctx, ColorInside);
      graphics_fill_circle(ctx, emHand, 1);
  }

  // Draw hour hand
  graphics_context_set_fill_color(ctx, ColorHours);
  graphics_context_set_stroke_color(ctx, ColorHours);
  gpath_move_to(hour_arrow, hc);
  gpath_rotate_to(hour_arrow, hh_angle);
  gpath_move_to(thour_hand, rHand);
  gpath_rotate_to(thour_hand, hh_angle);
  //#ifdef PBL_COLOR
  //  gpath_draw_filled_antialiased(ctx, hour_arrow, ColorHours);
  //  gpath_draw_filled_antialiased(ctx, thour_hand, ColorHours);
  //#else
    gpath_draw_filled(ctx, hour_arrow);
    gpath_draw_filled(ctx, thour_hand);
  //#endif
  if (hType != 3){
    graphics_fill_circle(ctx, eHand, 3);
  }
  if (DrawInside == true) {
        graphics_context_set_fill_color(ctx, ColorInside);
        graphics_context_set_stroke_color(ctx, ColorInside);
      if (hType==1) {
        gpath_move_to(hour_inside, iHand);
        gpath_rotate_to(hour_inside, hh_angle);
        gpath_draw_filled(ctx, hour_inside);
      }
    }
    if (hType != 3){
          graphics_context_set_fill_color(ctx, ColorInside);
          graphics_fill_circle(ctx, eHand, 1);
    }

  //draw seconds hand
  if (UseSeconds == true) {
      GPoint sc;
      int32_t sec_angle = TRIG_MAX_ANGLE / 60 * (now->tm_sec);
      sc.y = (int16_t)(-cos_lookup(sec_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hc.y;
      sc.x = ((int16_t)(sin_lookup(sec_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hc.x);
    //#ifdef PBL_COLOR
    //  graphics_draw_line_antialiased(ctx, hc, sc, ColorSeconds);
    //#else
      graphics_context_set_stroke_color(ctx, ColorSeconds);
      graphics_draw_line(ctx, hc, sc);
    //#endif
  }
  
  //draw centre circles
  graphics_context_set_fill_color(ctx, ColorHours);
  graphics_fill_circle(ctx, hc, 6);
   
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, ColorMinutes);
  #else
    graphics_context_set_fill_color(ctx, ColorInside);
  #endif
  graphics_fill_circle(ctx, hc, 2);
  
}

void handle_tick(struct tm *now, TimeUnits units_changed) {
  setlocale(LC_TIME, ""); 
  layer_mark_dirty(dial_layer);
  layer_mark_dirty(time_layer);
  layer_mark_dirty((Layer *)date);
  layer_mark_dirty((Layer *)BN12);
  layer_mark_dirty((Layer *)BN3);
  layer_mark_dirty((Layer *)BN6);
  layer_mark_dirty((Layer *)BN9);
  
  static char date_buf[] = "Mon.  1";
  // Set Centre, date and Battery every 5 minutes
  if (UseSeconds == false) {
      if ((now->tm_min) % 5 == 0 || a==1) {
      c_state = battery_state_service_peek();
      strftime(date_buf, sizeof("Mon.  1"), "%a. %e", now);
      text_layer_set_text(date, date_buf);
      hc = GPoint((int16_t)(-sin_lookup(TRIG_MAX_ANGLE / 60 * (now->tm_min)) *
  		     (int32_t)(Radio-72) / TRIG_MAX_RATIO) + 144 / 2, 
           (int16_t)(cos_lookup(TRIG_MAX_ANGLE / 60 * (now->tm_min)) *
  		     (int32_t)(Radio-72) / TRIG_MAX_RATIO) +  168 / 2);
      hs.x=hc.x+2;
      hs.y=hc.y+4;
      a = 0;
        
      next = GRect(hc.x-45,hc.y+15,90,90);
      nextBattery = GRect(hc.x-7,hc.y-33,15,15);
      nextBluetooth = GRect(hc.x-15,hc.y-41,30,30);
      layer_set_frame(text_layer_get_layer(date), next);
      layer_set_frame(battery, nextBattery);
      layer_set_update_proc(battery, battery_update_proc);
      layer_set_frame(bluetooth, nextBluetooth);
      layer_set_update_proc(bluetooth, bluetooth_update_proc); 
         
      if (numbType != 0) {
        nextBN12 = GRect(hc.x-15,hc.y-15-Radio*0.65,30,30);
        nextBN3 = GRect(hc.x-15+Radio*0.65,hc.y-20,30,30);
        nextBN6 = GRect(hc.x-15,hc.y-20+Radio*0.65,30,30);
        nextBN9 = GRect(hc.x-13-Radio*0.65,hc.y-20,30,30);
        if (numbType == 2 || numbType == 4) {
          text_layer_set_text(BN12, "12");
          text_layer_set_text(BN3, "3");
          text_layer_set_text(BN6, "6");
          text_layer_set_text(BN9, "9");
        }
        if (numbType == 1 || numbType == 1) {
          text_layer_set_text(BN12, "XII");
          text_layer_set_text(BN3, "III");
          text_layer_set_text(BN6, "VI");
          text_layer_set_text(BN9, "IX");
        }        
        layer_set_frame(text_layer_get_layer(BN12), nextBN12);
        layer_set_frame(text_layer_get_layer(BN3), nextBN3);
        layer_set_frame(text_layer_get_layer(BN6), nextBN6);
        layer_set_frame(text_layer_get_layer(BN9), nextBN9);
      }
        
     } 
  } else {
    if (((now->tm_min) % 5 == 0  && (now->tm_sec) == 0) || a==1) {
      c_state = battery_state_service_peek();
      strftime(date_buf, sizeof("Mon.  1"), "%a. %e", now);
      text_layer_set_text(date, date_buf);
      hc = GPoint((int16_t)(-sin_lookup(TRIG_MAX_ANGLE / 60 * (now->tm_min)) *
  		   (int32_t)(Radio-72) / TRIG_MAX_RATIO) + 144 / 2, 
         (int16_t)(cos_lookup(TRIG_MAX_ANGLE / 60 * (now->tm_min)) *
  		   (int32_t)(Radio-72) / TRIG_MAX_RATIO) +  168 / 2);
      hs.x=hc.x+2;
      hs.y=hc.y+3;
      a = 0;
      next = GRect(hc.x-45,hc.y+15,90,90);
      nextBattery = GRect(hc.x-7,hc.y-33,15,15);
      nextBluetooth = GRect(hc.x-15,hc.y-41,30,30);
      layer_set_frame(text_layer_get_layer(date), next);
      layer_set_frame(battery, nextBattery);
      layer_set_update_proc(battery, battery_update_proc);
      layer_set_frame(bluetooth, nextBluetooth);
      layer_set_update_proc(bluetooth, bluetooth_update_proc); 
      
      if (numbType != 0) {
        nextBN12 = GRect(hc.x-15,hc.y-15-Radio*0.65,30,30);
        nextBN3 = GRect(hc.x-15+Radio*0.65,hc.y-20,30,30);
        nextBN6 = GRect(hc.x-15,hc.y-20+Radio*0.65,30,30);
        nextBN9 = GRect(hc.x-13-Radio*0.65,hc.y-20,30,30);
        if (numbType == 2 || numbType == 4) {
          text_layer_set_text(BN12, "12");
          text_layer_set_text(BN3, "3");
          text_layer_set_text(BN6, "6");
          text_layer_set_text(BN9, "9");
        }
        if (numbType == 1 || numbType == 1) {
          text_layer_set_text(BN12, "XII");
          text_layer_set_text(BN3, "III");
          text_layer_set_text(BN6, "VI");
          text_layer_set_text(BN9, "IX");
        }        
        layer_set_frame(text_layer_get_layer(BN12), nextBN12);
        layer_set_frame(text_layer_get_layer(BN3), nextBN3);
        layer_set_frame(text_layer_get_layer(BN6), nextBN6);
        layer_set_frame(text_layer_get_layer(BN9), nextBN9);
      }
   }
  }
}

void handle_init(void) {
  w = window_create();
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  //Check for saved KEY options
  if (persist_exists(Key_UseSeconds)) {
    bool K_US = persist_read_bool(Key_UseSeconds);
    if(K_US == false) { UseSeconds = false; }  else  { UseSeconds = true; }
  }
  #ifdef PBL_COLOR
  if (persist_exists(Key_UseShadows)) {
    bool K_US = persist_read_bool(Key_UseShadows);
    if(K_US == false) { UseShadows = false; }  else  { UseShadows = true; }
  }
  #else
    UseShadows = false;
  #endif
  if (persist_exists(Key_Radio)) {
    Radio = persist_read_int(Key_Radio);
  }
  if (persist_exists(Key_ClockType)) {
    cType = persist_read_int(Key_ClockType);
  }
  if (persist_exists(Key_HandType)) {
    hType = persist_read_int(Key_HandType);
  }
  if (persist_exists(Key_Bluetooth)) {
    bool K_US = persist_read_bool(Key_Bluetooth);
    if(K_US == false) { viewBluetooth = false; }  else  { viewBluetooth = true; }
  }  
  if (persist_exists(Key_Numbers)) {
    numbType = persist_read_int(Key_Numbers );
  }
  
  //seteo colores
  setColors (cType);
   
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  
  window_set_fullscreen(w, true);
  GRect bounds = GRect(0,0,144,168); //layer_get_bounds(window_layer);
  minute_arrow = gpath_create(&HAND_POINTS);
  if (hType == 1){
   tminute_hand = gpath_create(&MINUTE_HAND);
  } else if (hType == 2) {
   tminute_hand = gpath_create(&MINUTE_HAND2);
  } else {
   tminute_hand = gpath_create(&MINUTE_HAND2); 
  }
  tminute_shadow = tminute_hand;
  hour_arrow = gpath_create(&HAND_POINTS);
  if (hType == 1){
   thour_hand = gpath_create(&HOUR_HAND);
  } else if (hType == 2) {
   thour_hand = gpath_create(&HOUR_HAND2); 
  } else {
   thour_hand = gpath_create(&HOUR_HAND2); 
  }
  thour_shadow = thour_hand;
  minute_inside = gpath_create(&INSIDE_HAND_POINTS);
  hour_inside = gpath_create(&INSIDE_HAND_POINTS);
  hour_shadow = hour_arrow;
  minute_shadow = minute_arrow;
  hour_square = gpath_create(&HOUR_POINTS);
  minute_square = gpath_create(&MINUTE_POINTS);
  hour_quarters = gpath_create(&HOUR_QUARTERS);
  dial_layer = layer_create(bounds);
  time_layer = layer_create(bounds);
  
  //date
  rDate = GRect(hc.x-45,hc.y+15,90,90);
  date = text_layer_create(rDate);
  text_layer_set_background_color(date, GColorClear);
  text_layer_set_text_color(date, ColorFont);
  text_layer_set_text(date, "");
  text_layer_set_text_alignment(date, GTextAlignmentCenter);
  text_layer_set_font(date, dBFont); 
  
  //BigNumbers
  rBNumbers = GRect(hc.x,hc.y,30,30);
  BN12 = text_layer_create(rBNumbers);
  text_layer_set_background_color(BN12, GColorClear);
  text_layer_set_text_color(BN12, ColorBNumbers);
  text_layer_set_text(BN12, "");
  text_layer_set_text_alignment(BN12, GTextAlignmentCenter);
  text_layer_set_font(BN12, nBFont);
  BN3 = text_layer_create(rBNumbers);
  text_layer_set_background_color(BN3, GColorClear);
  text_layer_set_text_color(BN3, ColorBNumbers);
  text_layer_set_text(BN3, "");
  text_layer_set_text_alignment(BN3, GTextAlignmentCenter);
  text_layer_set_font(BN3, nBFont);
  BN6 = text_layer_create(rBNumbers);
  text_layer_set_background_color(BN6, GColorClear);
  text_layer_set_text_color(BN6, ColorBNumbers);
  text_layer_set_text(BN6, "");
  text_layer_set_text_alignment(BN6, GTextAlignmentCenter);
  text_layer_set_font(BN6, nBFont);
  BN9 = text_layer_create(rBNumbers);
  text_layer_set_background_color(BN9, GColorClear);
  text_layer_set_text_color(BN9, ColorBNumbers);
  text_layer_set_text(BN9, "");
  text_layer_set_text_alignment(BN9, GTextAlignmentCenter);
  text_layer_set_font(BN9, nBFont);
  
  // battery
  rBattery = GRect(hc.x-7,hc.y-33,15,15);
  battery = layer_create(rBattery);

  // bluetooth
  rBluetooth = GRect(hc.x-15,hc.y-41, 30, 30);
  bluetooth = layer_create(rBluetooth);

  layer_set_update_proc(time_layer, time_layer_update);
  layer_set_update_proc(dial_layer, dial_layer_update);
  layer_set_update_proc(battery, battery_update_proc);
  layer_set_update_proc(bluetooth, bluetooth_update_proc);
  layer_add_child(window_get_root_layer(w), dial_layer);
  layer_add_child(window_get_root_layer(w), (Layer *)date);
  layer_add_child(window_get_root_layer(w), (Layer *)BN12);
  layer_add_child(window_get_root_layer(w), (Layer *)BN3);
  layer_add_child(window_get_root_layer(w), (Layer *)BN6);
  layer_add_child(window_get_root_layer(w), (Layer *)BN9);
  layer_add_child(window_get_root_layer(w), (Layer *)battery);
  layer_add_child(window_get_root_layer(w), (Layer *)bluetooth);
  layer_add_child(window_get_root_layer(w), time_layer);

  window_stack_push(w, true /*Animated*/);
  if (UseSeconds == true) {
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);  
  }
  else
  {
  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  }
}

void handle_deinit(void) {
  layer_destroy(time_layer);
  layer_destroy(dial_layer);
  layer_destroy((Layer *)date);
  layer_destroy((Layer *)BN12);
  layer_destroy((Layer *)BN3);
  layer_destroy((Layer *)BN6);
  layer_destroy((Layer *)BN9);
  layer_destroy(battery);
  layer_destroy(bluetooth);

  gpath_destroy(minute_arrow);
  gpath_destroy(hour_arrow);
  gpath_destroy(thour_hand); 
  gpath_destroy(tminute_hand); 
  gpath_destroy(hour_square);
  gpath_destroy(minute_square);
  gpath_destroy(hour_quarters);
  gpath_destroy(minute_inside);

  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  tick_timer_service_unsubscribe();
  window_destroy(w);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}

