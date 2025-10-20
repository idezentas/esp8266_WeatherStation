#include <Arduino.h>

#define VERSION "1.4"

String SysUser = "admin";
String SysPass = "esp8266";
const int WEBSERVER_PORT = 80; // The port you can access this device on over HTTP

const String configName = "/config.txt";
const String securityConfig = "/security.txt";
const String displayConfig = "/display.txt";
const String ApiKeyConfig = "/apikey.txt";

#define HOSTNAME "WeatherStat-AP"

// change for different NTP (time servers)
#define NTP_SERVERS "pool.ntp.org"

// Setup
int UPDATE_INTERVAL = 15; // Update every 15 minutes
int UPDATE_INTERVAL_SECS = UPDATE_INTERVAL * 60;

String timeDisplayTurnsOn = "08:00";  // 24 Hour Format HH:MM -- Leave blank for always on. (ie 05:30)
String timeDisplayTurnsOff = "00:00"; // 24 Hour Format HH:MM -- Leave blank for always on. Both must be set to work.

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = D2;
const int SDC_PIN = D1;
boolean INVERT_DISPLAY = false; // true = pins at top | false = pins at the bottom
int Disp_Contrast = 255;        // 0 = zero brightness | 255 = full brightness
bool IS_24HOUR = true;      	// true = use 24 hour | false = use 12 hour
bool SHOW_MNAME = true;      	// true = show month name | false = show month number

// OpenWeatherMap Settings
String OPEN_WEATHER_MAP_APP_ID = ""; // Sign up here to get an API key: https://openweathermap.org
float OPEN_WEATHER_MAP_LOCATION_LAT = 41.17;
float OPEN_WEATHER_MAP_LOCATION_LON = 28.86;
String Display_City_Name = "İstanbul";
String Display_Country_Name = "Türkiye";
String Display_TZ_NAME = "Europe/Istanbul";
boolean IS_METRIC = true;

// OpenCage Settings
String OPEN_CAGE_ID = ""; // Sign up here to get an API key: https://opencagedata.com