#include <Arduino.h>
#include "Translations.h"

#define VERSION "1.6"

String currentLang = "en"; // en, tr, it, es
lang_t language_id = LANG_EN;

String SysUser = "admin";
String SysPass = "esp8266";
const uint8_t WEBSERVER_PORT = 80; // The port you can access this device on over HTTP

const String configName = "/config.txt";
const String securityConfig = "/security.txt";
const String displayConfig = "/display.txt";
const String ApiKeyConfig = "/apikey.txt";
const String worldClockConfig = "/worldClock.txt";

#define HOSTNAME "WeatherStat-AP"

#define NTP_SERVERS "pool.ntp.org"

// Setup
int UPDATE_INTERVAL = 10; // Update every 10 minutes
int UPDATE_INTERVAL_SECS = UPDATE_INTERVAL * 60;
String timeDisplayTurnsOn = "08:00";  // 24 Hour Format HH:MM -- Leave blank for always on. (ie 05:30)
String timeDisplayTurnsOff = "00:00"; // 24 Hour Format HH:MM -- Leave blank for always on. Both must be set to work.
bool themeIsDark = true;              // true = dark theme | false = light theme

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = D2;
const int SDC_PIN = D1;
bool INVERT_DISPLAY = false; // true = pins at top | false = pins at the bottom
uint8_t Disp_Contrast = 255;        // 0 = zero brightness | 255 = full brightness
bool IS_24HOUR = true;      	// true = use 24 hour | false = use 12 hour
bool SHOW_MNAME = true;      	// true = show month name | false = show month number

// OpenWeatherMap Settings
String OPEN_WEATHER_MAP_APP_ID = ""; // Sign up here to get an API key: https://openweathermap.org
float OPEN_WEATHER_MAP_LOCATION_LAT = 41.0063820;
float OPEN_WEATHER_MAP_LOCATION_LON = 28.9758720;
String Display_City_Name = "İstanbul";
String Display_Country_Name = "Türkiye";
bool IS_METRIC = true;

// OpenCage Settings
String OPEN_CAGE_ID = ""; // Sign up here to get an API key: https://opencagedata.com

// TimeZoneDB Settings
String TimeZoneDB_KEY = ""; // Sign up here to get an API key: https://timezonedb.com

// World Clock Settings
String World_Clock1_City_Name = "London";
String World_Clock1_Country_Name = "United Kingdom";
float World_Clock1_LAT = 51.5074456;
float World_Clock1_LON = -0.1277653;

String World_Clock2_City_Name = "Milan";
String World_Clock2_Country_Name = "Italy";
float World_Clock2_LAT = 45.4641943;
float World_Clock2_LON = 9.1896346;

String World_Clock3_City_Name = "Los Angeles";
String World_Clock3_Country_Name = "United States of America";
float World_Clock3_LAT = 34.0536909;
float World_Clock3_LON = -118.242766;