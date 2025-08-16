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
#include <ArduinoJson.h>
#include "FS.h"
#include <ElegantOTA.h>
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include "Wire.h"
#include "OpenWeatherMapCurrent.h"
#include "OpenWeatherMapAir.h"
#include "OpenMeteoApiCurrent.h"
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"
#include "SunMoonCalc.h"
#include "MoonPhasesFont.h"
#include "WindDirectionImages.h"
#include "WeatherImages.h"
#include "DirectionImages.h"
#include "FloatRates.h"
#include "WebPages.h"
#include "settings.h"
#include "lang_tr.h" // System Language. Select one of them. lang_en or lang_tr

SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi ui(&display);

OpenWeatherMapCurrent currentWeatherClient;
OpenWeatherMapCurrentData currentWeather;

OpenWeatherMapAir currentAirClient;
OpenWeatherMapAirData currentAir;

OpenMeteoApiCurrent currentMeteoClient;
OpenMeteoApiCurrentData currentMeteo;

FloatRates currencyClient;
FloatRatesData currencyData1;
FloatRatesData currencyData2;

SunMoonCalc::Moon moonData;

bool readyForWeatherUpdate = false;

unsigned long timeSinceLastWUpdate = 0;

boolean displayOn = true;

String SSID_String;

unsigned long ota_progress_millis = 0;

String Display_TZ_POSIX = "<+03>-3";
String Display_TZ_NAME_SHORT = "+03";
int Display_GMTOffset = 3; // (utc+) TZ in hours

ESP8266WebServer server(WEBSERVER_PORT);

void drawProgress(OLEDDisplay *display, int percentage, String label);
void updateData(OLEDDisplay *display);
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeatherHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeatherAppe(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWind(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawAirQuality(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawMoon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawSun(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state);
void drawCity(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawIPInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWifiInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawUVIndex(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawUntilToNextUpdate(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void setReadyForWeatherUpdate();
String UpperText(String text);
String CleanText(String text);
int8_t getWifiQuality();
String encodeHtmlString(String msg);
String getMoonPhasesIcon(uint8_t icon);
int getUVindexText(int index);
void checkDisplay();
void enableDisplay(boolean enable);
void loadDeviceSettings();
void saveDeviceSettings();
void loadSecuritySettings();
void saveSecuritySettings();
void resetToDefaults();
void configModeCallback(WiFiManager *myWiFiManager);
void redirectHome();
void handleHomePage();
void handleDevicePage();
void handleSaveDevice();
void handleSecurityPage();
void handleSaveSecurity();
void handleResetDefaults();
void saveCurrencySettings();
void loadCurrencySettings();
void handleCurrencyPage();
void handleSaveCurrency();
void handleDisplayPage();
void handleSaveDisplay();
void handleApiKeyPage();
void handleSaveApiKey();
void loadDisplaySettings();
void saveDisplaySettings();
void loadAllConfigs();
void removeAllConfigs();
void saveApiKeySettings();
void loadApiKeySettings();
void onOTAStart();
void onOTAProgress(size_t current, size_t final);
void onOTAEnd(bool success);

FrameCallback frames[] = {drawDateTime, drawCity, drawCurrentWeather, drawCurrentWeatherHum, drawCurrentWeatherAppe, drawWind, drawAirQuality, drawUVIndex, drawSun, drawMoon, drawCurrency1, drawCurrency2, drawWifiInfo, drawIPInfo, drawUntilToNextUpdate};
int numberOfFrames = 15;

OverlayCallback overlays[] = {drawHeaderOverlay};
int numberOfOverlays = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  LittleFS.begin();

  loadAllConfigs();
  // removeAllConfigs();

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
  display.drawString(64, 18, CleanText(LOG_TEXT[0]));
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 30, "By idezentas");
  display.drawString(64, 46, "V" + String(VERSION));
  display.display();
  delay(500);

  WiFiManager wifiManager;

  wifiManager.setAPCallback(configModeCallback);

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

  Serial.printf_P(PSTR("%s: %d%%\n"), CleanText(LOG_TEXT[1]).c_str(), getWifiQuality());
  Serial.println();

  delay(500);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, "SSID");
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 20, wifiManager.getWiFiSSID());
  display.drawString(64, 35, String(getWifiQuality()) + "%");
  display.display();
  delay(500);

  String webAddress = "http://" + WiFi.localIP().toString() + ":" + String(WEBSERVER_PORT) + "/";
  Serial.printf_P(PSTR("%s %s\n"), CleanText(LOG_TEXT[2]).c_str(), webAddress.c_str());
  Serial.println();

  delay(500);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 10, "IP");
  display.drawString(64, 30, WiFi.localIP().toString());
  display.display();
  delay(500);

  configTime(0, 0, NTP_SERVERS);

  Serial.println(CleanText(LOG_TEXT[23]).c_str());
  time_t now;
  const time_t VALID_TIME = 24 * 3600; // 1 gün (epoch sonrası)
  while ((now = time(nullptr)) < VALID_TIME)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(F("\n"));
  Serial.println(CleanText(LOG_TEXT[24]).c_str());

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

  server.on("/", HTTP_GET, handleHomePage);
  server.on("/device", HTTP_GET, handleDevicePage);
  server.on("/savedevice", HTTP_GET, handleSaveDevice);
  server.on("/security", HTTP_GET, handleSecurityPage);
  server.on("/savesecurity", HTTP_GET, handleSaveSecurity);
  server.on("/resetdefault", HTTP_GET, handleResetDefaults);
  server.on("/currency", HTTP_GET, handleCurrencyPage);
  server.on("/savecurrency", HTTP_GET, handleSaveCurrency);
  server.on("/display", HTTP_GET, handleDisplayPage);
  server.on("/savedisplay", HTTP_GET, handleSaveDisplay);
  server.on("/apikey", HTTP_GET, handleApiKeyPage);
  server.on("/saveapikey", HTTP_GET, handleSaveApiKey);
  server.onNotFound(redirectHome);

  ElegantOTA.begin(&server);
  ElegantOTA.setAuth(SysUser.c_str(), SysPass.c_str());
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
  server.begin();

  updateData(&display);
}

void loop()
{

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

  ElegantOTA.loop();
  server.handleClient();
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
  drawProgress(display, 15, CleanText(PROGRESS_TEXT[0]));
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);
  Serial.printf_P(PSTR("%s, %02d/%02d/%04d\n"), CleanText(WDAY_NAMES[timeInfo.tm_wday]).c_str(), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  Serial.printf_P(PSTR("%02d:%02d:%02d\n"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  Serial.println();
  delay(500);
  drawProgress(display, 30, CleanText(PROGRESS_TEXT[1]));
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrent(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.println();
  delay(1000);
  currentAirClient.updateCurrent(&currentAir, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.println();
  delay(1000);
  drawProgress(display, 45, CleanText(PROGRESS_TEXT[2]));
  SunMoonCalc *smCalc = new SunMoonCalc(Display_Timestamp, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  moonData = smCalc->calculateSunAndMoonData().moon;
  Serial.println();
  delay(500);
  drawProgress(display, 60, CleanText(PROGRESS_TEXT[3]));
  currentMeteoClient.updateWeather(&currentMeteo, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.println();
  delay(1000);
  drawProgress(display, 75, CleanText(PROGRESS_TEXT[4]));
  currencyClient.updateCurrency(&currencyData1, BaseCurrency1, TargetCurrency1);
  Serial.println();
  delay(1000);
  currencyClient.updateCurrency(&currencyData2, BaseCurrency2, TargetCurrency2);
  Serial.println();
  delay(1000);
  readyForWeatherUpdate = false;
  drawProgress(display, 100, CleanText(PROGRESS_TEXT[5]));
  delay(500);
}

void drawIPInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 5 + y, "IP");
  display->drawString(64 + x, 25 + y, WiFi.localIP().toString());
}

void drawWifiInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 0 + y, "SSID");
  display->setFont(ArialMT_Plain_10);
  display->drawString(64 + x, 20 + y, CleanText(SSID_String));
  display->drawString(64 + x, 35 + y, String(getWifiQuality()) + "%");
}

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);
  struct tm timeInfoGMT;
  gmtime_r(&Display_Timestamp, &timeInfoGMT);

  char buff[16];
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  display->drawString(64 + x, 10 + y, String(buff));

  display->setFont(ArialMT_Plain_10);
  sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), CleanText(WDAY_NAMES[timeInfo.tm_wday]).c_str(), timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  display->drawString(64 + x, 0 + y, String(buff));

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  boolean isDST = (timeInfo.tm_isdst) > 0;
  Display_TZ_NAME_SHORT = tzname[isDST ? 1 : 0];
  display->drawString(0 + x, 38 + y, Display_TZ_NAME_SHORT);

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  Display_GMTOffset = timeInfo.tm_hour - timeInfoGMT.tm_hour;
  (Display_GMTOffset >= 0) ? (sprintf_P(buff, PSTR("%s +%d"), "GMT", Display_GMTOffset)) : (sprintf_P(buff, PSTR("%s %d"), "GMT", Display_GMTOffset));
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
  display->drawString(64 + x, 0 + y, CleanText(TIME_UPDATE_TEXT[0]));

  display->drawString(64 + x, 10 + y, CleanText(TIME_UPDATE_TEXT[1]));
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
  String temp = String(currentWeather.temp, 1) + (IS_METRIC ? "°C" : "°F");
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

void drawCurrentWeatherAppe(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String tempFeel = String(currentWeather.feelsLike, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(50 + x, 5 + y, tempFeel);
  display->drawXbm(0 + x, 0 + y, 45, 45, weather_feels_like);
}

void drawWind(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String wind = (IS_METRIC ? String((currentWeather.windSpeed * 3.6), 1) : String((currentWeather.windSpeed), 1)) + (IS_METRIC ? " km/h" : " mph");
  display->drawString(55 + x, 10 + y, wind);

  int val = floor(currentWeather.windDeg / 45);
  int dirID = (val % 8);

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(90 + x, 38 + y, CleanText(WIND_DIRECTION_LONG[dirID]));

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
  int aqiIndex = (currentAir.aqi) - 1;
  display->drawString(62 + x, 14 + y, CleanText(AIR_QUALITY_TEXT[aqiIndex]));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_air_quality);
}

void drawUVIndex(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  int uv_int = currentMeteo.uv_index;
  display->drawString(62 + x, 14 + y, CleanText(UV_INDEX_TEXT[getUVindexText(uv_int)]));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_uv_index);
}

void drawMoon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  struct tm timeMoonRiseInfo;
  localtime_r(&moonData.rise, &timeMoonRiseInfo);

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[2]), timeMoonRiseInfo.tm_hour, timeMoonRiseInfo.tm_min);
  display->drawString(50 + x, 5 + y, String(buff));

  struct tm timeMoonSetInfo;
  localtime_r(&moonData.set, &timeMoonSetInfo);

  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[3]), timeMoonSetInfo.tm_hour, timeMoonSetInfo.tm_min);
  display->drawString(50 + x, 20 + y, String(buff));

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(90 + x, 38 + y, CleanText(SUN_MOON_TEXT[1]));

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 38 + y, CleanText(MOON_PHASES[moonData.phase.index]));

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
  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[2]), timeRiseInfo.tm_hour, timeRiseInfo.tm_min);
  display->drawString(50 + x, 5 + y, String(buff));

  time_t sunsetTimestamp = currentWeather.sunset;
  struct tm timeSetInfo;
  localtime_r(&sunsetTimestamp, &timeSetInfo);

  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[3]), timeSetInfo.tm_hour, timeSetInfo.tm_min);
  display->drawString(50 + x, 20 + y, String(buff));

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(25 + x, 0 + y, "B");

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(SUN_MOON_TEXT[0]));
}

void drawCurrency1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  sprintf_P(buff, PSTR("%s/%s"), UpperText(BaseCurrency1), UpperText(TargetCurrency1));
  display->drawString(64 + x, 5 + y, String(buff));

  sprintf_P(buff, PSTR("%.2f"), currencyData1.rate);
  display->drawString(64 + x, 25 + y, String(buff));
}

void drawCurrency2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  display->setFont(ArialMT_Plain_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  sprintf_P(buff, PSTR("%s/%s"), UpperText(BaseCurrency2), UpperText(TargetCurrency2));
  display->drawString(64 + x, 5 + y, String(buff));

  sprintf_P(buff, PSTR("%.2f"), currencyData2.rate);
  display->drawString(64 + x, 25 + y, String(buff));
}

void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state)
{
  time_t Display_Timestamp = time(nullptr);
  setenv("TZ", Display_TZ_POSIX.c_str(), 1);
  tzset();
  struct tm timeInfo;
  localtime_r(&Display_Timestamp, &timeInfo);

  char buff[16];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(30, 54, String(buff));

  display->drawString(0, 54, String(state->currentFrame + 1) + "/" + String(numberOfFrames));

  sprintf_P(buff, PSTR("%02d/%02d"), timeInfo.tm_mday, timeInfo.tm_mon + 1);
  display->drawString(85, 54, String(buff));

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

int getUVindexText(int index)
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
  Serial.println(CleanText(LOG_TEXT[3]).c_str());
  readyForWeatherUpdate = true;
}

void enableDisplay(boolean enable)
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
    Serial.println(CleanText(LOG_TEXT[4]).c_str() + currentTime);
  }
  else
  {
    display.displayOff();
    Serial.println(CleanText(LOG_TEXT[5]).c_str() + currentTime);
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
    Serial.println(CleanText(LOG_TEXT[6]).c_str() + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, CleanText(NIGHT_MODE_TEXT[0]));
    display.drawString(64, 30, CleanText(NIGHT_MODE_TEXT[2]));
    display.display();
    delay(1000);
    enableDisplay(true);
  }

  if (currentTime == timeDisplayTurnsOff && displayOn)
  {
    Serial.println(CleanText(LOG_TEXT[7]).c_str() + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, CleanText(NIGHT_MODE_TEXT[0]));
    display.drawString(64, 30, CleanText(NIGHT_MODE_TEXT[1]));
    display.display();
    delay(1000);
    enableDisplay(false);
  }
}

void loadDeviceSettings()
{
  if (!LittleFS.exists(configName))
  {
    Serial.println(configName);
    Serial.println(CleanText(LOG_TEXT[14]).c_str());
    Serial.println();
    return;
  }
  File f = LittleFS.open(configName, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]).c_str());
    Serial.println(error.c_str());
    f.close();
    return;
  }

  Display_GMTOffset = doc["device"]["Display_GMTOffset"].as<int>();
  Display_TZ_NAME = doc["device"]["Display_TZ_NAME"].as<String>();
  Display_TZ_POSIX = doc["device"]["Display_TZ_POSIX"].as<long>();
  Display_TZ_NAME_SHORT = doc["device"]["Display_TZ_NAME_SHORT"].as<String>();
  UPDATE_INTERVAL = doc["device"]["UPDATE_INTERVAL"].as<int>();
  OPEN_WEATHER_MAP_LOCATION_LAT = doc["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"].as<float>();
  OPEN_WEATHER_MAP_LOCATION_LON = doc["device"]["OPEN_WEATHER_MAP_LOCATION_LON"].as<float>();
  Display_City_Name = doc["device"]["Display_City_Name"].as<String>();
  Display_Country_Name = doc["device"]["Display_Country_Name"].as<String>();
  IS_METRIC = doc["device"]["IS_METRIC"].as<bool>();

  Serial.println();
  Serial.println(configName);
  Serial.printf_P(PSTR("Display_GMTOffset: %d\n"), doc["device"]["Display_GMTOffset"].as<int>());
  Serial.printf_P(PSTR("Display_TZ_NAME: %s\n"), doc["device"]["Display_TZ_NAME"].as<String>().c_str());
  Serial.printf_P(PSTR("Display_TZ_NAME_SHORT: %s\n"), doc["device"]["Display_TZ_NAME_SHORT"].as<String>().c_str());
  Serial.printf_P(PSTR("Display_TZ_POSIX: %ld\n"), doc["device"]["Display_TZ_POSIX"].as<long>());
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_LOCATION_LAT: %f\n"), doc["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"].as<float>());
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_LOCATION_LON: %f\n"), doc["device"]["OPEN_WEATHER_MAP_LOCATION_LON"].as<float>());
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_APP_ID: %s\n"), doc["device"]["OPEN_WEATHER_MAP_APP_ID"].as<String>().c_str());
  Serial.printf_P(PSTR("OPEN_CAGE_ID: %s\n"), doc["device"]["OPEN_CAGE_ID"].as<String>().c_str());
  Serial.printf_P(PSTR("Display_District_Name: %s\n"), doc["device"]["Display_District_Name"].as<String>().c_str());
  Serial.printf_P(PSTR("Display_City_Name: %s\n"), doc["device"]["Display_City_Name"].as<String>().c_str());
  Serial.printf_P(PSTR("Display_Country_Name: %s\n"), doc["device"]["Display_Country_Name"].as<String>().c_str());
  Serial.printf_P(PSTR("UPDATE_INTERVAL: %d\n"), doc["device"]["UPDATE_INTERVAL"].as<int>());
  Serial.printf_P(PSTR("IS_METRIC: %s\n"), doc["device"]["IS_METRIC"].as<bool>() ? "true" : "false");
  Serial.println();

  f.close();
}

void saveDeviceSettings()
{
  JsonDocument doc;
  doc["device"]["Display_GMTOffset"] = Display_GMTOffset;
  doc["device"]["Display_TZ_NAME"] = Display_TZ_NAME;
  doc["device"]["Display_TZ_POSIX"] = Display_TZ_POSIX;
  doc["device"]["Display_TZ_NAME_SHORT"] = Display_TZ_NAME_SHORT;
  doc["device"]["UPDATE_INTERVAL"] = UPDATE_INTERVAL;
  doc["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"] = OPEN_WEATHER_MAP_LOCATION_LAT;
  doc["device"]["OPEN_WEATHER_MAP_LOCATION_LON"] = OPEN_WEATHER_MAP_LOCATION_LON;
  doc["device"]["Display_City_Name"] = Display_City_Name;
  doc["device"]["Display_Country_Name"] = Display_Country_Name;
  doc["device"]["IS_METRIC"] = IS_METRIC;
  File f = LittleFS.open(configName, "w");
  serializeJson(doc, f);
  f.close();
}

void loadCurrencySettings()
{
  if (!LittleFS.exists(CurrencyConfig))
  {
    Serial.println(CurrencyConfig);
    Serial.println(CleanText(LOG_TEXT[14]).c_str());
    Serial.println();
    return;
  }
  File f = LittleFS.open(CurrencyConfig, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]).c_str());
    Serial.println(error.c_str());
    f.close();
    return;
  }

  BaseCurrency1 = doc["currency"]["BaseCurrency1"].as<String>();
  TargetCurrency1 = doc["currency"]["TargetCurrency1"].as<String>();
  BaseCurrency2 = doc["currency"]["BaseCurrency2"].as<String>();
  TargetCurrency2 = doc["currency"]["TargetCurrency2"].as<String>();

  Serial.println();
  Serial.println(CurrencyConfig);
  Serial.printf_P(PSTR("BaseCurrency1: %s\n"), doc["currency"]["BaseCurrency1"].as<String>().c_str());
  Serial.printf_P(PSTR("TargetCurrency1: %s\n"), doc["currency"]["TargetCurrency1"].as<String>().c_str());
  Serial.printf_P(PSTR("BaseCurrency2: %s\n"), doc["currency"]["BaseCurrency2"].as<String>().c_str());
  Serial.printf_P(PSTR("TargetCurrency2: %s\n"), doc["currency"]["TargetCurrency2"].as<String>().c_str());
  Serial.println();

  f.close();
}

void saveCurrencySettings()
{
  JsonDocument doc;
  doc["currency"]["BaseCurrency1"] = BaseCurrency1;
  doc["currency"]["TargetCurrency1"] = TargetCurrency1;
  doc["currency"]["BaseCurrency2"] = BaseCurrency2;
  doc["currency"]["TargetCurrency2"] = TargetCurrency2;
  File f = LittleFS.open(CurrencyConfig, "w");
  serializeJson(doc, f);
  f.close();
}

void loadSecuritySettings()
{
  if (!LittleFS.exists(securityConfig))
  {
    Serial.println(securityConfig);
    Serial.println(CleanText(LOG_TEXT[14]).c_str());
    Serial.println();
    return;
  }
  File f = LittleFS.open(securityConfig, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]).c_str());
    Serial.println(error.c_str());
    f.close();
    return;
  }
  SysUser = doc["security"]["user"].as<String>();
  SysPass = doc["security"]["pass"].as<String>();

  Serial.println();
  Serial.println(securityConfig);
  Serial.printf_P(PSTR("SysUser: %s\n"), doc["security"]["user"].as<String>().c_str());
  Serial.printf_P(PSTR("SysPass: %s\n"), doc["security"]["pass"].as<String>().c_str());
  Serial.println();

  f.close();
}

void saveSecuritySettings()
{
  JsonDocument doc;
  doc["security"]["user"] = SysUser;
  doc["security"]["pass"] = SysPass;
  File f = LittleFS.open(securityConfig, "w");
  serializeJson(doc, f);
  f.close();
}

void loadDisplaySettings()
{
  if (!LittleFS.exists(displayConfig))
  {
    Serial.println(displayConfig);
    Serial.println(CleanText(LOG_TEXT[14]).c_str());
    Serial.println();
    return;
  }
  File f = LittleFS.open(displayConfig, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]).c_str());
    Serial.println(error.c_str());
    f.close();
    return;
  }

  INVERT_DISPLAY = doc["display"]["INVERT_DISPLAY"].as<bool>();
  timeDisplayTurnsOn = doc["display"]["timeDisplayTurnsOn"].as<String>();
  timeDisplayTurnsOff = doc["display"]["timeDisplayTurnsOff"].as<String>();
  Disp_Contrast = doc["display"]["Disp_Contrast"].as<int>();

  Serial.println();
  Serial.println(displayConfig);
  Serial.printf_P(PSTR("INVERT_DISPLAY: %s\n"), doc["display"]["INVERT_DISPLAY"].as<bool>() ? "true" : "false");
  Serial.printf_P(PSTR("timeDisplayTurnsOn: %s\n"), doc["display"]["timeDisplayTurnsOn"].as<String>().c_str());
  Serial.printf_P(PSTR("timeDisplayTurnsOff: %s\n"), doc["display"]["timeDisplayTurnsOff"].as<String>().c_str());
  Serial.printf_P(PSTR("Disp_Contrast: %d\n"), doc["display"]["Disp_Contrast"].as<int>());
  Serial.println();

  f.close();
}

void saveDisplaySettings()
{
  JsonDocument doc;
  doc["display"]["INVERT_DISPLAY"] = INVERT_DISPLAY;
  doc["display"]["timeDisplayTurnsOn"] = timeDisplayTurnsOn;
  doc["display"]["timeDisplayTurnsOff"] = timeDisplayTurnsOff;
  doc["display"]["Disp_Contrast"] = Disp_Contrast;
  File f = LittleFS.open(displayConfig, "w");
  serializeJson(doc, f);
  f.close();
}

void loadApiKeySettings()
{
  if (!LittleFS.exists(ApiKeyConfig))
  {
    Serial.println(ApiKeyConfig);
    Serial.println(CleanText(LOG_TEXT[14]).c_str());
    Serial.println();
    return;
  }
  File f = LittleFS.open(ApiKeyConfig, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]).c_str());
    Serial.println(error.c_str());
    f.close();
    return;
  }

  OPEN_WEATHER_MAP_APP_ID = doc["apikey"]["OPEN_WEATHER_MAP_APP_ID"].as<String>();
  OPEN_CAGE_ID = doc["apikey"]["OPEN_CAGE_ID"].as<String>();

  Serial.println();
  Serial.println(ApiKeyConfig);
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_APP_ID: %s\n"), doc["apikey"]["OPEN_WEATHER_MAP_APP_ID"].as<String>().c_str());
  Serial.printf_P(PSTR("OPEN_CAGE_ID: %s\n"), doc["apikey"]["OPEN_CAGE_ID"].as<String>().c_str());
  Serial.println();

  f.close();
}

void saveApiKeySettings()
{
  JsonDocument doc;
  doc["apikey"]["OPEN_WEATHER_MAP_APP_ID"] = OPEN_WEATHER_MAP_APP_ID;
  doc["apikey"]["OPEN_CAGE_ID"] = OPEN_CAGE_ID;
  File f = LittleFS.open(ApiKeyConfig, "w");
  serializeJson(doc, f);
  f.close();
}

void removeAllConfigs()
{
  LittleFS.remove(configName);
  LittleFS.remove(securityConfig);
  LittleFS.remove(CurrencyConfig);
  LittleFS.remove(displayConfig);
  LittleFS.remove(ApiKeyConfig);
}

void loadAllConfigs()
{
  loadApiKeySettings();
  loadDeviceSettings();
  loadDisplaySettings();
  loadSecuritySettings();
  loadCurrencySettings();
}

void resetToDefaults()
{
  LittleFS.remove(configName);
  LittleFS.remove(securityConfig);
  LittleFS.remove(CurrencyConfig);
  LittleFS.remove(displayConfig);
  LittleFS.remove(ApiKeyConfig);

  JsonDocument docD;
  docD["device"]["Display_GMTOffset"] = 3;
  docD["device"]["Display_TZ_NAME"] = "Europe/Istanbul";
  docD["device"]["Display_TZ_POSIX"] = "<+03>-3";
  docD["device"]["Display_TZ_NAME_SHORT"] = "+03";
  docD["device"]["UPDATE_INTERVAL"] = 30;
  docD["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"] = 41.006381;
  docD["device"]["OPEN_WEATHER_MAP_LOCATION_LON"] = 28.9758715;
  docD["device"]["Display_City_Name"] = "İstanbul";
  docD["device"]["Display_Country_Name"] = "Türkiye";
  docD["device"]["IS_METRIC"] = true;

  JsonDocument docS;
  docS["security"]["user"] = SysUser;
  docS["security"]["pass"] = SysPass;

  JsonDocument docCur;
  docCur["currency"]["BaseCurrency1"] = "eur";
  docCur["currency"]["TargetCurrency1"] = "try";
  docCur["currency"]["BaseCurrency2"] = "usd";
  docCur["currency"]["TargetCurrency2"] = "try";

  JsonDocument docDisp;
  docDisp["display"]["INVERT_DISPLAY"] = false;
  docDisp["display"]["timeDisplayTurnsOn"] = "08:00";
  docDisp["display"]["timeDisplayTurnsOff"] = "00:00";
  docDisp["display"]["Disp_Contrast"] = 255;

  JsonDocument docApi;
  docApi["apikey"]["OPEN_CAGE_ID"] = "";
  docApi["apikey"]["OPEN_WEATHER_MAP_APP_ID"] = "";

  File d = LittleFS.open(configName, "w");
  serializeJson(docD, d);
  d.close();
  File s = LittleFS.open(securityConfig, "w");
  serializeJson(docS, s);
  s.close();
  File cur = LittleFS.open(CurrencyConfig, "w");
  serializeJson(docCur, cur);
  cur.close();
  File disp = LittleFS.open(displayConfig, "w");
  serializeJson(docDisp, disp);
  disp.close();
  File apik = LittleFS.open(ApiKeyConfig, "w");
  serializeJson(docApi, apik);
  apik.close();
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println(CleanText(LOG_TEXT[9]).c_str());
  Serial.println(WiFi.softAPIP());

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "WiFiManager");
  display.drawString(64, 10, CleanText(LOG_TEXT[10]));
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 26, myWiFiManager->getConfigPortalSSID());
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 46, CleanText(LOG_TEXT[11]));
  display.display();

  Serial.println(F("WiFiManager"));
  Serial.println(CleanText(LOG_TEXT[10]).c_str());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println(CleanText(LOG_TEXT[11]).c_str());
}

void redirectHome()
{
  server.sendHeader(F("Location"), "/", true);
  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.send(303);
  server.client().stop();
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
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%DEVICESETTINGS%", HTML_TEXT[1]);
  html.replace("%CITYTEXT%", HTML_TEXT[7]);
  html.replace("%CITYPLACE%", HTML_TEXT[8]);
  html.replace("%COUNTRYTEXT%", HTML_TEXT[9]);
  html.replace("%COUNTRYPLACE%", HTML_TEXT[10]);
  html.replace("%FINDGEOTIME%", HTML_TEXT[11]);
  html.replace("%LATTEXT%", HTML_TEXT[12]);
  html.replace("%LATPLACE%", HTML_TEXT[13]);
  html.replace("%LONTEXT%", HTML_TEXT[14]);
  html.replace("%LONPLACE%", HTML_TEXT[15]);
  html.replace("%TZNAMETEXT%", HTML_TEXT[16]);
  html.replace("%UNITTEXT%", HTML_TEXT[17]);
  html.replace("%METRICTEXT%", HTML_TEXT[18]);
  html.replace("%IMPERIALTEXT%", HTML_TEXT[19]);
  html.replace("%INTERVALTEXT%", HTML_TEXT[20]);
  html.replace("%SAVETEXT%", HTML_TEXT[21]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[22]);
  html.replace("%GEOALERT%", HTML_TEXT[23]);
  html.replace("%GEOALERT2%", HTML_TEXT[24]);
  html.replace("%GEOALERT3%", HTML_TEXT[25]);
  html.replace("%FORMSENDING%", HTML_TEXT[52]);
  html.replace("%TZALERT1%", HTML_TEXT[59]);
  html.replace("%TZALERT2%", HTML_TEXT[60]);
  html.replace("%TZALERT3%", HTML_TEXT[61]);
  html.replace("%TZNAME%", Display_TZ_NAME);
  html.replace("%LAT%", String(OPEN_WEATHER_MAP_LOCATION_LAT));
  html.replace("%LON%", String(OPEN_WEATHER_MAP_LOCATION_LON));
  html.replace("%CITY%", Display_City_Name);
  html.replace("%COUNTRY%", Display_Country_Name);
  html.replace("%GEOAPIKEY%", OPEN_CAGE_ID);
  html.replace("%METRIC%", IS_METRIC ? "selected" : "");
  html.replace("%IMPERIAL%", !IS_METRIC ? "selected" : "");
  html.replace("%UP10%", (UPDATE_INTERVAL == 10) ? "selected" : "");
  html.replace("%UP15%", (UPDATE_INTERVAL == 15) ? "selected" : "");
  html.replace("%UP20%", (UPDATE_INTERVAL == 20) ? "selected" : "");
  html.replace("%UP25%", (UPDATE_INTERVAL == 25) ? "selected" : "");
  html.replace("%UP30%", (UPDATE_INTERVAL == 30) ? "selected" : "");
  html.replace("%UP45%", (UPDATE_INTERVAL == 45) ? "selected" : "");
  html.replace("%UP60%", (UPDATE_INTERVAL == 60) ? "selected" : "");
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
    if (val == "up10")
    {
      UPDATE_INTERVAL = 10;
    }
    if (val == "up15")
    {
      UPDATE_INTERVAL = 15;
    }
    if (val == "up20")
    {
      UPDATE_INTERVAL = 20;
    }
    if (val == "up25")
    {
      UPDATE_INTERVAL = 25;
    }
    if (val == "up30")
    {
      UPDATE_INTERVAL = 30;
    }
    if (val == "up45")
    {
      UPDATE_INTERVAL = 45;
    }
    if (val == "up60")
    {
      UPDATE_INTERVAL = 60;
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

  Serial.println();
  Serial.println(CleanText(LOG_TEXT[12]).c_str());
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
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%DEVICESETTINGS%", HTML_TEXT[1]);
  html.replace("%DEVICETEXT%", HTML_TEXT[54]);
  server.sendContent(html);

  server.client().stop();

  checkDisplay();
  updateData(&display);
}

void handleCurrencyPage()
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

  String html = FPSTR(Currency_Page);
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%SAVETEXT%", HTML_TEXT[21]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[22]);
  html.replace("%CURRENCYSETTINGS%", HTML_TEXT[43]);
  html.replace("%APIWARNINGTEXT%", HTML_TEXT[44]);
  html.replace("%CURRENCY1TEXT%", HTML_TEXT[45]);
  html.replace("%BASETEXT%", HTML_TEXT[46]);
  html.replace("%TARGETTEXT%", HTML_TEXT[47]);
  html.replace("%CURRENCY2TEXT%", HTML_TEXT[48]);
  html.replace("%CURRENCYALERT%", HTML_TEXT[50]);
  html.replace("%CURRENCYALERT2%", HTML_TEXT[53]);
  html.replace("%FORMSENDING%", HTML_TEXT[52]);
  html.replace("%BASE1%", BaseCurrency1);
  html.replace("%TARGET1%", TargetCurrency1);
  html.replace("%BASE2%", BaseCurrency2);
  html.replace("%TARGET2%", TargetCurrency2);
  server.sendContent(html);

  server.client().stop();
}

void handleSaveCurrency()
{
  if (server.hasArg("base1"))
  {
    BaseCurrency1 = server.arg("base1");
  }

  if (server.hasArg("target1"))
  {
    TargetCurrency1 = server.arg("target1");
  }

  if (server.hasArg("base2"))
  {
    BaseCurrency2 = server.arg("base2");
  }

  if (server.hasArg("target2"))
  {
    TargetCurrency2 = server.arg("target2");
  }

  Serial.println();
  Serial.println(CleanText(LOG_TEXT[15]).c_str());
  Serial.printf_P(PSTR("BaseCurrency1: %s\n"), BaseCurrency1.c_str());
  Serial.printf_P(PSTR("TargetCurrency1: %s\n"), TargetCurrency1.c_str());
  Serial.printf_P(PSTR("BaseCurrency2: %s\n"), BaseCurrency2.c_str());
  Serial.printf_P(PSTR("TargetCurrency2: %s\n"), TargetCurrency2.c_str());
  Serial.println();

  saveCurrencySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Currency_Page);
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%CURRENCYSETTINGS%", HTML_TEXT[43]);
  html.replace("%CURRENCYTEXT%", HTML_TEXT[49]);
  server.sendContent(html);

  server.client().stop();

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
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%APIKEYSETTINGS%", HTML_TEXT[57]);
  html.replace("%OWMAPIKEYTEXT%", HTML_TEXT[3]);
  html.replace("%APIPLACE%", HTML_TEXT[4]);
  html.replace("%SHOWPASS%", HTML_TEXT[5]);
  html.replace("%GEOAPIKEYTEXT%", HTML_TEXT[6]);
  html.replace("%SAVETEXT%", HTML_TEXT[21]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[22]);
  html.replace("%HIDEPASS%", HTML_TEXT[35]);
  html.replace("%GETFROMTEXT%", HTML_TEXT[41]);
  html.replace("%HERETEXT%", HTML_TEXT[42]);
  html.replace("%FORMSENDING%", HTML_TEXT[52]);
  html.replace("%OWMAPIKEY%", OPEN_WEATHER_MAP_APP_ID);
  html.replace("%GEOAPIKEY%", OPEN_CAGE_ID);
  server.sendContent(html);

  server.sendContent("");
  server.client().stop();
}

void handleSaveApiKey()
{

  if (server.hasArg("OWMApiKey"))
  {
    OPEN_WEATHER_MAP_APP_ID = server.arg("OWMApiKey");
  }

  if (server.hasArg("geoApiKey"))
  {
    OPEN_CAGE_ID = server.arg("geoApiKey");
  }

  Serial.println();
  Serial.println(CleanText(LOG_TEXT[22]).c_str());
  Serial.printf_P(PSTR("OPEN_WEATHER_MAP_APP_ID: %s\n"), OPEN_WEATHER_MAP_APP_ID.c_str());
  Serial.printf_P(PSTR("OPEN_CAGE_ID: %s\n"), OPEN_CAGE_ID.c_str());
  Serial.println();

  saveApiKeySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_ApiKey_Page);
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%APIKEYSETTINGS%", HTML_TEXT[57]);
  html.replace("%APIKEYTEXT%", HTML_TEXT[58]);
  server.sendContent(html);

  server.client().stop();

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
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%HOMETITLE%", HTML_TEXT[0]);
  html.replace("%DEVICESETTINGS%", HTML_TEXT[1]);
  html.replace("%SECURITYSETTINGS%", HTML_TEXT[2]);
  html.replace("%CURRENCYSETTINGS%", HTML_TEXT[43]);
  html.replace("%DEFAULTSETTINGS%", HTML_TEXT[33]);
  html.replace("%DEFAULTALERT%", HTML_TEXT[34]);
  html.replace("%OTATEXT%", HTML_TEXT[36]);
  html.replace("%DISPLAYSETTINGS%", HTML_TEXT[55]);
  html.replace("%APIKEYSETTINGS%", HTML_TEXT[57]);
  html.replace("%ABOUTTEXT%", HTML_TEXT[26]);
  html.replace("%VERSION%", "V" + String(VERSION));
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
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%SECURITYSETTINGS%", HTML_TEXT[2]);
  html.replace("%SHOWPASS%", HTML_TEXT[5]);
  html.replace("%SAVETEXT%", HTML_TEXT[21]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[22]);
  html.replace("%PASSWORDTEXT%", HTML_TEXT[27]);
  html.replace("%USERTEXT%", HTML_TEXT[28]);
  html.replace("%USERPLACE%", HTML_TEXT[29]);
  html.replace("%PASSWORDPLACE2%", HTML_TEXT[30]);
  html.replace("%HIDEPASS%", HTML_TEXT[35]);
  html.replace("%USER%", SysUser);
  html.replace("%PASSWORD%", SysPass);
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

  Serial.println(CleanText(LOG_TEXT[13]).c_str());
  Serial.printf_P(PSTR("SysUser: %s\n"), SysUser.c_str());
  Serial.printf_P(PSTR("SysPass: %s\n"), SysPass.c_str());

  saveSecuritySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Security_Page);
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%SECURITYSETTINGS%", HTML_TEXT[2]);
  html.replace("%DEVICETEXT%", HTML_TEXT[53]);
  server.sendContent(html);

  server.client().stop();

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
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%SAVETEXT%", HTML_TEXT[21]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[22]);
  html.replace("%TIMEONTEXT%", HTML_TEXT[37]);
  html.replace("%TIMEOFFTEXT%", HTML_TEXT[38]);
  html.replace("%INVDISPTEXT%", HTML_TEXT[51]);
  html.replace("%FORMSENDING%", HTML_TEXT[52]);
  html.replace("%DISPLAYSETTINGS%", HTML_TEXT[55]);
  html.replace("%DISPLAYCONTRAST%", HTML_TEXT[62]);
  html.replace("%TIMEON%", timeDisplayTurnsOn);
  html.replace("%TIMEOFF%", timeDisplayTurnsOff);
  html.replace("%SETCONTRAST%", String(Disp_Contrast));
  html.replace("%IS_INVDISP_CHECKED%", INVERT_DISPLAY ? "checked" : "");
  server.sendContent(html);

  server.client().stop();
}

void handleSaveDisplay()
{
  boolean flipOld = INVERT_DISPLAY;
  int conOld = Disp_Contrast;

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

  INVERT_DISPLAY = server.hasArg("invDisp");

  Serial.println();
  Serial.println(CleanText(LOG_TEXT[12]).c_str());
  Serial.printf_P(PSTR("INVERT_DISPLAY: %s\n"), INVERT_DISPLAY ? "true" : "false");
  Serial.printf_P(PSTR("timeDisplayTurnsOn: %s\n"), timeDisplayTurnsOn.c_str());
  Serial.printf_P(PSTR("timeDisplayTurnsOff: %s\n"), timeDisplayTurnsOff.c_str());
  Serial.printf_P(PSTR("Disp_Contrast: %d\n"), Disp_Contrast);
  Serial.println();

  saveDisplaySettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Save_Display_Page);
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%DISPLAYSETTINGS%", HTML_TEXT[55]);
  html.replace("%DISPLAYTEXT%", HTML_TEXT[56]);
  server.sendContent(html);

  server.client().stop();

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

  resetToDefaults();

  WiFiManager wifiManager;
  wifiManager.resetSettings();

  server.sendHeader(F("Cache-Control"), F("no-cache, no-store"));
  server.sendHeader(F("Pragma"), F("no-cache"));
  server.sendHeader(F("Expires"), "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), "");

  String html = FPSTR(Reset_Default_Page);
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%DEFAULTSETTINGS%", HTML_TEXT[43]);
  html.replace("%DEFAULTTEXT%", HTML_TEXT[44]);
  server.sendContent(html);

  server.client().stop();

  delay(2000);
  ESP.restart();
}

void onOTAStart()
{
  Serial.println(CleanText(LOG_TEXT[16]).c_str());
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 10, CleanText(OTA_TEXT[0]));
  display.drawString(64, 30, CleanText(OTA_TEXT[1]));
  display.display();
  delay(500);
}

void onOTAProgress(size_t current, size_t final)
{
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000)
  {
    ota_progress_millis = millis();
    Serial.printf_P(PSTR("%s %s: %u bytes, %s: %u bytes\n"), CleanText(LOG_TEXT[17]).c_str(), CleanText(LOG_TEXT[18]).c_str(), current, CleanText(LOG_TEXT[19]).c_str(), final);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, CleanText(OTA_TEXT[0]));
    display.drawString(64, 30, CleanText(OTA_TEXT[3]));
    display.display();
  }
}

void onOTAEnd(bool success)
{
  if (success)
  {
    Serial.println(CleanText(LOG_TEXT[20]).c_str());
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, CleanText(OTA_TEXT[0]));
    display.drawString(64, 30, CleanText(OTA_TEXT[2]));
    display.display();
    delay(500);
    display.displayOff();
  }
  else
  {
    Serial.println(CleanText(LOG_TEXT[21]).c_str());
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, CleanText(OTA_TEXT[0]));
    display.drawString(64, 30, CleanText(OTA_TEXT[4]));
    display.display();
    delay(500);
  }
}