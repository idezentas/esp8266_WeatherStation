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
#include "SunMoonCalc.h"
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"
#include "MoonPhasesFont.h"
#include "WindDirectionImages.h"
#include "WeatherImages.h"
#include "DirectionImages.h"
#include "WebPages.h"
#include "settings.h"

SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi ui(&display);

#define DHTPIN 14

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const uint8_t MAX_FORECASTS = 5;

OpenWeatherMapCurrent currentWeatherClient;
OpenWeatherMapCurrentData currentWeather;

OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
OpenWeatherMapForecast forecastClient;

OpenWeatherMapAir currentAirClient;
OpenWeatherMapAirData currentAir;

OpenMeteoApiCurrent currentMeteoClient;
OpenMeteoApiCurrentData currentMeteo;

SunMoonCalc::Moon moonData;

bool readyForWeatherUpdate = false;

unsigned long timeSinceLastWUpdate = 0;

bool displayOn = true;

String SSID_String = "";

unsigned long ota_progress_millis = 0;

String Display_TZ_POSIX = "<+03>-3";
String Display_TZ_NAME_SHORT = "<+03>";
int8_t Display_GMTOffset = 3; // (utc+) TZ in hours

LanguagePack lang;

ESP8266WebServer server(WEBSERVER_PORT);

void drawProgress(OLEDDisplay *display, int percentage, String label);
void updateData(OLEDDisplay *display);
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeatherHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawForecast(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawForecast2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);
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
void setReadyForWeatherUpdate();
String UpperText(String text);
String CleanText(String text);
int8_t getWifiQuality();
String encodeHtmlString(String msg);
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
void onOTAStart();
void onOTAProgress(size_t current, size_t final);
void onOTAEnd(bool success);
uint8_t get12Hour(uint8_t hour);
String getAMPM(uint8_t hour);
void applyLanguage();

FrameCallback frames[] = {drawDateTime, drawCity, drawCurrentWeather, drawCurrentWeatherHum, drawCurrentRoomTemp, drawCurrentRoomHum, drawWind, drawAirQuality, drawUVIndex, drawPressure, drawForecast, drawForecast2, drawSun, drawMoon, drawWifiInfo, drawIPInfo, drawUntilToNextUpdate};
int numberOfFrames = 17;

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
  // removeAllConfigs();

  applyLanguage();

  display.init();

  if (INVERT_DISPLAY)
  {
    display.flipScreenVertically();
  }

  display.displayOn();
  display.clear();
  display.display();

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(Disp_Contrast);

  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 1, "ESP8266");
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 18, CleanText(lang.LOG_WEATHER_STATION));
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 30, "By idezentas");
  display.drawString(64, 46, "V" + String(VERSION));
  display.display();
  delay(250);

  WiFiManager wifiManager;

  wifiManager.setAPCallback(configModeCallback);

  wifiManager.setDarkMode(true);

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
    Serial.printf_P(PSTR("%s: %%%d\n"), CleanText(lang.LOG_RSSI).c_str(), getWifiQuality());
  }
  else if (currentLang == "en")
  {
    Serial.printf_P(PSTR("%s: %d%%\n"), CleanText(lang.LOG_RSSI).c_str(), getWifiQuality());
  }
  else
  {
    Serial.printf_P(PSTR("%s: %d%%\n"), CleanText(lang.LOG_RSSI).c_str(), getWifiQuality());
  }

  Serial.println();

  delay(250);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, "SSID");
  display.setFont(ArialMT_Plain_10);
  display.drawStringMaxWidth(64, 20, 128, wifiManager.getWiFiSSID());
  display.display();
  delay(250);

  String webAddress = "http://" + WiFi.localIP().toString() + ":" + String(WEBSERVER_PORT) + "/";
  Serial.printf_P(PSTR("%s %s\n"), CleanText(lang.LOG_USE_THIS_URL).c_str(), webAddress.c_str());
  Serial.println();

  delay(500);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 10, "IP");
  display.drawStringMaxWidth(64, 30, 128, WiFi.localIP().toString());
  display.display();
  delay(250);

  configTime(0, 0, NTP_SERVERS);

  Serial.println(CleanText(lang.LOG_WAIT_NTP).c_str());
  time_t now;
  const time_t VALID_TIME = 24 * 3600;
  while ((now = time(nullptr)) < VALID_TIME)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(F("\n"));
  Serial.println(CleanText(lang.LOG_NTP_SYNCED).c_str());

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

  if (millis() - timeSinceLastWUpdate > (1000UL * UPDATE_INTERVAL_SECS))
  {
    setReadyForWeatherUpdate();
    timeSinceLastWUpdate = millis();
  }

  if (readyForWeatherUpdate && ui.getUiState()->frameState == FIXED)
  {
    updateData(&display);
  }

  checkDisplay();

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
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}

void updateData(OLEDDisplay *display)
{
  drawProgress(display, 15, CleanText(lang.PROGRESS_TEXT[0]));
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);
  Serial.println(F("Display_Timestamp\n"));
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("%s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeInfo.tm_wday]).c_str(), timeInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeInfo.tm_mon]).c_str(), timeInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("%s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeInfo.tm_wday]).c_str(), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
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
  delay(250);
  drawProgress(display, 30, CleanText(lang.PROGRESS_TEXT[1]));
  Serial.printf_P(PSTR("%s\n"), Display_City_Name);
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(lang.OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrent(&currentWeather, OPEN_WEATHER_MAP_KEY, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  time_t observationTimestamp = currentWeather.observationTime;
  struct tm timeObserInfo;
  localtime_r(&observationTimestamp, &timeObserInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("observationTimestamp: %s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeObserInfo.tm_wday]).c_str(), timeObserInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeObserInfo.tm_mon]).c_str(), timeObserInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("observationTimestamp: %s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeObserInfo.tm_wday]).c_str(), timeObserInfo.tm_mday, timeObserInfo.tm_mon + 1, timeObserInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeObserInfo.tm_hour, timeObserInfo.tm_min, timeObserInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s\n"), get12Hour(timeObserInfo.tm_hour), timeObserInfo.tm_min, timeObserInfo.tm_sec, getAMPM(timeObserInfo.tm_hour));
  }
  time_t sunriseTimestamp = currentWeather.sunrise;
  struct tm timeRiseInfo;
  localtime_r(&sunriseTimestamp, &timeRiseInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("sunriseTimestamp: %s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeRiseInfo.tm_wday]).c_str(), timeRiseInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeRiseInfo.tm_mon]).c_str(), timeRiseInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("sunriseTimestamp: %s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeRiseInfo.tm_wday]).c_str(), timeRiseInfo.tm_mday, timeRiseInfo.tm_mon + 1, timeRiseInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeRiseInfo.tm_hour, timeRiseInfo.tm_min, timeRiseInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s\n"), get12Hour(timeRiseInfo.tm_hour), timeRiseInfo.tm_min, timeRiseInfo.tm_sec, getAMPM(timeRiseInfo.tm_hour));
  }
  time_t sunsetTimestamp = currentWeather.sunset;
  struct tm timeSetInfo;
  localtime_r(&sunsetTimestamp, &timeSetInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("sunsetTimestamp: %s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeSetInfo.tm_wday]).c_str(), timeSetInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeSetInfo.tm_mon]).c_str(), timeSetInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("sunsetTimestamp: %s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeSetInfo.tm_wday]).c_str(), timeSetInfo.tm_mday, timeSetInfo.tm_mon + 1, timeSetInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeSetInfo.tm_hour, timeSetInfo.tm_min, timeSetInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s\n"), get12Hour(timeSetInfo.tm_hour), timeSetInfo.tm_min, timeSetInfo.tm_sec, getAMPM(timeSetInfo.tm_hour));
  }
  Serial.println();
  delay(1000);
  forecastClient.setMetric(IS_METRIC);
  forecastClient.setLanguage(lang.OPEN_WEATHER_MAP_LANGUAGE);
  uint8_t allowedHours[] = {12};
  forecastClient.setAllowedHours(allowedHours, sizeof(allowedHours));
  forecastClient.updateForecasts(forecasts, OPEN_WEATHER_MAP_KEY, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON, MAX_FORECASTS);
  delay(1000);
  currentAirClient.updateCurrent(&currentAir, OPEN_WEATHER_MAP_KEY, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  time_t observationAirTimestamp = currentAir.observationTime;
  struct tm timeObserAirInfo;
  localtime_r(&observationAirTimestamp, &timeObserAirInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("observationAirTimestamp: %s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeObserAirInfo.tm_wday]).c_str(), timeObserAirInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeObserAirInfo.tm_mon]).c_str(), timeObserAirInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("observationAirTimestamp: %s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeObserAirInfo.tm_wday]).c_str(), timeObserAirInfo.tm_mday, timeObserAirInfo.tm_mon + 1, timeObserAirInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeObserAirInfo.tm_hour, timeObserAirInfo.tm_min, timeObserAirInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s\n"), get12Hour(timeObserAirInfo.tm_hour), timeObserAirInfo.tm_min, timeObserAirInfo.tm_sec, getAMPM(timeObserAirInfo.tm_hour));
  }
  Serial.println();
  delay(1000);
  drawProgress(display, 45, CleanText(lang.PROGRESS_TEXT[2]));
  SunMoonCalc *smCalc = new SunMoonCalc(Display_Timestamp, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  moonData = smCalc->calculateSunAndMoonData().moon;
  Serial.println();
  Serial.println(F("moonData"));
  struct tm timeMoonRiseInfo;
  localtime_r(&moonData.rise, &timeMoonRiseInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("moonData.rise: %s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeMoonRiseInfo.tm_wday]).c_str(), timeMoonRiseInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeMoonRiseInfo.tm_mon]).c_str(), timeMoonRiseInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("moonData.rise: %s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeMoonRiseInfo.tm_wday]).c_str(), timeMoonRiseInfo.tm_mday, timeMoonRiseInfo.tm_mon + 1, timeMoonRiseInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeMoonRiseInfo.tm_hour, timeMoonRiseInfo.tm_min, timeMoonRiseInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s\n"), get12Hour(timeMoonRiseInfo.tm_hour), timeMoonRiseInfo.tm_min, timeMoonRiseInfo.tm_sec, getAMPM(timeMoonRiseInfo.tm_hour));
  }
  struct tm timeMoonSetInfo;
  localtime_r(&moonData.set, &timeMoonSetInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("moonData.set: %s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeMoonSetInfo.tm_wday]).c_str(), timeMoonSetInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeMoonSetInfo.tm_mon]).c_str(), timeMoonSetInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("moonData.set: %s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeMoonSetInfo.tm_wday]).c_str(), timeMoonSetInfo.tm_mday, timeMoonSetInfo.tm_mon + 1, timeMoonSetInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeMoonSetInfo.tm_hour, timeMoonSetInfo.tm_min, timeMoonSetInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s\n"), get12Hour(timeMoonSetInfo.tm_hour), timeMoonSetInfo.tm_min, timeMoonSetInfo.tm_sec, getAMPM(timeMoonSetInfo.tm_hour));
  }
  Serial.printf_P(PSTR("moonData.phase.index: %d, "), moonData.phase.index);
  Serial.printf_P(PSTR("MOON_PHASE: %s\n"), CleanText(lang.MOON_PHASES[moonData.phase.index]).c_str());
  Serial.println();
  delay(250);
  drawProgress(display, 60, CleanText(lang.PROGRESS_TEXT[3]));
  currentMeteoClient.updateWeather(&currentMeteo, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  time_t timeTimestamp = currentMeteo.time;
  struct tm timeTimeInfo;
  localtime_r(&timeTimestamp, &timeTimeInfo);
  if (SHOW_MNAME)
  {
    Serial.printf_P(PSTR("timeTimestamp: %s, %02d %s %04d "), CleanText(lang.WDAY_NAMES[timeTimeInfo.tm_wday]).c_str(), timeTimeInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeTimeInfo.tm_mon]).c_str(), timeTimeInfo.tm_year + 1900);
  }
  else
  {
    Serial.printf_P(PSTR("timeTimestamp: %s, %02d/%02d/%04d "), CleanText(lang.WDAY_NAMES[timeTimeInfo.tm_wday]).c_str(), timeTimeInfo.tm_mday, timeTimeInfo.tm_mon + 1, timeTimeInfo.tm_year + 1900);
  }
  if (IS_24HOUR)
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeTimeInfo.tm_hour, timeTimeInfo.tm_min, timeTimeInfo.tm_sec);
  }
  else
  {
    Serial.printf_P(PSTR("%02d:%02d:%02d %s\n"), get12Hour(timeTimeInfo.tm_hour), timeTimeInfo.tm_min, timeTimeInfo.tm_sec, getAMPM(timeTimeInfo.tm_hour));
  }
  Serial.println();
  delay(1000);
  readyForWeatherUpdate = false;
  drawProgress(display, 100, CleanText(lang.PROGRESS_TEXT[4]));
  delay(250);
}

void drawIPInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 5 + y, "IP");
  display->drawStringMaxWidth(64 + x, 25 + y, 128, WiFi.localIP().toString());
}

void drawWifiInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 0 + y, "SSID");
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(64 + x, 20 + y, 128, CleanText(SSID_String));
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
  display->setFont(ArialMT_Plain_24);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  }
  else
  {
    sprintf_P(buff, PSTR("%02d:%02d:%02d"), get12Hour(timeInfo.tm_hour), timeInfo.tm_min, timeInfo.tm_sec);
  }

  display->drawString(64 + x, 10 + y, String(buff));

  display->setFont(ArialMT_Plain_10);

  if (SHOW_MNAME)
  {
    sprintf_P(buff, PSTR("%s, %02d %s %04d"), CleanText(lang.WDAY_NAMES[timeInfo.tm_wday]).c_str(), timeInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeInfo.tm_mon]).c_str(), timeInfo.tm_year + 1900);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), CleanText(lang.WDAY_NAMES[timeInfo.tm_wday]).c_str(), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  }

  display->drawString(64 + x, 0 + y, String(buff));

  if (!IS_24HOUR)
  {
    display->drawString(64 + x, 38 + y, getAMPM(timeInfo.tm_hour));
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  bool isDST = (timeInfo.tm_isdst) > 0;
  Display_TZ_NAME_SHORT = tzname[isDST ? 1 : 0];
  display->drawString(0 + x, 38 + y, Display_TZ_NAME_SHORT);

  Display_GMTOffset = currentWeather.timezone / 3600;
  (Display_GMTOffset >= 0) ? (sprintf_P(buff, PSTR("%s+%d"), "GMT", Display_GMTOffset)) : (sprintf_P(buff, PSTR("%s%d"), "GMT", Display_GMTOffset));
  display->drawString(90 + x, 38 + y, String(buff));
}

void drawUntilToNextUpdate(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  unsigned long timeLeftMillis = ((millis() - timeSinceLastWUpdate) < ((1000UL * UPDATE_INTERVAL_SECS))) ? (((1000UL * UPDATE_INTERVAL_SECS)) - (millis() - timeSinceLastWUpdate)) : 0;
  unsigned long minutesLeft = timeLeftMillis / 60000;
  unsigned long secondsLeft = (timeLeftMillis % 60000) / 1000;

  char buff[16];
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  sprintf_P(buff, PSTR("%02d:%02d"), minutesLeft, secondsLeft);
  display->drawString(64 + x, 20 + y, String(buff));

  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(64 + x, 0 + y, 128, CleanText(lang.NEXT_UPDATE_TEXT));
}

void drawCity(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 10 + y, UpperText(CleanText(Display_City_Name)));
  display->drawString(64 + x, 30 + y, UpperText(CleanText(Display_Country_Name)));
}

void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, currentWeather.descriptionClean);

  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWeather.temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->drawString(50 + x, 5 + y, temp);

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(25 + x, 0 + y, currentWeather.iconMeteoCon);
}

void drawCurrentWeatherHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(70 + x, 5 + y, String(currentWeather.humidity));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_humidity);
}

void drawCurrentRoomTemp(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(lang.INDOOR_TEMP_TEXT));
  display->setFont(ArialMT_Plain_24);
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
  display->drawString(64 + x, 5 + y, temp);
}

void drawCurrentRoomHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(lang.INDOOR_HUM_TEXT));
  display->setFont(ArialMT_Plain_24);
  float h = dht.readHumidity();
  String hum;
  if (currentLang == "tr")
  {
    hum = ("%") + String(h, 0);
  }
  else if (currentLang == "en")
  {
    hum = String(h, 0) + ("%");
  }
  else
  {
    hum = String(h, 0) + ("%");
  }
  display->drawString(64 + x, 5 + y, hum);
}

void drawForecast(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  drawForecastDetails(display, x, y, 0);
  drawForecastDetails(display, x + 44, y, 1);
  drawForecastDetails(display, x + 88, y, 2);
}

void drawForecast2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  drawForecastDetails(display, x + 10, y, 3);
  drawForecastDetails(display, x + 70, y, 4);
}

void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex)
{
  time_t observationTimestamp = forecasts[dayIndex].observationTime;
  struct tm timeInfo;
  localtime_r(&observationTimestamp, &timeInfo);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y, CleanText(lang.WDAY_NAMES[timeInfo.tm_wday]));

  display->setFont(Meteocons_Plain_21);
  display->drawString(x + 20, y + 12, forecasts[dayIndex].iconMeteoCon);

  String temp = String(forecasts[dayIndex].temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y + 34, temp);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawWind(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String wind = (IS_METRIC ? String((currentWeather.windSpeed * 3.6), 0) : String((currentWeather.windSpeed), 0)) + (IS_METRIC ? " km/h" : " mph");
  display->drawString(55 + x, 10 + y, wind);

  int val = floor(currentWeather.windDeg / 45);
  int dirID = (val % 8);

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(90 + x, 38 + y, CleanText(lang.WIND_DIRECTION_LONG[dirID]));

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
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  uint8_t aqiIndex = (currentAir.aqi) - 1;
  display->drawString(62 + x, 14 + y, CleanText(lang.AIR_QUALITY_TEXT[aqiIndex]));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_air_quality);
}

void drawUVIndex(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  uint8_t uv_int = currentMeteo.uv_index;
  display->drawString(62 + x, 14 + y, CleanText(lang.UV_INDEX_TEXT[getUVindexText(uv_int)]));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_uv_index);
}

void drawPressure(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String pres = String(currentWeather.pressure) + " hPa";
  display->drawStringMaxWidth(58 + x, 14 + y, 128, pres);
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_pressure);
}

void drawMoon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  struct tm timeMoonRiseInfo;
  localtime_r(&moonData.rise, &timeMoonRiseInfo);

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);

  display->drawXbm(45 + x, 5 + y, 10, 10, weather_weather_rise);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanText(lang.WDAY_NAMES[timeMoonRiseInfo.tm_wday]), timeMoonRiseInfo.tm_hour, timeMoonRiseInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanText(lang.WDAY_NAMES[timeMoonRiseInfo.tm_wday]), get12Hour(timeMoonRiseInfo.tm_hour), timeMoonRiseInfo.tm_min, getAMPM(timeMoonRiseInfo.tm_hour));
  }

  display->drawString(60 + x, 5 + y, String(buff));

  struct tm timeMoonSetInfo;
  localtime_r(&moonData.set, &timeMoonSetInfo);

  display->drawXbm(45 + x, 20 + y, 10, 10, weather_weather_set);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanText(lang.WDAY_NAMES[timeMoonSetInfo.tm_wday]), timeMoonSetInfo.tm_hour, timeMoonSetInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanText(lang.WDAY_NAMES[timeMoonSetInfo.tm_wday]), get12Hour(timeMoonSetInfo.tm_hour), timeMoonSetInfo.tm_min, getAMPM(timeMoonSetInfo.tm_hour));
  }

  display->drawString(60 + x, 20 + y, String(buff));

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(lang.SUN_MOON_TEXT[1]));

  display->setFont(Moon_Phases_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(25 + x, 0 + y, getMoonPhasesIcon(moonData.phase.index));
}

void drawSun(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  time_t sunriseTimestamp = currentWeather.sunrise;
  struct tm timeRiseInfo;
  localtime_r(&sunriseTimestamp, &timeRiseInfo);

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);

  display->drawXbm(45 + x, 5 + y, 10, 10, weather_weather_rise);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanText(lang.WDAY_NAMES[timeRiseInfo.tm_wday]), timeRiseInfo.tm_hour, timeRiseInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanText(lang.WDAY_NAMES[timeRiseInfo.tm_wday]), get12Hour(timeRiseInfo.tm_hour), timeRiseInfo.tm_min, getAMPM(timeRiseInfo.tm_hour));
  }

  display->drawString(60 + x, 5 + y, String(buff));

  time_t sunsetTimestamp = currentWeather.sunset;
  struct tm timeSetInfo;
  localtime_r(&sunsetTimestamp, &timeSetInfo);

  display->drawXbm(45 + x, 20 + y, 10, 10, weather_weather_set);

  if (IS_24HOUR)
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d"), CleanText(lang.WDAY_NAMES[timeSetInfo.tm_wday]), timeSetInfo.tm_hour, timeSetInfo.tm_min);
  }
  else
  {
    sprintf_P(buff, PSTR("%s, %02d:%02d %s"), CleanText(lang.WDAY_NAMES[timeSetInfo.tm_wday]), get12Hour(timeSetInfo.tm_hour), timeSetInfo.tm_min, getAMPM(timeSetInfo.tm_hour));
  }

  display->drawString(60 + x, 20 + y, String(buff));

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(25 + x, 0 + y, "B");

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(lang.SUN_MOON_TEXT[0]));
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

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(30, 54, String(buff));

  display->drawString(0, 54, String(state->currentFrame + 1) + "/" + String(numberOfFrames));

  if (SHOW_MNAME)
  {
    sprintf_P(buff, PSTR("%02d %s"), timeInfo.tm_mday, CleanText(lang.MONTH_NAMES[timeInfo.tm_mon]).c_str());
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

String encodeHtmlString(String msg)
{
  String encodedMsg = msg;
  encodedMsg.replace(" ", "%20");
  encodedMsg.replace("Ç", "%C3%87");
  encodedMsg.replace("ç", "%C3%A7");
  encodedMsg.replace("Ö", "%C3%96");
  encodedMsg.replace("ö", "%C3%B6");
  encodedMsg.replace("Ü", "%C3%9C");
  encodedMsg.replace("ü", "%C3%BC");
  encodedMsg.replace("ı", "%C4%B1");
  encodedMsg.replace("İ", "%C4%B0");
  encodedMsg.replace("Ş", "%C5%9E");
  encodedMsg.replace("ş", "%C5%9F");
  encodedMsg.replace("Ğ", "%C4%9E");
  encodedMsg.replace("ğ", "%C4%9F");
  encodedMsg.trim();
  return encodedMsg;
}

String UpperText(String text)
{
  text.toUpperCase();
  return text;
}

String CleanText(String text)
{
  text.replace("Ç", "C");
  text.replace("ç", "c");
  text.replace("Ş", "S");
  text.replace("ş", "s");
  text.replace("Ö", "O");
  text.replace("ö", "o");
  text.replace("İ", "I");
  text.replace("ı", "i");
  text.replace("Ü", "U");
  text.replace("ü", "u");
  text.replace("ğ", "g");
  text.replace("Ğ", "G");
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
  Serial.println(CleanText(lang.LOG_READY_FOR_UPDATE).c_str());
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
    Serial.println(CleanText(lang.LOG_SCREEN_OPENED).c_str() + currentTime);
  }
  else
  {
    display.displayOff();
    Serial.println(CleanText(lang.LOG_SCREEN_CLOSED).c_str() + currentTime);
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
    Serial.println(CleanText(lang.LOG_SCREEN_OPEN_TIME).c_str() + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawStringMaxWidth(64, 10, 128, CleanText(lang.NIGHT_MODE_FINISH_TEXT));
    display.display();
    delay(1000);
    enableDisplay(true);
  }

  if (currentTime == timeDisplayTurnsOff && displayOn)
  {
    Serial.println(CleanText(lang.LOG_SCREEN_CLOSE_TIME).c_str() + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawStringMaxWidth(64, 10, 128, CleanText(lang.NIGHT_MODE_START_TEXT));
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
    Serial.println(CleanText(lang.LOG_FILE_NOT_FOUND).c_str());
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
    Serial.println();

    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);

    if (key == "Display_GMTOffset")
    {
      Display_GMTOffset = value.toInt();
    }
    if (key == "Display_TZ_NAME")
    {
      Display_TZ_NAME = value;
    }
    if (key == "Display_TZ_POSIX")
    {
      Display_TZ_POSIX = value;
    }
    if (key == "Display_TZ_NAME_SHORT")
    {
      Display_TZ_NAME_SHORT = value;
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
    if (key == "currentLang")
    {
      currentLang = value;
    }
  }

  f.close();
}

void saveDeviceSettings()
{
  File f = LittleFS.open(configName, "w");
  if (!f)
  {
    Serial.println((CleanText(lang.LOG_FILE_OPEN_FAIL).c_str()));
  }
  else
  {
    Serial.println((CleanText(lang.LOG_SAVING_SETTINGS).c_str()));
    f.println(F("Display_GMTOffset=") + String(Display_GMTOffset));
    f.println(F("Display_TZ_NAME=") + Display_TZ_NAME);
    f.println(F("Display_TZ_POSIX=") + Display_TZ_POSIX);
    f.println(F("Display_TZ_NAME_SHORT=") + Display_TZ_NAME_SHORT);
    f.println(F("UPDATE_INTERVAL=") + String(UPDATE_INTERVAL));
    f.println(F("OPEN_WEATHER_MAP_LOCATION_LAT=") + String(OPEN_WEATHER_MAP_LOCATION_LAT, 7));
    f.println(F("OPEN_WEATHER_MAP_LOCATION_LON=") + String(OPEN_WEATHER_MAP_LOCATION_LON, 7));
    f.println(F("Display_City_Name=") + Display_City_Name);
    f.println(F("Display_Country_Name=") + Display_Country_Name);
    f.println(F("IS_METRIC=") + String(IS_METRIC));
    f.println(F("currentLang=") + currentLang);
  }
  f.close();

  loadDeviceSettings();
}

void loadSecuritySettings()
{
  if (LittleFS.exists(securityConfig) == false)
  {
    Serial.println(securityConfig);
    Serial.println(CleanText(lang.LOG_FILE_NOT_FOUND).c_str());
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
    Serial.println();

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
}

void saveSecuritySettings()
{
  File f = LittleFS.open(securityConfig, "w");
  if (!f)
  {
    Serial.println((CleanText(lang.LOG_FILE_OPEN_FAIL).c_str()));
  }
  else
  {
    Serial.println((CleanText(lang.LOG_SAVING_SETTINGS).c_str()));
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
    Serial.println(CleanText(lang.LOG_FILE_NOT_FOUND).c_str());
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
    Serial.println();

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
}

void saveDisplaySettings()
{
  File f = LittleFS.open(displayConfig, "w");
  if (!f)
  {
    Serial.println((CleanText(lang.LOG_FILE_OPEN_FAIL).c_str()));
  }
  else
  {
    Serial.println((CleanText(lang.LOG_SAVING_SETTINGS).c_str()));
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
    Serial.println(CleanText(lang.LOG_FILE_NOT_FOUND).c_str());
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
    Serial.println();

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
  }

  f.close();
}

void saveApiKeySettings()
{
  File f = LittleFS.open(ApiKeyConfig, "w");
  if (!f)
  {
    Serial.println((CleanText(lang.LOG_FILE_OPEN_FAIL).c_str()));
  }
  else
  {
    Serial.println((CleanText(lang.LOG_SAVING_SETTINGS).c_str()));
    f.println(F("OPEN_WEATHER_MAP_KEY=") + OPEN_WEATHER_MAP_KEY);
    f.println(F("OPEN_CAGE_KEY=") + OPEN_CAGE_KEY);
  }
  f.close();

  loadApiKeySettings();
}

void applyLanguage()
{
  const LanguagePack *selected;
  if (currentLang == "tr")
  {
    selected = &LANG_TR;
  }
  else if (currentLang == "en")
  {
    selected = &LANG_EN;
  }
  else
  {
    selected = &LANG_EN;
  }
  lang = *selected;
}

void removeAllConfigs()
{
  LittleFS.remove(configName);
  LittleFS.remove(securityConfig);
  LittleFS.remove(displayConfig);
  LittleFS.remove(ApiKeyConfig);
}

void loadAllConfigs()
{
  loadApiKeySettings();
  loadDeviceSettings();
  loadDisplaySettings();
  loadSecuritySettings();
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println(CleanText(lang.LOG_ENTER_CONFIG_MODE).c_str());
  Serial.println(WiFi.softAPIP());

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "WiFiManager");
  display.drawString(64, 10, CleanText(lang.LOG_CONNECT_TO_AP));
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 26, myWiFiManager->getConfigPortalSSID());
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 46, CleanText(lang.LOG_WIFI_SETUP));
  display.display();

  Serial.println(F("WiFiManager"));
  Serial.println(CleanText(lang.LOG_CONNECT_TO_AP).c_str());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println(CleanText(lang.LOG_WIFI_SETUP).c_str());
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
  html.replace(F("%DEVICESETTINGS%"), lang.HTML_TEXT_DEVICE_SETTINGS);
  html.replace(F("%CITYTEXT%"), lang.HTML_TEXT_CITY_NAME);
  html.replace(F("%CITYPLACE%"), lang.HTML_TEXT_CITY_EXAMPLE);
  html.replace(F("%COUNTRYTEXT%"), lang.HTML_TEXT_COUNTRY_NAME);
  html.replace(F("%COUNTRYPLACE%"), lang.HTML_TEXT_COUNTRY_EXAMPLE);
  html.replace(F("%FINDGEOTIME%"), lang.HTML_TEXT_FIND_LOCATION);
  html.replace(F("%LATTEXT%"), lang.HTML_TEXT_LATITUDE);
  html.replace(F("%LATPLACE%"), lang.HTML_TEXT_LATITUDE_EXAMPLE);
  html.replace(F("%LONTEXT%"), lang.HTML_TEXT_LONGITUDE);
  html.replace(F("%LONPLACE%"), lang.HTML_TEXT_LONGITUDE_EXAMPLE);
  html.replace(F("%TZNAMETEXT%"), lang.HTML_TEXT_TIMEZONE);
  html.replace(F("%UNITTEXT%"), lang.HTML_TEXT_UNIT_SELECT);
  html.replace(F("%METRICTEXT%"), lang.HTML_TEXT_UNIT_METRIC);
  html.replace(F("%IMPERIALTEXT%"), lang.HTML_TEXT_UNIT_IMPERIAL);
  html.replace(F("%INTERVALTEXT%"), lang.HTML_TEXT_UPDATE_INTERVAL);
  html.replace(F("%SAVETEXT%"), lang.HTML_TEXT_SAVE);
  html.replace(F("%HOMETEXT%"), lang.HTML_TEXT_HOME);
  html.replace(F("%GEOALERT%"), lang.HTML_TEXT_MISSING_INPUT);
  html.replace(F("%GEOALERT2%"), lang.HTML_TEXT_LOCATION_NOT_FOUND);
  html.replace(F("%GEOALERT3%"), lang.HTML_TEXT_API_FAIL);
  html.replace(F("%FORMSENDING%"), lang.HTML_TEXT_SAVING);
  html.replace(F("%TZALERT1%"), lang.HTML_TEXT_TZ_NOT_FOUND);
  html.replace(F("%TZALERT2%"), lang.HTML_TEXT_TZ_FETCH_FAIL);
  html.replace(F("%TZALERT3%"), lang.HTML_TEXT_TZ_LOAD_FAIL);
  html.replace(F("%APIKEYSETTINGS%"), lang.HTML_TEXT_API_SETTINGS);
  html.replace(F("%DISPLAYSETTINGS%"), lang.HTML_TEXT_DISPLAY_SETTINGS);
  html.replace(F("%SECURITYSETTINGS%"), lang.HTML_TEXT_SECURITY_SETTINGS);
  html.replace(F("%ABOUTTEXT%"), lang.HTML_TEXT_ABOUT);
  html.replace(F("%OTATEXT%"), lang.HTML_TEXT_OTA_UPDATE);
  html.replace(F("%SHOWMAPTEXT%"), lang.HTML_TEXT_SHOW_ON_MAP);
  html.replace(F("%SYSLANG%"), lang.HTML_SYSTEM_LANG);
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
  html.replace(F("%LANGTR%"), (currentLang == "tr") ? "selected" : "");
  html.replace(F("%LANGEN%"), (currentLang == "en") ? "selected" : "");
  server.sendContent(html);

  server.client().stop();
}

void handleSaveDevice()
{

  if (server.hasArg("city"))
  {
    Display_City_Name = server.arg("city");
  }

  if (server.hasArg("country"))
  {
    Display_Country_Name = server.arg("country");
  }

  if (server.hasArg("posixInput"))
  {
    Display_TZ_POSIX = server.arg("posixInput");
  }

  if (server.hasArg("latitude"))
  {
    OPEN_WEATHER_MAP_LOCATION_LAT = server.arg("latitude").toFloat();
  }

  if (server.hasArg("longitude"))
  {
    OPEN_WEATHER_MAP_LOCATION_LON = server.arg("longitude").toFloat();
  }

  if (server.hasArg("updateInterval"))
  {
    String val = server.arg("updateInterval");

    if (val.startsWith("up"))
    {
      uint8_t interval = val.substring(2).toInt(); // "up10" → "10" → int(10)

      if (interval == 10 || interval == 15 || interval == 20 || interval == 30 || interval == 60)
      {
        UPDATE_INTERVAL = interval;
      }
    }
  }

  if (server.hasArg("units"))
  {
    String val = server.arg("units");
    IS_METRIC = (val == "metric") ? true : false;
  }

  if (server.hasArg("timezoneName"))
  {
    String val = server.arg("timezoneName");
    Display_TZ_NAME = val;
  }

  if (server.hasArg("sysLang"))
  {
    String val = server.arg("sysLang");

    if (val.startsWith("lang-"))
    {
      String langs = val.substring(5); // "lang-en" → "en"

      if (langs == "tr" || langs == "en")
      {
        currentLang = langs;
      }
    }
    applyLanguage();
  }

  UPDATE_INTERVAL_SECS = UPDATE_INTERVAL * 60;

  Serial.println();
  Serial.println(CleanText(lang.LOG_DEVICE_SETTINGS_UPDATED).c_str());
  Serial.printf_P(PSTR("currentLang: %s\n"), currentLang.c_str());
  Serial.printf_P(PSTR("Display_TZ_NAME: %s\n"), Display_TZ_NAME.c_str());
  Serial.printf_P(PSTR("Display_TZ_POSIX: %s\n"), Display_TZ_POSIX.c_str());
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_LOCATION_LAT: %f\n"), OPEN_WEATHER_MAP_LOCATION_LAT);
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_LOCATION_LON: %f\n"), OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.printf_P(PSTR("Display_City_Name: %s\n"), Display_City_Name.c_str());
  Serial.printf_P(PSTR("Display_Country_Name: %s\n"), Display_Country_Name.c_str());
  Serial.printf_P(PSTR("UPDATE_INTERVAL: %d\n"), UPDATE_INTERVAL);
  Serial.printf_P(PSTR("IS_METRIC: %s\n"), IS_METRIC ? "true" : "false");
  Serial.println();

  saveDeviceSettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Settings_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%DEVICESETTINGS%"), lang.HTML_TEXT_DEVICE_SETTINGS);
  html.replace(F("%DEVICETEXT%"), lang.HTML_TEXT_DEVICE_SAVED);
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
  html.replace(F("%APIKEYSETTINGS%"), lang.HTML_TEXT_API_SETTINGS);
  html.replace(F("%OWMAPIKEYTEXT%"), lang.HTML_TEXT_OWM_API_KEY);
  html.replace(F("%APIPLACE%"), lang.HTML_TEXT_ENTER_API_KEY);
  html.replace(F("%SHOWPASS%"), lang.HTML_TEXT_SHOW);
  html.replace(F("%GEOAPIKEYTEXT%"), lang.HTML_TEXT_OC_API_KEY);
  html.replace(F("%SAVETEXT%"), lang.HTML_TEXT_SAVE);
  html.replace(F("%HIDEPASS%"), lang.HTML_TEXT_HIDE);
  html.replace(F("%GETFROMTEXT%"), lang.HTML_TEXT_FROM_HERE);
  html.replace(F("%HERETEXT%"), lang.HTML_TEXT_GET);
  html.replace(F("%FORMSENDING%"), lang.HTML_TEXT_SAVING);
  html.replace(F("%HOMETEXT%"), lang.HTML_TEXT_HOME);
  html.replace(F("%DEVICESETTINGS%"), lang.HTML_TEXT_DEVICE_SETTINGS);
  html.replace(F("%DISPLAYSETTINGS%"), lang.HTML_TEXT_DISPLAY_SETTINGS);
  html.replace(F("%SECURITYSETTINGS%"), lang.HTML_TEXT_SECURITY_SETTINGS);
  html.replace(F("%OTATEXT%"), lang.HTML_TEXT_OTA_UPDATE);
  html.replace(F("%ABOUTTEXT%"), lang.HTML_TEXT_ABOUT);
  html.replace(F("%OWMAPIKEY%"), OPEN_WEATHER_MAP_KEY);
  html.replace(F("%GEOAPIKEY%"), OPEN_CAGE_KEY);
  server.sendContent(html);

  server.client().stop();
}

void handleSaveApiKey()
{

  if (server.hasArg("OWMApiKey"))
  {
    OPEN_WEATHER_MAP_KEY = server.arg("OWMApiKey");
  }

  if (server.hasArg("geoApiKey"))
  {
    OPEN_CAGE_KEY = server.arg("geoApiKey");
  }

  Serial.println();
  Serial.println(CleanText(lang.LOG_API_KEYS_UPDATED).c_str());
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_APP_ID: %s\n"), OPEN_WEATHER_MAP_KEY.c_str());
  Serial.printf_P(PSTR("OPEN_CAGE_ID: %s\n"), OPEN_CAGE_KEY.c_str());
  Serial.println();

  saveApiKeySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_ApiKey_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%APIKEYSETTINGS%"), lang.HTML_TEXT_API_SETTINGS);
  html.replace(F("%APIKEYTEXT%"), lang.HTML_TEXT_API_SAVED);
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
  html.replace(F("%HOMETITLE%"), lang.HTML_TEXT_TITLE);
  html.replace(F("%DEVICESETTINGS%"), lang.HTML_TEXT_DEVICE_SETTINGS);
  html.replace(F("%SECURITYSETTINGS%"), lang.HTML_TEXT_SECURITY_SETTINGS);
  html.replace(F("%DEFAULTSETTINGS%"), lang.HTML_TEXT_DEFAULT_RESET);
  html.replace(F("%DEFAULTALERT%"), lang.HTML_TEXT_DEFAULT_CONFIRM);
  html.replace(F("%OTATEXT%"), lang.HTML_TEXT_OTA_UPDATE);
  html.replace(F("%DISPLAYSETTINGS%"), lang.HTML_TEXT_DISPLAY_SETTINGS);
  html.replace(F("%APIKEYSETTINGS%"), lang.HTML_TEXT_API_SETTINGS);
  html.replace(F("%ABOUTTEXT%"), lang.HTML_TEXT_ABOUT);
  html.replace(F("%UPDATEDATATEXT%"), lang.HTML_TEXT_REFRESH_DATA);
  html.replace(F("%VERSION%"), "V" + String(VERSION));
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
  html.replace(F("%SECURITYSETTINGS%"), lang.HTML_TEXT_SECURITY_SETTINGS);
  html.replace(F("%SHOWPASS%"), lang.HTML_TEXT_SHOW);
  html.replace(F("%SAVETEXT%"), lang.HTML_TEXT_SAVE);
  html.replace(F("%PASSWORDTEXT%"), lang.HTML_TEXT_PASSWORD);
  html.replace(F("%USERTEXT%"), lang.HTML_TEXT_USERNAME);
  html.replace(F("%USERPLACE%"), lang.HTML_TEXT_ENTER_USERNAME);
  html.replace(F("%PASSWORDPLACE2%"), lang.HTML_TEXT_ENTER_PASSWORD);
  html.replace(F("%HIDEPASS%"), lang.HTML_TEXT_HIDE);
  html.replace(F("%HOMETEXT%"), lang.HTML_TEXT_HOME);
  html.replace(F("%DEVICESETTINGS%"), lang.HTML_TEXT_DEVICE_SETTINGS);
  html.replace(F("%APIKEYSETTINGS%"), lang.HTML_TEXT_API_SETTINGS);
  html.replace(F("%DISPLAYSETTINGS%"), lang.HTML_TEXT_DISPLAY_SETTINGS);
  html.replace(F("%OTATEXT%"), lang.HTML_TEXT_OTA_UPDATE);
  html.replace(F("%ABOUTTEXT%"), lang.HTML_TEXT_ABOUT);
  html.replace(F("%USER%"), SysUser);
  html.replace(F("%PASSWORD%"), SysPass);
  server.sendContent(html);

  server.client().stop();
}

void handleSaveSecurity()
{
  if (server.hasArg("user"))
  {
    SysUser = server.arg("user");
  }

  if (server.hasArg("password"))
  {
    SysPass = server.arg("password");
  }

  Serial.println(CleanText(lang.LOG_SECURITY_SETTINGS_UPDATED).c_str());
  Serial.printf_P(PSTR("SysUser: %s\n"), SysUser.c_str());
  Serial.printf_P(PSTR("SysPass: %s\n"), SysPass.c_str());

  saveSecuritySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Security_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%SECURITYSETTINGS%"), lang.HTML_TEXT_SECURITY_SETTINGS);
  html.replace(F("%SECURITYTEXT%"), lang.HTML_TEXT_SECURITY_SAVED);
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
  html.replace(F("%SAVETEXT%"), lang.HTML_TEXT_SAVE);
  html.replace(F("%TIMEONTEXT%"), lang.HTML_TEXT_ON_TIME);
  html.replace(F("%TIMEOFFTEXT%"), lang.HTML_TEXT_OFF_TIME);
  html.replace(F("%INVDISPTEXT%"), lang.HTML_TEXT_ROTATE_DISPLAY);
  html.replace(F("%FORMSENDING%"), lang.HTML_TEXT_SAVING);
  html.replace(F("%DISPLAYSETTINGS%"), lang.HTML_TEXT_DISPLAY_SETTINGS);
  html.replace(F("%DISPLAYCONTRAST%"), lang.HTML_TEXT_BRIGHTNESS);
  html.replace(F("%HOURTEXT%"), lang.HTML_TEXT_TIME_FORMAT);
  html.replace(F("%HOUR24TEXT%"), lang.HTML_TEXT_24H);
  html.replace(F("%HOUR12TEXT%"), lang.HTML_TEXT_12H);
  html.replace(F("%MONTHTEXT%"), lang.HTML_TEXT_MONTH_FORMAT);
  html.replace(F("%ASNUMBERTEXT%"), lang.HTML_TEXT_MONTH_NUMERIC);
  html.replace(F("%ASNAMETEXT%"), lang.HTML_TEXT_MONTH_TEXT);
  html.replace(F("%TIMEWARNINGTEXT%"), lang.HTML_TEXT_ENTER_TIME);
  html.replace(F("%HOMETEXT%"), lang.HTML_TEXT_HOME);
  html.replace(F("%DEVICESETTINGS%"), lang.HTML_TEXT_DEVICE_SETTINGS);
  html.replace(F("%APIKEYSETTINGS%"), lang.HTML_TEXT_API_SETTINGS);
  html.replace(F("%SECURITYSETTINGS%"), lang.HTML_TEXT_SECURITY_SETTINGS);
  html.replace(F("%OTATEXT%"), lang.HTML_TEXT_OTA_UPDATE);
  html.replace(F("%ABOUTTEXT%"), lang.HTML_TEXT_ABOUT);
  html.replace(F("%TIMEON%"), timeDisplayTurnsOn);
  html.replace(F("%TIMEOFF%"), timeDisplayTurnsOff);
  html.replace(F("%SETCONTRAST%"), String(Disp_Contrast));
  html.replace(F("%IS_INVDISP_CHECKED%"), INVERT_DISPLAY ? "checked" : "");
  html.replace(F("%HOUR24%"), IS_24HOUR ? "selected" : "");
  html.replace(F("%HOUR12%"), !IS_24HOUR ? "selected" : "");
  html.replace(F("%ASNUMBER%"), !SHOW_MNAME ? "selected" : "");
  html.replace(F("%ASNAME%"), SHOW_MNAME ? "selected" : "");
  server.sendContent(html);

  server.client().stop();
}

void handleSaveDisplay()
{
  bool flipOld = INVERT_DISPLAY;
  uint8_t conOld = Disp_Contrast;

  if (server.hasArg("turnOffTime"))
  {
    timeDisplayTurnsOff = server.arg("turnOffTime");
  }

  if (server.hasArg("turnOnTime"))
  {
    timeDisplayTurnsOn = server.arg("turnOnTime");
  }

  if (server.hasArg("setConstrast"))
  {
    Disp_Contrast = server.arg("setConstrast").toInt();
  }

  if (server.hasArg("hours"))
  {
    String val = server.arg("hours");
    IS_24HOUR = (val == "hour24") ? true : false;
  }

  if (server.hasArg("months"))
  {
    String val = server.arg("months");
    SHOW_MNAME = (val == "asName") ? true : false;
  }

  INVERT_DISPLAY = server.hasArg("invDisp");

  Serial.println();
  Serial.println(CleanText(lang.LOG_DISPLAY_SETTINGS_UPDATED).c_str());
  Serial.printf_P(PSTR("INVERT_DISPLAY: %s\n"), INVERT_DISPLAY ? "true" : "false");
  Serial.printf_P(PSTR("timeDisplayTurnsOn: %s\n"), timeDisplayTurnsOn.c_str());
  Serial.printf_P(PSTR("timeDisplayTurnsOff: %s\n"), timeDisplayTurnsOff.c_str());
  Serial.printf_P(PSTR("Disp_Contrast: %d\n"), Disp_Contrast);
  Serial.printf_P(PSTR("IS_24HOUR: %s\n"), IS_24HOUR ? "true" : "false");
  Serial.printf_P(PSTR("SHOW_MNAME: %s\n"), SHOW_MNAME ? "true" : "false");
  Serial.println();

  saveDisplaySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Display_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%DISPLAYSETTINGS%"), lang.HTML_TEXT_DISPLAY_SETTINGS);
  html.replace(F("%DISPLAYTEXT%"), lang.HTML_TEXT_DISPLAY_SAVED);
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

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Reset_Default_Page);
  html.replace(F("%LANG%"), currentLang);
  html.replace(F("%DEFAULTSETTINGS%"), lang.HTML_TEXT_DEFAULT_SETTINGS);
  html.replace(F("%DEFAULTTEXT%"), lang.HTML_TEXT_DEFAULT_RESTORED);
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
  Serial.println(CleanText(lang.LOG_OTA_STARTED).c_str());
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawStringMaxWidth(64, 10, 128, CleanText(lang.OTA_START_TEXT));
  display.display();
  delay(250);
}

void onOTAProgress(size_t current, size_t final)
{
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000)
  {
    ota_progress_millis = millis();
    Serial.printf_P(PSTR("%s %s: %u bytes, %s: %u bytes\n"), CleanText(lang.LOG_OTA_PROGRESS).c_str(), CleanText(lang.LOG_NOW).c_str(), current, CleanText(lang.LOG_FINAL).c_str(), final);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawStringMaxWidth(64, 10, 128, CleanText(lang.OTA_PROGRESS_TEXT));
    display.display();
  }
}

void onOTAEnd(bool success)
{
  if (success)
  {
    Serial.println(CleanText(lang.LOG_OTA_SUCCESS).c_str());
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawStringMaxWidth(64, 10, 128, CleanText(lang.OTA_FINISH_TEXT));
    display.display();
    delay(250);
    display.displayOff();
  }
  else
  {
    Serial.println(CleanText(lang.LOG_OTA_ERROR).c_str());
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawStringMaxWidth(64, 10, 128, CleanText(lang.OTA_ERROR_TEXT));
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