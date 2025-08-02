#include <Arduino.h>
#pragma once

#define VERSION "1.0"

String SysUser = "admin";
String SysPass = "esp8266";
const int WEBSERVER_PORT = 80; // The port you can access this device on over HTTP

String configName = "/config.json";
String securityConfig = "/security.json";
String worldClockConfig = "/worldClock.json";
String CurrencyConfig = "/currency.json";

#define HOSTNAME "WeatherStat-AP" 

// change for different NTP (time servers)
#define NTP_SERVERS "pool.ntp.org"
// #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"

int TZ = 3;     // (utc+) TZ in hours
int DST_MN = 0; // use 60mn for summer time in some countries
String TZ_NAME = "Europe/Istanbul";
String TZ_NAME_SHORT = "+03";

// Setup
int UPDATE_INTERVAL = 30; // Update every 30 minutes
int UPDATE_INTERVAL_SECS = UPDATE_INTERVAL * 60;

String timeDisplayTurnsOn = "08:00";  // 24 Hour Format HH:MM -- Leave blank for always on. (ie 05:30)
String timeDisplayTurnsOff = "00:00"; // 24 Hour Format HH:MM -- Leave blank for always on. Both must be set to work.

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = D2;
const int SDC_PIN = D1;

// OpenWeatherMap Settings
String OPEN_WEATHER_MAP_APP_ID = ""; // Sign up here to get an API key: https://openweathermap.org
float OPEN_WEATHER_MAP_LOCATION_LAT = 41.1719525;
float OPEN_WEATHER_MAP_LOCATION_LON = 28.8594870;
String Display_District_Name = "Sarıyer";
String Display_City_Name = "İstanbul";
String Display_Country_Name = "TR";
boolean IS_METRIC = true;

// OpenCage Settings
String OPEN_CAGE_ID = "";

// World Clock Settings
String World_Clock1_City_Name = "Los Angeles";
String World_Clock1_Country_Name = "US";
int World_Clock1_TZ = -7;     // (utc+) TZ in hours
int World_Clock1_DST_MN = 0; // use 60mn for summer time in some countries
String World_Clock1_TZ_NAME = "America/Los_Angeles";
String World_Clock1_TZ_NAME_SHORT = "PST";

String World_Clock2_City_Name = "Londra";
String World_Clock2_Country_Name = "GB";
int World_Clock2_TZ = 0;     // (utc+) TZ in hours
int World_Clock2_DST_MN = 0; // use 60mn for summer time in some countries
String World_Clock2_TZ_NAME = "Europe/London";
String World_Clock2_TZ_NAME_SHORT = "GMT";

String World_Clock3_City_Name = "Seul";
String World_Clock3_Country_Name = "KR";
int World_Clock3_TZ = 9;     // (utc+) TZ in hours
int World_Clock3_DST_MN = 0; // use 60mn for summer time in some countries
String World_Clock3_TZ_NAME = "Asia/Seoul";
String World_Clock3_TZ_NAME_SHORT = "KST";

// Currency Settings
// https://raw.githubusercontent.com/idezentas/esp8266_WeatherStation/refs/heads/main/currency-en.json
// https://raw.githubusercontent.com/idezentas/esp8266_WeatherStation/refs/heads/main/currency-tr.json

String BaseCurrency1 = "eur";
String TargetCurrency1 = "try";

String BaseCurrency2 = "usd"; 
String TargetCurrency2 = "try";

