/**The MIT License (MIT)

 Copyright (c) 2025 by idezentas

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <coredecls.h> // settimeofday_cb()
#include <JsonListener.h>
#include <time.h>     // time() ctime()
#include <sys/time.h> // struct timeval
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <ElegantOTA.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "FS.h"
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include "Wire.h"
#include "OpenWeatherMapCurrent.h"
#include "OpenWeatherMapForecast.h"
#include "OpenWeatherMapAir.h"
#include "OpenMeteoApiCurrent.h"
#include "TimeZoneDB.h"
#include "SunMoonCalc.h"
#include "TZDatabase.h"
#include "WeatherStationImages.h"
#include "WindDirectionImages.h"
#include "WeatherImages.h"
#include "DirectionImages.h"
#include "WeatherStationFonts.h"
#include "MoonPhasesFont.h"
#include "OLEDFonts.h"
#include "WebPages.h"
#include "settings.h"

SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi ui(&display);

#define DHTPIN 14

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const uint8_t DAILY_MAX_FORECASTS = 5;

OpenWeatherMapCurrent currentWeatherClient;
OpenWeatherMapCurrentData currentWeather;

OpenWeatherMapCurrent currentWorldWeatherClient1;
OpenWeatherMapCurrentData currentWorldWeather1;

OpenWeatherMapCurrent currentWorldWeatherClient2;
OpenWeatherMapCurrentData currentWorldWeather2;

OpenWeatherMapCurrent currentWorldWeatherClient3;
OpenWeatherMapCurrentData currentWorldWeather3;

OpenWeatherMapForecastData forecastDaily[DAILY_MAX_FORECASTS];
OpenWeatherMapForecast forecastDailyClient;

OpenWeatherMapAir currentAirClient;
OpenWeatherMapAirData currentAir;

OpenMeteoApiCurrent currentMeteoClient;
OpenMeteoApiCurrentData currentMeteo;

TimeZoneDB tzdbDisplayClient;
TimeZoneDBData tzdbDisplay;

TimeZoneDB tzdbWorldClient1;
TimeZoneDBData tzdbWorld1;

TimeZoneDB tzdbWorldClient2;
TimeZoneDBData tzdbWorld2;

TimeZoneDB tzdbWorldClient3;
TimeZoneDBData tzdbWorld3;

TZDatabase tzDB;

SunMoonCalc::Moon moonData;

bool readyForWeatherUpdate = false;

unsigned long timeSinceLastWUpdate = 0;

bool displayOn = true;

String SSID_String;

unsigned long ota_progress_millis = 0;

String Display_TZ_POSIX = "<+03>-3";
int8_t Display_GMTOffset = 3;
String Display_TZ_NAME = "Europe/Istanbul";
String Display_TZ_NAME_SHORT = "TRT";

int8_t World_Clock1_GMTOffset = 0;
String World_Clock1_TZ_NAME_SHORT = "GMT";

int8_t World_Clock2_GMTOffset = 1;
String World_Clock2_TZ_NAME_SHORT = "CET";

int8_t World_Clock3_GMTOffset = -8;
String World_Clock3_TZ_NAME_SHORT = "PST";

ESP8266WebServer server(WEBSERVER_PORT);

void drawProgress(OLEDDisplay *display, int percentage, String label);
void updateData(OLEDDisplay *display);
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeatherHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawDailyForecast(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawDailyForecast2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawDailyForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);
void drawWind(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawAirQuality(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawMoon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawSun(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state);
void drawCity(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawIPInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWifiInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawUVIndex(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawUntilToNextUpdate(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentRoomTemp(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentRoomHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawPressure(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWorldClock1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWorldClock2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWorldClock3(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWorldClock1Weather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWorldClock2Weather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWorldClock3Weather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void setReadyForWeatherUpdate();
String UpperCleanText(String text);
String CleanTextOLED(String text);
int8_t getWifiQuality();
String getMoonPhasesIcon(uint8_t icon);
uint8_t getUVindexText(uint8_t index);
void checkDisplay();
void enableDisplay(bool enable);
void loadDeviceSettings();
void saveDeviceSettings();
void loadSecuritySettings();
void saveSecuritySettings();
void loadDisplaySettings();
void saveDisplaySettings();
void saveApiKeySettings();
void loadApiKeySettings();
void loadWorldClockSettings();
void saveWorldClockSettings();
void loadAllConfigs();
void removeAllConfigs();
void configModeCallback(WiFiManager *myWiFiManager);
void redirectHome();
void handleHomePage();
void handleDevicePage();
void handleSaveDevice();
void handleSecurityPage();
void handleSaveSecurity();
void handleResetDefaults();
void handleDisplayPage();
void handleSaveDisplay();
void handleApiKeyPage();
void handleSaveApiKey();
void handleApiKeyPage();
void handleUpdateData();
void handleWorldClockPage();
void handleSaveWorldClock();
void onOTAStart();
void onOTAProgress(size_t current, size_t final);
void onOTAEnd(bool success);
uint8_t get12Hour(uint8_t hour);
String getAMPM(uint8_t hour);
void applyLanguage(String lang);
String getDayName(int weekday);
String getMonthName(int month);
String getProgressName(uint8_t index);
String getAirQulityName(uint8_t index);
String getUVIndexName(uint8_t index);
String getWindDirectionName(uint8_t index);
String getMoonPhaseName(uint8_t index);

FrameCallback frames[] = {drawDateTime, drawCity, drawCurrentWeather, drawCurrentWeatherHum, drawCurrentRoomTemp, drawCurrentRoomHum, drawWind, drawAirQuality, drawUVIndex, drawPressure, drawDailyForecast, drawDailyForecast2, drawSun, drawMoon, drawWorldClock1, drawWorldClock1Weather, drawWorldClock2, drawWorldClock2Weather, drawWorldClock3, drawWorldClock3Weather, drawWifiInfo, drawIPInfo, drawUntilToNextUpdate};
int numberOfFrames = 23;

OverlayCallback overlays[] = {drawHeaderOverlay};
int numberOfOverlays = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  LittleFS.begin();
  dht.begin();
  delay(10);

  loadAllConfigs();

  applyLanguage(currentLang);
  Serial.println();

  display.init();

  if (INVERT_DISPLAY)
  {
    display.flipScreenVertically();
  }

  display.displayOn();
  display.clear();
  display.display();

  display.setFont(ArialMT_Plain_10_TR);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(Disp_Contrast);

  display.setFont(ArialMT_Plain_16_TR);
  display.drawString(64, 1, "ESP8266");
  display.setFont(ArialMT_Plain_10_TR);
  display.drawString(64, 18, CleanTextOLED(getTranslationStr(TR_WEATHERSTATION)));
  display.setFont(ArialMT_Plain_16_TR);
  display.drawString(64, 30, "By idezentas");
  display.drawString(64, 46, "V" + String(VERSION));
  display.display();
  delay(2000);

  display.clear();
  display.drawXbm(34, 10, 60, 36, WiFi_Logo_bits);
  display.display();
  delay(250);

  WiFiManager wifiManager;

  wifiManager.setAPCallback(configModeCallback);

  if (themeIsDark)
  {
    wifiManager.setDarkMode(true);
  }

  String hostname(HOSTNAME);
  if (!wifiManager.autoConnect((const char *)hostname.c_str()))
  {
    delay(3000);
    WiFi.disconnect(true);
    ESP.restart();
    delay(5000);
  }

  Serial.printf_P(PSTR("SSID: %s\n"), wifiManager.getWiFiSSID().c_str());
  SSID_String = wifiManager.getWiFiSSID();
  Serial.println();

  if (currentLang == "tr")
  {
    Serial.printf_P(PSTR("%s: %%%d\n"), getTranslationStr(TR_LOGRSSI).c_str(), getWifiQuality());
  }
  else if (currentLang == "en" || currentLang == "it")
  {
    Serial.printf_P(PSTR("%s: %d%%\n"), getTranslationStr(TR_LOGRSSI).c_str(), getWifiQuality());
  }
  else
  {
    Serial.printf_P(PSTR("%s: %d %%\n"), getTranslationStr(TR_LOGRSSI).c_str(), getWifiQuality());
  }

  Serial.println();

  delay(250);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16_TR);
  display.drawString(64, 0, "SSID");
  display.setFont(ArialMT_Plain_10_TR);
  display.drawStringMaxWidth(64, 20, 128, wifiManager.getWiFiSSID());
  display.display();
  delay(250);

  String webAddress = "http://" + WiFi.localIP().toString() + ":" + String(WEBSERVER_PORT);
  Serial.printf_P(PSTR("%s %s\n"), getTranslationStr(TR_LOGUSETHISURL).c_str(), webAddress.c_str());
  Serial.println();

  delay(500);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16_TR);
  display.drawString(64, 10, "IP");
  display.drawStringMaxWidth(64, 30, 128, WiFi.localIP().toString());
  display.display();
  delay(250);

  configTime(0, 0, NTP_SERVERS);

  Serial.println(getTranslationStr(TR_LOGWAITNTP).c_str());
  time_t now;
  const time_t VALID_TIME = 24 * 3600;
  while ((now = time(nullptr)) < VALID_TIME)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(F("\n"));
  Serial.println(getTranslationStr(TR_LOGNTPSYNCED).c_str());

  ui.setTargetFPS(30);

  ui.setActiveSymbol(activeSymbole);
  ui.setInactiveSymbol(inactiveSymbole);

  ui.disableAllIndicators();

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  ui.setFrames(frames, numberOfFrames);

  ui.setOverlays(overlays, numberOfOverlays);

  ui.init();

  if (INVERT_DISPLAY)
  {
    display.flipScreenVertically();
  }

  Serial.println();

  updateData(&display);

  server.on("/", HTTP_GET, handleHomePage);
  server.on("/device", HTTP_GET, handleDevicePage);
  server.on("/savedevice", HTTP_GET, handleSaveDevice);
  server.on("/security", HTTP_GET, handleSecurityPage);
  server.on("/savesecurity", HTTP_GET, handleSaveSecurity);
  server.on("/resetdefault", HTTP_GET, handleResetDefaults);
  server.on("/display", HTTP_GET, handleDisplayPage);
  server.on("/savedisplay", HTTP_GET, handleSaveDisplay);
  server.on("/apikey", HTTP_GET, handleApiKeyPage);
  server.on("/saveapikey", HTTP_GET, handleSaveApiKey);
  server.on("/updatedata", HTTP_GET, handleUpdateData);
  server.on("/worldclock", HTTP_GET, handleWorldClockPage);
  server.on("/saveworldclocks", HTTP_GET, handleSaveWorldClock);
  server.onNotFound(redirectHome);

  ElegantOTA.begin(&server);
  ElegantOTA.setAuth(SysUser.c_str(), SysPass.c_str());
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
  server.begin();
}

void loop()
{

  ElegantOTA.loop();
  server.handleClient();
  checkDisplay();

  if (millis() - timeSinceLastWUpdate > (1000UL * UPDATE_INTERVAL_SECS))
  {
    setReadyForWeatherUpdate();
    timeSinceLastWUpdate = millis();
  }

  if (displayOn && readyForWeatherUpdate && ui.getUiState()->frameState == FIXED)
  {
    updateData(&display);
  }

  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0)
  {
    delay(remainingTimeBudget);
  }
}

void drawProgress(OLEDDisplay *display, int percentage, String label)
{
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10_TR);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}

void updateData(OLEDDisplay *display)
{
  drawProgress(display, 15, getProgressName(0));
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);
  Serial.println(F("Display_Timestamp\n"));
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("%s, %02d %s %04d "), getDayName(timeInfo.tm_wday).c_str(), timeInfo.tm_mday, getMonthName(timeInfo.tm_mon + 1).c_str(), timeInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("%s, %02d/%02d/%04d "), getDayName(timeInfo.tm_wday).c_str(), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d "), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeInfo.tm_hour), timeInfo.tm_min, timeInfo.tm_sec, getAMPM(timeInfo.tm_hour));
  }
  (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
  Serial.println();
  delay(500);
  if (TimeZoneDB_KEY != "")
  {
    drawProgress(display, 30, getProgressName(1));
    Serial.printf_P(PSTR("%s\n"), Display_City_Name.c_str());
    tzdbDisplayClient.updateCurrent(&tzdbDisplay, TimeZoneDB_KEY, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
    Serial.print(F("Display_GMTOffset: "));
    Display_GMTOffset = tzdbDisplay.gmtOffset / 3600;
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    Display_TZ_NAME = tzdbDisplay.zoneName;
    Serial.printf_P(PSTR("Display_TZ_NAME: %s\n"), Display_TZ_NAME.c_str());
    Display_TZ_NAME_SHORT = tzdbDisplay.abbreviation;
    Serial.printf_P(PSTR("Display_TZ_NAME_SHORT: %s\n"), Display_TZ_NAME_SHORT.c_str());
    Display_TZ_POSIX = tzDB.getPosix(Display_TZ_NAME);
    Serial.printf_P(PSTR("Display_TZ_POSIX: %s\n"), Display_TZ_POSIX.c_str());
    Serial.println();
    delay(2000);
    Serial.printf_P(PSTR("%s\n"), World_Clock1_City_Name.c_str());
    tzdbWorldClient1.updateCurrent(&tzdbWorld1, TimeZoneDB_KEY, World_Clock1_LAT, World_Clock1_LON);
    Serial.print(F("World_Clock1_GMTOffset: "));
    World_Clock1_GMTOffset = tzdbWorld1.gmtOffset / 3600;
    (World_Clock1_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", World_Clock1_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", World_Clock1_GMTOffset));
    World_Clock1_TZ_NAME_SHORT = tzdbWorld1.abbreviation;
    Serial.printf_P(PSTR("World_Clock1_TZ_NAME_SHORT: %s\n"), World_Clock1_TZ_NAME_SHORT.c_str());
    Serial.println();
    delay(2000);
    Serial.printf_P(PSTR("%s\n"), World_Clock2_City_Name.c_str());
    tzdbWorldClient2.updateCurrent(&tzdbWorld2, TimeZoneDB_KEY, World_Clock2_LAT, World_Clock2_LON);
    Serial.print(F("World_Clock2_GMTOffset: "));
    World_Clock2_GMTOffset = tzdbWorld2.gmtOffset / 3600;
    (World_Clock2_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", World_Clock2_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", World_Clock2_GMTOffset));
    World_Clock2_TZ_NAME_SHORT = tzdbWorld2.abbreviation;
    Serial.printf_P(PSTR("World_Clock2_TZ_NAME_SHORT: %s\n"), World_Clock2_TZ_NAME_SHORT.c_str());
    Serial.println();
    delay(2000);
    Serial.printf_P(PSTR("%s\n"), World_Clock3_City_Name.c_str());
    tzdbWorldClient3.updateCurrent(&tzdbWorld3, TimeZoneDB_KEY, World_Clock3_LAT, World_Clock3_LON);
    Serial.print(F("World_Clock3_GMTOffset: "));
    World_Clock3_GMTOffset = tzdbWorld3.gmtOffset / 3600;
    (World_Clock3_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", World_Clock3_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", World_Clock3_GMTOffset));
    World_Clock3_TZ_NAME_SHORT = tzdbWorld3.abbreviation;
    Serial.printf_P(PSTR("World_Clock3_TZ_NAME_SHORT: %s\n"), World_Clock3_TZ_NAME_SHORT.c_str());
    Serial.println();
    delay(250);
  }
  if (OPEN_WEATHER_MAP_KEY != "")
  {
    drawProgress(display, 45, getProgressName(2));
    Serial.printf_P(PSTR("%s\n"), Display_City_Name.c_str());
    currentWeatherClient.setMetric(IS_METRIC);
    currentWeatherClient.setLanguage(currentLang);
    currentWeatherClient.updateCurrent(&currentWeather, OPEN_WEATHER_MAP_KEY, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
    time_t observationTimestamp = currentWeather.observationTime;
    struct tm timeObserInfo;
    localtime_r(&observationTimestamp, &timeObserInfo);
    if (SHOW_MNAME)
    {
      Serial.printf_P(PSTR("observationTimestamp: %s, %02d %s %04d "), getDayName(timeObserInfo.tm_wday).c_str(), timeObserInfo.tm_mday, getMonthName(timeObserInfo.tm_mon + 1).c_str(), timeObserInfo.tm_year + 1900);
    }
    else
    {
      Serial.printf_P(PSTR("observationTimestamp: %s, %02d/%02d/%04d "), getDayName(timeObserInfo.tm_wday).c_str(), timeObserInfo.tm_mday, timeObserInfo.tm_mon + 1, timeObserInfo.tm_year + 1900);
    }
    if (IS_24HOUR)
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d "), timeObserInfo.tm_hour, timeObserInfo.tm_min, timeObserInfo.tm_sec);
    }
    else
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeObserInfo.tm_hour), timeObserInfo.tm_min, timeObserInfo.tm_sec, getAMPM(timeObserInfo.tm_hour));
    }
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    time_t sunriseTimestamp = currentWeather.sunrise;
    struct tm timeRiseInfo;
    localtime_r(&sunriseTimestamp, &timeRiseInfo);
    if (SHOW_MNAME)
    {
      Serial.printf_P(PSTR("sunriseTimestamp: %s, %02d %s %04d "), getDayName(timeRiseInfo.tm_wday).c_str(), timeRiseInfo.tm_mday, getMonthName(timeRiseInfo.tm_mon + 1).c_str(), timeRiseInfo.tm_year + 1900);
    }
    else
    {
      Serial.printf_P(PSTR("sunriseTimestamp: %s, %02d/%02d/%04d "), getDayName(timeRiseInfo.tm_wday).c_str(), timeRiseInfo.tm_mday, timeRiseInfo.tm_mon + 1, timeRiseInfo.tm_year + 1900);
    }
    if (IS_24HOUR)
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d "), timeRiseInfo.tm_hour, timeRiseInfo.tm_min, timeRiseInfo.tm_sec);
    }
    else
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeRiseInfo.tm_hour), timeRiseInfo.tm_min, timeRiseInfo.tm_sec, getAMPM(timeRiseInfo.tm_hour));
    }
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    time_t sunsetTimestamp = currentWeather.sunset;
    struct tm timeSetInfo;
    localtime_r(&sunsetTimestamp, &timeSetInfo);
    if (SHOW_MNAME)
    {
      Serial.printf_P(PSTR("sunsetTimestamp: %s, %02d %s %04d "), getDayName(timeSetInfo.tm_wday).c_str(), timeSetInfo.tm_mday, getMonthName(timeSetInfo.tm_mon + 1).c_str(), timeSetInfo.tm_year + 1900);
    }
    else
    {
      Serial.printf_P(PSTR("sunsetTimestamp: %s, %02d/%02d/%04d "), getDayName(timeSetInfo.tm_wday).c_str(), timeSetInfo.tm_mday, timeSetInfo.tm_mon + 1, timeSetInfo.tm_year + 1900);
    }
    if (IS_24HOUR)
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d "), timeSetInfo.tm_hour, timeSetInfo.tm_min, timeSetInfo.tm_sec);
    }
    else
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeSetInfo.tm_hour), timeSetInfo.tm_min, timeSetInfo.tm_sec, getAMPM(timeSetInfo.tm_hour));
    }
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    Serial.println();
    delay(1000);
    Serial.printf_P(PSTR("%s\n"), World_Clock1_City_Name.c_str());
    currentWorldWeatherClient1.setMetric(IS_METRIC);
    currentWorldWeatherClient1.setLanguage(currentLang);
    currentWorldWeatherClient1.updateCurrent(&currentWorldWeather1, OPEN_WEATHER_MAP_KEY, World_Clock1_LAT, World_Clock1_LON);
    time_t observationWorldTimestamp1 = currentWorldWeather1.observationTime;
    struct tm timeWorldObserInfo1;
    localtime_r(&observationWorldTimestamp1, &timeWorldObserInfo1);
    if (SHOW_MNAME)
    {
      Serial.printf_P(PSTR("observationWorldTimestamp1: %s, %02d %s %04d "), getDayName(timeWorldObserInfo1.tm_wday).c_str(), timeWorldObserInfo1.tm_mday, getMonthName(timeWorldObserInfo1.tm_mon + 1).c_str(), timeWorldObserInfo1.tm_year + 1900);
    }
    else
    {
      Serial.printf_P(PSTR("observationWorldTimestamp1: %s, %02d/%02d/%04d "), getDayName(timeWorldObserInfo1.tm_wday).c_str(), timeWorldObserInfo1.tm_mday, timeWorldObserInfo1.tm_mon + 1, timeWorldObserInfo1.tm_year + 1900);
    }
    if (IS_24HOUR)
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d "), timeWorldObserInfo1.tm_hour, timeWorldObserInfo1.tm_min, timeWorldObserInfo1.tm_sec);
    }
    else
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeWorldObserInfo1.tm_hour), timeWorldObserInfo1.tm_min, timeWorldObserInfo1.tm_sec, getAMPM(timeWorldObserInfo1.tm_hour));
    }
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    Serial.println();
    delay(1000);
    Serial.printf_P(PSTR("%s\n"), World_Clock2_City_Name.c_str());
    currentWorldWeatherClient2.setMetric(IS_METRIC);
    currentWorldWeatherClient2.setLanguage(currentLang);
    currentWorldWeatherClient2.updateCurrent(&currentWorldWeather2, OPEN_WEATHER_MAP_KEY, World_Clock2_LAT, World_Clock2_LON);
    time_t observationWorldTimestamp2 = currentWorldWeather2.observationTime;
    struct tm timeWorldObserInfo2;
    localtime_r(&observationWorldTimestamp2, &timeWorldObserInfo2);
    if (SHOW_MNAME)
    {
      Serial.printf_P(PSTR("observationWorldTimestamp2: %s, %02d %s %04d "), getDayName(timeWorldObserInfo2.tm_wday).c_str(), timeWorldObserInfo2.tm_mday, getMonthName(timeWorldObserInfo2.tm_mon + 1).c_str(), timeWorldObserInfo2.tm_year + 1900);
    }
    else
    {
      Serial.printf_P(PSTR("observationWorldTimestamp2: %s, %02d/%02d/%04d "), getDayName(timeWorldObserInfo2.tm_wday).c_str(), timeWorldObserInfo2.tm_mday, timeWorldObserInfo2.tm_mon + 1, timeWorldObserInfo2.tm_year + 1900);
    }
    if (IS_24HOUR)
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d "), timeWorldObserInfo2.tm_hour, timeWorldObserInfo2.tm_min, timeWorldObserInfo2.tm_sec);
    }
    else
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeWorldObserInfo2.tm_hour), timeWorldObserInfo2.tm_min, timeWorldObserInfo2.tm_sec, getAMPM(timeWorldObserInfo2.tm_hour));
    }
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    Serial.println();
    delay(1000);
    Serial.printf_P(PSTR("%s\n"), World_Clock3_City_Name.c_str());
    currentWorldWeatherClient3.setMetric(IS_METRIC);
    currentWorldWeatherClient3.setLanguage(currentLang);
    currentWorldWeatherClient3.updateCurrent(&currentWorldWeather3, OPEN_WEATHER_MAP_KEY, World_Clock3_LAT, World_Clock3_LON);
    time_t observationWorldTimestamp3 = currentWorldWeather3.observationTime;
    struct tm timeWorldObserInfo3;
    localtime_r(&observationWorldTimestamp3, &timeWorldObserInfo3);
    if (SHOW_MNAME)
    {
      Serial.printf_P(PSTR("observationWorldTimestamp3: %s, %02d %s %04d "), getDayName(timeWorldObserInfo3.tm_wday).c_str(), timeWorldObserInfo3.tm_mday, getMonthName(timeWorldObserInfo3.tm_mon + 1).c_str(), timeWorldObserInfo3.tm_year + 1900);
    }
    else
    {
      Serial.printf_P(PSTR("observationWorldTimestamp3: %s, %02d/%02d/%04d "), getDayName(timeWorldObserInfo3.tm_wday).c_str(), timeWorldObserInfo3.tm_mday, timeWorldObserInfo3.tm_mon + 1, timeWorldObserInfo3.tm_year + 1900);
    }
    if (IS_24HOUR)
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d "), timeWorldObserInfo3.tm_hour, timeWorldObserInfo3.tm_min, timeWorldObserInfo3.tm_sec);
    }
    else
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeWorldObserInfo3.tm_hour), timeWorldObserInfo3.tm_min, timeWorldObserInfo3.tm_sec, getAMPM(timeWorldObserInfo3.tm_hour));
    }
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    Serial.println();
    delay(1000);
    forecastDailyClient.setMetric(IS_METRIC);
    forecastDailyClient.setLanguage(currentLang);
    uint8_t allowedDailyHours[] = {12};
    forecastDailyClient.setAllowedHours(allowedDailyHours, sizeof(allowedDailyHours));
    forecastDailyClient.updateForecasts(forecastDaily, OPEN_WEATHER_MAP_KEY, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON, DAILY_MAX_FORECASTS);
    delay(1000);
    currentAirClient.updateCurrent(&currentAir, OPEN_WEATHER_MAP_KEY, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
    uint8_t aqiIndex = (currentAir.aqi) - 1;
    Serial.printf_P(PSTR("aqiIndex: %s\n"), getAirQulityName(aqiIndex).c_str());
    time_t observationAirTimestamp = currentAir.observationTime;
    struct tm timeObserAirInfo;
    localtime_r(&observationAirTimestamp, &timeObserAirInfo);
    if (SHOW_MNAME)
    {
      Serial.printf_P(PSTR("observationAirTimestamp: %s, %02d %s %04d "), getDayName(timeObserAirInfo.tm_wday).c_str(), timeObserAirInfo.tm_mday, getMonthName(timeObserAirInfo.tm_mon + 1).c_str(), timeObserAirInfo.tm_year + 1900);
    }
    else
    {
      Serial.printf_P(PSTR("observationAirTimestamp: %s, %02d/%02d/%04d "), getDayName(timeObserAirInfo.tm_wday).c_str(), timeObserAirInfo.tm_mday, timeObserAirInfo.tm_mon + 1, timeObserAirInfo.tm_year + 1900);
    }
    if (IS_24HOUR)
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d "), timeObserAirInfo.tm_hour, timeObserAirInfo.tm_min, timeObserAirInfo.tm_sec);
    }
    else
    {
      Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeObserAirInfo.tm_hour), timeObserAirInfo.tm_min, timeObserAirInfo.tm_sec, getAMPM(timeObserAirInfo.tm_hour));
    }
    (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
    Serial.println();
    delay(1000);
  }
  drawProgress(display, 60, getProgressName(3));
  SunMoonCalc *smCalc = new SunMoonCalc(Display_Timestamp, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  moonData = smCalc->calculateSunAndMoonData().moon;
  Serial.println(F("moonData"));
  struct tm timeMoonRiseInfo;
  localtime_r(&moonData.rise, &timeMoonRiseInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("moonData.rise: %s, %02d %s %04d "), getDayName(timeMoonRiseInfo.tm_wday).c_str(), timeMoonRiseInfo.tm_mday, getMonthName(timeMoonRiseInfo.tm_mon + 1).c_str(), timeMoonRiseInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("moonData.rise: %s, %02d/%02d/%04d "), getDayName(timeMoonRiseInfo.tm_wday).c_str(), timeMoonRiseInfo.tm_mday, timeMoonRiseInfo.tm_mon + 1, timeMoonRiseInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d "), timeMoonRiseInfo.tm_hour, timeMoonRiseInfo.tm_min, timeMoonRiseInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeMoonRiseInfo.tm_hour), timeMoonRiseInfo.tm_min, timeMoonRiseInfo.tm_sec, getAMPM(timeMoonRiseInfo.tm_hour));
  }
  (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
  struct tm timeMoonSetInfo;
  localtime_r(&moonData.set, &timeMoonSetInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("moonData.set: %s, %02d %s %04d "), getDayName(timeMoonSetInfo.tm_wday).c_str(), timeMoonSetInfo.tm_mday, getMonthName(timeMoonSetInfo.tm_mon + 1).c_str(), timeMoonSetInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("moonData.set: %s, %02d/%02d/%04d "), getDayName(timeMoonSetInfo.tm_wday).c_str(), timeMoonSetInfo.tm_mday, timeMoonSetInfo.tm_mon + 1, timeMoonSetInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d "), timeMoonSetInfo.tm_hour, timeMoonSetInfo.tm_min, timeMoonSetInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeMoonSetInfo.tm_hour), timeMoonSetInfo.tm_min, timeMoonSetInfo.tm_sec, getAMPM(timeMoonSetInfo.tm_hour));
  }
  (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
  Serial.printf_P(PSTR("moonData.phase.index: %d, "), moonData.phase.index);
  Serial.printf_P(PSTR("MOON_PHASE: %s\n"), getMoonPhaseName(moonData.phase.index).c_str());
  Serial.println();
  delay(250);
  drawProgress(display, 75, getProgressName(4));
  currentMeteoClient.updateWeather(&currentMeteo, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  uint8_t uv_int = currentMeteo.uv_index;
  Serial.printf_P(PSTR("uv_index: %s\n"), getUVIndexName(getUVindexText(uv_int)).c_str());
  time_t timeTimestamp = currentMeteo.time;
  struct tm timeTimeInfo;
  localtime_r(&timeTimestamp, &timeTimeInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("timeTimestamp: %s, %02d %s %04d "), getDayName(timeTimeInfo.tm_wday).c_str(), timeTimeInfo.tm_mday, getMonthName(timeTimeInfo.tm_mon + 1).c_str(), timeTimeInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("timeTimestamp: %s, %02d/%02d/%04d "), getDayName(timeTimeInfo.tm_wday).c_str(), timeTimeInfo.tm_mday, timeTimeInfo.tm_mon + 1, timeTimeInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d "), timeTimeInfo.tm_hour, timeTimeInfo.tm_min, timeTimeInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s "), get12Hour(timeTimeInfo.tm_hour), timeTimeInfo.tm_min, timeTimeInfo.tm_sec, getAMPM(timeTimeInfo.tm_hour));
  }
  (Display_GMTOffset >= 0) ? (Serial.printf_P(PSTR("%s+%d\n"), "GMT", Display_GMTOffset)) : (Serial.printf_P(PSTR("%s%d\n"), "GMT", Display_GMTOffset));
  Serial.println();
  delay(1000);
  readyForWeatherUpdate = false;
  drawProgress(display, 100, getProgressName(5));
  delay(250);
}

void drawIPInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 5 + y, "IP");
  display->drawStringMaxWidth(64 + x, 25 + y, 128, WiFi.localIP().toString());
}

void drawWifiInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 0 + y, "SSID");
  display->setFont(ArialMT_Plain_10_TR);
  display->drawStringMaxWidth(64 + x, 20 + y, 128, CleanTextOLED(SSID_String));
}

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);

  char buff[16];
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24_TR);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  }
  else
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), get12Hour(timeInfo.tm_hour), timeInfo.tm_min, timeInfo.tm_sec);
  }

  display->drawString(64 + x, 10 + y, String(buff));

  display->setFont(ArialMT_Plain_10_TR);

  if (SHOW_MNAME)
  {
    sprintf_P(buff, PSTR("%s, %02d %s %04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, CleanTextOLED(getMonthName(timeInfo.tm_mon + 1)), timeInfo.tm_year + 1900);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  }

  display->drawString(64 + x, 0 + y, String(buff));

  if (!IS_24HOUR)
  {
    display->drawString(64 + x, 38 + y, getAMPM(timeInfo.tm_hour));
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  (Display_GMTOffset >= 0) ? (sprintf_P(buff, PSTR("%s+%d"), "GMT", Display_GMTOffset)) : (sprintf_P(buff, PSTR("%s%d"), "GMT", Display_GMTOffset));
  display->drawString(0 + x, 38 + y, String(buff));

  display->drawString(105 + x, 38 + y, Display_TZ_NAME_SHORT);
}

void drawUntilToNextUpdate(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  unsigned long timeLeftMillis = ((millis() - timeSinceLastWUpdate) < ((1000UL * UPDATE_INTERVAL_SECS))) ? (((1000UL * UPDATE_INTERVAL_SECS)) - (millis() - timeSinceLastWUpdate)) : 0;
  unsigned long minutesLeft = timeLeftMillis / 60000;
  unsigned long secondsLeft = (timeLeftMillis % 60000) / 1000;

  char buff[16];
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24_TR);
  sprintf_P(buff, PSTR("%02d:%02d"), minutesLeft, secondsLeft);
  display->drawString(64 + x, 25 + y, String(buff));

  display->setFont(ArialMT_Plain_10_TR);
  display->drawStringMaxWidth(64 + x, 0 + y, 128, CleanTextOLED(getTranslationStr(TR_NEXTUPDATE)));
}

void drawCity(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawStringMaxWidth(64 + x, 10 + y, 128, CleanTextOLED(UpperCleanText(Display_City_Name)));
}

void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanTextOLED(UpperCleanText(currentWeather.description)));

  display->setFont(ArialMT_Plain_24_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWeather.temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->drawString(55 + x, 5 + y, temp);

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(25 + x, 0 + y, currentWeather.iconMeteoCon);
}

void drawCurrentWeatherHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(70 + x, 10 + y, String(currentWeather.humidity));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_humidity);
}

void drawCurrentRoomTemp(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp;
  if (IS_METRIC)
  {
    float t = dht.readTemperature();
    temp = String(t, 0) + ("°C");
  }
  else
  {
    float f = dht.readTemperature(true);
    temp = String(f, 0) + ("°F");
  }
  display->drawString(70 + x, 10 + y, temp);
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_room_temperature);
}

void drawCurrentRoomHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  float h = dht.readHumidity();
  String hum = String(h, 0);
  display->drawString(70 + x, 10 + y, hum);
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_room_humidity);
}

void drawDailyForecast(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  drawDailyForecastDetails(display, x, y, 0);
  drawDailyForecastDetails(display, x + 44, y, 1);
  drawDailyForecastDetails(display, x + 88, y, 2);
}

void drawDailyForecast2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  drawDailyForecastDetails(display, x + 10, y, 3);
  drawDailyForecastDetails(display, x + 70, y, 4);
}

void drawDailyForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex)
{
  time_t observationTimestamp = forecastDaily[dayIndex].observationTime;
  struct tm timeInfo;
  localtime_r(&observationTimestamp, &timeInfo);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10_TR);
  display->drawString(x + 20, y, CleanTextOLED(getDayName(timeInfo.tm_wday)));

  display->setFont(Meteocons_Plain_21);
  display->drawString(x + 20, y + 12, forecastDaily[dayIndex].iconMeteoCon);

  String temp = String(forecastDaily[dayIndex].temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->setFont(ArialMT_Plain_10_TR);
  display->drawString(x + 20, y + 34, temp);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawWind(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String wind = (IS_METRIC ? String((currentWeather.windSpeed * 3.6), 0) : String((currentWeather.windSpeed), 0)) + (IS_METRIC ? " km/h" : " mph");
  display->drawString(55 + x, 10 + y, wind);

  int val = floor(currentWeather.windDeg / 45);
  int dirID = (val % 8);

  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(90 + x, 38 + y, CleanTextOLED(getWindDirectionName(dirID)));

  if (dirID == 0)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_north);
  }

  if (dirID == 1)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_northeast);
  }

  if (dirID == 2)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_east);
  }

  if (dirID == 3)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_southeast);
  }

  if (dirID == 4)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_south);
  }

  if (dirID == 5)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_southwest);
  }

  if (dirID == 6)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_west);
  }

  if (dirID == 7)
  {
    display->drawXbm(0 + x, 0 + y, 45, 45, direction_northwest);
  }
}

void drawAirQuality(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  uint8_t aqiIndex = (currentAir.aqi) - 1;
  display->drawStringMaxWidth(90 + x, 12 + y, 80, CleanTextOLED(getAirQulityName(aqiIndex)));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawXbm(0 + x, 0 + y, 45, 45, weather_air_quality);
}

void drawUVIndex(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  uint8_t uv_int = currentMeteo.uv_index;
  display->drawStringMaxWidth(90 + x, 12 + y, 80, CleanTextOLED(getUVIndexName(getUVindexText(uv_int))));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawXbm(0 + x, 0 + y, 45, 45, weather_uv_index);
}

void drawPressure(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String pres = String(currentWeather.pressure) + " hPa";
  display->drawStringMaxWidth(53 + x, 14 + y, 128, pres);
  display->drawXbm(5 + x, 0 + y, 45, 45, weather_pressure);
}

void drawMoon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  struct tm timeMoonRiseInfo;
  localtime_r(&moonData.rise, &timeMoonRiseInfo);

  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);

  display->drawXbm(40 + x, 5 + y, 10, 10, weather_weather_rise);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanTextOLED(getDayName(timeMoonRiseInfo.tm_wday)), timeMoonRiseInfo.tm_hour, timeMoonRiseInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanTextOLED(getDayName(timeMoonRiseInfo.tm_wday)), get12Hour(timeMoonRiseInfo.tm_hour), timeMoonRiseInfo.tm_min, getAMPM(timeMoonRiseInfo.tm_hour));
  }

  display->drawString(55 + x, 5 + y, String(buff));

  struct tm timeMoonSetInfo;
  localtime_r(&moonData.set, &timeMoonSetInfo);

  display->drawXbm(40 + x, 20 + y, 10, 10, weather_weather_set);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanTextOLED(getDayName(timeMoonSetInfo.tm_wday)), timeMoonSetInfo.tm_hour, timeMoonSetInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanTextOLED(getDayName(timeMoonSetInfo.tm_wday)), get12Hour(timeMoonSetInfo.tm_hour), timeMoonSetInfo.tm_min, getAMPM(timeMoonSetInfo.tm_hour));
  }

  display->drawString(55 + x, 20 + y, String(buff));

  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanTextOLED(getMoonPhaseName(moonData.phase.index)));

  display->setFont(Moon_Phases_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(20 + x, 0 + y, getMoonPhasesIcon(moonData.phase.index));
}

void drawSun(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  time_t sunriseTimestamp = currentWeather.sunrise;
  struct tm timeRiseInfo;
  localtime_r(&sunriseTimestamp, &timeRiseInfo);

  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);

  display->drawXbm(40 + x, 5 + y, 10, 10, weather_weather_rise);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanTextOLED(getDayName(timeRiseInfo.tm_wday)), timeRiseInfo.tm_hour, timeRiseInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanTextOLED(getDayName(timeRiseInfo.tm_wday)), get12Hour(timeRiseInfo.tm_hour), timeRiseInfo.tm_min, getAMPM(timeRiseInfo.tm_hour));
  }

  display->drawString(55 + x, 5 + y, String(buff));

  time_t sunsetTimestamp = currentWeather.sunset;
  struct tm timeSetInfo;
  localtime_r(&sunsetTimestamp, &timeSetInfo);

  display->drawXbm(40 + x, 20 + y, 10, 10, weather_weather_set);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanTextOLED(getDayName(timeSetInfo.tm_wday)), timeSetInfo.tm_hour, timeSetInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanTextOLED(getDayName(timeSetInfo.tm_wday)), get12Hour(timeSetInfo.tm_hour), timeSetInfo.tm_min, getAMPM(timeSetInfo.tm_hour));
  }

  display->drawString(55 + x, 20 + y, String(buff));

  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanTextOLED(getTranslationStr(TR_SUN)));

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(20 + x, 0 + y, "B");
}

void drawWorldClock1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  int8_t offsetDiff = World_Clock1_GMTOffset - Display_GMTOffset;
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  time_t World_Clock1_Timestamp = Display_Timestamp + (offsetDiff * 3600);
  struct tm timeInfo;
  localtime_r(&World_Clock1_Timestamp, &timeInfo);

  char buff[16];
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16_TR);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  }
  else
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), get12Hour(timeInfo.tm_hour), timeInfo.tm_min, timeInfo.tm_sec);
  }

  display->drawString(64 + x, 22 + y, String(buff));

  display->setFont(ArialMT_Plain_10_TR);

  display->drawString(64 + x, 0 + y, CleanTextOLED(UpperCleanText(World_Clock1_City_Name)));

  if (SHOW_MNAME)
  {
    sprintf_P(buff, PSTR("%s, %02d %s %04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, CleanTextOLED(getMonthName(timeInfo.tm_mon + 1)), timeInfo.tm_year + 1900);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  }

  display->drawString(64 + x, 11 + y, String(buff));

  if (!IS_24HOUR)
  {
    display->drawString(64 + x, 38 + y, getAMPM(timeInfo.tm_hour));
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  (World_Clock1_GMTOffset >= 0) ? (sprintf_P(buff, PSTR("%s+%d"), "GMT", World_Clock1_GMTOffset)) : (sprintf_P(buff, PSTR("%s%d"), "GMT", World_Clock1_GMTOffset));
  display->drawString(0 + x, 38 + y, String(buff));

  display->drawString(105 + x, 38 + y, World_Clock1_TZ_NAME_SHORT);
}

void drawWorldClock2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  int8_t offsetDiff = World_Clock2_GMTOffset - Display_GMTOffset;
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  time_t World_Clock2_Timestamp = Display_Timestamp + (offsetDiff * 3600);
  struct tm timeInfo;
  localtime_r(&World_Clock2_Timestamp, &timeInfo);

  char buff[16];
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16_TR);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  }
  else
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), get12Hour(timeInfo.tm_hour), timeInfo.tm_min, timeInfo.tm_sec);
  }

  display->drawString(64 + x, 22 + y, String(buff));

  display->setFont(ArialMT_Plain_10_TR);

  display->drawString(64 + x, 0 + y, CleanTextOLED(UpperCleanText(World_Clock2_City_Name)));

  if (SHOW_MNAME)
  {
    sprintf_P(buff, PSTR("%s, %02d %s %04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, CleanTextOLED(getMonthName(timeInfo.tm_mon + 1)), timeInfo.tm_year + 1900);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  }

  display->drawString(64 + x, 11 + y, String(buff));

  if (!IS_24HOUR)
  {
    display->drawString(64 + x, 38 + y, getAMPM(timeInfo.tm_hour));
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  (World_Clock2_GMTOffset >= 0) ? (sprintf_P(buff, PSTR("%s+%d"), "GMT", World_Clock2_GMTOffset)) : (sprintf_P(buff, PSTR("%s%d"), "GMT", World_Clock2_GMTOffset));
  display->drawString(0 + x, 38 + y, String(buff));

  display->drawString(105 + x, 38 + y, World_Clock2_TZ_NAME_SHORT);
}

void drawWorldClock3(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  int8_t offsetDiff = World_Clock3_GMTOffset - Display_GMTOffset;
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  time_t World_Clock3_Timestamp = Display_Timestamp + (offsetDiff * 3600);
  struct tm timeInfo;
  localtime_r(&World_Clock3_Timestamp, &timeInfo);

  char buff[16];
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16_TR);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  }
  else
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), get12Hour(timeInfo.tm_hour), timeInfo.tm_min, timeInfo.tm_sec);
  }

  display->drawString(64 + x, 22 + y, String(buff));

  display->setFont(ArialMT_Plain_10_TR);

  display->drawString(64 + x, 0 + y, CleanTextOLED(UpperCleanText(World_Clock3_City_Name)));

  if (SHOW_MNAME)
  {
    sprintf_P(buff, PSTR("%s, %02d %s %04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, CleanTextOLED(getMonthName(timeInfo.tm_mon + 1)), timeInfo.tm_year + 1900);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), CleanTextOLED(getDayName(timeInfo.tm_wday)), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  }

  display->drawString(64 + x, 11 + y, String(buff));

  if (!IS_24HOUR)
  {
    display->drawString(64 + x, 38 + y, getAMPM(timeInfo.tm_hour));
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  (World_Clock3_GMTOffset >= 0) ? (sprintf_P(buff, PSTR("%s+%d"), "GMT", World_Clock3_GMTOffset)) : (sprintf_P(buff, PSTR("%s%d"), "GMT", World_Clock3_GMTOffset));
  display->drawString(0 + x, 38 + y, String(buff));

  display->drawString(105 + x, 38 + y, World_Clock3_TZ_NAME_SHORT);
}

void drawWorldClock1Weather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 0 + y, CleanTextOLED(UpperCleanText(World_Clock1_City_Name)));

  display->drawString(64 + x, 38 + y, CleanTextOLED(UpperCleanText(currentWorldWeather1.description)));

  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWorldWeather1.temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 15 + y, temp);

  display->setFont(Meteocons_Plain_21);
  display->drawString(30 + x, 15 + y, currentWorldWeather1.iconMeteoCon);
}
void drawWorldClock2Weather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 0 + y, CleanTextOLED(UpperCleanText(World_Clock2_City_Name)));

  display->drawString(64 + x, 38 + y, CleanTextOLED(UpperCleanText(currentWorldWeather2.description)));

  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWorldWeather2.temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 15 + y, temp);

  display->setFont(Meteocons_Plain_21);
  display->drawString(30 + x, 15 + y, currentWorldWeather2.iconMeteoCon);
}
void drawWorldClock3Weather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 0 + y, CleanTextOLED(UpperCleanText(World_Clock3_City_Name)));

  display->drawString(64 + x, 38 + y, CleanTextOLED(UpperCleanText(currentWorldWeather3.description)));

  display->setFont(ArialMT_Plain_16_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWorldWeather3.temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 15 + y, temp);

  display->setFont(Meteocons_Plain_21);
  display->drawString(30 + x, 15 + y, currentWorldWeather3.iconMeteoCon);
}

void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state)
{
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);

  char buff[16];

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%02d:%02d %s"), get12Hour(timeInfo.tm_hour), timeInfo.tm_min, getAMPM(timeInfo.tm_hour));
  }

  display->setFont(ArialMT_Plain_10_TR);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(30, 54, String(buff));

  display->drawString(0, 54, String(state->currentFrame + 1) + "/" + String(numberOfFrames));

  if (SHOW_MNAME)
  {
    sprintf_P(buff, PSTR("%02d %s"), timeInfo.tm_mday, CleanTextOLED(getMonthName(timeInfo.tm_mon + 1)));
  }
  else
  {
    sprintf_P(buff, PSTR("%02d/%02d"), timeInfo.tm_mday, timeInfo.tm_mon + 1);
  }

  display->drawString(80, 54, String(buff));

  int8_t quality = getWifiQuality();
  for (int8_t i = 0; i < 4; i++)
  {
    for (int8_t j = 0; j < 2 * (i + 1); j++)
    {
      if (quality > i * 25 || j == 0)
      {
        display->setPixel(120 + 2 * i, 63 - j);
      }
    }
  }

  display->drawHorizontalLine(0, 52, 128);
}

int8_t getWifiQuality()
{
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100)
  {
    return 0;
  }
  else if (dbm >= -50)
  {
    return 100;
  }
  else
  {
    return 2 * (dbm + 100);
  }
}

String UpperCleanText(String text)
{
  if (currentLang == "tr")
  {
    text.replace(F("i"), F("İ"));
  }
  text.toUpperCase();
  text.replace(F("ç"), F("Ç"));
  text.replace(F("ş"), F("Ş"));
  text.replace(F("ı"), F("I"));
  text.replace(F("ğ"), F("Ğ"));
  text.replace(F("ü"), F("Ü"));
  text.replace(F("ö"), F("Ö"));
  return text;
}

String CleanTextOLED(String text)
{
  text.replace(F("Ş"), F("\xC2\xB6"));
  text.replace(F("ş"), F("\xC2\xBE"));
  text.replace(F("İ"), F("\xC2\xA6"));
  text.replace(F("ı"), F("\xC2\xBC"));
  text.replace(F("ğ"), F("\xC2\xA7"));
  text.replace(F("Ğ"), F("\xC2\xAC"));
  return text;
}

String getMoonPhasesIcon(uint8_t icon)
{
  // New Moon
  if (icon == 0)
  {
    return "1";
  }
  // Waxing Crescent
  if (icon == 1)
  {
    return "d";
  }
  // First Quarter
  if (icon == 2)
  {
    return "g";
  }
  // Waxing Gibbous
  if (icon == 3)
  {
    return "j";
  }
  // Full Moon
  if (icon == 4)
  {
    return "0";
  }
  // Waning Gibbous
  if (icon == 5)
  {
    return "q";
  }
  // Third Quarter
  if (icon == 6)
  {
    return "t";
  }
  // Waning Crescent
  if (icon == 7)
  {
    return "w";
  }
  // Nothing matched: N/A
  return ")";
}

uint8_t getUVindexText(uint8_t index)
{
  // Low
  if (index > 0 && index < 3)
  {
    return 0;
  }
  // Moderate
  if (index > 2 && index < 6)
  {
    return 1;
  }
  // High
  if (index > 5 && index < 8)
  {
    return 2;
  }
  // Very High
  if (index > 7 && index < 11)
  {
    return 3;
  }
  // Extreme
  if (index > 11)
  {
    return 4;
  }
  // Nothing matched: N/A
  return 0;
}

void setReadyForWeatherUpdate()
{
  Serial.println(getTranslationStr(TR_LOGREADYFORUPDATE).c_str());
  readyForWeatherUpdate = true;
}

void enableDisplay(bool enable)
{
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);

  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min);
  String currentTime = String(buff);

  displayOn = enable;
  if (enable)
  {
    display.displayOn();
    updateData(&display);
    Serial.println(getTranslationStr(TR_LOGSCREENOPENED).c_str() + currentTime);
  }
  else
  {
    display.displayOff();
    Serial.println(getTranslationStr(TR_LOGSCREENCLOSED).c_str() + currentTime);
  }
}

void checkDisplay()
{
  if (timeDisplayTurnsOn == "" || timeDisplayTurnsOff == "")
  {
    return;
  }

  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);

  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min);
  String currentTime = String(buff);

  if (currentTime == timeDisplayTurnsOn && !displayOn)
  {
    Serial.println(getTranslationStr(TR_LOGSCREENOPENTIME).c_str() + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16_TR);
    display.drawStringMaxWidth(64, 10, 128, CleanTextOLED(getTranslationStr(TR_NIGHTMODEFINISH)));
    display.display();
    delay(1000);
    enableDisplay(true);
  }

  if (currentTime == timeDisplayTurnsOff && displayOn)
  {
    Serial.println(getTranslationStr(TR_LOGSCREENCLOSETIME).c_str() + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16_TR);
    display.drawStringMaxWidth(64, 10, 128, CleanTextOLED(getTranslationStr(TR_NIGHTMODESTART)));
    display.display();
    delay(1000);
    enableDisplay(false);
  }
}

void loadDeviceSettings()
{
  if (LittleFS.exists(configName) == false)
  {
    Serial.println(configName);
    Serial.println(getTranslationStr(TR_LOGFILENOTFOUND).c_str());
    Serial.println();
    saveDeviceSettings();
    return;
  }

  Serial.println(configName);

  File f = LittleFS.open(configName, "r");
  while (f.available())
  {
    String line = f.readStringUntil('\n');
    line.trim();
    int idx;
    if (line.length() == 0)
    {
      continue;
    }

    int sep = line.indexOf('=');
    if (sep == -1)
    {
      continue;
    }

    // print all lines in file
    if (1)
    {
      idx = line.indexOf("Key");
      if (idx < 0)
      {
        idx = line.indexOf("KEY");
      }
      if (idx < 0)
      {
        idx = line.indexOf("Pass");
      }
      if (idx > 0)
      {
        idx = line.indexOf("=");
      }
      if ((idx > 0) && (line.substring(idx + 1).length() > 0))
      {
        // do not print keys or passwords
        Serial.print(line.substring(0, idx + 1));
        Serial.println("***");
      }
      else
      {
        Serial.println(line);
      }
    }

    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);

    if (key == "currentLang")
    {
      currentLang = value;
    }
    if (key == "Display_TZ_NAME")
    {
      Display_TZ_NAME = value;
    }
    if (key == "Display_TZ_POSIX")
    {
      Display_TZ_POSIX = value;
    }
    if (key == "UPDATE_INTERVAL")
    {
      UPDATE_INTERVAL = value.toInt();
    }
    if (key == "OPEN_WEATHER_MAP_LOCATION_LAT")
    {
      OPEN_WEATHER_MAP_LOCATION_LAT = value.toFloat();
    }
    if (key == "OPEN_WEATHER_MAP_LOCATION_LON")
    {
      OPEN_WEATHER_MAP_LOCATION_LON = value.toFloat();
    }
    if (key == "Display_City_Name")
    {
      Display_City_Name = value;
    }
    if (key == "Display_Country_Name")
    {
      Display_Country_Name = value;
    }
    if (key == "IS_METRIC")
    {
      IS_METRIC = (value == "1" || value == "true");
    }
    if (key == "themeIsDark")
    {
      themeIsDark = (value == "1" || value == "true");
    }
  }
  f.close();
  Serial.println();
}

void saveDeviceSettings()
{
  File f = LittleFS.open(configName, "w");
  if (!f)
  {
    Serial.println(getTranslationStr(TR_LOGFILEOPENFAIL).c_str());
  }
  else
  {
    Serial.println(getTranslationStr(TR_LOGSAVINGSETTINGS).c_str());
    f.println(F("currentLang=") + currentLang);
    f.println(F("Display_TZ_NAME=") + Display_TZ_NAME);
    f.println(F("Display_TZ_POSIX=") + Display_TZ_POSIX);
    f.println(F("UPDATE_INTERVAL=") + String(UPDATE_INTERVAL));
    f.println(F("OPEN_WEATHER_MAP_LOCATION_LAT=") + String(OPEN_WEATHER_MAP_LOCATION_LAT, 7));
    f.println(F("OPEN_WEATHER_MAP_LOCATION_LON=") + String(OPEN_WEATHER_MAP_LOCATION_LON, 7));
    f.println(F("Display_City_Name=") + Display_City_Name);
    f.println(F("Display_Country_Name=") + Display_Country_Name);
    f.println(F("IS_METRIC=") + String(IS_METRIC));
    f.println(F("themeIsDark=") + String(themeIsDark));
  }
  f.close();

  loadDeviceSettings();
}

void loadWorldClockSettings()
{
  if (LittleFS.exists(worldClockConfig) == false)
  {
    Serial.println(worldClockConfig);
    Serial.println(getTranslationStr(TR_LOGFILENOTFOUND).c_str());
    Serial.println();
    saveWorldClockSettings();
    return;
  }

  Serial.println(worldClockConfig);

  File f = LittleFS.open(worldClockConfig, "r");
  while (f.available())
  {
    String line = f.readStringUntil('\n');
    line.trim();
    int idx;
    if (line.length() == 0)
    {
      continue;
    }

    int sep = line.indexOf('=');
    if (sep == -1)
    {
      continue;
    }

    // print all lines in file
    if (1)
    {
      idx = line.indexOf("Key");
      if (idx < 0)
      {
        idx = line.indexOf("KEY");
      }
      if (idx < 0)
      {
        idx = line.indexOf("Pass");
      }
      if (idx > 0)
      {
        idx = line.indexOf("=");
      }
      if ((idx > 0) && (line.substring(idx + 1).length() > 0))
      {
        // do not print keys or passwords
        Serial.print(line.substring(0, idx + 1));
        Serial.println("***");
      }
      else
      {
        Serial.println(line);
      }
    }

    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);

    if (key == "World_Clock1_LAT")
    {
      World_Clock1_LAT = value.toFloat();
    }
    if (key == "World_Clock1_LON")
    {
      World_Clock1_LON = value.toFloat();
    }
    if (key == "World_Clock1_City_Name")
    {
      World_Clock1_City_Name = value;
    }
    if (key == "World_Clock1_Country_Name")
    {
      World_Clock1_Country_Name = value;
    }
    if (key == "World_Clock2_LAT")
    {
      World_Clock2_LAT = value.toFloat();
    }
    if (key == "World_Clock2_LON")
    {
      World_Clock2_LON = value.toFloat();
    }
    if (key == "World_Clock2_City_Name")
    {
      World_Clock2_City_Name = value;
    }
    if (key == "World_Clock2_Country_Name")
    {
      World_Clock2_Country_Name = value;
    }
    if (key == "World_Clock3_LAT")
    {
      World_Clock3_LAT = value.toFloat();
    }
    if (key == "World_Clock3_LON")
    {
      World_Clock3_LON = value.toFloat();
    }
    if (key == "World_Clock3_City_Name")
    {
      World_Clock3_City_Name = value;
    }
    if (key == "World_Clock3_Country_Name")
    {
      World_Clock3_Country_Name = value;
    }
  }
  f.close();
  Serial.println();
}

void saveWorldClockSettings()
{
  File f = LittleFS.open(worldClockConfig, "w");
  if (!f)
  {
    Serial.println(getTranslationStr(TR_LOGFILEOPENFAIL).c_str());
  }
  else
  {
    Serial.println(getTranslationStr(TR_LOGSAVINGSETTINGS).c_str());
    f.println(F("World_Clock1_LAT=") + String(World_Clock1_LAT, 7));
    f.println(F("World_Clock1_LON=") + String(World_Clock1_LON, 7));
    f.println(F("World_Clock1_City_Name=") + World_Clock1_City_Name);
    f.println(F("World_Clock1_Country_Name=") + World_Clock1_Country_Name);
    f.println(F("World_Clock2_LAT=") + String(World_Clock2_LAT, 7));
    f.println(F("World_Clock2_LON=") + String(World_Clock2_LON, 7));
    f.println(F("World_Clock2_City_Name=") + World_Clock2_City_Name);
    f.println(F("World_Clock2_Country_Name=") + World_Clock2_Country_Name);
    f.println(F("World_Clock3_LAT=") + String(World_Clock3_LAT, 7));
    f.println(F("World_Clock3_LON=") + String(World_Clock3_LON, 7));
    f.println(F("World_Clock3_City_Name=") + World_Clock3_City_Name);
    f.println(F("World_Clock3_Country_Name=") + World_Clock3_Country_Name);
  }
  f.close();

  loadWorldClockSettings();
}

void loadSecuritySettings()
{
  if (LittleFS.exists(securityConfig) == false)
  {
    Serial.println(securityConfig);
    Serial.println(getTranslationStr(TR_LOGFILENOTFOUND).c_str());
    Serial.println();
    saveSecuritySettings();
    return;
  }

  Serial.println(securityConfig);

  File f = LittleFS.open(securityConfig, "r");
  while (f.available())
  {
    String line = f.readStringUntil('\n');
    line.trim();
    int idx;
    if (line.length() == 0)
    {
      continue;
    }

    int sep = line.indexOf('=');
    if (sep == -1)
    {
      continue;
    }

    // print all lines in file
    if (1)
    {
      idx = line.indexOf("Key");
      if (idx < 0)
      {
        idx = line.indexOf("KEY");
      }
      if (idx < 0)
      {
        idx = line.indexOf("Pass");
      }
      if (idx > 0)
      {
        idx = line.indexOf("=");
      }
      if ((idx > 0) && (line.substring(idx + 1).length() > 0))
      {
        // do not print keys or passwords
        Serial.print(line.substring(0, idx + 1));
        Serial.println("***");
      }
      else
      {
        Serial.println(line);
      }
    }

    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);

    if (key == "SysUser")
    {
      SysUser = value;
    }
    if (key == "SysPass")
    {
      SysPass = value;
    }
  }
  f.close();
  Serial.println();
}

void saveSecuritySettings()
{
  File f = LittleFS.open(securityConfig, "w");
  if (!f)
  {
    Serial.println(getTranslationStr(TR_LOGFILEOPENFAIL).c_str());
  }
  else
  {
    Serial.println(getTranslationStr(TR_LOGSAVINGSETTINGS).c_str());
    f.println(F("SysUser=") + SysUser);
    f.println(F("SysPass=") + SysPass);
  }
  f.close();

  loadSecuritySettings();
}

void loadDisplaySettings()
{
  if (LittleFS.exists(displayConfig) == false)
  {
    Serial.println(displayConfig);
    Serial.println(getTranslationStr(TR_LOGFILENOTFOUND).c_str());
    Serial.println();
    saveDisplaySettings();
    return;
  }

  Serial.println(displayConfig);

  File f = LittleFS.open(displayConfig, "r");
  while (f.available())
  {
    String line = f.readStringUntil('\n');
    line.trim();
    int idx;
    if (line.length() == 0)
    {
      continue;
    }

    int sep = line.indexOf('=');
    if (sep == -1)
    {
      continue;
    }

    // print all lines in file
    if (1)
    {
      idx = line.indexOf("Key");
      if (idx < 0)
      {
        idx = line.indexOf("KEY");
      }
      if (idx < 0)
      {
        idx = line.indexOf("Pass");
      }
      if (idx > 0)
      {
        idx = line.indexOf("=");
      }
      if ((idx > 0) && (line.substring(idx + 1).length() > 0))
      {
        // do not print keys or passwords
        Serial.print(line.substring(0, idx + 1));
        Serial.println("***");
      }
      else
      {
        Serial.println(line);
      }
    }

    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);

    if (key == "timeDisplayTurnsOn")
    {
      timeDisplayTurnsOn = value;
    }
    if (key == "timeDisplayTurnsOff")
    {
      timeDisplayTurnsOff = value;
    }
    if (key == "Disp_Contrast")
    {
      Disp_Contrast = value.toInt();
    }
    if (key == "INVERT_DISPLAY")
    {
      INVERT_DISPLAY = (value == "1" || value == "true");
    }
    if (key == "IS_24HOUR")
    {
      IS_24HOUR = (value == "1" || value == "true");
    }
    if (key == "SHOW_MNAME")
    {
      SHOW_MNAME = (value == "1" || value == "true");
    }
  }
  f.close();
  Serial.println();
}

void saveDisplaySettings()
{
  File f = LittleFS.open(displayConfig, "w");
  if (!f)
  {
    Serial.println(getTranslationStr(TR_LOGFILEOPENFAIL).c_str());
  }
  else
  {
    Serial.println(getTranslationStr(TR_LOGSAVINGSETTINGS).c_str());
    f.println(F("IS_24HOUR=") + String(IS_24HOUR));
    f.println(F("SHOW_MNAME=") + String(SHOW_MNAME));
    f.println(F("Disp_Contrast=") + String(Disp_Contrast));
    f.println(F("timeDisplayTurnsOn=") + timeDisplayTurnsOn);
    f.println(F("timeDisplayTurnsOff=") + timeDisplayTurnsOff);
    f.println(F("INVERT_DISPLAY=") + String(INVERT_DISPLAY));
  }
  f.close();

  loadDisplaySettings();
}

void loadApiKeySettings()
{
  if (LittleFS.exists(ApiKeyConfig) == false)
  {
    Serial.println(ApiKeyConfig);
    Serial.println(getTranslationStr(TR_LOGFILENOTFOUND).c_str());
    Serial.println();
    saveApiKeySettings();
    return;
  }

  Serial.println(ApiKeyConfig);

  File f = LittleFS.open(ApiKeyConfig, "r");
  while (f.available())
  {
    String line = f.readStringUntil('\n');
    line.trim();
    int idx;
    if (line.length() == 0)
    {
      continue;
    }

    int sep = line.indexOf('=');
    if (sep == -1)
    {
      continue;
    }

    // print all lines in file
    if (1)
    {
      idx = line.indexOf("Key");
      if (idx < 0)
      {
        idx = line.indexOf("KEY");
      }
      if (idx < 0)
      {
        idx = line.indexOf("Pass");
      }
      if (idx > 0)
      {
        idx = line.indexOf("=");
      }
      if ((idx > 0) && (line.substring(idx + 1).length() > 0))
      {
        // do not print keys or passwords
        Serial.print(line.substring(0, idx + 1));
        Serial.println("***");
      }
      else
      {
        Serial.println(line);
      }
    }

    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);

    if (key == "OPEN_WEATHER_MAP_KEY")
    {
      OPEN_WEATHER_MAP_KEY = value;
    }
    if (key == "OPEN_CAGE_KEY")
    {
      OPEN_CAGE_KEY = value;
    }
    if (key == "TimeZoneDB_KEY")
    {
      TimeZoneDB_KEY = value;
    }
  }
  f.close();
  Serial.println();
}

void saveApiKeySettings()
{
  File f = LittleFS.open(ApiKeyConfig, "w");
  if (!f)
  {
    Serial.println(getTranslationStr(TR_LOGFILEOPENFAIL).c_str());
  }
  else
  {
    Serial.println(getTranslationStr(TR_LOGSAVINGSETTINGS).c_str());
    f.println(F("OPEN_WEATHER_MAP_KEY=") + OPEN_WEATHER_MAP_KEY);
    f.println(F("OPEN_CAGE_KEY=") + OPEN_CAGE_KEY);
    f.println(F("TimeZoneDB_KEY=") + TimeZoneDB_KEY);
  }
  f.close();

  loadApiKeySettings();
}

void applyLanguage(String lang)
{
  Serial.println();
  language_id = getLanguageIdFromCode(lang.c_str());
  setCurrentLanguageId(language_id);
  Serial.printf_P(PSTR("currentLang: %s (%s)\n"), getLanguageCode(language_id), getLanguageName(language_id));
}

void removeAllConfigs()
{
  LittleFS.remove(configName);
  LittleFS.remove(securityConfig);
  LittleFS.remove(displayConfig);
  LittleFS.remove(ApiKeyConfig);
  LittleFS.remove(worldClockConfig);
}

void loadAllConfigs()
{
  loadApiKeySettings();
  loadDeviceSettings();
  loadDisplaySettings();
  loadSecuritySettings();
  loadWorldClockSettings();
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println(getTranslationStr(TR_LOGENTERCONFIGMODE).c_str());
  Serial.println(WiFi.softAPIP());

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10_TR);
  display.drawString(64, 0, "WiFiManager");
  display.drawString(64, 10, CleanTextOLED(getTranslationStr(TR_LOGCONNECTTOAP)));
  display.setFont(ArialMT_Plain_16_TR);
  display.drawString(64, 26, myWiFiManager->getConfigPortalSSID());
  display.setFont(ArialMT_Plain_10_TR);
  display.drawString(64, 46, CleanTextOLED(getTranslationStr(TR_LOGWIFISETUP)));
  display.display();

  Serial.println(F("WiFiManager"));
  Serial.println(getTranslationStr(TR_LOGCONNECTTOAP).c_str());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println(getTranslationStr(TR_LOGWIFISETUP).c_str());
}

void redirectHome()
{
  server.sendHeader(F("Location"), "/");
  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.send(303);
  delay(1000);
}

void handleDevicePage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Device_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%DEVICESETTINGS%"), getTranslationStr(TR_HTMLDEVICESETTINGS));
  html.replace(F("%CITYTEXT%"), getTranslationStr(TR_HTMLCITYNAME));
  html.replace(F("%COUNTRYTEXT%"), getTranslationStr(TR_HTMLCOUNTRYNAME));
  html.replace(F("%SEARCHCITY%"), getTranslationStr(TR_HTMLFINDLOCATION));
  html.replace(F("%LATTEXT%"), getTranslationStr(TR_HTMLLATITUDE));
  html.replace(F("%LONTEXT%"), getTranslationStr(TR_HTMLLONGITUDE));
  html.replace(F("%TZNAMETEXT%"), getTranslationStr(TR_HTMLTIMEZONE));
  html.replace(F("%UNITTEXT%"), getTranslationStr(TR_HTMLUNITSELECT));
  html.replace(F("%METRICTEXT%"), getTranslationStr(TR_HTMLUNITMETRIC));
  html.replace(F("%IMPERIALTEXT%"), getTranslationStr(TR_HTMLUNITIMPERIAL));
  html.replace(F("%INTERVALTEXT%"), getTranslationStr(TR_HTMLUPDATEINTERVAL));
  html.replace(F("%SAVETEXT%"), getTranslationStr(TR_HTMLSAVE));
  html.replace(F("%HOMETEXT%"), getTranslationStr(TR_HTMLHOME));
  html.replace(F("%GEOALERT%"), getTranslationStr(TR_HTMLMISSINGINPUT));
  html.replace(F("%GEOALERT2%"), getTranslationStr(TR_HTMLLOCATIONNOTFOUND));
  html.replace(F("%GEOALERT3%"), getTranslationStr(TR_HTMLAPIFAIL));
  html.replace(F("%FORMSENDING%"), getTranslationStr(TR_HTMLSAVING));
  html.replace(F("%APIKEYSETTINGS%"), getTranslationStr(TR_HTMLAPISETTINGS));
  html.replace(F("%DISPLAYSETTINGS%"), getTranslationStr(TR_HTMLDISPLAYSETTINGS));
  html.replace(F("%WORLDCLOCKSETTINGS%"), getTranslationStr(TR_HTMLWORLDCLOCKSETTINGS));
  html.replace(F("%SECURITYSETTINGS%"), getTranslationStr(TR_HTMLSECURITYSETTINGS));
  html.replace(F("%ABOUTTEXT%"), getTranslationStr(TR_HTMLABOUT));
  html.replace(F("%OTATEXT%"), getTranslationStr(TR_HTMLOTAUPDATE));
  html.replace(F("%SHOWMAPTEXT%"), getTranslationStr(TR_HTMLSHOWONMAP));
  html.replace(F("%SYSLANG%"), getTranslationStr(TR_HTMLSYSTEMLANG));
  html.replace(F("%QUERYSELECT%"), getTranslationStr(TR_HTMLQUERYOPTION));
  html.replace(F("%QUERYADDRESS%"), getTranslationStr(TR_HTMLQUERYADDRESS));
  html.replace(F("%QUERYLATLON%"), getTranslationStr(TR_HTMLQUERYLATLON));
  html.replace(F("%QUERYADDRESSPLACE%"), getTranslationStr(TR_HTMLQUERYADDRESSPLACE));
  html.replace(F("%QUERYLATPLACE%"), getTranslationStr(TR_HTMLQUERYLATPLACE));
  html.replace(F("%QUERYLONPLACE%"), getTranslationStr(TR_HTMLQUERYLONPLACE));
  html.replace(F("%ADDRESSALERT%"), getTranslationStr(TR_HTMLADDRESSERROR));
  html.replace(F("%ADDRESSALERT2%"), getTranslationStr(TR_HTMLADDRESSNOTFOUND));
  html.replace(F("%ADDRESSALERT3%"), getTranslationStr(TR_HTMLADDRESSNOVALID));
  html.replace(F("%SEARCHINFOTEXT%"), getTranslationStr(TR_HTMLSEARCHINFO));
  html.replace(F("%TZNAME%"), Display_TZ_NAME);
  html.replace(F("%LAT%"), String(OPEN_WEATHER_MAP_LOCATION_LAT, 7));
  html.replace(F("%LON%"), String(OPEN_WEATHER_MAP_LOCATION_LON, 7));
  html.replace(F("%CITY%"), Display_City_Name);
  html.replace(F("%COUNTRY%"), Display_Country_Name);
  html.replace(F("%GEOAPIKEY%"), OPEN_CAGE_KEY);
  html.replace(F("%METRIC%"), IS_METRIC ? "selected" : "");
  html.replace(F("%IMPERIAL%"), !IS_METRIC ? "selected" : "");
  html.replace(F("%UP10%"), (UPDATE_INTERVAL == 10) ? "selected" : "");
  html.replace(F("%UP15%"), (UPDATE_INTERVAL == 15) ? "selected" : "");
  html.replace(F("%UP20%"), (UPDATE_INTERVAL == 20) ? "selected" : "");
  html.replace(F("%UP30%"), (UPDATE_INTERVAL == 30) ? "selected" : "");
  html.replace(F("%UP60%"), (UPDATE_INTERVAL == 60) ? "selected" : "");
  html.replace(F("%THEMETYPE%"), themeIsDark ? "dark" : "light");
  String langOptions;
  for (int l = 0; l < NUM_LANGUAGES; l++)
  {
    langOptions += F("<option value='");
    langOptions += getLanguageCode(static_cast<lang_t>(l));
    if (l == (int)language_id)
    {
      langOptions += F("' selected>");
    }
    else
    {
      langOptions += F("'>");
    }
    langOptions += getLanguageName(static_cast<lang_t>(l));
    langOptions += F("</option>");
  }
  html.replace(F("%LANG_OPT%"), langOptions);
  server.sendContent(html);

  server.client().stop();
}

void handleSaveDevice()
{

  if (server.hasArg(F("city")))
  {
    Display_City_Name = server.arg(F("city"));
  }

  if (server.hasArg(F("country")))
  {
    Display_Country_Name = server.arg(F("country"));
  }

  if (server.hasArg(F("latitude")))
  {
    OPEN_WEATHER_MAP_LOCATION_LAT = server.arg(F("latitude")).toFloat();
  }

  if (server.hasArg(F("longitude")))
  {
    OPEN_WEATHER_MAP_LOCATION_LON = server.arg(F("longitude")).toFloat();
  }

  if (server.hasArg(F("updateInterval")))
  {
    String val = server.arg(F("updateInterval"));

    if (val.startsWith("up"))
    {
      uint8_t interval = val.substring(2).toInt(); // "up10" → "10" → int(10)
      UPDATE_INTERVAL = interval;
    }
  }

  if (server.hasArg(F("units")))
  {
    String val = server.arg(F("units"));
    IS_METRIC = (val == "metric") ? true : false;
  }

  if (server.hasArg(F("timezoneName")))
  {
    Display_TZ_NAME = server.arg(F("timezoneName"));
  }

  if (server.hasArg(F("sysLang")))
  {
    currentLang = server.arg(F("sysLang"));
    applyLanguage(currentLang);
  }

  if (server.hasArg(F("themeToggle")))
  {
    String val = server.arg(F("themeToggle"));
    themeIsDark = (val == "on") ? false : true;
  }

  if (!(server.hasArg(F("themeToggle"))))
  {
    themeIsDark = true;
  }

  UPDATE_INTERVAL_SECS = UPDATE_INTERVAL * 60;

  Serial.println();
  Serial.println(getTranslationStr(TR_LOGDEVICESETTINGSUPDATED).c_str());
  Serial.printf_P(PSTR("currentLang: %s\n"), currentLang.c_str());
  Serial.printf_P(PSTR("Display_TZ_NAME: %s\n"), Display_TZ_NAME.c_str());
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_LOCATION_LAT: %f\n"), OPEN_WEATHER_MAP_LOCATION_LAT);
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_LOCATION_LON: %f\n"), OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.printf_P(PSTR("Display_City_Name: %s\n"), Display_City_Name.c_str());
  Serial.printf_P(PSTR("Display_Country_Name: %s\n"), Display_Country_Name.c_str());
  Serial.printf_P(PSTR("UPDATE_INTERVAL: %d\n"), UPDATE_INTERVAL);
  Serial.printf_P(PSTR("IS_METRIC: %s\n"), IS_METRIC ? "true" : "false");
  Serial.printf_P(PSTR("themeIsDark: %s\n"), themeIsDark ? "true" : "false");
  Serial.println();

  saveDeviceSettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Settings_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%DEVICESETTINGS%"), getTranslationStr(TR_HTMLDEVICESETTINGS));
  html.replace(F("%DEVICETEXT%"), getTranslationStr(TR_HTMLDEVICESAVED));
  server.sendContent(html);

  checkDisplay();
  updateData(&display);
}

void handleWorldClockPage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(World_Clock_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%WORLDCLOCKSETTINGS%"), getTranslationStr(TR_HTMLWORLDCLOCKSETTINGS));
  html.replace(F("%CITYTEXT%"), getTranslationStr(TR_HTMLCITYNAME));
  html.replace(F("%COUNTRYTEXT%"), getTranslationStr(TR_HTMLCOUNTRYNAME));
  html.replace(F("%SEARCHCITY%"), getTranslationStr(TR_HTMLFINDLOCATION));
  html.replace(F("%LATTEXT%"), getTranslationStr(TR_HTMLLATITUDE));
  html.replace(F("%LONTEXT%"), getTranslationStr(TR_HTMLLONGITUDE));
  html.replace(F("%CITY_1_TEXT%"), getTranslationStr(TR_HTMLWORLDCLOCKCITY1));
  html.replace(F("%CITY_2_TEXT%"), getTranslationStr(TR_HTMLWORLDCLOCKCITY2));
  html.replace(F("%CITY_3_TEXT%"), getTranslationStr(TR_HTMLWORLDCLOCKCITY3));
  html.replace(F("%TZNAMETEXT%"), getTranslationStr(TR_HTMLTIMEZONE));
  html.replace(F("%SAVETEXT%"), getTranslationStr(TR_HTMLSAVE));
  html.replace(F("%HOMETEXT%"), getTranslationStr(TR_HTMLHOME));
  html.replace(F("%GEOALERT%"), getTranslationStr(TR_HTMLMISSINGINPUT));
  html.replace(F("%GEOALERT2%"), getTranslationStr(TR_HTMLLOCATIONNOTFOUND));
  html.replace(F("%GEOALERT3%"), getTranslationStr(TR_HTMLAPIFAIL));
  html.replace(F("%FORMSENDING%"), getTranslationStr(TR_HTMLSAVING));
  html.replace(F("%APIKEYSETTINGS%"), getTranslationStr(TR_HTMLAPISETTINGS));
  html.replace(F("%DEVICESETTINGS%"), getTranslationStr(TR_HTMLDEVICESETTINGS));
  html.replace(F("%DISPLAYSETTINGS%"), getTranslationStr(TR_HTMLDISPLAYSETTINGS));
  html.replace(F("%SECURITYSETTINGS%"), getTranslationStr(TR_HTMLSECURITYSETTINGS));
  html.replace(F("%ABOUTTEXT%"), getTranslationStr(TR_HTMLABOUT));
  html.replace(F("%OTATEXT%"), getTranslationStr(TR_HTMLOTAUPDATE));
  html.replace(F("%SHOWMAPTEXT%"), getTranslationStr(TR_HTMLSHOWONMAP));
  html.replace(F("%QUERYSELECT%"), getTranslationStr(TR_HTMLQUERYOPTION));
  html.replace(F("%QUERYADDRESS%"), getTranslationStr(TR_HTMLQUERYADDRESS));
  html.replace(F("%QUERYLATLON%"), getTranslationStr(TR_HTMLQUERYLATLON));
  html.replace(F("%QUERYADDRESSPLACE%"), getTranslationStr(TR_HTMLQUERYADDRESSPLACE));
  html.replace(F("%QUERYLATPLACE%"), getTranslationStr(TR_HTMLQUERYLATPLACE));
  html.replace(F("%QUERYLONPLACE%"), getTranslationStr(TR_HTMLQUERYLONPLACE));
  html.replace(F("%LOCEDIT%"), getTranslationStr(TR_HTMLLOCATIONEDITOR));
  html.replace(F("%SETTINGSTEXT%"), getTranslationStr(TR_HTMLCITYSETTINGS));
  html.replace(F("%ADDRESSALERT%"), getTranslationStr(TR_HTMLADDRESSERROR));
  html.replace(F("%ADDRESSALERT2%"), getTranslationStr(TR_HTMLADDRESSNOTFOUND));
  html.replace(F("%ADDRESSALERT3%"), getTranslationStr(TR_HTMLADDRESSNOVALID));
  html.replace(F("%TABALERT%"), getTranslationStr(TR_HTMLNOACTIVETAB));
  html.replace(F("%SEARCHINFOTEXT%"), getTranslationStr(TR_HTMLSEARCHINFO));
  html.replace(F("%GEOAPIKEY%"), OPEN_CAGE_KEY);
  html.replace(F("%LAT1%"), String(World_Clock1_LAT, 7));
  html.replace(F("%LON1%"), String(World_Clock1_LON, 7));
  html.replace(F("%CITY1%"), World_Clock1_City_Name);
  html.replace(F("%COUNTRY1%"), World_Clock1_Country_Name);
  html.replace(F("%LAT2%"), String(World_Clock2_LAT, 7));
  html.replace(F("%LON2%"), String(World_Clock2_LON, 7));
  html.replace(F("%CITY2%"), World_Clock2_City_Name);
  html.replace(F("%COUNTRY2%"), World_Clock2_Country_Name);
  html.replace(F("%LAT3%"), String(World_Clock3_LAT, 7));
  html.replace(F("%LON3%"), String(World_Clock3_LON, 7));
  html.replace(F("%CITY3%"), World_Clock3_City_Name);
  html.replace(F("%COUNTRY3%"), World_Clock3_Country_Name);
  html.replace(F("%THEMETYPE%"), themeIsDark ? "dark" : "light");
  server.sendContent(html);

  server.client().stop();
}

void handleSaveWorldClock()
{

  if (server.hasArg(F("city1")))
  {
    World_Clock1_City_Name = server.arg(F("city1"));
  }

  if (server.hasArg(F("country1")))
  {
    World_Clock1_Country_Name = server.arg(F("country1"));
  }

  if (server.hasArg(F("latitude1")))
  {
    World_Clock1_LAT = server.arg(F("latitude1")).toFloat();
  }

  if (server.hasArg(F("longitude1")))
  {
    World_Clock1_LON = server.arg(F("longitude1")).toFloat();
  }

  if (server.hasArg(F("city2")))
  {
    World_Clock2_City_Name = server.arg(F("city2"));
  }

  if (server.hasArg(F("country2")))
  {
    World_Clock2_Country_Name = server.arg(F("country2"));
  }

  if (server.hasArg(F("latitude2")))
  {
    World_Clock2_LAT = server.arg(F("latitude2")).toFloat();
  }

  if (server.hasArg(F("longitude2")))
  {
    World_Clock2_LON = server.arg(F("longitude2")).toFloat();
  }

  if (server.hasArg(F("city3")))
  {
    World_Clock3_City_Name = server.arg(F("city3"));
  }

  if (server.hasArg(F("country3")))
  {
    World_Clock3_Country_Name = server.arg(F("country3"));
  }

  if (server.hasArg(F("latitude3")))
  {
    World_Clock3_LAT = server.arg(F("latitude3")).toFloat();
  }

  if (server.hasArg(F("longitude3")))
  {
    World_Clock3_LON = server.arg(F("longitude3")).toFloat();
  }

  Serial.println();
  Serial.println(getTranslationStr(TR_LOGWORLDCLOCKSETTINGSUPDATED).c_str());
  Serial.printf_P(PSTR("World_Clock1_LAT: %f\n"), World_Clock1_LAT);
  Serial.printf_P(PSTR("World_Clock1_LON: %f\n"), World_Clock1_LON);
  Serial.printf_P(PSTR("World_Clock1_City_Name: %s\n"), World_Clock1_City_Name.c_str());
  Serial.printf_P(PSTR("World_Clock1_Country_Name: %s\n"), World_Clock1_Country_Name.c_str());
  Serial.printf_P(PSTR("World_Clock2_LAT: %f\n"), World_Clock2_LAT);
  Serial.printf_P(PSTR("World_Clock2_LON: %f\n"), World_Clock2_LON);
  Serial.printf_P(PSTR("World_Clock2_City_Name: %s\n"), World_Clock2_City_Name.c_str());
  Serial.printf_P(PSTR("World_Clock2_Country_Name: %s\n"), World_Clock2_Country_Name.c_str());
  Serial.printf_P(PSTR("World_Clock3_LAT: %f\n"), World_Clock3_LAT);
  Serial.printf_P(PSTR("World_Clock3_LON: %f\n"), World_Clock3_LON);
  Serial.printf_P(PSTR("World_Clock3_City_Name: %s\n"), World_Clock3_City_Name.c_str());
  Serial.printf_P(PSTR("World_Clock3_Country_Name: %s\n"), World_Clock3_Country_Name.c_str());
  Serial.println();

  saveWorldClockSettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_World_Clock_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%WORLDCLOCKSETTINGS%"), getTranslationStr(TR_HTMLWORLDCLOCKSETTINGS));
  html.replace(F("%WORLDCLOCKTEXT%"), getTranslationStr(TR_HTMLWORLDCLOCKSAVED));
  server.sendContent(html);

  checkDisplay();
  updateData(&display);
}

void handleApiKeyPage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(ApiKey_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%APIKEYSETTINGS%"), getTranslationStr(TR_HTMLAPISETTINGS));
  html.replace(F("%OWMAPIKEYTEXT%"), getTranslationStr(TR_HTMLOWMAPIKEY));
  html.replace(F("%APIPLACE%"), getTranslationStr(TR_HTMLENTERAPIKEY));
  html.replace(F("%SHOWPASS%"), getTranslationStr(TR_HTMLSHOW));
  html.replace(F("%GEOAPIKEYTEXT%"), getTranslationStr(TR_HTMLOCAPIKEY));
  html.replace(F("%SAVETEXT%"), getTranslationStr(TR_HTMLSAVE));
  html.replace(F("%HIDEPASS%"), getTranslationStr(TR_HTMLHIDE));
  html.replace(F("%GETFROMTEXT%"), getTranslationStr(TR_HTMLGETFROM));
  html.replace(F("%HERETEXT%"), getTranslationStr(TR_HTMLHERE));
  html.replace(F("%FORMSENDING%"), getTranslationStr(TR_HTMLSAVING));
  html.replace(F("%HOMETEXT%"), getTranslationStr(TR_HTMLHOME));
  html.replace(F("%DEVICESETTINGS%"), getTranslationStr(TR_HTMLDEVICESETTINGS));
  html.replace(F("%DISPLAYSETTINGS%"), getTranslationStr(TR_HTMLDISPLAYSETTINGS));
  html.replace(F("%WORLDCLOCKSETTINGS%"), getTranslationStr(TR_HTMLWORLDCLOCKSETTINGS));
  html.replace(F("%SECURITYSETTINGS%"), getTranslationStr(TR_HTMLSECURITYSETTINGS));
  html.replace(F("%ABOUTTEXT%"), getTranslationStr(TR_HTMLABOUT));
  html.replace(F("%OTATEXT%"), getTranslationStr(TR_HTMLOTAUPDATE));
  html.replace(F("%TIMEAPIKEYTEXT%"), getTranslationStr(TR_HTMLTIMEAPIKEY));
  html.replace(F("%OWMAPIKEY%"), OPEN_WEATHER_MAP_KEY);
  html.replace(F("%GEOAPIKEY%"), OPEN_CAGE_KEY);
  html.replace(F("%TIMEAPIKEY%"), TimeZoneDB_KEY);
  html.replace(F("%THEMETYPE%"), themeIsDark ? "dark" : "light");
  server.sendContent(html);

  server.client().stop();
}

void handleSaveApiKey()
{

  if (server.hasArg(F("OWMApiKey")))
  {
    OPEN_WEATHER_MAP_KEY = server.arg(F("OWMApiKey"));
  }

  if (server.hasArg(F("geoApiKey")))
  {
    OPEN_CAGE_KEY = server.arg(F("geoApiKey"));
  }

  if (server.hasArg(F("timeApiKey")))
  {
    TimeZoneDB_KEY = server.arg(F("timeApiKey"));
  }

  Serial.println();
  Serial.println(getTranslationStr(TR_LOGAPIKEYSUPDATED).c_str());
  Serial.println(F("OPEN_WEATHER_MAP_KEY: ***"));
  Serial.println(F("OPEN_CAGE_KEY: ***"));
  Serial.println(F("TimeZoneDB_KEY: ***"));
  Serial.println();

  saveApiKeySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_ApiKey_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%APIKEYSETTINGS%"), getTranslationStr(TR_HTMLAPISETTINGS));
  html.replace(F("%APIKEYTEXT%"), getTranslationStr(TR_HTMLAPISAVED));
  server.sendContent(html);

  checkDisplay();
  updateData(&display);
}

void handleHomePage()
{

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Home_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%HOMETITLE%"), getTranslationStr(TR_HTMLTITLE));
  html.replace(F("%DEVICESETTINGS%"), getTranslationStr(TR_HTMLDEVICESETTINGS));
  html.replace(F("%WORLDCLOCKSETTINGS%"), getTranslationStr(TR_HTMLWORLDCLOCKSETTINGS));
  html.replace(F("%SECURITYSETTINGS%"), getTranslationStr(TR_HTMLSECURITYSETTINGS));
  html.replace(F("%DEFAULTSETTINGS%"), getTranslationStr(TR_HTMLDEFAULTRESET));
  html.replace(F("%DEFAULTALERT%"), getTranslationStr(TR_HTMLDEFAULTCONFIRM));
  html.replace(F("%OTATEXT%"), getTranslationStr(TR_HTMLOTAUPDATE));
  html.replace(F("%DISPLAYSETTINGS%"), getTranslationStr(TR_HTMLDISPLAYSETTINGS));
  html.replace(F("%APIKEYSETTINGS%"), getTranslationStr(TR_HTMLAPISETTINGS));
  html.replace(F("%ABOUTTEXT%"), getTranslationStr(TR_HTMLABOUT));
  html.replace(F("%UPDATEDATATEXT%"), getTranslationStr(TR_HTMLREFRESHDATA));
  html.replace(F("%VERSION%"), "V" + String(VERSION));
  html.replace(F("%THEMETYPE%"), themeIsDark ? "dark" : "light");
  server.sendContent(html);

  server.client().stop();
}

void handleSecurityPage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Security_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%SECURITYSETTINGS%"), getTranslationStr(TR_HTMLSECURITYSETTINGS));
  html.replace(F("%SHOWPASS%"), getTranslationStr(TR_HTMLSHOW));
  html.replace(F("%SAVETEXT%"), getTranslationStr(TR_HTMLSAVE));
  html.replace(F("%PASSWORDTEXT%"), getTranslationStr(TR_HTMLPASSWORD));
  html.replace(F("%USERTEXT%"), getTranslationStr(TR_HTMLUSERNAME));
  html.replace(F("%USERPLACE%"), getTranslationStr(TR_HTMLENTERUSERNAME));
  html.replace(F("%PASSWORDPLACE2%"), getTranslationStr(TR_HTMLENTERPASSWORD));
  html.replace(F("%HIDEPASS%"), getTranslationStr(TR_HTMLHIDE));
  html.replace(F("%HOMETEXT%"), getTranslationStr(TR_HTMLHOME));
  html.replace(F("%DEVICESETTINGS%"), getTranslationStr(TR_HTMLDEVICESETTINGS));
  html.replace(F("%APIKEYSETTINGS%"), getTranslationStr(TR_HTMLAPISETTINGS));
  html.replace(F("%DISPLAYSETTINGS%"), getTranslationStr(TR_HTMLDISPLAYSETTINGS));
  html.replace(F("%WORLDCLOCKSETTINGS%"), getTranslationStr(TR_HTMLWORLDCLOCKSETTINGS));
  html.replace(F("%ABOUTTEXT%"), getTranslationStr(TR_HTMLABOUT));
  html.replace(F("%OTATEXT%"), getTranslationStr(TR_HTMLOTAUPDATE));
  html.replace(F("%USER%"), SysUser);
  html.replace(F("%PASSWORD%"), SysPass);
  html.replace(F("%THEMETYPE%"), themeIsDark ? "dark" : "light");
  server.sendContent(html);

  server.client().stop();
}

void handleSaveSecurity()
{
  if (server.hasArg(F("user")))
  {
    SysUser = server.arg(F("user"));
  }

  if (server.hasArg(F("password")))
  {
    SysPass = server.arg(F("password"));
  }

  Serial.println(getTranslationStr(TR_LOGSECURITYSETTINGSUPDATED).c_str());
  Serial.printf_P(PSTR("SysUser: %s\n"), SysUser.c_str());
  Serial.println(F("SysPass: ***"));

  saveSecuritySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Security_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%SECURITYSETTINGS%"), getTranslationStr(TR_HTMLSECURITYSETTINGS));
  html.replace(F("%SECURITYTEXT%"), getTranslationStr(TR_HTMLSECURITYSAVED));
  server.sendContent(html);

  checkDisplay();
}

void handleDisplayPage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Display_Settings_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%SAVETEXT%"), getTranslationStr(TR_HTMLSAVE));
  html.replace(F("%TIMEONTEXT%"), getTranslationStr(TR_HTMLONTIME));
  html.replace(F("%TIMEOFFTEXT%"), getTranslationStr(TR_HTMLOFFTIME));
  html.replace(F("%INVDISPTEXT%"), getTranslationStr(TR_HTMLROTATEDISPLAY));
  html.replace(F("%INVDISPYESTEXT%"), getTranslationStr(TR_HTMLYES));
  html.replace(F("%INVDISPNOTEXT%"), getTranslationStr(TR_HTMLNO));
  html.replace(F("%FORMSENDING%"), getTranslationStr(TR_HTMLSAVING));
  html.replace(F("%DISPLAYSETTINGS%"), getTranslationStr(TR_HTMLDISPLAYSETTINGS));
  html.replace(F("%DISPLAYCONTRAST%"), getTranslationStr(TR_HTMLBRIGHTNESS));
  html.replace(F("%HOURTEXT%"), getTranslationStr(TR_HTMLTIMEFORMAT));
  html.replace(F("%HOUR24TEXT%"), getTranslationStr(TR_HTML24H));
  html.replace(F("%HOUR12TEXT%"), getTranslationStr(TR_HTML12H));
  html.replace(F("%MONTHTEXT%"), getTranslationStr(TR_HTMLMONTHFORMAT));
  html.replace(F("%ASNUMBERTEXT%"), getTranslationStr(TR_HTMLMONTHNUMERIC));
  html.replace(F("%ASNAMETEXT%"), getTranslationStr(TR_HTMLMONTH));
  html.replace(F("%TIMEWARNINGTEXT%"), getTranslationStr(TR_HTMLENTERTIME));
  html.replace(F("%HOMETEXT%"), getTranslationStr(TR_HTMLHOME));
  html.replace(F("%APIKEYSETTINGS%"), getTranslationStr(TR_HTMLAPISETTINGS));
  html.replace(F("%DEVICESETTINGS%"), getTranslationStr(TR_HTMLDEVICESETTINGS));
  html.replace(F("%WORLDCLOCKSETTINGS%"), getTranslationStr(TR_HTMLWORLDCLOCKSETTINGS));
  html.replace(F("%SECURITYSETTINGS%"), getTranslationStr(TR_HTMLSECURITYSETTINGS));
  html.replace(F("%ABOUTTEXT%"), getTranslationStr(TR_HTMLABOUT));
  html.replace(F("%OTATEXT%"), getTranslationStr(TR_HTMLOTAUPDATE));
  html.replace(F("%TIMEON%"), timeDisplayTurnsOn);
  html.replace(F("%TIMEOFF%"), timeDisplayTurnsOff);
  html.replace(F("%SETCONTRAST%"), String(Disp_Contrast));
  html.replace(F("%INVDISPYES%"), INVERT_DISPLAY ? "selected" : "");
  html.replace(F("%INVDISPNO%"), !INVERT_DISPLAY ? "selected" : "");
  html.replace(F("%HOUR24%"), IS_24HOUR ? "selected" : "");
  html.replace(F("%HOUR12%"), !IS_24HOUR ? "selected" : "");
  html.replace(F("%ASNUMBER%"), !SHOW_MNAME ? "selected" : "");
  html.replace(F("%ASNAME%"), SHOW_MNAME ? "selected" : "");
  html.replace(F("%THEMETYPE%"), themeIsDark ? "dark" : "light");
  server.sendContent(html);

  server.client().stop();
}

void handleSaveDisplay()
{
  bool flipOld = INVERT_DISPLAY;
  uint8_t conOld = Disp_Contrast;

  if (server.hasArg(F("turnOffTime")))
  {
    timeDisplayTurnsOff = server.arg(F("turnOffTime"));
  }

  if (server.hasArg(F("turnOnTime")))
  {
    timeDisplayTurnsOn = server.arg(F("turnOnTime"));
  }

  if (server.hasArg(F("setConstrast")))
  {
    Disp_Contrast = server.arg(F("setConstrast")).toInt();
  }

  if (server.hasArg(F("hours")))
  {
    String val = server.arg(F("hours"));
    IS_24HOUR = (val == "hour24") ? true : false;
  }

  if (server.hasArg(F("months")))
  {
    String val = server.arg(F("months"));
    SHOW_MNAME = (val == "asName") ? true : false;
  }

  if (server.hasArg(F("invDisp")))
  {
    String val = server.arg(F("invDisp"));
    INVERT_DISPLAY = (val == "yes") ? true : false;
  }

  Serial.println();
  Serial.println(getTranslationStr(TR_LOGDISPLAYSETTINGSUPDATED).c_str());
  Serial.printf_P(PSTR("timeDisplayTurnsOn: %s\n"), timeDisplayTurnsOn.c_str());
  Serial.printf_P(PSTR("timeDisplayTurnsOff: %s\n"), timeDisplayTurnsOff.c_str());
  Serial.printf_P(PSTR("Disp_Contrast: %d\n"), Disp_Contrast);
  Serial.printf_P(PSTR("IS_24HOUR: %s\n"), IS_24HOUR ? "true" : "false");
  Serial.printf_P(PSTR("SHOW_MNAME: %s\n"), SHOW_MNAME ? "true" : "false");
  Serial.printf_P(PSTR("INVERT_DISPLAY: %s\n"), INVERT_DISPLAY ? "true" : "false");
  Serial.println();

  saveDisplaySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Display_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%DISPLAYSETTINGS%"), getTranslationStr(TR_HTMLDISPLAYSETTINGS));
  html.replace(F("%DISPLAYTEXT%"), getTranslationStr(TR_HTMLDISPLAYSAVED));
  server.sendContent(html);

  if (INVERT_DISPLAY != flipOld)
  {
    ui.init();
    if (INVERT_DISPLAY)
    {
      display.flipScreenVertically();
    }
    ui.update();
  }

  if (Disp_Contrast != conOld)
  {
    ui.init();
    display.setContrast(Disp_Contrast);
    ui.update();
  }

  checkDisplay();
}

void handleResetDefaults()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }

  LittleFS.remove(configName);
  LittleFS.remove(securityConfig);
  LittleFS.remove(displayConfig);
  LittleFS.remove(ApiKeyConfig);
  LittleFS.remove(worldClockConfig);

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Reset_Default_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%DEFAULTSETTINGS%"), getTranslationStr(TR_HTMLDEFAULTSETTINGS));
  html.replace(F("%DEFAULTTEXT%"), getTranslationStr(TR_HTMLDEFAULTRESTORED));
  server.sendContent(html);

  server.client().stop();

  delay(2000);
  ESP.restart();
}

void handleUpdateData()
{
  checkDisplay();
  updateData(&display);
  redirectHome();
}

void onOTAStart()
{
  Serial.println(getTranslationStr(TR_LOGOTASTARTED).c_str());
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16_TR);
  display.drawStringMaxWidth(64, 10, 128, CleanTextOLED(getTranslationStr(TR_OTASTART)));
  display.display();
  delay(250);
}

void onOTAProgress(size_t current, size_t final)
{
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000)
  {
    ota_progress_millis = millis();
    Serial.printf_P(PSTR("%s %s: %u bytes, %s: %u bytes\n"), getTranslationStr(TR_LOGOTAPROGRESS).c_str(), getTranslationStr(TR_LOGNOW).c_str(), current, getTranslationStr(TR_LOGFINAL).c_str(), final);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16_TR);
    display.drawStringMaxWidth(64, 10, 128, CleanTextOLED(getTranslationStr(TR_OTAPROGRESS)));
    display.display();
  }
}

void onOTAEnd(bool success)
{
  if (success)
  {
    Serial.println(getTranslationStr(TR_LOGOTASUCCESS).c_str());
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16_TR);
    display.drawStringMaxWidth(64, 10, 128, CleanTextOLED(getTranslationStr(TR_OTAFINISH)));
    display.display();
    delay(250);
    display.displayOff();
  }
  else
  {
    Serial.println(getTranslationStr(TR_LOGOTAERROR).c_str());
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16_TR);
    display.drawStringMaxWidth(64, 10, 128, CleanTextOLED(getTranslationStr(TR_OTAERROR)));
    display.display();
    delay(250);
  }
}

uint8_t get12Hour(uint8_t hour)
{
  if (hour == 0 || hour == 12)
  {
    return 12;
  }
  else if (hour > 12)
  {
    uint8_t tH = hour - 12;
    return tH;
  }
  else
  {
    return hour;
  }
}

String getAMPM(uint8_t hour)
{
  if (hour == 0)
  {
    return "AM";
  }
  else if (hour == 12)
  {
    return "PM";
  }
  else if (hour > 12)
  {
    return "PM";
  }
  else
  {
    return "AM";
  }
}

String getDayName(int weekday)
{
  return findWordInCommaList(getTranslationStr(TR_DAYNAMESHORT), weekday, 7);
}

String getMonthName(int month)
{
  return findWordInCommaList(getTranslationStr(TR_MONTHNAMESHORT), month - 1, 12);
}

String getProgressName(uint8_t index)
{
  return findWordInCommaList(getTranslationStr(TR_PROGRESSES), index, 6);
}

String getAirQulityName(uint8_t index)
{
  return findWordInCommaList(getTranslationStr(TR_AIRQUALITYS), index, 5);
}

String getUVIndexName(uint8_t index)
{
  return findWordInCommaList(getTranslationStr(TR_UVINDEXES), index, 5);
}

String getWindDirectionName(uint8_t index)
{
  return findWordInCommaList(getTranslationStr(TR_WINDDIRECTIONSLONG), index, 8);
}

String getMoonPhaseName(uint8_t index)
{
  return findWordInCommaList(getTranslationStr(TR_MOONPHASES), index, 8);
}