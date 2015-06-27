#include <pebble.h>

//Fonts
#define sSFont fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD) 
#define nSFont fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD) 
#define nBFont fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD) 
  
#define K_UseSeconds 0
#define K_UseShadows 1  
#define K_Radio 2
#define K_ClockType 3
#define K_HandType 4
#define K_Bluetooth 5
#define K_Numbers 6
#define K_DateBox 7
#define K_Crown 8
#define K_Ticks 9
#define K_SrcSaver 10
#define K_Time 11
#define K_Light 12
#define K_Battery 13

static int mTicks = 60, Radio = 100, a = 1, cType = 1, hType = 1, numbType = 5, uBluetooth = 1, grosor = 4, hTicks = 3, control = 1;
static int bType=0, SrcSaver = 0, sTime=1, iTimer=0, current = 0, maxSaver = 25;
static int lTime =7, iLight=0;
static int debug = 0;//to activate debug messages set 1
  
bool UseSeconds = false, UseShadows = true, DateBox = true, UseCrown = true;
int32_t hh_angle, mi_angle, ss_angle, a_angle;

Window *w, *s;
Layer *dial_layer, *SrcSaver_layer, *marks_layer, *time_layer, *time_second, *shadow_layer, *shadow_second, *battery, *bluetooth;
TextLayer *date, *Batt, *SN1, *SN2, *BN3, *SN4, *SN5, *BN6, *SN7, *SN8, *BN9, *SN10, *SN11, *BN12;
GPoint hc, hs, mHb, mHt, hHb, hHt;
#ifdef PBL_COLOR
  GPoint smHb,smHt,slmH,srmH,sp1,sp2,sp3,sp4,sp5,sp6,slH,srH,sh1,sh2,sh3,sh4,sh5;
  GPoint shHb,shHt,lmH,rmH,p1,p2,p3,p4,p5,p6,lH,rH,h1,h2,h3,h4,h5;
#endif

static GRect rDate, rBatt, next, rBattery, nextBattery, rBluetooth, nextBluetooth, rBNumbers, rSNumbers;
static GRect nextSN1, nextSN2, nextBN3, nextSN4, nextSN5, nextBN6, nextSN7, nextSN8, nextBN9, nextSN10, nextSN11, nextBN12;
static BatteryChargeState c_state;
#ifdef PBL_COLOR
  static GColor8 CSphere,BColor,CSeconds,CMinutes,CHours,CMMarks,CHMarks,CQMarks,CShadow;
  static GColor8 CFont,CBattery,CBNumbers,CSNumbers,CDBox,CCrown;
#else
  static GColor CSphere,BColor,CSeconds,CMinutes,CHours,CMMarks,CHMarks,CQMarks,CShadow;
  static GColor CFont,CBattery,CBNumbers,CSNumbers,CDBox,CCrown;
#endif
  
GPathInfo MINUTE_POINTS = { 4, (GPoint []) { { -1, 1 }, { 1, 1}, { 1, -6}, { -1, -6} } };
GPathInfo HOUR_POINTS = { 4, (GPoint []) { { -1, 3 }, { 1, 3}, { 1, -12}, { -1, -12} } };
GPathInfo HOUR_QUARTERS = { 4, (GPoint []) { { -3, 3 }, { 3, 3}, { 3, -17}, { -3, -17} } };

static GPath *minute_square, *hour_square, *hour_quarters;

#ifdef PBL_COLOR
  BitmapLayer *bitmap_layer;
  GBitmap *ScreenSvr;
#endif

static void setColors (int clockType){
  switch(clockType){
    case 0: //Sport Man
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CHours = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorBlueMoon,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorLightGray,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    CCrown = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    break;
    case 1: //Fluor Classic
    BColor = COLOR_FALLBACK(GColorRed,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorBlack, GColorBlack);
    CQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    CMinutes = COLOR_FALLBACK(GColorElectricBlue,GColorWhite);
    CSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CDBox = COLOR_FALLBACK( GColorDarkCandyAppleRed,GColorWhite);
    CCrown = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 2: //PinkPanter lovers
    BColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    CSphere = COLOR_FALLBACK(GColorFashionMagenta, GColorBlack);
    CQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CHours = COLOR_FALLBACK(GColorCyan,GColorWhite);
    CMinutes = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    CSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CShadow = COLOR_FALLBACK(GColorDarkGray,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CSNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CDBox = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    CCrown = COLOR_FALLBACK(GColorFolly,GColorWhite);
    break;
    case 3: //Welcome to the Future
    BColor = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    CSphere = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    CQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CHours = COLOR_FALLBACK(GColorKellyGreen,GColorWhite);
    CMinutes = COLOR_FALLBACK(GColorCyan,GColorWhite);
    CSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CBNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CDBox = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorMelon,GColorWhite);
    break;
    case 4: //Green Grass
    BColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    CSphere = COLOR_FALLBACK(GColorBrightGreen, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorDarkGreen,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CHours = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorRed,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorDarkGray,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CCrown = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    break;
    case 5: //Passion o'clock
    BColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    CSphere = COLOR_FALLBACK(GColorRed, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorPastelYellow,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorPastelYellow,GColorBlack);
    CHours = COLOR_FALLBACK(GColorElectricBlue,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CBattery = COLOR_FALLBACK(GColorRichBrilliantLavender,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorImperialPurple,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorChromeYellow,GColorBlack);
    break;
    case 6: //Wine Taste
    BColor = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorBulgarianRose, GColorBlack);
    CQMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CHMarks = COLOR_FALLBACK(GColorWhite,GColorWhite); 
    CMMarks = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    CMinutes = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    CSeconds = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CBattery = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CSNumbers = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CDBox = COLOR_FALLBACK(GColorMidnightGreen,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    break;
    case 7: //Bottle clock
    BColor = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorDarkGreen, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorWhite,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorWhite,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CHours = COLOR_FALLBACK(GColorRed,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorInchworm,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorYellow,GColorWhite);
    CBattery = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorArmyGreen,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorOrange,GColorWhite);
    break;
#ifdef PBL_COLOR
    case 8: //Navy man
    BColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    CSphere = COLOR_FALLBACK(GColorCeleste, GColorBlack);
    CQMarks = COLOR_FALLBACK(GColorDukeBlue,GColorWhite); 
    CHMarks = COLOR_FALLBACK(GColorDukeBlue,GColorWhite); 
    CMMarks = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    CHours = COLOR_FALLBACK(GColorIslamicGreen,GColorWhite);
    CMinutes = COLOR_FALLBACK(GColorBlue,GColorWhite);
    CSeconds = COLOR_FALLBACK(GColorRed,GColorWhite);
    CShadow = COLOR_FALLBACK(GColorDarkGray,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CBattery = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    CSNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    CBNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    CDBox = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    CCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 9: //Rainbow Warrior
    BColor = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    CSphere = COLOR_FALLBACK(GColorYellow, GColorBlack);
    CQMarks = COLOR_FALLBACK(GColorBulgarianRose,GColorWhite); 
    CHMarks = COLOR_FALLBACK(GColorDarkGreen,GColorWhite); 
    CMMarks = COLOR_FALLBACK(GColorBlue,GColorWhite);
    CHours = COLOR_FALLBACK(GColorMagenta,GColorWhite);
    CMinutes = COLOR_FALLBACK(GColorBlue,GColorWhite);
    CSeconds = COLOR_FALLBACK(GColorRed,GColorWhite);
    CShadow = COLOR_FALLBACK(GColorDarkGray,GColorBlack);  
    CFont = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    CBattery = COLOR_FALLBACK(GColorOrange,GColorWhite);
    CSNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorWhite);
    CBNumbers = COLOR_FALLBACK(GColorDarkGreen,GColorWhite);
    CDBox = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    CCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 10: //Papyrus
    BColor = COLOR_FALLBACK(GColorBlack,GColorWhite);
    CSphere = COLOR_FALLBACK(GColorPastelYellow,GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorOxfordBlue,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    CHours = COLOR_FALLBACK(GColorIndigo,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorRed,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorOxfordBlue,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorWhite,GColorWhite);
    CCrown = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    break;
    case 11: //Classic Taste
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorDarkGray,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorWindsorTan,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    CHours = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorArmyGreen,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorBulgarianRose,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorWindsorTan,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorPastelYellow,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 12: //Orange Obsesion
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorOrange, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CHours = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorCyan,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorOxfordBlue,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorWindsorTan,GColorWhite);
    break;
    case 13: //Simply Red
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorRed,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorRed,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CHours = COLOR_FALLBACK(GColorRed,GColorBlack);
    CMinutes = COLOR_FALLBACK( GColorRed,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorRed,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorRed,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorMelon,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorRed,GColorWhite);
    break;
    case 14: //Basic Green
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CHours = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorMintGreen,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorIslamicGreen,GColorWhite);
    break;
    case 15: //Blue Base
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorDukeBlue,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorDukeBlue,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CHours = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorDukeBlue,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorBabyBlueEyes,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorDukeBlue,GColorWhite);
    break;
    case 16: //Yellow moon
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorDarkGray, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorYellow,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CHours = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorRed,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorDarkCandyAppleRed,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorYellow,GColorWhite);
    break;
    case 17: //Folly day
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorPastelYellow, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorFolly,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorFolly,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorRed,GColorBlack);
    CHours = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorBlue,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorDarkGray,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorRed,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorFolly,GColorWhite);
    break;
    case 18: //Clear White
    BColor = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorWhite, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorRed,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorBlue,GColorBlack);
    CHours = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorIslamicGreen,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorBlue,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorLightGray,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorFolly,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorBlue,GColorWhite);
    break;
    case 19: //Platinum Edition
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorDarkGray, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorCeleste,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorCeleste,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CHours = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorCyan,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorLightGray,GColorWhite);
    break;
    case 20: //Graphite
    BColor = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorBlack, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorRed,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorOrange,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CHours = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorCyan,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorRed,GColorWhite);
    break;
    case 21: //Black
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorBlack, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorLightGray,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorLightGray,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    CHours = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorCyan,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorRed,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorYellow,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorWhite,GColorWhite);
    break;
    case 22: //Pride Day
    BColor = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CSphere = COLOR_FALLBACK(GColorPastelYellow, GColorWhite);
    CQMarks = COLOR_FALLBACK(GColorBlack,GColorBlack); 
    CHMarks = COLOR_FALLBACK(GColorBlack,GColorBlack); 
    CMMarks = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CHours = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CMinutes = COLOR_FALLBACK(GColorDarkGray,GColorBlack);
    CSeconds = COLOR_FALLBACK(GColorCyan,GColorBlack);
    CShadow = COLOR_FALLBACK(GColorBlack,GColorWhite);  
    CFont = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBattery = COLOR_FALLBACK(GColorWhite,GColorBlack);
    CSNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CBNumbers = COLOR_FALLBACK(GColorBlack,GColorBlack);
    CDBox = COLOR_FALLBACK(GColorLightGray,GColorBlack);
    CCrown = COLOR_FALLBACK(GColorJazzberryJam,GColorWhite);
    break;
#endif
  }
}

static void setImage (int ScreenSaver){
   #ifdef PBL_COLOR
   if (ScreenSvr != NULL) {gbitmap_destroy(ScreenSvr);}
   if (bitmap_layer != NULL) {bitmap_layer_destroy(bitmap_layer);}
   if (ScreenSaver == maxSaver) {
          srand (time (NULL)+rand() % 60);
          ScreenSaver = 2+rand() % (maxSaver-3);
   }
  if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "set_image: Setting Screen Saver %d", ScreenSaver); }
  if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "Heap Available Before: %d", heap_bytes_free()); } 
   switch(ScreenSaver){
    case 2: //SKully
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_SKULLY);
    break;
    case 3: //Zebra
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC1);
    break;
    case 4: //Picasso1
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC2);
    break;
    case 5: //Picasso 3
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC3);
    break;
    case 6: //Picasso 4
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC4);
    break;
    case 7: //Picasso 5
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC5);
    break;
    case 8: //Picasso 6
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC6);
    break;
    case 9: //Vangogh 1
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC7);
    break;
    case 10: //Vangogh 2
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC8);
    break;
    case 11: //Vangogh 3
      if (ScreenSvr != NULL) {gbitmap_destroy(ScreenSvr);}
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC9);
    break;
    case 12: //Vangogh 4
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC10);
    break;
    case 13: //Modigliani 1
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC11);
    break;
    case 14: //Modigliani 2
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC12);
    break;
    case 15: //Modigliani 3
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC13);
    break;
    case 16: //Modigliani 4
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC14);
    break;
    case 17: //Chagall 1
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC15);
    break;
    case 18: //Chagall 2
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC16);
    break;
    case 19: //Chagall 3
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC17);
    break;
    case 20: //Munch
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC18);
    break;
    case 21: //MonaLisa
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC19);
    break;
    case 22: //Lempika 1
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC20);
    break;
    case 23: //Lempika 2
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC21);
    break;
    case 24: //Dali
      ScreenSvr = gbitmap_create_with_resource(RESOURCE_ID_PIC22);
    break;
   }
  if (ScreenSvr != NULL) {
    bitmap_layer = bitmap_layer_create(GRect(0,0,144,168));
    bitmap_layer_set_bitmap(bitmap_layer, ScreenSvr);
    bitmap_layer_set_compositing_mode(bitmap_layer, GCompOpAssign);
    layer_add_child(SrcSaver_layer, bitmap_layer_get_layer(bitmap_layer));
    if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "Heap Available After: %d", heap_bytes_free());}
  } 
  #endif
}

static void handle_battery(BatteryChargeState c_state) {
      layer_mark_dirty(battery);
      layer_mark_dirty((Layer *)Batt);
}

void handle_bluetooth(bool connected) {
      layer_mark_dirty(bluetooth);
      if (uBluetooth ==2 && connected == false) {
              vibes_double_pulse();
              vibes_double_pulse();
      }
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
     static BatteryChargeState c_state;
     c_state = battery_state_service_peek();
     if (bType == 0){  
       graphics_context_set_stroke_color(ctx, c_state.is_charging ? CBattery : CSphere);
       graphics_draw_rect(ctx, GRect(0,0,15,15));
       graphics_context_set_stroke_color(ctx, CBattery);
       graphics_context_set_fill_color(ctx, CBattery);

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
}

void bluetooth_update_proc(Layer *layer, GContext *ctx) {
  if (uBluetooth > 0 ) {
    //graphics_context_set_stroke_color(ctx, bluetooth_connection_service_peek() ? CSphere : CBattery);
    graphics_context_set_stroke_color(ctx, CBattery);
    if (bluetooth_connection_service_peek()==false){
      GPoint bt;
      if (bType==0){
        bt.x = 20;
        bt.y = 15;
        graphics_draw_circle(ctx, bt, 10);
      } else {
        bt.x = 20;
        bt.y = 20;
        #ifdef PBL_COLOR
          graphics_context_set_antialiased(ctx, true);
          graphics_context_set_stroke_width(ctx, 3);
        #endif 
        GRect sDBth = GRect(bt.x-17,bt.y-18,35,25);
        graphics_draw_rect(ctx, sDBth);
      }    
    }
  }
}

void dial_layer_update(Layer *me, GContext *ctx) {
  #ifdef PBL_COLOR
    graphics_context_set_antialiased(ctx, true);
  #endif 
    
  //BColor
  graphics_context_set_fill_color(ctx, BColor);
  graphics_fill_rect(ctx, layer_get_bounds(me), 0, GCornerNone);
  
  //Clock Sphere
  graphics_context_set_fill_color(ctx, CSphere);
  graphics_fill_circle(ctx, hc, Radio);
  #ifdef PBL_COLOR
  if (cType==22){
    graphics_context_set_fill_color(ctx, GColorYellow);
    graphics_fill_circle(ctx, hc, Radio*0.9);
    graphics_context_set_fill_color(ctx, GColorChromeYellow);
    graphics_fill_circle(ctx, hc, Radio*0.8);
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_fill_circle(ctx, hc, Radio*0.7);
    graphics_context_set_fill_color(ctx, GColorGreen);
    graphics_fill_circle(ctx, hc, Radio*0.6);
    graphics_context_set_fill_color(ctx, GColorIslamicGreen);
    graphics_fill_circle(ctx, hc, Radio*0.5);
    graphics_context_set_fill_color(ctx, GColorCeleste);
    graphics_fill_circle(ctx, hc, Radio*0.4);
    graphics_context_set_fill_color(ctx, GColorBlue);
    graphics_fill_circle(ctx, hc, Radio*0.3);
    graphics_context_set_fill_color(ctx, GColorMagenta);
    graphics_fill_circle(ctx, hc, Radio*0.2);
    graphics_context_set_fill_color(ctx, GColorJazzberryJam);
    graphics_fill_circle(ctx, hc, Radio*0.1);
  }
  #endif
  if (UseCrown==true){
    #ifdef PBL_COLOR
      if (UseShadows==true){
         graphics_context_set_stroke_width(ctx, 4);
         graphics_context_set_stroke_color(ctx, CShadow);
         graphics_draw_circle(ctx, hs, Radio); 
         graphics_context_set_stroke_color(ctx, BColor);
         graphics_draw_circle(ctx, hc, Radio+5);
      }
      graphics_context_set_stroke_width(ctx, 5);
      graphics_context_set_stroke_color(ctx, CCrown);
    #else
      graphics_context_set_stroke_color(ctx, CCrown);
    #endif
    graphics_draw_circle(ctx, hc, Radio);
  }

  if (DateBox == true){
    #ifdef PBL_COLOR
    if (UseShadows == true){
      graphics_context_set_fill_color(ctx, CShadow);
      graphics_fill_rect(ctx, next, 6, GCornersAll);
      if (bType ==1) {graphics_fill_rect(ctx, nextBattery, 6, GCornersAll);}
    }
    #endif
    GRect sDBox = GRect(hc.x-28,hc.y+16,58,24);
    graphics_context_set_fill_color(ctx, CDBox);
    graphics_fill_rect(ctx, sDBox, 6, GCornersAll);
    if (bType ==1) {
      GRect sDBat = GRect(hc.x-13,hc.y-37,28,22);
      graphics_fill_rect(ctx, sDBat, 6, GCornersAll);}
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
	      (int32_t)(Radio-12) / TRIG_MAX_RATIO) + hc.y;
      sEnd.x = (int16_t)(sin_lookup(angle) *
	      (int32_t)(Radio-12) / TRIG_MAX_RATIO) + hc.x;
    
    if (x % 5 != 0) {
      if (hTicks>2){
        graphics_context_set_stroke_color(ctx, CMMarks);
        graphics_context_set_fill_color(ctx, CMMarks);
        gpath_move_to(minute_square, ray);
        gpath_rotate_to(minute_square, angle);
        gpath_draw_filled(ctx, minute_square);
        gpath_draw_outline(ctx, minute_square);
      }
    } else if (x % 15 != 0) {
      if (hTicks>1){ 
        graphics_context_set_stroke_color(ctx, CHMarks);
        graphics_context_set_fill_color(ctx, CHMarks);
        gpath_move_to(hour_square, ray);
        gpath_rotate_to(hour_square, angle);
        gpath_draw_filled(ctx, hour_square);
        gpath_draw_outline(ctx, hour_square);
      }
    }
    else{
      if (hTicks>0){      
        graphics_context_set_stroke_color(ctx, CQMarks);
        graphics_context_set_fill_color(ctx, CQMarks);
        gpath_move_to(hour_quarters, ray);
        gpath_rotate_to(hour_quarters, angle);
        gpath_draw_filled(ctx, hour_quarters);
        gpath_draw_outline(ctx, hour_quarters);
        graphics_context_set_stroke_color(ctx, CSphere);
        graphics_draw_line(ctx, ray, sEnd);
      }
    }
  }
}

void shadow_layer_update(Layer *me, GContext *ctx) {
  #ifdef PBL_COLOR
  int end = grosor;
  int start = -1*grosor;
  int x;
  int h_offset = 0;
  int subCtrl = control;
  if (hType == 2) {
     h_offset = Radio*0.4;
  }
  if (hType == 3 || hType == 6) {
     h_offset = Radio*0.15;
  }
    graphics_context_set_antialiased(ctx, true);
    if (hType == 1||hType == 3){
       start = 0;
       end = 0;
    }
    
  //center shadow
  if (UseShadows==true){
    graphics_context_set_fill_color(ctx, CShadow);
    graphics_context_set_stroke_color(ctx, CShadow);
    graphics_fill_circle(ctx, hs, 7);
   if (subCtrl ==1){
	  for(x=start; x <= end; x++) {
          if (hType != 4) {
               smHb.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
               smHb.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));
          } else {
               smHb.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(-1*Radio*0.2+h_offset) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
               smHb.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(-1*Radio*0.2+h_offset) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));           
          }
          smHt.x = ((int32_t)( sin_lookup(mi_angle) * (int32_t)(Radio*0.8) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(mi_angle)*x/TRIG_MAX_RATIO));
          smHt.y = ((int32_t)(-cos_lookup(mi_angle) * (int32_t)(Radio*0.8) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(mi_angle)*x/TRIG_MAX_RATIO));
             if (hType == 4) {
              slmH.x = ((int32_t)( sin_lookup(mi_angle-a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + smHt.x 
                  + (int16_t)(cos_lookup(mi_angle-a_angle)*x/TRIG_MAX_RATIO));
              slmH.y = ((int32_t)(-cos_lookup(mi_angle-a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + smHt.y 
                  + (int16_t)(sin_lookup(mi_angle-a_angle)*x/TRIG_MAX_RATIO));
              srmH.x = ((int32_t)( sin_lookup(mi_angle+a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + smHt.x 
                  + (int16_t)(cos_lookup(mi_angle+a_angle)*x/TRIG_MAX_RATIO));
              srmH.y = ((int32_t)(-cos_lookup(mi_angle+a_angle) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + smHt.y 
                  + (int16_t)(sin_lookup(mi_angle+a_angle)*x/TRIG_MAX_RATIO));
            }
           if (hType != 4) {
             if (hType == 2) {
                if (x == start){
                 sp1 = smHb;
                 sp2 = smHt;
                }
                if (x == end ){
                 sp3 = smHb;
                 sp4 = smHt;
                }
            } else {
                if (x==0){
                 sp1 = smHb;
                 sp2 = smHt;
                }
            }
           } else {
                if (x == start ){
                   sp1 = slmH;
                   sp3 = smHt;
                }
                if (x == end ){
                    sp2 = srmH;
                    sp4 = smHt;
                }
                 if (x==0){
                   sp6 = smHb;
                  }
            }
          }
       }//control
       if (hType == 4) {
                     graphics_context_set_stroke_width(ctx, 4);
                     graphics_fill_circle(ctx, sp6, grosor/2+2);
                     graphics_draw_line  (ctx, sp6, hs);
                     graphics_context_set_stroke_width(ctx, 3);
                     graphics_draw_line  (ctx, sp1, sp3);
                     graphics_draw_line  (ctx, sp2, sp4);
                     graphics_draw_line  (ctx, sp1, sp2);
                     graphics_context_set_stroke_width(ctx, 4);
                     sp5.x = (sp1.x+sp2.x)/2;
                     sp5.y = (sp1.y+sp2.y)/2;
                     graphics_draw_line  (ctx, hs, sp5);
                     sp5.x = (sp3.x+sp4.x)/2;
                     sp5.y = (sp3.y+sp4.y)/2;               
                     graphics_fill_circle(ctx, sp5, grosor/2+2);
              }
              if (hType == 2) {
                     graphics_context_set_stroke_width(ctx, 2);
                     graphics_draw_line  (ctx, sp1, sp2);
                     graphics_draw_line  (ctx, sp3, sp4);
                     sp5.x = (sp1.x+sp3.x)/2;
                     sp5.y = (sp1.y+sp3.y)/2;
                     graphics_context_set_stroke_width(ctx, 4);
                     graphics_draw_line  (ctx, hs, sp5);
                     graphics_fill_circle(ctx, sp5, grosor/2+2);
                     sp5.x = (sp2.x+sp4.x)/2;
                     sp5.y = (sp2.y+sp4.y)/2;
                     graphics_fill_circle(ctx, sp5, grosor/2+2);
              }
              if (hType == 1 || hType == 3 || hType > 4) {
                     graphics_context_set_stroke_width(ctx, grosor*2);
                     graphics_draw_line  (ctx, sp1, sp2);
              }

         if (subCtrl ==1){
	        for(x=start; x <= end; x++) {
            shHb.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            shHb.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(-1*Radio*0.15+h_offset) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            shHt.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(Radio*0.5) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            shHt.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.5) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
            if (hType == 4) {
             shHt.x = ((int32_t)( sin_lookup(hh_angle) * (int32_t)(Radio*0.6) / TRIG_MAX_RATIO) + hs.x 
                  + (int16_t)(cos_lookup(hh_angle)*x/TRIG_MAX_RATIO));
             shHt.y = ((int32_t)(-cos_lookup(hh_angle) * (int32_t)(Radio*0.6) / TRIG_MAX_RATIO) + hs.y 
                  + (int16_t)(sin_lookup(hh_angle)*x/TRIG_MAX_RATIO));
             slH.x = ((int32_t)( sin_lookup(hh_angle+a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + shHt.x 
                  + (int16_t)(cos_lookup(hh_angle+a_angle/3)*x/TRIG_MAX_RATIO));
             slH.y = ((int32_t)(-cos_lookup(hh_angle+a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + shHt.y 
                  + (int16_t)(sin_lookup(hh_angle+a_angle/3)*x/TRIG_MAX_RATIO));
             srH.x = ((int32_t)( sin_lookup(hh_angle-a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + shHt.x 
                  + (int16_t)(cos_lookup(hh_angle-a_angle/3)*x/TRIG_MAX_RATIO));
             srH.y = ((int32_t)(-cos_lookup(hh_angle-a_angle/3) * (int32_t)(-1*Radio*0.3) / TRIG_MAX_RATIO) + shHt.y 
                  + (int16_t)(sin_lookup(hh_angle-a_angle/3)*x/TRIG_MAX_RATIO));
            }
          if (hType != 4) {
             if (hType == 2) {
              if (x == start){
                 sh1 = shHb;
                 sh2 = shHt;
              }
              if (x == end ){
                 sh3 = shHb;
                 sh4 = shHt;
              }
        } else {
                sh1 = shHb;
                sh2 = shHt;
            }
           } else {
                if (x == start ){
                  sh1 = slH;
                }
                if (x == end ){
                  sh2 = srH;
                }
                  if (x==0){sh3 = shHt;}
          }
      }
   }//control
     if (hType ==4) {
           graphics_context_set_stroke_width(ctx, 5);
           sh4.x = (sh2.x+sh1.x)/2;
           sh4.y = (sh2.y+sh1.y)/2;
           graphics_draw_line  (ctx, hs, sh4);
           graphics_context_set_stroke_width(ctx, 2);
           graphics_draw_line  (ctx, sh1, sh2);
           graphics_draw_line  (ctx, sh1, sh3);
           graphics_draw_line  (ctx, sh2, sh3);
     }
     if (hType == 2) {
           graphics_context_set_stroke_width(ctx, 2);
           graphics_draw_line  (ctx, sh1, sh2);
           graphics_draw_line  (ctx, sh3, sh4);
           sh5.x = (sh1.x+sh3.x)/2;
           sh5.y = (sh1.y+sh3.y)/2;
           graphics_context_set_stroke_width(ctx, 4);
           graphics_draw_line  (ctx, hs, sh5);
           graphics_fill_circle(ctx, sh5, grosor/2+2);
           sh5.x = (sh2.x+sh4.x)/2;
           sh5.y = (sh2.y+sh4.y)/2;
           graphics_fill_circle(ctx, sh5, grosor/2+2);
      }
      if (hType == 1 || hType == 3 || hType > 4) {
           graphics_context_set_stroke_width(ctx, grosor*2);
           graphics_draw_line  (ctx, sh1, sh2);
      }
  }
  #endif
}

void shadow_second_update(Layer *me, GContext *ctx) {
  #ifdef PBL_COLOR
  // Seconds Shadow
  if (UseSeconds == true&&UseShadows==true) {
      GPoint ss;
      GPoint as;
      graphics_context_set_antialiased(ctx, true);
      graphics_context_set_stroke_width(ctx, 2);
      ss.y = (int16_t)(-cos_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hs.y;
      ss.x = ((int16_t)(sin_lookup(ss_angle) *
		  (int32_t)Radio*0.9 / TRIG_MAX_RATIO) + hs.x);
      as.y = (int16_t)(-cos_lookup(ss_angle) *
		  (int32_t)Radio*-0.2 / TRIG_MAX_RATIO) + hs.y;
      as.x = ((int16_t)(sin_lookup(ss_angle) *
		  (int32_t)Radio*-0.2 / TRIG_MAX_RATIO) + hs.x);
      graphics_context_set_stroke_color(ctx, CShadow);
      graphics_draw_line(ctx, as, ss);
  }
  #endif   
}

void time_layer_update(Layer *me, GContext *ctx) {
  int end = grosor;
  int start = -1*grosor;
  int x;
  int h_offset = 0;
  int subCtrl = control;
  if (hType == 2) {
     h_offset = Radio*0.4;
  }
  if (hType == 3 || hType == 6) {
     h_offset = Radio*0.15;
  }
  
  #ifdef PBL_COLOR
    graphics_context_set_antialiased(ctx, true);
    if (hType == 1||hType == 3||hType > 4){
       start = 0;
       end = 0;
    }
  #else
    subCtrl = 1;
  #endif  
    
 //Draw minute hand
  graphics_context_set_fill_color(ctx, CMinutes);
  graphics_context_set_stroke_color(ctx, CMinutes);
  #ifdef PBL_BW
   if (cos_lookup(mi_angle)==0||sin_lookup(mi_angle)==0) {
        end = grosor-1;
   } else {
        end = grosor;
   }
  #endif 
  if (subCtrl == 1) {
	 for(x=start; x <= end; x++) {
         if (hType != 4) {
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
         if (hType == 4) {
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
       if (hType != 4) {
             if (hType == 2) {
              #ifdef PBL_COLOR
                if (x == start){
                 p1 = mHb;
                 p2 = mHt;
                }
                if (x == end ){
                 p3 = mHb;
                 p4 = mHt;
                }
              #else
                graphics_draw_line  (ctx, mHb, mHt);
                if (x >= start/2 && x <= end/2 ){
                graphics_draw_line  (ctx, hc, mHb);
                }
              #endif
            } else {
              #ifdef PBL_COLOR
                if (x==0){
                 p1 = mHb;
                 p2 = mHt;
                }
              #else
                 graphics_draw_line  (ctx, mHb, mHt);
              #endif
            }
            if (x==0){
              #ifdef PBL_BW
                 graphics_fill_circle(ctx, mHt, grosor/2+2);
              #endif
            }
           } else {
              #ifdef PBL_COLOR
                if (x == start ){
                   p1 = lmH;
                   p3 = mHt;
                }
                if (x == end ){
                    p2 = rmH;
                    p4 = mHt;
                }
                 if (x==0){
                   p6 = mHb;
                  }
              #endif             
            }
          }
       }//control
  #ifdef PBL_COLOR
             if (hType == 4) {
                     graphics_context_set_stroke_width(ctx, 4);
                     graphics_fill_circle(ctx, p6, grosor/2+2);
                     graphics_draw_line  (ctx, p6, hc);
                     graphics_context_set_stroke_width(ctx, 3);
                     graphics_draw_line  (ctx, p1, p3);
                     graphics_draw_line  (ctx, p2, p4);
                     graphics_draw_line  (ctx, p1, p2);
                     graphics_context_set_stroke_width(ctx, 4);
                     p5.x = (p1.x+p2.x)/2;
                     p5.y = (p1.y+p2.y)/2;
                     graphics_draw_line  (ctx, hc, p5);
                     p5.x = (p3.x+p4.x)/2;
                     p5.y = (p3.y+p4.y)/2;               
                     graphics_fill_circle(ctx, p5, grosor/2+2);
              }
              if (hType == 2) {
                     graphics_context_set_stroke_width(ctx, 2);
                     graphics_draw_line  (ctx, p1, p2);
                     graphics_draw_line  (ctx, p3, p4);
                     p5.x = (p1.x+p3.x)/2;
                     p5.y = (p1.y+p3.y)/2;
                     graphics_context_set_stroke_width(ctx, 4);
                     graphics_draw_line  (ctx, hc, p5);
                     graphics_fill_circle(ctx, p5, grosor/2+2);
                     p5.x = (p2.x+p4.x)/2;
                     p5.y = (p2.y+p4.y)/2;
                     graphics_fill_circle(ctx, p5, grosor/2+2);
              }
              if (hType == 1 || hType == 3 || hType == 5 || hType == 6) {
                     graphics_context_set_stroke_width(ctx, grosor*2);
                     graphics_draw_line  (ctx, p1, p2);
                   if (hType == 5 || hType == 6){
                      graphics_context_set_stroke_width(ctx, 3);
                      if (cType==20 || cType==5 || cType==8|| cType==19){
                                  graphics_context_set_stroke_color(ctx, CCrown);
                       } else { if (cType==13 || cType==14 || cType==15 || cType==16|| cType==17|| cType==21){
                                  graphics_context_set_stroke_color(ctx, CSphere);
                                } else {graphics_context_set_stroke_color(ctx, CHours);}
                              }
                      graphics_draw_line  (ctx, p1, p2);     
                   } 
              }
  #endif
  
  // Draw hour hand
  graphics_context_set_fill_color(ctx, CHours);
  graphics_context_set_stroke_color(ctx, CHours);
 if (subCtrl == 1){
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
     if (hType ==4 ) {
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
    if (hType != 4) {
             if (hType == 2) {
              #ifdef PBL_COLOR
              if (x == start){
                   h1 = hHb;
                   h2 = hHt;
              }
              if (x == end ){
                   h3 = hHb;
                   h4 = hHt;
              }
              #else
                graphics_draw_line  (ctx, hHb, hHt);
                if (x >= start/2 && x <= end/2 ){
                    graphics_draw_line  (ctx, hc, hHb);
                }
              #endif
            } else {
              #ifdef PBL_COLOR
                h1 = hHb;
                h2 = hHt;
              #else
                graphics_draw_line  (ctx, hHb, hHt);
              #endif
            }
            if (x==0){
              #ifdef PBL_COLOR
              #else
                graphics_fill_circle(ctx, hHt, grosor/2+2);
              #endif
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
              #endif  
          }
      }
   }//control
 
  #ifdef PBL_COLOR  
     if (hType ==4) {
           graphics_context_set_stroke_width(ctx, 5);
           h4.x = (h2.x+h1.x)/2;
           h4.y = (h2.y+h1.y)/2;
           graphics_draw_line  (ctx, hc, h4);
           graphics_context_set_stroke_width(ctx, 2);
           graphics_draw_line  (ctx, h1, h2);
           graphics_draw_line  (ctx, h1, h3);
           graphics_draw_line  (ctx, h2, h3);
     }
     if (hType == 2) {
           graphics_context_set_stroke_width(ctx, 2);
           graphics_draw_line  (ctx, h1, h2);
           graphics_draw_line  (ctx, h3, h4);
           h5.x = (h1.x+h3.x)/2;
           h5.y = (h1.y+h3.y)/2;
           graphics_context_set_stroke_width(ctx, 4);
           graphics_draw_line  (ctx, hc, h5);
           graphics_fill_circle(ctx, h5, grosor/2+2);
           h5.x = (h2.x+h4.x)/2;
           h5.y = (h2.y+h4.y)/2;
           graphics_fill_circle(ctx, h5, grosor/2+2);
      }
      if (hType == 1 || hType == 3 || hType > 4) {
           graphics_context_set_stroke_width(ctx, grosor*2);
           graphics_draw_line  (ctx, h1, h2);
           if (hType == 5 || hType == 6){
                graphics_context_set_stroke_width(ctx, 3);
                if (cType==20 || cType==5 || cType==8|| cType==19){
                  graphics_context_set_stroke_color(ctx, CCrown);
                } else { if (cType==13 || cType==14 || cType==15|| cType==16|| cType==17){
                         graphics_context_set_stroke_color(ctx, CSphere);
                        } else {graphics_context_set_stroke_color(ctx, CMinutes);}
                       }
                  
             graphics_draw_line  (ctx, h1, h2);     
             }  
      }
   #endif                  
  //draw centre circles
  graphics_context_set_fill_color(ctx, CHours);
  graphics_fill_circle(ctx, hc, 7);
  #ifdef PBL_COLOR
    if (gcolor_equal(CHours, CMinutes)){
      graphics_context_set_fill_color(ctx, CSeconds);
    } else {
      graphics_context_set_fill_color(ctx, CMinutes);
    }
    graphics_context_set_stroke_width(ctx, 1);
  #else
    graphics_context_set_fill_color(ctx, CSphere);
  #endif
  graphics_fill_circle(ctx, hc, 3);
}

void time_second_update(Layer *me, GContext *ctx) {
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
      graphics_context_set_stroke_color(ctx, CSeconds);
      graphics_context_set_fill_color(ctx, CSeconds);
      graphics_draw_line(ctx, ac, sc);
      graphics_fill_circle(ctx, hc, 2);
  }
}

void handle_tick(struct tm *now, TimeUnits units_changed) {
  setlocale(LC_TIME, "");
  if (lTime>0 && iLight>-1){
      iLight= iLight+1;
      if (iLight >= lTime){
        light_enable(false);
        iLight = -1;
        if (UseSeconds == false){
          tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
        }
      } else {
         tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
      }
    }

  hh_angle = (TRIG_MAX_ANGLE*(((now->tm_hour%12)*6)+(now->tm_min/10)))/(12*6);
  mi_angle = TRIG_MAX_ANGLE * (now->tm_min) / 60;
  a_angle =  TRIG_MAX_ANGLE * 1 / 60;
  if (SrcSaver>0 && current== 0 && iLight == -1){
          iTimer= iTimer+1;
          if (iTimer > sTime){
              iTimer = 0;
              if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "handle_tick: Before Setting Screen Saver %d", SrcSaver);}
              if (SrcSaver==1){
                  if (UseSeconds == true) { 
                          if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "Set seconds off");}
                          a = 1;
                          UseSeconds = false;
                          current = 1; 
                          sTime = sTime/60+1;
                          tick_timer_service_subscribe(MINUTE_UNIT, handle_tick); }
             } else {
                     #ifdef PBL_COLOR
                          if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "Set screen saver on");}
                          tick_timer_service_subscribe(DAY_UNIT, handle_tick);
                          current = 1; 
                          if (SrcSaver==maxSaver) {setImage(SrcSaver);}
                          layer_mark_dirty(shadow_layer);
                          layer_mark_dirty(time_layer);
                          layer_mark_dirty(shadow_second);
                          layer_mark_dirty(time_second);
                          window_stack_push(s, true);
                     #endif
              }
          }
  }
  if (UseSeconds == true){
      ss_angle = TRIG_MAX_ANGLE / 60 * (now->tm_sec);
      if((now->tm_sec)==0||a==1){control = 1;} else {control=0;}
  }
  
  static char date_buf[] = "Mon.  1";
  static char battery_buf[16];
  int key = 0;
  int sector;
  if ((now->tm_min)==7||(now->tm_min)==23||(now->tm_min)==37||(now->tm_min)==53) {
      if (UseSeconds == false) {key = 1;
                                control = 1;} 
      else {if((now->tm_sec)==0){key = 1;
                                 control = 1;}}
  }
  if (a==1){
          key = 1;
          a=0; 
          control = 1;
  }
     layer_mark_dirty(shadow_layer);
     layer_mark_dirty(time_layer);
     layer_mark_dirty(shadow_second);
     layer_mark_dirty(time_second);
  
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
     layer_mark_dirty(marks_layer);
     layer_mark_dirty(dial_layer);
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
 
     c_state = battery_state_service_peek();
     strftime(date_buf, sizeof("Mon.  1"), "%a. %e", now);
     text_layer_set_text(date, date_buf);
     snprintf(battery_buf, sizeof(battery_buf), "%d", c_state.charge_percent);
   
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
      if (bType ==0){
         nextBattery = GRect(hc.x-7,hc.y-33,15,15);
      } else {
         nextBattery = GRect(hc.x-15,hc.y-38,30,22);
      }
      nextBluetooth = GRect(hc.x-20,hc.y-41,40,40);
      text_layer_set_text_color(date, CFont);
      layer_set_frame(text_layer_get_layer(date), next);
      if (bType == 1){
        if (c_state.is_charging==true){
          text_layer_set_text(Batt, "§");
        } else {
          text_layer_set_text(Batt, battery_buf);
        }
        text_layer_set_text_color(Batt, CFont);
        layer_set_frame(text_layer_get_layer(Batt), nextBattery);
      } else {
        text_layer_set_text(Batt, "");
        layer_set_frame(battery, nextBattery);
        layer_set_update_proc(battery, battery_update_proc);
      }
        layer_set_frame(bluetooth, nextBluetooth);
        layer_set_update_proc(bluetooth, bluetooth_update_proc);
    
      if (numbType != 1) {
        text_layer_set_text_color(BN3, CBNumbers);
        text_layer_set_text_color(BN6, CBNumbers);
        text_layer_set_text_color(BN9, CBNumbers);
        text_layer_set_text_color(BN12, CBNumbers);
        nextBN3 = GRect(hc.x-13+Radio*0.65,hc.y-18,28,28);
        nextBN6 = GRect(hc.x-13,hc.y-18+Radio*0.6,28,28);
        nextBN9 = GRect(hc.x-13-Radio*0.65,hc.y-18,28,28);
        nextBN12 = GRect(hc.x-13,hc.y-18-Radio*0.6,28,28);
        if (numbType == 4 || numbType ==5 || numbType ==6) {
          text_layer_set_text_color(SN1, CSNumbers);
          text_layer_set_text_color(SN2, CSNumbers);
          text_layer_set_text_color(SN4, CSNumbers);
          text_layer_set_text_color(SN5, CSNumbers);
          text_layer_set_text_color(SN7, CSNumbers);
          text_layer_set_text_color(SN8, CSNumbers);
          text_layer_set_text_color(SN10, CSNumbers);
          text_layer_set_text_color(SN11, CSNumbers);
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
        if (numbType == 6 || numbType == 7) {
          if (numbType == 6) {
            text_layer_set_text(SN1, "5");
            text_layer_set_text(SN2, "10");
            text_layer_set_text(SN4, "20");
            text_layer_set_text(SN5, "25");            
            text_layer_set_text(SN7, "35");
            text_layer_set_text(SN8, "40");
            text_layer_set_text(SN10, "50");
            text_layer_set_text(SN11, "55");         
          }
          text_layer_set_text(BN3, "15");
          text_layer_set_text(BN6, "30");
          text_layer_set_text(BN9, "45");
          text_layer_set_text(BN12, "60");
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
        if (numbType == 4 || numbType == 5 || numbType == 6) {
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
  if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "Enter in_recv_handler");}
  //Get Tuple
  Tuple *t = dict_read_first(iterator);
  a = 1;
  //Process all pairs present
  while(t != NULL) {
    if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "Iterator %d received with value %s", (int)t->key, t->value->cstring);}
    // Process this pair's key
    switch(t->key)
    {
    case K_UseSeconds:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        UseSeconds = true;
        persist_write_bool(K_UseSeconds, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        UseSeconds = false;
        persist_write_bool(K_UseSeconds, false);
      };
      break;
    case K_UseShadows:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        UseShadows = true;
        persist_write_bool(K_UseShadows, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        UseShadows = false;
        persist_write_bool(K_UseShadows, false);
      };
      break;
    case K_Radio:
       Radio = atoi(t->value->cstring);
       persist_write_int(K_Radio, Radio);
       break;
    case K_ClockType:
       cType = atoi(t->value->cstring);
       persist_write_int(K_ClockType, cType);
       break;
    case K_HandType:
       hType = atoi(t->value->cstring);
       persist_write_int(K_HandType, hType);
       break;
    case K_Bluetooth:
       uBluetooth = atoi(t->value->cstring);
       persist_write_int(K_Bluetooth, uBluetooth);
       break;
    case K_Numbers:
       numbType = atoi(t->value->cstring);
       persist_write_int(K_Numbers, numbType);
       break;
    case K_DateBox:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        DateBox = true;
        persist_write_bool(K_DateBox, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        DateBox = false;
        persist_write_bool(K_DateBox, false);
      };
      break;
    case K_Crown:
      if(strcmp(t->value->cstring, "0") == 0)
      {
        UseCrown = true;
        persist_write_bool(K_Crown, true);
      }
      else if(strcmp(t->value->cstring, "1") == 0)
      {
        UseCrown = false;
        persist_write_bool(K_Crown, false);
      };
      break;      
    case K_Ticks:
       hTicks = atoi(t->value->cstring);
       persist_write_int(K_Ticks, hTicks);
       break;
    case K_SrcSaver:
       SrcSaver = atoi(t->value->cstring);
       persist_write_int(K_SrcSaver, SrcSaver);
       break;
    case K_Time:
       sTime = atoi(t->value->cstring);
       iTimer=0;
       persist_write_int(K_Time, sTime);
       break;
    case K_Light:
       lTime = atoi(t->value->cstring);
       iLight=0;
       if (lTime == 0){iLight = -1;}
       persist_write_int(K_Light, lTime);
       break;
    case K_Battery:
       bType = atoi(t->value->cstring);
       persist_write_int(K_Battery, bType);
       break;
    }
   // Get next pair, if any
   t = dict_read_next(iterator);
 }
  if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "Exit loop message received");}
  setColors (cType);
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
  if (SrcSaver == 1){UseSeconds = false;}
  if (UseSeconds == true) {sTime = sTime * 60;
                           tick_timer_service_subscribe(SECOND_UNIT, handle_tick); }
                   else  { sTime = (sTime+1);
                           tick_timer_service_subscribe(MINUTE_UNIT, handle_tick); }
  if (SrcSaver>1){
       current = 0;
       if (SrcSaver!=maxSaver){
         setImage (SrcSaver);
       }
      if(w != NULL) {window_stack_push(w, true);}
  }
}

static void setSrcSaver (int ScreenSaver)
{ 
  //Screen Saver Seconds ON/OFF
  if (SrcSaver==1){
    if (UseSeconds == false) { 
              iTimer=0;
              if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "sTime seconds while %d", sTime);}
              sTime = (sTime-1) * 60;
              a = 1;
              current = 0; 
              UseSeconds = true;
              tick_timer_service_subscribe(SECOND_UNIT, handle_tick); }
  }
  if (SrcSaver>1){ 
    layer_mark_dirty(SrcSaver_layer);
    if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "sTime srcsaver %d", sTime);}
    if (current == 1){
      iTimer=0;
      a = 1;
      current = 0;
      if (UseSeconds == true) { 
              tick_timer_service_subscribe(SECOND_UNIT, handle_tick); }
      else  { tick_timer_service_subscribe(MINUTE_UNIT, handle_tick); } 
      window_stack_push(w, true);
    }
  }
  if (debug==1){APP_LOG(APP_LOG_LEVEL_INFO, "YOU SHAKE IT ¡¡¡");}
}

static void handle_tap(AccelAxisType axis, int32_t direction) {
  setSrcSaver(SrcSaver);
  if (lTime>0){
      light_enable(true);
      iLight = 0;
      //if (UseSeconds == false){sTime = (sTime) * 60;}
      tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
    }
}

void handle_init(void) {
 //Check for saved KEY options
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  #ifdef PBL_COLOR //shadows only on Pebble Time
  if (persist_exists(K_UseShadows)) {UseShadows = persist_read_bool(K_UseShadows); }
  #else
  UseShadows = false;
  #endif
  if (persist_exists(K_UseSeconds)) {UseSeconds = persist_read_bool(K_UseSeconds); }
  if (persist_exists(K_Radio)) {Radio = persist_read_int(K_Radio); }
  if (persist_exists(K_ClockType)) {cType = persist_read_int(K_ClockType); }
  if (persist_exists(K_HandType)) {hType = persist_read_int(K_HandType); }
  if (persist_exists(K_Bluetooth)) {uBluetooth = persist_read_int(K_Bluetooth); }  
  if (persist_exists(K_Numbers)) {numbType = persist_read_int(K_Numbers); }
  if (persist_exists(K_DateBox)) {DateBox = persist_read_bool(K_DateBox); }  
  if (persist_exists(K_Crown)) {UseCrown = persist_read_bool(K_Crown); }  
  if (persist_exists(K_Ticks)) {hTicks = persist_read_int(K_Ticks); }
  if (persist_exists(K_SrcSaver)) {SrcSaver = persist_read_int(K_SrcSaver); }
  if (persist_exists(K_Time)) {sTime = persist_read_int(K_Time); }
  if (persist_exists(K_Light)) {lTime = persist_read_int(K_Light); }
  if (persist_exists(K_Battery)) {bType = persist_read_int(K_Battery); }
  if (SrcSaver==1){UseSeconds = false;}
  if (UseSeconds == false){sTime = sTime +1;} else {sTime = sTime * 60;}
  if (lTime == 0){iLight = -1;}
  //set colors
  setColors (cType);
  
  //Subscribe battery and bluetooth services
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  accel_tap_service_subscribe(&handle_tap);

#ifdef PBL_COLOR
  //set ScreenSaver
  if (SrcSaver>1){
    s = window_create();
    GRect sbounds = GRect(0,0,144,168);
    SrcSaver_layer = layer_create(sbounds);
    layer_add_child((Layer *)s, SrcSaver_layer);
    iTimer = 0;
    setImage (SrcSaver);
  }
#endif  
  
  //set window
  w = window_create();
  #ifdef PBL_SDK_2
   window_set_fullscreen(w, true);
  #endif
  GRect bounds = GRect(0,0,144,168);
  dial_layer = layer_create(bounds);
  marks_layer = layer_create(bounds);
  shadow_layer = layer_create(bounds);
  shadow_second = layer_create(bounds);
  time_layer = layer_create(bounds);
  time_second = layer_create(bounds);

  //Clock points
  hour_square = gpath_create(&HOUR_POINTS);
  minute_square = gpath_create(&MINUTE_POINTS);
  hour_quarters = gpath_create(&HOUR_QUARTERS);

  //Layer for battery
  rBatt = GRect(0,0,30,22);
  Batt  = text_layer_create(rBatt);
  text_layer_set_background_color(Batt, GColorClear);
  text_layer_set_text_color(Batt, CFont);
  text_layer_set_text_alignment(Batt, GTextAlignmentCenter);
  text_layer_set_font(Batt, nSFont); 
  rBattery = GRect(hc.x-7,hc.y-33,15,15);
  battery = layer_create(rBattery);   
  
  //building date layer
  rDate = GRect(hc.x-30,hc.y+15,60,25);
  date = text_layer_create(rDate);
  text_layer_set_background_color(date, GColorClear);
  text_layer_set_text_color(date, CFont);
  text_layer_set_text_alignment(date, GTextAlignmentCenter);
  text_layer_set_font(date, nSFont); 

   //building dial numbers layers
   //bigNumbers (Quarters)
   rBNumbers = GRect(hc.x,hc.y,28,28);
   BN3 = text_layer_create(rBNumbers);
   text_layer_set_background_color(BN3, GColorClear);
   text_layer_set_text_alignment(BN3, GTextAlignmentCenter);
   text_layer_set_font(BN3, nBFont);
   BN6 = text_layer_create(rBNumbers);
   text_layer_set_background_color(BN6, GColorClear);
   text_layer_set_text_alignment(BN6, GTextAlignmentCenter);
   text_layer_set_font(BN6, nBFont);
   BN9 = text_layer_create(rBNumbers);
   text_layer_set_background_color(BN9, GColorClear);
   text_layer_set_text_alignment(BN9, GTextAlignmentCenter);
   text_layer_set_font(BN9, nBFont);
   BN12 = text_layer_create(rBNumbers);
   text_layer_set_background_color(BN12, GColorClear);
   text_layer_set_text_alignment(BN12, GTextAlignmentCenter);
   text_layer_set_font(BN12, nBFont);
  
   //SmallNumbers
   rSNumbers = GRect(hc.x,hc.y,28,28);
   SN1 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN1, GColorClear);
   text_layer_set_text_alignment(SN1, GTextAlignmentCenter);
   text_layer_set_font(SN1, nSFont);
   SN2 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN2, GColorClear);
   text_layer_set_text_alignment(SN2, GTextAlignmentCenter);
   text_layer_set_font(SN2, nSFont);
   SN4 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN4, GColorClear);
   text_layer_set_text_alignment(SN4, GTextAlignmentCenter);
   text_layer_set_font(SN4, nSFont);
   SN5 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN5, GColorClear);
   text_layer_set_text_alignment(SN5, GTextAlignmentCenter);
   text_layer_set_font(SN5, nSFont);
   SN7 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN7, GColorClear);
   text_layer_set_text_alignment(SN7, GTextAlignmentCenter);
   text_layer_set_font(SN7, nSFont);
   SN8 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN8, GColorClear);
   text_layer_set_text_alignment(SN8, GTextAlignmentCenter);
   text_layer_set_font(SN8, nSFont);
   SN10 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN10, GColorClear);
   text_layer_set_text_alignment(SN10, GTextAlignmentCenter);
   text_layer_set_font(SN10, nSFont);
   SN11 = text_layer_create(rSNumbers);
   text_layer_set_background_color(SN11, GColorClear);
   text_layer_set_text_alignment(SN11, GTextAlignmentCenter);
   text_layer_set_font(SN11, nSFont);
  
  // Layer for bluetooth
  rBluetooth = GRect(hc.x-15,hc.y-41, 30, 30);
  bluetooth = layer_create(rBluetooth);

  // Update layers
  layer_set_update_proc(shadow_layer, shadow_layer_update);
  layer_set_update_proc(shadow_second, shadow_second_update);
  layer_set_update_proc(time_layer, time_layer_update);
  layer_set_update_proc(time_second, time_second_update);
  layer_set_update_proc(dial_layer, dial_layer_update);
  layer_set_update_proc(marks_layer, marks_layer_update);
  layer_set_update_proc(battery, battery_update_proc);
  layer_set_update_proc(bluetooth, bluetooth_update_proc);
  
  // Adding layers to main window
  layer_add_child(window_get_root_layer(w), dial_layer);
  layer_add_child(window_get_root_layer(w), (Layer *)bluetooth);
  layer_add_child(window_get_root_layer(w), shadow_layer);
  layer_add_child(window_get_root_layer(w), shadow_second);
  layer_add_child(window_get_root_layer(w), marks_layer);
  layer_add_child(window_get_root_layer(w), (Layer *)date);
  layer_add_child(window_get_root_layer(w), (Layer *)Batt);    
  layer_add_child(window_get_root_layer(w), (Layer *)battery);
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
  layer_add_child(window_get_root_layer(w), time_second);
  
  if (SrcSaver>1){
     setSrcSaver(SrcSaver);
  }

  window_stack_push(w, true);
  current = 0;
  
  if (UseSeconds == true) { 
          tick_timer_service_subscribe(SECOND_UNIT, handle_tick); }
  else  { tick_timer_service_subscribe(MINUTE_UNIT, handle_tick); }
}

void handle_deinit(void) {
  layer_destroy(shadow_layer);
  layer_destroy(shadow_second);
  layer_destroy(time_layer);
  layer_destroy(time_second);
  layer_destroy(dial_layer);
  layer_destroy(SrcSaver_layer);
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
  layer_destroy((Layer *)Batt);   
  layer_destroy(bluetooth);
  
  gpath_destroy(hour_square);
  gpath_destroy(minute_square);
  gpath_destroy(hour_quarters);
  
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();

  #ifdef PBL_COLOR
    if (SrcSaver>1){
            gbitmap_destroy(ScreenSvr);
            window_destroy(s);
    }
  #endif
    
  window_destroy(w);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
