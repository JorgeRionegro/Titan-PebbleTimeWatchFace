#include <pebble.h>

//Fonts
#define dBFont fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD)
#define nSFont fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD) 
#define nBFont fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD)
  
#define Key_UseSeconds 0
#define Key_UseShadows 1  
#define Key_Radio 2
#define Key_ClockType 3
#define Key_HandType 4
#define Key_Bluetooth 5
#define Key_Numbers 6
#define Key_DateBox 7
#define Key_Crown 8
#define Key_Ticks 9

static int mTicks = 60, Radio = 94, a = 1, cType = 0, hType = 3, numbType = 5, grosor = 4, hTicks = 3;
bool UseSeconds = true, UseShadows = true, viewBluetooth = true, DateBox = true, UseCrown = true;
int32_t hh_angle, mi_angle, ss_angle, a_angle;

Window *w;
Layer *dial_layer, *marks_layer, *time_layer, *shadow_layer, *battery, *bluetooth;
TextLayer *date, *SN1, *SN2, *BN3, *SN4, *SN5, *BN6, *SN7, *SN8, *BN9, *SN10, *SN11, *BN12;
static GPoint hc, hs;
static GRect rDate, next, rBattery, nextBattery, rBluetooth, nextBluetooth, rBNumbers, rSNumbers;
static GRect nextSN1, nextSN2, nextBN3, nextSN4, nextSN5, nextBN6, nextSN7, nextSN8, nextBN9, nextSN10, nextSN11, nextBN12;
static BatteryChargeState c_state;
#ifdef PBL_COLOR
  static GColor8 ColorSphere,BackColor,ColorSeconds,ColorMinutes,ColorHours,ColorMMarks,ColorHMarks,ColorQMarks,ColorShadow;
  static GColor8 ColorFont,ColorBattery,ColorBNumbers,ColorSNumbers,ColorDBox,ColorCrown;
#else
  static GColor ColorSphere,BackColor,ColorSeconds,ColorMinutes,ColorHours,ColorMMarks,ColorHMarks,ColorQMarks,ColorShadow;
  static GColor ColorFont,ColorBattery,ColorBNumbers,ColorSNumbers,ColorDBox,ColorCrown;
#endif
  
static GPathInfo HOUR_POINTS = { 4, (GPoint []) { { -1, 1 }, { 1, 1}, { 1, -14}, { -1, -14} } };
static GPathInfo MINUTE_POINTS = { 4, (GPoint []) { { -1, 1 }, { 1, 1}, { 1, -6}, { -1, -6} } };
static GPathInfo HOUR_QUARTERS = { 4, (GPoint []) { { -3, 3 }, { 3, 3}, { 3, -17}, { -3, -17} } };

static GPath *minute_square, *hour_square, *hour_quarters;

static void setColors (int clockType){
  switch(clockType){
    case 0: //Sport Man
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorBlueMoon,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorCrown = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    break;
    case 1: //Fluor Classic
    BackColor = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorBlack, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorElectricBlue,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorDBox = COLOR_FALLBACK( GColorDarkCandyAppleRed,GColorWhite);
    ColorCrown = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 2: //PinkPanter lovers
    BackColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorFashionMagenta, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorGreen,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorDarkGray,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorDBox = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    ColorCrown = COLOR_FALLBACK(GColorFolly,GColorWhite);
    break;
    case 3: //Welcome to the Future
    BackColor = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorShockingPink,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorCyan,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorDBox = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorMelon,GColorWhite);
    break;
    case 4: //Green Grass
    BackColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorBrightGreen, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorDarkGreen,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorDarkGray,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorCrown = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    break;
    case 5: //Passion o'clock
    BackColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorRed, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorPastelYellow,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorPastelYellow,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorElectricBlue,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorImperialPurple,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorChromeYellow,GColorBlack);
    break;
    case 6: //Wine Taste
    BackColor = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorBulgarianRose, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorDBox = COLOR_FALLBACK(GColorMidnightGreen,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    break;
    case 7: //Bottle clock
    BackColor = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorDarkGreen, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorWhite,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorWhite,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorInchworm,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorArmyGreen,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorOrange,GColorWhite);
    break;
    case 8: //Navy man
    BackColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorCeleste, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorDukeBlue,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorDukeBlue,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorIslamicGreen,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorBlue,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorRed,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    ColorDBox = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    ColorCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 9: //Rainbow Warrior
    BackColor = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorYellow, GColorBlack);
    ColorQMarks = COLOR_FALLBACK(GColorBulgarianRose,GColorWhite); 
    ColorHMarks = COLOR_FALLBACK(GColorDarkGreen,GColorWhite); 
    ColorMMarks = COLOR_FALLBACK(GColorBlue,GColorWhite);
    ColorHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    ColorMinutes = COLOR_FALLBACK(GColorBlue,GColorWhite);
    ColorSeconds = COLOR_FALLBACK(GColorRed,GColorWhite);
    ColorShadow = COLOR_FALLBACK(GColorDarkGray,GColorBlack);  
    ColorFont = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    ColorBattery = COLOR_FALLBACK(GColorOrange,GColorWhite);
    ColorSNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    ColorBNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    ColorDBox = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 10: //Papyrus
    BackColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    ColorSphere = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorOxfordBlue,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorIndigo,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorOxfordBlue,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorWhite,GColorWhite);
    ColorCrown = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    break;
    case 11: //Classic Taste
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorWindsorTan,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorArmyGreen,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorPastelYellow,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 12: //Orange Obsesion
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorOrange, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorMinutes = COLOR_FALLBACK( GColorGreen,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorCyan,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorArmyGreen,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorArmyGreen,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 13: //Simply Red
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorRed,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorRed,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorMinutes = COLOR_FALLBACK( GColorRed,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorMelon,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorRed,GColorWhite);
    break;
    case 14: //Basic Green
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorMintGreen,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorIslamicGreen,GColorWhite);
    break;
    case 15: //Blue Base
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorDukeBlue,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorDukeBlue,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorBabyBlueEyes,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    break;
    case 16: //Yellow moon
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorDarkGray, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorYellow,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 17: //Folly day
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorPastelYellow, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorFolly,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorFolly,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBlue,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorDarkGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorRed,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorFolly,GColorWhite);
    break;
    case 18: //Clear White
    BackColor = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorRed,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorBlue,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorBlue,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorFolly,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorBlue,GColorWhite);
    break;
    case 19: //Platinum Edition
    BackColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorSphere = COLOR_FALLBACK(GColorDarkGray, GColorWhite);
    ColorQMarks = COLOR_FALLBACK(GColorCeleste,GColorBlack); 
    ColorHMarks = COLOR_FALLBACK(GColorCeleste,GColorBlack); 
    ColorMMarks = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorHours = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorMinutes = COLOR_FALLBACK(GColorCyan,GColorBlack);
    ColorSeconds = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    ColorShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    ColorFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorBattery = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    ColorSNumbers = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    ColorBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    ColorDBox = COLOR_FALLBACK(GColorBlack,GColorBlack);
    ColorCrown = COLOR_FALLBACK(GColorLightGray,GColorWhite);
    break;
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
  graphics_context_set_stroke_color(ctx, bluetooth_connection_service_peek() ? ColorSphere : ColorBattery);
  GPoint bt;
  bt.x = 15;
  bt.y = 15;
  graphics_draw_circle(ctx, bt, 10);
  }
}

void dial_layer_update(Layer *me, GContext *ctx) {
  
  #ifdef PBL_COLOR
    graphics_context_set_antialiased(ctx, true);
  #endif 
    
  //BackColor
  graphics_context_set_fill_color(ctx, BackColor);
  graphics_fill_rect(ctx, layer_get_bounds(me), 0, GCornerNone);
  
  //Clock Sphere
  graphics_context_set_fill_color(ctx, ColorSphere);
  graphics_fill_circle(ctx, hc, Radio);
  if (UseCrown==true){
    #ifdef PBL_COLOR
      if (UseShadows==true){
         graphics_context_set_stroke_width(ctx, 4);
         graphics_context_set_stroke_color(ctx, ColorShadow);
         graphics_draw_circle(ctx, hs, Radio); 
         graphics_context_set_stroke_color(ctx, BackColor);
         graphics_draw_circle(ctx, hc, Radio+5);
      }
      graphics_context_set_stroke_width(ctx, 5);
      graphics_context_set_stroke_color(ctx, ColorCrown);
    #else
      graphics_context_set_stroke_color(ctx, ColorCrown);
    #endif
    graphics_draw_circle(ctx, hc, Radio);
  }

  if (DateBox == true){
    if (UseShadows == true){
      graphics_context_set_fill_color(ctx, ColorShadow);
      graphics_fill_rect(ctx, next, 6, GCornersAll);
    }
    GRect sDBox = GRect(hc.x-28,hc.y+16,58,24);
    graphics_context_set_fill_color(ctx, ColorDBox);
    graphics_fill_rect(ctx, sDBox, 6, GCornersAll);
  }
}

void marks_layer_update(Layer *me, GContext *ctx) {
   #ifdef PBL_COLOR
     graphics_context_set_antialiased(ctx, true);
   #endif 

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
      if (hTicks>2){
        graphics_context_set_stroke_color(ctx, ColorMMarks);
        graphics_context_set_fill_color(ctx, ColorMMarks);
        gpath_move_to(minute_square, ray);
        gpath_rotate_to(minute_square, angle);
        gpath_draw_filled(ctx, minute_square);
        gpath_draw_outline(ctx, minute_square);
      }
    } else if (x % 15 != 0) {
      if (hTicks>1){ 
        graphics_context_set_stroke_color(ctx, ColorHMarks);
        graphics_context_set_fill_color(ctx, ColorHMarks);
        gpath_move_to(hour_square, ray);
        gpath_rotate_to(hour_square, angle);
        gpath_draw_filled(ctx, hour_square);
        gpath_draw_outline(ctx, hour_square);
      }
    }
    else{
      if (hTicks>0){      
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
}

void shadow_layer_update(Layer *me, GContext *ctx) {

  int end = grosor;
  int start = -1*grosor;
  int x;
  int h_offset = 0;

  if (hType == 2) {
     h_offset = Radio*0.4;
  }
  
  if (hType == 3) {
     h_offset = Radio*0.15;
  }
  
  #ifdef PBL_COLOR
    graphics_context_set_antialiased(ctx, true);
    if (hType == 2||hType > 3){
      if (hType == 2) {
        graphics_context_set_stroke_width(ctx, 2);
      } else {
        graphics_context_set_stroke_width(ctx, 3);
      }
    } else {
      graphics_context_set_stroke_width(ctx, grosor*2);
      start = 0;
      end = 0;
    }
  #endif
    
  //center shadow
  if (UseShadows==true){
    graphics_context_set_fill_color(ctx, ColorShadow);
    graphics_fill_circle(ctx, hs, 6);
  }
  
  //draw shadows
  if (UseShadows==true){
          GPoint mHb, mHt;
          #ifdef PBL_COLOR
            GPoint lmH, rmH;
            GPoint p1,p2,p3,p4,p5;
          #endif
          graphics_context_set_fill_color(ctx, ColorShadow);
          graphics_context_set_stroke_color(ctx, ColorShadow);
      if (cos_lookup(mi_angle)==0||sin_lookup(mi_angle)==0) {
        #ifdef PBL_COLOR
          graphics_context_set_stroke_width(ctx, (grosor-1)*2);
        #else
          end = grosor-1;
        #endif     
      } else {
        #ifdef PBL_COLOR
          graphics_context_set_stroke_width(ctx, grosor*2);
        #else
          end = grosor;
        #endif     
     }
	        for(x=start; x <= end; x++) {
            if (hType <= 3) {
               mHb.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
               mHb.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));
            } else {
               mHb.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(-1*Radio*0.2+h_offset) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
               mHb.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(-1*Radio*0.2+h_offset) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));           
           }
            mHt.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(Radio*0.8) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
            mHt.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(Radio*0.8) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));
          #ifdef PBL_COLOR
           if (hType > 3) {
            lmH.x = ((int32_t)( sin_lookup(mi_angle-a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.x 
                  + (int16_t)(cos_lookup(mi_angle-a_angle)*x/TRIG_MAX_RATIO));
            lmH.y = ((int32_t)(-cos_lookup(mi_angle-a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.y 
                  + (int16_t)(sin_lookup(mi_angle-a_angle)*x/TRIG_MAX_RATIO));
            rmH.x = ((int32_t)( sin_lookup(mi_angle+a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.x 
                  + (int16_t)(cos_lookup(mi_angle+a_angle)*x/TRIG_MAX_RATIO));
            rmH.y = ((int32_t)(-cos_lookup(mi_angle+a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.y 
                  + (int16_t)(sin_lookup(mi_angle+a_angle)*x/TRIG_MAX_RATIO));
            }
          #endif
           if (hType <= 3) {
             if (hType == 2) {
              #ifdef PBL_COLOR
                if (x == start || x == end ){
                 graphics_context_set_stroke_width(ctx, 2);
                 graphics_draw_line  (ctx, mHb, mHt);
                }
                graphics_context_set_stroke_width(ctx, grosor-1);
                graphics_draw_line  (ctx, hs, mHb);                
              #else
                graphics_draw_line  (ctx, mHb, mHt);
                if (x >= start/2 && x <= end/2 ){
                graphics_draw_line  (ctx, hs, mHb);
                }
              #endif
            } else {
              graphics_draw_line  (ctx, mHb, mHt);
            }
            if (x==0){
              graphics_fill_circle(ctx, mHt, grosor/2+2);
            }
           } else {
              #ifdef PBL_COLOR
                graphics_context_set_stroke_width(ctx, 2);
                if (x == start ){
                   p1 = lmH;
                   p3 = mHt;
                   graphics_draw_line  (ctx, p1, p3);
                }
                if (x == end ){
                    p2 = rmH;
                    p4 = mHt;
                    graphics_draw_line  (ctx, p2, p4);
                }
                 if (x==0){
                   graphics_context_set_stroke_width(ctx, 4);
                   graphics_draw_line  (ctx, mHb, lmH);
                   graphics_draw_line  (ctx, mHb, rmH);
                   graphics_fill_circle(ctx, mHb, grosor/2+2);
                  }
              #endif             
            }
          }
         if (hType > 3) {
                  #ifdef PBL_COLOR
                     graphics_context_set_stroke_width(ctx, 2);
                     p5.x = (p1.x+p2.x)/2;
                     p5.y = (p1.y+p2.y)/2;
                     graphics_draw_line  (ctx, hs, p5);
                     graphics_draw_line  (ctx, p1, p2);
                     graphics_draw_line  (ctx, p3, p4);
                  #endif
          }
    
          GPoint hHb, hHt;
          #ifdef PBL_COLOR
              GPoint lH, rH;
              GPoint h1,h2,h3;
          #endif
          graphics_context_set_fill_color(ctx, ColorShadow);
          graphics_context_set_stroke_color(ctx, ColorShadow);
	        for(x=start; x <= end; x++) {
            hHb.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            hHb.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            hHt.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(Radio*0.5) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            hHt.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.5) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
           #ifdef PBL_COLOR
            if (hType > 3) {
             hHt.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(Radio*0.6) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
             hHt.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.6) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
             lH.x = ((int32_t)( sin_lookup(hh_angle+a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.x 
                  + (int16_t)(cos_lookup(hh_angle+a_angle/3)*x/TRIG_MAX_RATIO));
             lH.y = ((int32_t)(-cos_lookup(hh_angle+a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.y 
                  + (int16_t)(sin_lookup(hh_angle+a_angle/3)*x/TRIG_MAX_RATIO));
             rH.x = ((int32_t)( sin_lookup(hh_angle-a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.x 
                  + (int16_t)(cos_lookup(hh_angle-a_angle/3)*x/TRIG_MAX_RATIO));
             rH.y = ((int32_t)(-cos_lookup(hh_angle-a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.y 
                  + (int16_t)(sin_lookup(hh_angle-a_angle/3)*x/TRIG_MAX_RATIO));
            }
          #endif
            if (hType <= 3) {
             if (hType == 2) {
              #ifdef PBL_COLOR
              if (x == start || x == end ){
                 graphics_context_set_stroke_width(ctx, 2);
                 graphics_draw_line  (ctx, hHb, hHt);
              }
                graphics_context_set_stroke_width(ctx, grosor-1);
                graphics_draw_line  (ctx, hs, hHb);                
              #else
                graphics_draw_line  (ctx, hHb, hHt);
                if (x >= start/2 && x <= end/2 ){
                graphics_draw_line  (ctx, hs, hHb);
                }
              #endif
            } else {
              graphics_draw_line  (ctx, hHb, hHt);
            }
            if (x==0){
              graphics_fill_circle(ctx, hHt, grosor/2+2);
            }
           } else {
              #ifdef PBL_COLOR
                if (x == start ){
                  h1 = lH;
                }
                if (x == end ){
                  h2 = rH;
                }
                  if (x==0){h3 = hHt;}
                  graphics_context_set_stroke_width(ctx, 2);
                  graphics_draw_line  (ctx, hs, lH);
                  graphics_draw_line  (ctx, hs, rH);
              #endif  
          }
      }
     if (hType > 3) {
       #ifdef PBL_COLOR
           graphics_context_set_stroke_width(ctx, 2);
           graphics_draw_line  (ctx, h1, h2);
           graphics_draw_line  (ctx, h1, h3);
           graphics_draw_line  (ctx, h2, h3);
        #endif
     } 
  }
  // Seconds Shadow
  if (UseSeconds == true&&UseShadows==true) {
      GPoint ss;
      GPoint as;
      #ifdef PBL_COLOR
        graphics_context_set_stroke_width(ctx, 2);
      #endif    
      ss.y = (int16_t)(-cos_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hs.y;
      ss.x = ((int16_t)(sin_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hs.x);
      as.y = (int16_t)(-cos_lookup(ss_angle) *
		  (int32_t)Radio*-0.2 / TRIG_MAX_RATIO) + hs.y;
      as.x = ((int16_t)(sin_lookup(ss_angle) *
		  (int32_t)Radio*-0.2 / TRIG_MAX_RATIO) + hs.x);
      graphics_context_set_stroke_color(ctx, ColorShadow);
      graphics_draw_line(ctx, as, ss);
  }
}

void time_layer_update(Layer *me, GContext *ctx) {

  int end = grosor;
  int start = -1*grosor;
  int x;
  int h_offset = 0;

  if (hType == 2) {
     h_offset = Radio*0.4;
  }
  if (hType == 3) {
     h_offset = Radio*0.15;
  }
  
  #ifdef PBL_COLOR
    graphics_context_set_antialiased(ctx, true);
    if (hType == 2||hType > 3){
      if (hType == 2) {
        graphics_context_set_stroke_width(ctx, 2);
      } else {
        graphics_context_set_stroke_width(ctx, 3);
      }
    } else {
      graphics_context_set_stroke_width(ctx, grosor*2);
      start = 0;
      end = 0;
    }
  #endif
    
 // Draw minute hand
  graphics_context_set_fill_color(ctx, ColorMinutes);
  graphics_context_set_stroke_color(ctx, ColorMinutes);
  GPoint mHb, mHt;
  #ifdef PBL_COLOR
    GPoint lmH, rmH;
    GPoint p1, p2, p3, p4;
    GPoint p5;
  #endif  
  graphics_context_set_fill_color(ctx, ColorMinutes);
  graphics_context_set_stroke_color(ctx, ColorMinutes);

   if (cos_lookup(mi_angle)==0||sin_lookup(mi_angle)==0) {
    #ifdef PBL_COLOR
        graphics_context_set_stroke_width(ctx, (grosor-1)*2);
    #else
        end = grosor-1;
    #endif     
   } else {
    #ifdef PBL_COLOR
        graphics_context_set_stroke_width(ctx, grosor*2);
    #else
        end = grosor;
    #endif     
   }
  
	 for(x=start; x <= end; x++) {
         if (hType <= 3) {
            mHb.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hc.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
            mHb.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hc.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));
          } else {
            mHb.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(-1*Radio*0.2+h_offset) / TRIG_MAX_RATIO) + hc.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
            mHb.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(-1*Radio*0.2+h_offset) / TRIG_MAX_RATIO) + hc.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));           
          }
            mHt.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(Radio*0.8) / TRIG_MAX_RATIO) + hc.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
            mHt.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(Radio*0.8) / TRIG_MAX_RATIO) + hc.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));
       #ifdef PBL_COLOR
         if (hType > 3) {
            lmH.x = ((int32_t)( sin_lookup(mi_angle-a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.x 
                  + (int16_t)(cos_lookup(mi_angle-a_angle)*x/TRIG_MAX_RATIO));
            lmH.y = ((int32_t)(-cos_lookup(mi_angle-a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.y 
                  + (int16_t)(sin_lookup(mi_angle-a_angle)*x/TRIG_MAX_RATIO));
            rmH.x = ((int32_t)( sin_lookup(mi_angle+a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.x 
                  + (int16_t)(cos_lookup(mi_angle+a_angle)*x/TRIG_MAX_RATIO));
            rmH.y = ((int32_t)(-cos_lookup(mi_angle+a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + mHt.y 
                  + (int16_t)(sin_lookup(mi_angle+a_angle)*x/TRIG_MAX_RATIO));
         }
       #endif
       if (hType <= 3) {
            if (hType == 2) {
              #ifdef PBL_COLOR
                if (x == start || x == end ){
                 graphics_context_set_stroke_width(ctx, 2);
                 graphics_draw_line  (ctx, mHb, mHt);
                }
                graphics_context_set_stroke_width(ctx, grosor-1);
                graphics_draw_line  (ctx, hc, mHb);
                if (x==0){
                    graphics_fill_circle(ctx, mHt, grosor/2+2);
                }
              #else
                graphics_draw_line  (ctx, mHb, mHt);
                if (x >= start/2 && x <= end/2 ){
                graphics_draw_line  (ctx, hc, mHb);
                } else {
                  graphics_draw_line  (ctx, mHb, mHt);
                }
                if (x==0){
                    graphics_fill_circle(ctx, mHt, grosor/2+1);
                }
              #endif
            } else {
              graphics_draw_line  (ctx, mHb, mHt);
              if (x==0){
                 graphics_fill_circle(ctx, mHt, grosor/2+1);
              }
            }
      } else {
              #ifdef PBL_COLOR
                graphics_context_set_stroke_width(ctx, 2);
                if (x == start ){
                   p1 = lmH;
                   p3 = mHt;
                  if (hType == 4){
                   graphics_draw_line  (ctx, p1, p3);
                  }
                }
                if (x == end ){
                    p2 = rmH;
                    p4 = mHt;
                  if (hType == 4){
                    graphics_draw_line  (ctx, p2, p4);
                  }
                }
                 if (x==0){
                   graphics_context_set_stroke_width(ctx, 4);
                   graphics_draw_line  (ctx, mHb, lmH);
                   graphics_draw_line  (ctx, mHb, rmH);
                   graphics_fill_circle(ctx, mHb, grosor/2+2);
                  }
              #endif  
       }
  }
  if (hType == 4){
                  #ifdef PBL_COLOR
                     graphics_context_set_stroke_width(ctx, 2);
                     p5.x = (p1.x+p2.x)/2;
                     p5.y = (p1.y+p2.y)/2;
                     graphics_draw_line  (ctx, hc, p5);
                     graphics_draw_line  (ctx, p1, p2);
                     graphics_draw_line  (ctx, p3, p4);
                  #endif
                 }
  
  //draw seconds hand
  if (UseSeconds == true) {
      GPoint sc;
      GPoint ac;
      #ifdef PBL_COLOR
        graphics_context_set_stroke_width(ctx, 2);
      #endif 
      sc.y = (int16_t)(-cos_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hc.y;
      sc.x = ((int16_t)(sin_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hc.x);
      ac.y = (int16_t)(-cos_lookup(ss_angle) *
		  (int32_t)Radio*-0.2 / TRIG_MAX_RATIO) + hc.y;
      ac.x = ((int16_t)(sin_lookup(ss_angle) *
		  (int32_t)Radio*-0.2 / TRIG_MAX_RATIO) + hc.x);
      graphics_context_set_stroke_color(ctx, ColorSeconds);
      graphics_draw_line(ctx, ac, sc);
  }
  
  // Draw hour hand
  #ifdef PBL_COLOR
      graphics_context_set_stroke_width(ctx, grosor*2);
      GPoint h1, h2, h3;
      GPoint lH, rH;
  #endif 
  GPoint hHb, hHt;
  graphics_context_set_fill_color(ctx, ColorHours);
  graphics_context_set_stroke_color(ctx, ColorHours);
  graphics_context_set_fill_color(ctx, ColorHours);
  graphics_context_set_stroke_color(ctx, ColorHours);
	for(x=start; x <= end; x++) {
            hHb.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hc.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            hHb.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hc.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            hHt.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(Radio*0.5) / TRIG_MAX_RATIO) + hc.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            hHt.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.5) / TRIG_MAX_RATIO) + hc.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
  #ifdef PBL_COLOR 
     if (hType > 3) {
            hHt.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(Radio*0.6) / TRIG_MAX_RATIO) + hc.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            hHt.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.6) / TRIG_MAX_RATIO) + hc.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            lH.x = ((int32_t)( sin_lookup(hh_angle+a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.x 
                  + (int16_t)(cos_lookup(hh_angle+a_angle/3)*x/TRIG_MAX_RATIO));
            lH.y = ((int32_t)(-cos_lookup(hh_angle+a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.y 
                  + (int16_t)(sin_lookup(hh_angle+a_angle/3)*x/TRIG_MAX_RATIO));
            rH.x = ((int32_t)( sin_lookup(hh_angle-a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.x 
                  + (int16_t)(cos_lookup(hh_angle-a_angle/3)*x/TRIG_MAX_RATIO));
            rH.y = ((int32_t)(-cos_lookup(hh_angle-a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + hHt.y 
                  + (int16_t)(sin_lookup(hh_angle-a_angle/3)*x/TRIG_MAX_RATIO));
         }
    #endif
        if (hType <= 3) {
           if (hType == 2) {
              #ifdef PBL_COLOR
                if (x == start || x == end ){
                 graphics_context_set_stroke_width(ctx, 2);
                 graphics_draw_line  (ctx, hHb, hHt); 
                }
                graphics_context_set_stroke_width(ctx, grosor-1);
                graphics_draw_line  (ctx, hc, hHb);
                if (x==0){
                   graphics_fill_circle(ctx, hHt, grosor/2+2);
                }
              #else
                graphics_draw_line  (ctx, hHb, hHt); 
                if (x >= start/2 && x <= end/2 ){
                graphics_draw_line  (ctx, hc, hHb);
                }
                if (x==0){
                    graphics_fill_circle(ctx, hHt, grosor/2+1);
                }
              #endif
            } else {
              graphics_draw_line  (ctx, hHb, hHt); 
              if (x==0){
                 graphics_fill_circle(ctx, hHt, grosor/2+1);
              }
           }
        } else {
              #ifdef PBL_COLOR
                if (x == start ){
                   h1 = lH;
                }
                if (x == end ){
                   h2 = rH;
                } 
                if (x==0){
                   h3 = hHt;
                }
                  graphics_context_set_stroke_width(ctx, 2);
                if (hType == 4){
                  graphics_draw_line  (ctx, hc, lH);
                  graphics_draw_line  (ctx, hc, rH);
                }
             #endif  
         }
  }
    if (hType == 4){
    #ifdef PBL_COLOR
       graphics_context_set_stroke_width(ctx, 2);
       graphics_draw_line  (ctx, h1, h2);
       graphics_draw_line  (ctx, h1, h3);
       graphics_draw_line  (ctx, h2, h3);
    #endif
  }
                  
  //draw centre circles
  graphics_context_set_fill_color(ctx, ColorHours);
  graphics_fill_circle(ctx, hc, 6);
  #ifdef PBL_COLOR
    if (gcolor_equal(ColorHours, ColorMinutes)){
      graphics_context_set_fill_color(ctx, ColorSeconds);
    } else {
      graphics_context_set_fill_color(ctx, ColorMinutes);
    }
    graphics_context_set_stroke_width(ctx, 1);
  #else
    graphics_context_set_fill_color(ctx, ColorSphere);
  #endif
  graphics_fill_circle(ctx, hc, 2);
}


void handle_tick(struct tm *now, TimeUnits units_changed) {
  setlocale(LC_TIME, ""); 
  layer_mark_dirty(marks_layer);
  layer_mark_dirty(dial_layer);
  layer_mark_dirty(shadow_layer);
  layer_mark_dirty(time_layer);
  layer_mark_dirty((Layer *)date);
  layer_mark_dirty((Layer *)SN1);
  layer_mark_dirty((Layer *)SN2);
  layer_mark_dirty((Layer *)BN3);
  layer_mark_dirty((Layer *)SN4);
  layer_mark_dirty((Layer *)SN5);
  layer_mark_dirty((Layer *)BN6);
  layer_mark_dirty((Layer *)SN7);
  layer_mark_dirty((Layer *)SN8);
  layer_mark_dirty((Layer *)BN9);
  layer_mark_dirty((Layer *)SN10);
  layer_mark_dirty((Layer *)SN11);
  layer_mark_dirty((Layer *)BN12);

  hh_angle = (TRIG_MAX_ANGLE*(((now->tm_hour%12)*6)+(now->tm_min/10)))/(12*6);
  mi_angle = TRIG_MAX_ANGLE * (now->tm_min) / 60;
  a_angle =  TRIG_MAX_ANGLE * 1 / 60;
  if (UseSeconds == true){
      ss_angle = TRIG_MAX_ANGLE / 60 * (now->tm_sec);
  }
  
  static char date_buf[] = "Mon.  1";
  int key = 0;
  int sector;
  if ((now->tm_min)==7||(now->tm_min)==23||(now->tm_min)==37||(now->tm_min)==53) {
      if (UseSeconds == false) {key = 1;} else {if((now->tm_sec)==30){key = 1;}}
  }
  if (a==1){
          key = 1;
          a=0;   
  }
  if ((now->tm_min)<=6){
     sector = 0;
  } else if ((now->tm_min)<=22) {
     sector = 15;
  } else if ((now->tm_min)<=36) {
     sector = 30;
  } else if ((now->tm_min)<=52) {
     sector = 45;
  } else {
     sector = 0;
  } 
    
  // Set Centre, date and Battery
  if (key == 1) {
      c_state = battery_state_service_peek();
      strftime(date_buf, sizeof("Mon.  1"), "%a. %e", now);
      text_layer_set_text(date, date_buf);
    if (Radio<=72){
           hc = GPoint(71,84);
    } else {
           hc = GPoint((int16_t)(-sin_lookup(TRIG_MAX_ANGLE / 60 * sector) *
  		     (int32_t)(Radio-68) / TRIG_MAX_RATIO) + 144 / 2, 
           (int16_t)(cos_lookup(TRIG_MAX_ANGLE / 60 * sector) *
  		     (int32_t)(Radio-80) / TRIG_MAX_RATIO) +  168 / 2);
    }
      hs.x=hc.x+1;
      hs.y=hc.y+4;
      
      next = GRect(hc.x-30,hc.y+15,60,25);
      nextBattery = GRect(hc.x-7,hc.y-33,15,15);
      nextBluetooth = GRect(hc.x-15,hc.y-41,30,30);
      text_layer_set_text_color(date, ColorFont);
      layer_set_frame(text_layer_get_layer(date), next);
      layer_set_frame(battery, nextBattery);
      layer_set_update_proc(battery, battery_update_proc);
      layer_set_frame(bluetooth, nextBluetooth);
      layer_set_update_proc(bluetooth, bluetooth_update_proc);
    
      if (numbType != 1) {
        text_layer_set_text_color(BN3, ColorBNumbers);
        text_layer_set_text_color(BN6, ColorBNumbers);
        text_layer_set_text_color(BN9, ColorBNumbers);
        text_layer_set_text_color(BN12, ColorBNumbers);
        nextBN3 = GRect(hc.x-13+Radio*0.65,hc.y-18,28,28);
        nextBN6 = GRect(hc.x-13,hc.y-18+Radio*0.65,28,28);
        nextBN9 = GRect(hc.x-13-Radio*0.65,hc.y-18,28,28);
        nextBN12 = GRect(hc.x-13,hc.y-18-Radio*0.65,28,28);
        if (numbType == 4 || numbType ==5) {
          text_layer_set_text_color(SN1, ColorBNumbers);
          text_layer_set_text_color(SN2, ColorBNumbers);
          text_layer_set_text_color(SN4, ColorBNumbers);
          text_layer_set_text_color(SN5, ColorBNumbers);
          text_layer_set_text_color(SN7, ColorBNumbers);
          text_layer_set_text_color(SN8, ColorBNumbers);
          text_layer_set_text_color(SN10, ColorBNumbers);
          text_layer_set_text_color(SN11, ColorBNumbers);
          nextSN1 = GRect((hc.x-12)+Radio*0.62*( sin_lookup( 5*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.62*(-cos_lookup( 5*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
          nextSN2 = GRect((hc.x-12)+Radio*0.62*( sin_lookup(10*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.62*(-cos_lookup(10*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
          nextSN4 = GRect((hc.x-12)+Radio*0.62*( sin_lookup(20*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.62*(-cos_lookup(20*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
          nextSN5 = GRect((hc.x-12)+Radio*0.62*( sin_lookup(25*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.620*(-cos_lookup(25*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
          nextSN7 = GRect((hc.x-12)+Radio*0.62*( sin_lookup(35*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.62*(-cos_lookup(35*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
          nextSN8 = GRect((hc.x-12)+Radio*0.62*( sin_lookup(40*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.62*(-cos_lookup(40*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
          nextSN10= GRect((hc.x-12)+Radio*0.62*( sin_lookup(50*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.62*(-cos_lookup(50*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
          nextSN11= GRect((hc.x-12)+Radio*0.62*( sin_lookup(55*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          (hc.y-12)+Radio*0.62*(-cos_lookup(55*TRIG_MAX_ANGLE / 60))/ TRIG_MAX_RATIO,
                          28,28);
        }
        if (numbType == 3 || numbType == 5) {
          text_layer_set_text(BN3, "3");
          text_layer_set_text(BN6, "6");
          text_layer_set_text(BN9, "9");
          text_layer_set_text(BN12, "12");
          if (numbType == 5) {
            text_layer_set_text(SN1, "1");
            text_layer_set_text(SN2, "2");
            text_layer_set_text(SN4, "4");
            text_layer_set_text(SN5, "5");            
            text_layer_set_text(SN7, "7");
            text_layer_set_text(SN8, "8");
            text_layer_set_text(SN10, "10");
            text_layer_set_text(SN11, "11");         
          }
        }
        if (numbType == 2 || numbType == 4) {
          text_layer_set_text(BN3, "III");
          text_layer_set_text(BN6, "VI");
          text_layer_set_text(BN9, "IX");
          text_layer_set_text(BN12, "XII");
          if (numbType == 4) {
            text_layer_set_text(SN1, "I");
            text_layer_set_text(SN2, "II");
            text_layer_set_text(SN4, "IV");
            text_layer_set_text(SN5, "V");            
            text_layer_set_text(SN7, "VII");
            text_layer_set_text(SN8, "VIII");
            text_layer_set_text(SN10, "X");
            text_layer_set_text(SN11, "XI");         
          }
        }        
        layer_set_frame(text_layer_get_layer(BN3), nextBN3);
        layer_set_frame(text_layer_get_layer(BN6), nextBN6);
        layer_set_frame(text_layer_get_layer(BN9), nextBN9);
        layer_set_frame(text_layer_get_layer(BN12), nextBN12);
        if (numbType == 4 || numbType == 5 ) {
          layer_set_frame(text_layer_get_layer(SN1), nextSN1);
          layer_set_frame(text_layer_get_layer(SN2), nextSN2);
          layer_set_frame(text_layer_get_layer(SN4), nextSN4);
          layer_set_frame(text_layer_get_layer(SN5), nextSN5);
          layer_set_frame(text_layer_get_layer(SN7), nextSN7);
          layer_set_frame(text_layer_get_layer(SN8), nextSN8);
          layer_set_frame(text_layer_get_layer(SN10), nextSN10);
          layer_set_frame(text_layer_get_layer(SN11), nextSN11);
        }
     }
  }  
}

static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Enter in_recv_handler");
  //Get Tuple
  Tuple *t = dict_read_first(iterator);
  a = 1;
  //Process all pairs present
  while(t != NULL) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Iterator %d", (int)t->key);
    APP_LOG(APP_LOG_LEVEL_INFO, "Iterator received with value %s", t->value->cstring);
    // Process this pair's key
    switch(t->key)
    {
    case Key_UseSeconds:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        UseSeconds = true;
        persist_write_bool(Key_UseSeconds, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        UseSeconds = false;
        persist_write_bool(Key_UseSeconds, false);
      };
      break;
    case Key_UseShadows:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        UseShadows = true;
        persist_write_bool(Key_UseShadows, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        UseShadows = false;
        persist_write_bool(Key_UseShadows, false);
      };
      break;
    case Key_Radio:
       Radio = atoi(t->value->cstring);
       persist_write_int(Key_Radio, Radio);
       //APP_LOG(APP_LOG_LEVEL_INFO, "Radio %d", Radio);
       break;
    case Key_ClockType:
       cType = atoi(t->value->cstring);
       //seteo colores
       setColors (cType);
       persist_write_int(Key_ClockType, cType);
       //APP_LOG(APP_LOG_LEVEL_INFO, "ClockType %d", cType);
       break;
    case Key_HandType:
       hType = atoi(t->value->cstring);
       persist_write_int(Key_HandType, hType);
       //APP_LOG(APP_LOG_LEVEL_INFO, "HandType %d", hType);
       break;
    case Key_Bluetooth:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        viewBluetooth = true;
        persist_write_bool(Key_Bluetooth, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        viewBluetooth = false;
        persist_write_bool(Key_Bluetooth, false);
      };
      break;
    case Key_Numbers:
       numbType = atoi(t->value->cstring);
       persist_write_int(Key_Numbers, numbType);
       break;
    case Key_DateBox:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        DateBox = true;
        persist_write_bool(Key_DateBox, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        DateBox = false;
        persist_write_bool(Key_DateBox, false);
      };
      break;
    case Key_Crown:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        UseCrown = true;
        persist_write_bool(Key_Crown, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        UseCrown = false;
        persist_write_bool(Key_Crown, false);
      };
      break;      
    case Key_Ticks:
       hTicks = atoi(t->value->cstring);
       persist_write_int(Key_Ticks, hTicks);
       break;
    }
   // Get next pair, if any
   t = dict_read_next(iterator);
 }
  //Empty sphere numbers to recalculate
  text_layer_set_text(BN3, "");
  text_layer_set_text(BN6, "");
  text_layer_set_text(BN9, "");
  text_layer_set_text(BN12, "");
  text_layer_set_text(SN1, "");
  text_layer_set_text(SN2, "");
  text_layer_set_text(SN4, "");
  text_layer_set_text(SN5, "");            
  text_layer_set_text(SN7, "");
  text_layer_set_text(SN8, "");
  text_layer_set_text(SN10, "");
  text_layer_set_text(SN11, ""); 
  if (UseSeconds == true) { tick_timer_service_subscribe(SECOND_UNIT, handle_tick); }
                   else  { tick_timer_service_subscribe(MINUTE_UNIT, handle_tick); }
}

void handle_init(void) {
  w = window_create();
 //Check for saved KEY options
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  if (persist_exists(Key_UseSeconds)) {
    UseSeconds = persist_read_bool(Key_UseSeconds);
  }
  #ifdef PBL_COLOR //shadows only on Pebble Time
  if (persist_exists(Key_UseShadows)) {
    UseShadows = persist_read_bool(Key_UseShadows);
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
    viewBluetooth = persist_read_bool(Key_Bluetooth);
  }  
  if (persist_exists(Key_Numbers)) {
    numbType = persist_read_int(Key_Numbers );
  }
  if (persist_exists(Key_DateBox)) {
    DateBox = persist_read_bool(Key_DateBox);
  }  
  if (persist_exists(Key_Crown)) {
    UseCrown = persist_read_bool(Key_Crown);
  }  
  if (persist_exists(Key_Ticks)) {
    hTicks = persist_read_int(Key_Ticks);
  }

  //set colors
  setColors (cType);
  
  //Subscribe battery and bluetooth services
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  
  //set window
  #ifdef PBL_SDK_2
   window_set_fullscreen(w, true);
  #endif
  GRect bounds = GRect(0,0,144,168); //layer_get_bounds(window_layer);
  dial_layer = layer_create(bounds);
  marks_layer = layer_create(bounds);
  shadow_layer = layer_create(bounds);
  time_layer = layer_create(bounds);

  //Clock points
  hour_square = gpath_create(&HOUR_POINTS);
  minute_square = gpath_create(&MINUTE_POINTS);
  hour_quarters = gpath_create(&HOUR_QUARTERS);

  //Layer for battery
  rBattery = GRect(hc.x-7,hc.y-33,15,15);
  battery = layer_create(rBattery);
  
  //building date layer
  rDate = GRect(hc.x-30,hc.y+15,60,25);
  date = text_layer_create(rDate);
  text_layer_set_background_color(date, GColorClear);
  text_layer_set_text_color(date, ColorFont);
  text_layer_set_text(date, "");
  text_layer_set_text_alignment(date, GTextAlignmentCenter);
  text_layer_set_font(date, dBFont); 

  //building dial numbers layers
   //bigNumbers (Quarters)
   rBNumbers = GRect(hc.x,hc.y,28,28);
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
   BN12 = text_layer_create(rBNumbers);
   text_layer_set_background_color(BN12, GColorClear);
   text_layer_set_text_color(BN12, ColorBNumbers);
   text_layer_set_text(BN12, "");
   text_layer_set_text_alignment(BN12, GTextAlignmentCenter);
   text_layer_set_font(BN12, nBFont);
  
   //SmallNumbers
   rSNumbers = GRect(hc.x,hc.y,28,28);
   SN1 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN1, GColorClear);
   text_layer_set_text_color(SN1, ColorSNumbers);
   text_layer_set_text(SN1, "");
   text_layer_set_text_alignment(SN1, GTextAlignmentCenter);
   text_layer_set_font(SN1, nSFont);
   SN2 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN2, GColorClear);
   text_layer_set_text_color(SN2, ColorSNumbers);
   text_layer_set_text(SN2, "");
   text_layer_set_text_alignment(SN2, GTextAlignmentCenter);
   text_layer_set_font(SN2, nSFont);
   SN4 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN4, GColorClear);
   text_layer_set_text_color(SN4, ColorSNumbers);
   text_layer_set_text(SN4, "");
   text_layer_set_text_alignment(SN4, GTextAlignmentCenter);
   text_layer_set_font(SN4, nSFont);
   SN5 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN5, GColorClear);
   text_layer_set_text_color(SN5, ColorSNumbers);
   text_layer_set_text(SN5, "");
   text_layer_set_text_alignment(SN5, GTextAlignmentCenter);
   text_layer_set_font(SN5, nSFont);
   SN7 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN7, GColorClear);
   text_layer_set_text_color(SN7, ColorSNumbers);
   text_layer_set_text(SN7, "");
   text_layer_set_text_alignment(SN7, GTextAlignmentCenter);
   text_layer_set_font(SN7, nSFont);
   SN8 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN8, GColorClear);
   text_layer_set_text_color(SN8, ColorSNumbers);
   text_layer_set_text(SN8, "");
   text_layer_set_text_alignment(SN8, GTextAlignmentCenter);
   text_layer_set_font(SN8, nSFont);
   SN10 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN10, GColorClear);
   text_layer_set_text_color(SN10, ColorSNumbers);
   text_layer_set_text(SN10, "");
   text_layer_set_text_alignment(SN10, GTextAlignmentCenter);
   text_layer_set_font(SN10, nSFont);
   SN11 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN11, GColorClear);
   text_layer_set_text_color(SN11, ColorSNumbers);
   text_layer_set_text(SN11, "");
   text_layer_set_text_alignment(SN11, GTextAlignmentCenter);
   text_layer_set_font(SN11, nSFont);
  
  // Layer for bluetooth
  rBluetooth = GRect(hc.x-15,hc.y-41, 30, 30);
  bluetooth = layer_create(rBluetooth);

  // Update layers
  layer_set_update_proc(shadow_layer, shadow_layer_update);
  layer_set_update_proc(time_layer, time_layer_update);
  layer_set_update_proc(dial_layer, dial_layer_update);
  layer_set_update_proc(marks_layer, marks_layer_update);
  layer_set_update_proc(battery, battery_update_proc);
  layer_set_update_proc(bluetooth, bluetooth_update_proc);
  
  // Adding layers to main window
  layer_add_child(window_get_root_layer(w), dial_layer);
  layer_add_child(window_get_root_layer(w), (Layer *)bluetooth);
  layer_add_child(window_get_root_layer(w), (Layer *)battery);
  layer_add_child(window_get_root_layer(w), shadow_layer);
  layer_add_child(window_get_root_layer(w), marks_layer);
  layer_add_child(window_get_root_layer(w), (Layer *)date);
  layer_add_child(window_get_root_layer(w), (Layer *)BN3);
  layer_add_child(window_get_root_layer(w), (Layer *)BN6);
  layer_add_child(window_get_root_layer(w), (Layer *)BN9);
  layer_add_child(window_get_root_layer(w), (Layer *)BN12);
  layer_add_child(window_get_root_layer(w), (Layer *)SN1);
  layer_add_child(window_get_root_layer(w), (Layer *)SN2);
  layer_add_child(window_get_root_layer(w), (Layer *)SN4);
  layer_add_child(window_get_root_layer(w), (Layer *)SN5);
  layer_add_child(window_get_root_layer(w), (Layer *)SN7);
  layer_add_child(window_get_root_layer(w), (Layer *)SN8);
  layer_add_child(window_get_root_layer(w), (Layer *)SN10);
  layer_add_child(window_get_root_layer(w), (Layer *)SN11);
  layer_add_child(window_get_root_layer(w), time_layer);
  
  window_stack_push(w, true);
  
  if (UseSeconds == true) { tick_timer_service_subscribe(SECOND_UNIT, handle_tick); }
                    else  { tick_timer_service_subscribe(MINUTE_UNIT, handle_tick); }

}

void handle_deinit(void) {
  layer_destroy(shadow_layer);
  layer_destroy(time_layer);
  layer_destroy(dial_layer);
  layer_destroy(marks_layer);
  layer_destroy((Layer *)date);
  layer_destroy((Layer *)BN3);
  layer_destroy((Layer *)BN6);
  layer_destroy((Layer *)BN9);
  layer_destroy((Layer *)BN12);
  layer_destroy((Layer *)SN1);
  layer_destroy((Layer *)SN2);
  layer_destroy((Layer *)SN4);
  layer_destroy((Layer *)SN5);
  layer_destroy((Layer *)SN7);
  layer_destroy((Layer *)SN8);
  layer_destroy((Layer *)SN10);
  layer_destroy((Layer *)SN11);
  layer_destroy(battery);
  layer_destroy(bluetooth);
  
  gpath_destroy(hour_square);
  gpath_destroy(minute_square);
  gpath_destroy(hour_quarters);

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
