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
#include "OpenMeteoApi.h"
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

OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapCurrent currentWeatherClient;

OpenWeatherMapAirData currentAir;
OpenWeatherMapAir currentAirClient;

OpenMeteoApiData currentMeteo;
OpenMeteoApi currentMeteoClient;

FloatRates currencyClient;
FloatRatesData currencyData1;
FloatRatesData currencyData2;

SunMoonCalc::Moon moonData;

int TZ_MN = ((TZ) * 60);
int TZ_SEC = ((TZ) * 3600);
int DST_SEC = ((DST_MN) * 60);
time_t now;

bool readyForWeatherUpdate = false;

String lastUpdate = "--";

unsigned long timeSinceLastWUpdate = 0;

boolean displayOn = true;

String SSID_String;

ESP8266WebServer server(WEBSERVER_PORT);

void drawProgress(OLEDDisplay *display, int percentage, String label);
void updateData(OLEDDisplay *display);
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeatherHum(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeatherMin(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeatherMax(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWind(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawAirQuality(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawMoon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawSun(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state);
void drawCity(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawIPInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWifiInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWordlClock1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWordlClock2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawWordlClock3(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawUVIndex(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
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
void handleResetDeafults();
void handleWorldClockPage();
void handleSaveWorldClock();
void loadWorldClockSettings();
void saveWorldClockSettings();
void saveCurrencySettings();
void loadCurrencySettings();
void handleCurrencyPage();
void handleSaveCurrency();

FrameCallback frames[] = {drawDateTime, drawCity, drawCurrentWeather, drawCurrentWeatherHum, drawCurrentWeatherMin, drawCurrentWeatherMax, drawWind, drawAirQuality, drawUVIndex, drawSun, drawMoon, drawWordlClock1, drawWordlClock2, drawWordlClock3, drawCurrency1, drawCurrency2, drawWifiInfo, drawIPInfo};
int numberOfFrames = 18;

OverlayCallback overlays[] = {drawHeaderOverlay};
int numberOfOverlays = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  LittleFS.begin();

  loadDeviceSettings();
  loadSecuritySettings();
  loadWorldClockSettings();
  loadCurrencySettings();

  display.init();
  display.clear();
  display.display();

  // display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);

  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 1, "ESP8266");
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 18, CleanText(LOG_TEXT[0]));
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 30, "By idezentas");
  display.drawString(64, 46, "V" + String(VERSION));
  display.display();
  delay(1000);

  WiFiManager wifiManager;

  wifiManager.setAPCallback(configModeCallback);

  String hostname(HOSTNAME);
  if (!wifiManager.autoConnect((const char *)hostname.c_str()))
  { // new addition
    delay(3000);
    WiFi.disconnect(true);
    ESP.reset();
    delay(5000);
  }

  Serial.print("SSID: ");
  Serial.println(wifiManager.getWiFiSSID());
  SSID_String = wifiManager.getWiFiSSID();
  Serial.println();

  Serial.print(CleanText(LOG_TEXT[1]));
  Serial.print(getWifiQuality());
  Serial.println("%");
  Serial.println();

  delay(1000);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, "SSID");
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 20, wifiManager.getWiFiSSID());
  display.drawString(64, 35, String(getWifiQuality()) + "%");
  display.display();
  delay(1000);

  String webAddress = "http://" + WiFi.localIP().toString() + ":" + String(WEBSERVER_PORT) + "/";
  Serial.println(CleanText(LOG_TEXT[2]) + webAddress);
  Serial.println();

  delay(1000);
  display.clear();
  display.display();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 10, "IP");
  display.drawString(64, 30, WiFi.localIP().toString());
  display.display();
  delay(1000);

  configTime(TZ_SEC, DST_SEC, NTP_SERVERS);

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

  Serial.println();

  server.on("/", HTTP_GET, handleHomePage);
  server.on("/device", HTTP_GET, handleDevicePage);
  server.on("/savedevice", HTTP_GET, handleSaveDevice);
  server.on("/security", HTTP_GET, handleSecurityPage);
  server.on("/savesecurity", HTTP_GET, handleSaveSecurity);
  server.on("/resetdefault", HTTP_GET, handleResetDeafults);
  server.on("/worldclock", HTTP_GET, handleWorldClockPage);
  server.on("/saveworldclock", HTTP_GET, handleSaveWorldClock);
  server.on("/currency", HTTP_GET, handleCurrencyPage);
  server.on("/savecurrency", HTTP_GET, handleSaveCurrency);
  server.onNotFound(redirectHome);

  ElegantOTA.begin(&server);
  ElegantOTA.setAuth(SysUser.c_str(), SysPass.c_str());
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

  ui.update();

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
  delay(1000);
  drawProgress(display, 30, CleanText(PROGRESS_TEXT[1]));
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrent(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.println();
  delay(1000);
  drawProgress(display, 45, CleanText(PROGRESS_TEXT[6]));
  currentMeteoClient.updateWeather(&currentMeteo, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  delay(1000);
  Serial.println();
  drawProgress(display, 60, CleanText(PROGRESS_TEXT[3]));
  currentAirClient.updateCurrent(&currentAir, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.println();
  delay(1000);
  drawProgress(display, 75, CleanText(PROGRESS_TEXT[4]));
  SunMoonCalc *smCalc = new SunMoonCalc(now, OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  moonData = smCalc->calculateSunAndMoonData().moon;
  Serial.println();
  delay(1000);
  drawProgress(display, 90, CleanText(PROGRESS_TEXT[7]));
  currencyClient.updateCurrency(&currencyData1, BaseCurrency1, TargetCurrency1);
  Serial.println();
  delay(1000);
  currencyClient.updateCurrency(&currencyData2, BaseCurrency2, TargetCurrency2);
  Serial.println();
  delay(1000);
  readyForWeatherUpdate = false;
  drawProgress(display, 100, CleanText(PROGRESS_TEXT[5]));
  delay(1000);
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
  now = time(nullptr);
  struct tm *timeInfo;
  timeInfo = localtime(&now);
  char buff[16];

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), CleanText(WDAY_NAMES[timeInfo->tm_wday]).c_str(), timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
  display->drawString(64 + x, 0 + y, String(buff));

  display->setFont(ArialMT_Plain_24);
  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display->drawString(64 + x, 10 + y, String(buff));

  display->setFont(ArialMT_Plain_10);
  (TZ >= 0) ? (sprintf_P(buff, PSTR("%s +%d"), "GMT", TZ)) : (sprintf_P(buff, PSTR("%s %d"), "GMT", TZ));
  display->drawString(64 + x, 40 + y, String(buff));
}

void drawCity(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 0 + y, UpperText(CleanText(Display_District_Name)));
  display->drawString(64 + x, 15 + y, UpperText(CleanText(Display_City_Name)));
  display->drawString(64 + x, 30 + y, Display_Country_Name);
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

void drawCurrentWeatherMin(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String tempMi = String(currentWeather.tempMin, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 5 + y, tempMi);
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_min_temp);
}

void drawCurrentWeatherMax(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String tempMa = String(currentWeather.tempMax, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 5 + y, tempMa);
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_max_temp);
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
  display->drawString(90 + x, 35 + y, CleanText(WIND_DIRECTION_LONG[dirID]));

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
  display->drawString(62 + x, 16 + y, CleanText(AIR_QUALITY_TEXT[aqiIndex]));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_air_quality);
}

void drawUVIndex(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  int uv_int = currentMeteo.uv_index;
  display->drawString(62 + x, 16 + y, CleanText(UV_INDEX_TEXT[getUVindexText(uv_int)]));
  display->drawXbm(10 + x, 0 + y, 45, 45, weather_uv_index);
}

void drawMoon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  char buff[16];
  struct tm *timeMoonRiseInfo;
  timeMoonRiseInfo = localtime(&moonData.rise);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[2]), timeMoonRiseInfo->tm_hour, timeMoonRiseInfo->tm_min);
  display->drawString(50 + x, 5 + y, String(buff));

  struct tm *timeMoonSetInfo;
  timeMoonSetInfo = localtime(&moonData.set);
  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[3]), timeMoonSetInfo->tm_hour, timeMoonSetInfo->tm_min);
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
  struct tm *timeRiseInfo = localtime(&sunriseTimestamp);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[2]), timeRiseInfo->tm_hour, timeRiseInfo->tm_min);
  display->drawString(50 + x, 5 + y, String(buff));

  time_t sunsetTimestamp = currentWeather.sunset;
  struct tm *timeSetInfo = localtime(&sunsetTimestamp);
  sprintf_P(buff, PSTR("%0s: %02d:%02d"), CleanText(SUN_MOON_TEXT[3]), timeSetInfo->tm_hour, timeSetInfo->tm_min);
  display->drawString(50 + x, 20 + y, String(buff));

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(25 + x, 0 + y, "B");

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(SUN_MOON_TEXT[0]));
}

void drawWordlClock1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  now = time(nullptr);
  time_t wordlClockTimestamp = now + ((World_Clock1_TZ - TZ) * 3600);
  struct tm *timeInfo;
  timeInfo = localtime(&wordlClockTimestamp);
  char buff[16];
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(64 + x, 0 + y, CleanText(WDAY_NAMES[timeInfo->tm_wday]).c_str());

  sprintf_P(buff, PSTR("%02d/%02d/%04d"), timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
  display->drawString(64 + x, 12 + y, String(buff));

  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  display->drawString(64 + x, 25 + y, String(buff));

  (World_Clock1_TZ >= 0) ? (sprintf_P(buff, PSTR("%s +%d"), "GMT", World_Clock1_TZ)) : (sprintf_P(buff, PSTR("%s %d"), "GMT", World_Clock1_TZ));
  display->drawString(0 + x, 0 + y, String(buff));

  display->drawString(0 + x, 12 + y, World_Clock1_TZ_NAME_SHORT);

  display->drawString(0 + x, 25 + y, World_Clock1_Country_Name);

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(World_Clock1_City_Name));
}

void drawWordlClock2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  now = time(nullptr);
  time_t wordlClockTimestamp = now + ((World_Clock2_TZ - TZ) * 3600);
  struct tm *timeInfo;
  timeInfo = localtime(&wordlClockTimestamp);
  char buff[16];
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(64 + x, 0 + y, CleanText(WDAY_NAMES[timeInfo->tm_wday]).c_str());

  sprintf_P(buff, PSTR("%02d/%02d/%04d"), timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
  display->drawString(64 + x, 12 + y, String(buff));

  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  display->drawString(64 + x, 25 + y, String(buff));

  (World_Clock2_TZ >= 0) ? (sprintf_P(buff, PSTR("%s +%d"), "GMT", World_Clock2_TZ)) : (sprintf_P(buff, PSTR("%s %d"), "GMT", World_Clock2_TZ));
  display->drawString(0 + x, 0 + y, String(buff));

  display->drawString(0 + x, 12 + y, World_Clock2_TZ_NAME_SHORT);

  display->drawString(0 + x, 25 + y, World_Clock2_Country_Name);

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(World_Clock2_City_Name));
}

void drawWordlClock3(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  now = time(nullptr);
  time_t wordlClockTimestamp = now + ((World_Clock3_TZ - TZ) * 3600);
  struct tm *timeInfo;
  timeInfo = localtime(&wordlClockTimestamp);
  char buff[16];
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(64 + x, 0 + y, CleanText(WDAY_NAMES[timeInfo->tm_wday]).c_str());

  sprintf_P(buff, PSTR("%02d/%02d/%04d"), timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
  display->drawString(64 + x, 12 + y, String(buff));

  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  display->drawString(64 + x, 25 + y, String(buff));

  (World_Clock3_TZ >= 0) ? (sprintf_P(buff, PSTR("%s +%d"), "GMT", World_Clock3_TZ)) : (sprintf_P(buff, PSTR("%s %d"), "GMT", World_Clock3_TZ));
  display->drawString(0 + x, 0 + y, String(buff));

  display->drawString(0 + x, 12 + y, World_Clock3_TZ_NAME_SHORT);

  display->drawString(0 + x, 25 + y, World_Clock3_Country_Name);

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, CleanText(World_Clock3_City_Name));
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
  now = time(nullptr);
  struct tm *timeInfo;
  timeInfo = localtime(&now);
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(30, 54, String(buff));

  display->drawString(0, 54, String(state->currentFrame + 1) + "/" + String(numberOfFrames));

  sprintf_P(buff, PSTR("%02d/%02d/%04d"), timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
  display->drawString(62, 54, String(buff));

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
  Serial.println(CleanText(LOG_TEXT[3]));
  readyForWeatherUpdate = true;
}

void enableDisplay(boolean enable)
{
  now = time(nullptr);
  struct tm *timeInfo;
  timeInfo = localtime(&now);
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  String currentTime = String(buff);

  displayOn = enable;
  if (enable)
  {
    display.displayOn();
    updateData(&display);
    Serial.println(CleanText(LOG_TEXT[4]) + currentTime);
  }
  else
  {
    display.displayOff();
    Serial.println(CleanText(LOG_TEXT[5]) + currentTime);
  }
}

void checkDisplay()
{
  if (timeDisplayTurnsOn == "" || timeDisplayTurnsOff == "")
  {
    return;
  }

  now = time(nullptr);
  struct tm *timeInfo;
  timeInfo = localtime(&now);
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  String currentTime = String(buff);

  if (currentTime == timeDisplayTurnsOn && !displayOn)
  {
    Serial.println(CleanText(LOG_TEXT[6]) + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, CleanText(NIGHT_MODE_TEXT[0]));
    display.drawString(64, 30, CleanText(NIGHT_MODE_TEXT[2]));
    display.display();
    delay(2000);
    enableDisplay(true);
  }

  if (currentTime == timeDisplayTurnsOff && displayOn)
  {
    Serial.println(CleanText(LOG_TEXT[7]) + currentTime);
    display.clear();
    display.display();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 10, CleanText(NIGHT_MODE_TEXT[0]));
    display.drawString(64, 30, CleanText(NIGHT_MODE_TEXT[1]));
    display.display();
    delay(2000);
    enableDisplay(false);
  }
}

void loadDeviceSettings()
{
  if (!LittleFS.exists(configName))
  {
    Serial.println(configName);
    Serial.println(CleanText(LOG_TEXT[14]));
    Serial.println();
    return;
  }
  File f = LittleFS.open(configName, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]));
    Serial.println(error.c_str());
    f.close();
    return;
  }

  TZ = doc["device"]["TZ"].as<int>();
  DST_MN = doc["device"]["DST_MN"].as<int>();
  TZ_NAME = doc["device"]["TZ_NAME"].as<String>();
  TZ_NAME_SHORT = doc["device"]["TZ_NAME_SHORT"].as<String>();
  UPDATE_INTERVAL = doc["device"]["UPDATE_INTERVAL"].as<int>();
  OPEN_WEATHER_MAP_APP_ID = doc["device"]["OPEN_WEATHER_MAP_APP_ID"].as<String>();
  OPEN_WEATHER_MAP_LOCATION_LAT = doc["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"].as<float>();
  OPEN_WEATHER_MAP_LOCATION_LON = doc["device"]["OPEN_WEATHER_MAP_LOCATION_LON"].as<float>();
  Display_District_Name = doc["device"]["Display_District_Name"].as<String>();
  Display_City_Name = doc["device"]["Display_City_Name"].as<String>();
  Display_Country_Name = doc["device"]["Display_Country_Name"].as<String>();
  IS_METRIC = doc["device"]["IS_METRIC"].as<bool>();
  OPEN_CAGE_ID = doc["device"]["OPEN_CAGE_ID"].as<String>();
  timeDisplayTurnsOn = doc["device"]["timeDisplayTurnsOn"].as<String>();
  timeDisplayTurnsOff = doc["device"]["timeDisplayTurnsOff"].as<String>();

  Serial.println();
  Serial.println(configName);
  Serial.printf("TZ: %d, TZ_NAME: %s, TZ_NAME_SHORT: %s, DST_MN: %d\n", doc["device"]["TZ"].as<int>(), doc["device"]["TZ_NAME"].as<String>().c_str(), doc["device"]["TZ_NAME_SHORT"].as<String>().c_str(), doc["device"]["DST_MN"].as<int>());
  Serial.printf("OPEN_WEATHER_MAP_LOCATION_LAT: %f, OPEN_WEATHER_MAP_LOCATION_LON: %f\n", doc["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"].as<float>(), doc["device"]["OPEN_WEATHER_MAP_LOCATION_LON"].as<float>());
  Serial.printf("OPEN_WEATHER_MAP_APP_ID: %s\n", doc["device"]["OPEN_WEATHER_MAP_APP_ID"].as<String>().c_str());
  Serial.printf("OPEN_CAGE_ID: %s\n", doc["device"]["OPEN_CAGE_ID"].as<String>().c_str());
  Serial.printf("Display_District_Name: %s\n", doc["device"]["Display_District_Name"].as<String>().c_str());
  Serial.printf("Display_City_Name: %s\n", doc["device"]["Display_City_Name"].as<String>().c_str());
  Serial.printf("Display_Country_Name: %s\n", doc["device"]["Display_Country_Name"].as<String>().c_str());
  Serial.printf("UPDATE_INTERVAL: %d\n", doc["device"]["UPDATE_INTERVAL"].as<int>());
  Serial.printf("IS_METRIC: %s\n", doc["device"]["IS_METRIC"].as<bool>() ? "true" : "false");
  Serial.printf("timeDisplayTurnsOn: %s\n", doc["device"]["timeDisplayTurnsOn"].as<String>().c_str());
  Serial.printf("timeDisplayTurnsOff: %s\n", doc["device"]["timeDisplayTurnsOff"].as<String>().c_str());
  Serial.println();

  f.close();
}

void saveDeviceSettings()
{
  JsonDocument doc;
  doc["device"]["TZ"] = TZ;
  doc["device"]["DST_MN"] = DST_MN;
  doc["device"]["TZ_NAME"] = TZ_NAME;
  doc["device"]["TZ_NAME_SHORT"] = TZ_NAME_SHORT;
  doc["device"]["UPDATE_INTERVAL"] = UPDATE_INTERVAL;
  doc["device"]["OPEN_WEATHER_MAP_APP_ID"] = OPEN_WEATHER_MAP_APP_ID;
  doc["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"] = OPEN_WEATHER_MAP_LOCATION_LAT;
  doc["device"]["OPEN_WEATHER_MAP_LOCATION_LON"] = OPEN_WEATHER_MAP_LOCATION_LON;
  doc["device"]["Display_District_Name"] = Display_District_Name;
  doc["device"]["Display_City_Name"] = Display_City_Name;
  doc["device"]["Display_Country_Name"] = Display_Country_Name;
  doc["device"]["IS_METRIC"] = IS_METRIC;
  doc["device"]["OPEN_CAGE_ID"] = OPEN_CAGE_ID;
  doc["device"]["timeDisplayTurnsOn"] = timeDisplayTurnsOn;
  doc["device"]["timeDisplayTurnsOff"] = timeDisplayTurnsOff;
  File f = LittleFS.open(configName, "w");
  serializeJson(doc, f);
  f.close();
}

void loadWorldClockSettings()
{
  if (!LittleFS.exists(worldClockConfig))
  {
    Serial.println(worldClockConfig);
    Serial.println(CleanText(LOG_TEXT[14]));
    Serial.println();
    return;
  }
  File f = LittleFS.open(worldClockConfig, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]));
    Serial.println(error.c_str());
    f.close();
    return;
  }

  World_Clock1_TZ = doc["worldClock"]["World_Clock1_TZ"].as<int>();
  World_Clock1_DST_MN = doc["worldClock"]["World_Clock1_DST_MN"].as<int>();
  World_Clock1_TZ_NAME = doc["worldClock"]["World_Clock1_TZ_NAME"].as<String>();
  World_Clock1_TZ_NAME_SHORT = doc["worldClock"]["World_Clock1_TZ_NAME_SHORT"].as<String>();
  World_Clock1_City_Name = doc["worldClock"]["World_Clock1_City_Name"].as<String>();
  World_Clock1_Country_Name = doc["worldClock"]["World_Clock1_Country_Name"].as<String>();

  World_Clock2_TZ = doc["worldClock"]["World_Clock2_TZ"].as<int>();
  World_Clock2_DST_MN = doc["worldClock"]["World_Clock2_DST_MN"].as<int>();
  World_Clock2_TZ_NAME = doc["worldClock"]["World_Clock2_TZ_NAME"].as<String>();
  World_Clock2_TZ_NAME_SHORT = doc["worldClock"]["World_Clock2_TZ_NAME_SHORT"].as<String>();
  World_Clock2_City_Name = doc["worldClock"]["World_Clock2_City_Name"].as<String>();
  World_Clock2_Country_Name = doc["worldClock"]["World_Clock2_Country_Name"].as<String>();

  World_Clock3_TZ = doc["worldClock"]["World_Clock3_TZ"].as<int>();
  World_Clock3_DST_MN = doc["worldClock"]["World_Clock3_DST_MN"].as<int>();
  World_Clock3_TZ_NAME = doc["worldClock"]["World_Clock3_TZ_NAME"].as<String>();
  World_Clock3_TZ_NAME_SHORT = doc["worldClock"]["World_Clock3_TZ_NAME_SHORT"].as<String>();
  World_Clock3_City_Name = doc["worldClock"]["World_Clock3_City_Name"].as<String>();
  World_Clock3_Country_Name = doc["worldClock"]["World_Clock3_Country_Name"].as<String>();

  Serial.println();
  Serial.println(worldClockConfig);
  Serial.printf("World_Clock1_TZ: %d, World_Clock1_TZ_NAME: %s, World_Clock1_TZ_NAME_SHORT: %s, World_Clock1_DST_MN: %d\n", doc["worldClock"]["World_Clock1_TZ"].as<int>(), doc["worldClock"]["World_Clock1_TZ_NAME"].as<String>().c_str(), doc["worldClock"]["World_Clock1_TZ_NAME_SHORT"].as<String>().c_str(), doc["worldClock"]["World_Clock1_DST_MN"].as<int>());
  Serial.printf("World_Clock1_City_Name: %s\n", doc["worldClock"]["World_Clock1_City_Name"].as<String>().c_str());
  Serial.printf("World_Clock1_Country_Name: %s\n", doc["worldClock"]["World_Clock1_Country_Name"].as<String>().c_str());
  Serial.println();
  Serial.printf("World_Clock2_TZ: %d, World_Clock2_TZ_NAME: %s, World_Clock2_TZ_NAME_SHORT: %s, World_Clock2_DST_MN: %d\n", doc["worldClock"]["World_Clock2_TZ"].as<int>(), doc["worldClock"]["World_Clock2_TZ_NAME"].as<String>().c_str(), doc["worldClock"]["World_Clock2_TZ_NAME_SHORT"].as<String>().c_str(), doc["worldClock"]["World_Clock2_DST_MN"].as<int>());
  Serial.printf("World_Clock2_City_Name: %s\n", doc["worldClock"]["World_Clock2_City_Name"].as<String>().c_str());
  Serial.printf("World_Clock2_Country_Name: %s\n", doc["worldClock"]["World_Clock2_Country_Name"].as<String>().c_str());
  Serial.println();
  Serial.printf("World_Clock3_TZ: %d, World_Clock3_TZ_NAME: %s World_Clock3_TZ_NAME_SHORT: %s World_Clock3_DST_MN: %d\n", doc["worldClock"]["World_Clock3_TZ"].as<int>(), doc["worldClock"]["World_Clock3_TZ_NAME"].as<String>().c_str(), doc["worldClock"]["World_Clock3_TZ_NAME_SHORT"].as<String>().c_str(), doc["worldClock"]["World_Clock3_DST_MN"].as<int>());
  Serial.printf("World_Clock3_City_Name: %s\n", doc["worldClock"]["World_Clock3_City_Name"].as<String>().c_str());
  Serial.printf("World_Clock3_Country_Name: %s\n", doc["worldClock"]["World_Clock3_Country_Name"].as<String>().c_str());
  Serial.println();
  f.close();
}

void saveWorldClockSettings()
{
  JsonDocument doc;
  doc["worldClock"]["World_Clock1_TZ"] = World_Clock1_TZ;
  doc["worldClock"]["World_Clock1_DST_MN"] = World_Clock1_DST_MN;
  doc["worldClock"]["World_Clock1_TZ_NAME"] = World_Clock1_TZ_NAME;
  doc["worldClock"]["World_Clock1_TZ_NAME_SHORT"] = World_Clock1_TZ_NAME_SHORT;
  doc["worldClock"]["World_Clock1_City_Name"] = World_Clock1_City_Name;
  doc["worldClock"]["World_Clock1_Country_Name"] = World_Clock1_Country_Name;
  doc["worldClock"]["World_Clock2_TZ"] = World_Clock2_TZ;
  doc["worldClock"]["World_Clock2_DST_MN"] = World_Clock2_DST_MN;
  doc["worldClock"]["World_Clock2_TZ_NAME"] = World_Clock2_TZ_NAME;
  doc["worldClock"]["World_Clock2_TZ_NAME_SHORT"] = World_Clock2_TZ_NAME_SHORT;
  doc["worldClock"]["World_Clock2_City_Name"] = World_Clock2_City_Name;
  doc["worldClock"]["World_Clock2_Country_Name"] = World_Clock2_Country_Name;
  doc["worldClock"]["World_Clock3_TZ"] = World_Clock3_TZ;
  doc["worldClock"]["World_Clock3_DST_MN"] = World_Clock3_DST_MN;
  doc["worldClock"]["World_Clock3_TZ_NAME"] = World_Clock3_TZ_NAME;
  doc["worldClock"]["World_Clock3_TZ_NAME_SHORT"] = World_Clock3_TZ_NAME_SHORT;
  doc["worldClock"]["World_Clock3_City_Name"] = World_Clock3_City_Name;
  doc["worldClock"]["World_Clock3_Country_Name"] = World_Clock3_Country_Name;
  File f = LittleFS.open(worldClockConfig, "w");
  serializeJson(doc, f);
  f.close();
}

void loadCurrencySettings()
{
  if (!LittleFS.exists(CurrencyConfig))
  {
    Serial.println(CurrencyConfig);
    Serial.println(CleanText(LOG_TEXT[14]));
    Serial.println();
    return;
  }
  File f = LittleFS.open(CurrencyConfig, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]));
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
  Serial.printf("BaseCurrency1: %s\n", doc["currency"]["BaseCurrency1"].as<String>().c_str());
  Serial.printf("TargetCurrency1: %s\n", doc["currency"]["TargetCurrency1"].as<String>().c_str());
  Serial.printf("BaseCurrency2: %s\n", doc["currency"]["BaseCurrency2"].as<String>().c_str());
  Serial.printf("TargetCurrency2: %s\n", doc["currency"]["TargetCurrency2"].as<String>().c_str());
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
    Serial.println(CleanText(LOG_TEXT[14]));
    Serial.println();
    return;
  }
  File f = LittleFS.open(securityConfig, "r");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error)
  {
    Serial.print(CleanText(LOG_TEXT[8]));
    Serial.println(error.c_str());
    f.close();
    return;
  }
  SysUser = doc["security"]["user"].as<String>();
  SysPass = doc["security"]["pass"].as<String>();

  Serial.println();
  Serial.println(securityConfig);
  Serial.printf("SysUser: %s\n", doc["security"]["user"].as<String>().c_str());
  Serial.printf("SysPass: %s\n", doc["security"]["pass"].as<String>().c_str());
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

void resetToDefaults()
{
  LittleFS.remove(configName);
  LittleFS.remove(securityConfig);
  LittleFS.remove(worldClockConfig);
  LittleFS.remove(CurrencyConfig);

  JsonDocument docD;
  docD["device"]["TZ"] = 3;
  docD["device"]["DST_MN"] = 0;
  docD["device"]["TZ_NAME"] = "Europe/Istanbul";
  docD["device"]["TZ_NAME_SHORT"] = "+03";
  docD["device"]["UPDATE_INTERVAL"] = 30;
  docD["device"]["OPEN_WEATHER_MAP_APP_ID"] = "";
  docD["device"]["OPEN_WEATHER_MAP_LOCATION_LAT"] = 41.1719525;
  docD["device"]["OPEN_WEATHER_MAP_LOCATION_LON"] = 28.8594870;
  docD["device"]["Display_District_Name"] = "Sarıyer";
  docD["device"]["Display_City_Name"] = "İstanbul";
  docD["device"]["Display_Country_Name"] = "TR";
  docD["device"]["IS_METRIC"] = true;
  docD["device"]["OPEN_CAGE_ID"] = "";
  docD["device"]["timeDisplayTurnsOn"] = "08:00";
  docD["device"]["timeDisplayTurnsOff"] = "00:00";

  JsonDocument docS;
  docS["security"]["user"] = SysUser;
  docS["security"]["pass"] = SysPass;

  JsonDocument docWC;
  docWC["worldClock"]["World_Clock1_TZ"] = -7;
  docWC["worldClock"]["World_Clock1_DST_MN"] = 0;
  docWC["worldClock"]["World_Clock1_TZ_NAME"] = "America/Los_Angeles";
  docWC["worldClock"]["World_Clock1_TZ_NAME_SHORT"] = "PST";
  docWC["worldClock"]["World_Clock1_City_Name"] = "Los Angeles";
  docWC["worldClock"]["World_Clock1_Country_Name"] = "US";
  docWC["worldClock"]["World_Clock2_TZ"] = 0;
  docWC["worldClock"]["World_Clock2_DST_MN"] = 0;
  docWC["worldClock"]["World_Clock2_TZ_NAME"] = "Europe/London";
  docWC["worldClock"]["World_Clock2_TZ_NAME_SHORT"] = "GMT";
  docWC["worldClock"]["World_Clock2_City_Name"] = "Londra";
  docWC["worldClock"]["World_Clock2_Country_Name"] = "GB";
  docWC["worldClock"]["World_Clock3_TZ"] = 9;
  docWC["worldClock"]["World_Clock3_DST_MN"] = 0;
  docWC["worldClock"]["World_Clock3_TZ_NAME"] = "Asia/Seoul";
  docWC["worldClock"]["World_Clock3_TZ_NAME_SHORT"] = "KST";
  docWC["worldClock"]["World_Clock3_City_Name"] = "Seul";
  docWC["worldClock"]["World_Clock3_Country_Name"] = "KR";

  JsonDocument docCur;
  docCur["currency"]["BaseCurrency1"] = "eur";
  docCur["currency"]["TargetCurrency1"] = "try";
  docCur["currency"]["BaseCurrency2"] = "usd";
  docCur["currency"]["TargetCurrency2"] = "try";

  File d = LittleFS.open(configName, "w");
  serializeJson(docD, d);
  d.close();
  File s = LittleFS.open(securityConfig, "w");
  serializeJson(docS, s);
  s.close();
  File wc = LittleFS.open(worldClockConfig, "w");
  serializeJson(docWC, wc);
  wc.close();
  File cur = LittleFS.open(CurrencyConfig, "w");
  serializeJson(docCur, cur);
  cur.close();
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println(CleanText(LOG_TEXT[9]));
  Serial.println(WiFi.softAPIP());

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Wifi Manager");
  display.drawString(64, 10, CleanText(LOG_TEXT[10]));
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 26, myWiFiManager->getConfigPortalSSID());
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 46, CleanText(LOG_TEXT[11]));
  display.display();

  Serial.println("Wifi Manager");
  Serial.println(CleanText(LOG_TEXT[10]));
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println(CleanText(LOG_TEXT[11]));
}

void redirectHome()
{
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDevicePage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }
  String html = Device_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%DEVICESETTINGS%", HTML_TEXT[1]);
  html.replace("%OWMAPIKEYTEXT%", HTML_TEXT[4]);
  html.replace("%APIPLACE%", HTML_TEXT[5]);
  html.replace("%SHOWPASS%", HTML_TEXT[6]);
  html.replace("%GEOAPIKEYTEXT%", HTML_TEXT[7]);
  html.replace("%DISTRICTTEXT%", HTML_TEXT[8]);
  html.replace("%DISTRICTPLACE%", HTML_TEXT[9]);
  html.replace("%CITYTEXT%", HTML_TEXT[10]);
  html.replace("%CITYPLACE%", HTML_TEXT[11]);
  html.replace("%COUNTRYTEXT%", HTML_TEXT[12]);
  html.replace("%COUNTRYPLACE%", HTML_TEXT[13]);
  html.replace("%FINDGEOTIME%", HTML_TEXT[14]);
  html.replace("%LATTEXT%", HTML_TEXT[15]);
  html.replace("%LATPLACE%", HTML_TEXT[16]);
  html.replace("%LONTEXT%", HTML_TEXT[17]);
  html.replace("%LONPLACE%", HTML_TEXT[18]);
  html.replace("%TZTEXT%", HTML_TEXT[19]);
  html.replace("%TZPLACE%", HTML_TEXT[20]);
  html.replace("%DSTTEXT%", HTML_TEXT[21]);
  html.replace("%TZNAMETEXT%", HTML_TEXT[22]);
  html.replace("%TZNAMEPLACE%", HTML_TEXT[23]);
  html.replace("%TZNAMESHORTTEXT%", HTML_TEXT[24]);
  html.replace("%TZNAMESHORTPLACE%", HTML_TEXT[25]);
  html.replace("%UNITTEXT%", HTML_TEXT[26]);
  html.replace("%METRICTEXT%", HTML_TEXT[27]);
  html.replace("%IMPERIALTEXT%", HTML_TEXT[28]);
  html.replace("%INTERVALTEXT%", HTML_TEXT[29]);
  html.replace("%INTERVALPLACE%", HTML_TEXT[30]);
  html.replace("%SAVETEXT%", HTML_TEXT[31]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[32]);
  html.replace("%GEOALERT%", HTML_TEXT[33]);
  html.replace("%GEOALERT2%", HTML_TEXT[34]);
  html.replace("%GEOALERT3%", HTML_TEXT[35]);
  html.replace("%HIDEPASS%", HTML_TEXT[47]);
  html.replace("%TIMEONTEXT%", HTML_TEXT[61]);
  html.replace("%TIMEONPLACE%", HTML_TEXT[62]);
  html.replace("%TIMEOFFTEXT%", HTML_TEXT[63]);
  html.replace("%TIMEOFFPLACE%", HTML_TEXT[64]);
  html.replace("%GETFROMTEXT%", HTML_TEXT[67]);
  html.replace("%HERETEXT%", HTML_TEXT[68]);
  html.replace("%TZ%", String(TZ));
  html.replace("%TZNAME%", TZ_NAME);
  html.replace("%TZNAMESHORT%", TZ_NAME_SHORT);
  html.replace("%LAT%", String(OPEN_WEATHER_MAP_LOCATION_LAT));
  html.replace("%LON%", String(OPEN_WEATHER_MAP_LOCATION_LON));
  html.replace("%DISTRICT%", Display_District_Name);
  html.replace("%CITY%", Display_City_Name);
  html.replace("%COUNTRY%", Display_Country_Name);
  html.replace("%OWMAPIKEY%", OPEN_WEATHER_MAP_APP_ID);
  html.replace("%INTERVAL%", String(UPDATE_INTERVAL));
  html.replace("%GEOAPIKEY%", OPEN_CAGE_ID);
  html.replace("%TIMEON%", timeDisplayTurnsOn);
  html.replace("%TIMEOFF%", timeDisplayTurnsOff);
  html.replace("%IS_DST%", (DST_MN == 60) ? "checked" : "");
  html.replace("%METRIC%", IS_METRIC ? "selected" : "");
  html.replace("%IMPERIAL%", !IS_METRIC ? "selected" : "");
  server.send(200, "text/html", html);
}

void handleSaveDevice()
{
  if (server.hasArg("city"))
  {
    Display_City_Name = server.arg("city");
  }

  if (server.hasArg("district"))
  {
    Display_District_Name = server.arg("district");
  }

  if (server.hasArg("country"))
  {
    Display_Country_Name = server.arg("country");
  }

  if (server.hasArg("OWMApiKey"))
  {
    OPEN_WEATHER_MAP_APP_ID = server.arg("OWMApiKey");
  }

  if (server.hasArg("geoApiKey"))
  {
    OPEN_CAGE_ID = server.arg("geoApiKey");
  }

  if (server.hasArg("timezoneName"))
  {
    TZ_NAME = server.arg("timezoneName");
  }

  if (server.hasArg("timezoneNameShort"))
  {
    TZ_NAME_SHORT = server.arg("timezoneNameShort");
  }

  if (server.hasArg("timezone"))
  {
    TZ = server.arg("timezone").toInt();
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
    UPDATE_INTERVAL = server.arg("updateInterval").toInt();
  }

  if (server.hasArg("units"))
  {
    String val = server.arg("units");
    IS_METRIC = (val == "metric") ? true : false;
  }

  if (server.hasArg("turnOffTime"))
  {
    timeDisplayTurnsOff = server.arg("turnOffTime");
  }

  if (server.hasArg("turnOnTime"))
  {
    timeDisplayTurnsOn = server.arg("turnOnTime");
  }

  DST_MN = server.hasArg("isDST") ? 60 : 0;

  Serial.println();
  Serial.println(CleanText(LOG_TEXT[12]));
  Serial.printf("TZ: %d, TZ_NAME: %s TZ_NAME_SHORT: %s DST_MN: %d\n", TZ, TZ_NAME.c_str(), TZ_NAME_SHORT.c_str(), DST_MN);
  Serial.printf("OPEN_WEATHER_MAP_LOCATION_LAT: %f, OPEN_WEATHER_MAP_LOCATION_LON: %f\n", OPEN_WEATHER_MAP_LOCATION_LAT, OPEN_WEATHER_MAP_LOCATION_LON);
  Serial.printf("OPEN_WEATHER_MAP_APP_ID: %s\n", OPEN_WEATHER_MAP_APP_ID.c_str());
  Serial.printf("OPEN_CAGE_ID: %s\n", OPEN_CAGE_ID.c_str());
  Serial.printf("Display_District_Name: %s\n", Display_District_Name.c_str());
  Serial.printf("Display_City_Name: %s\n", Display_City_Name.c_str());
  Serial.printf("Display_Country_Name: %s\n", Display_Country_Name.c_str());
  Serial.printf("UPDATE_INTERVAL: %d\n", UPDATE_INTERVAL);
  Serial.printf("IS_METRIC: %s\n", IS_METRIC ? "true" : "false");
  Serial.printf("timeDisplayTurnsOn: %s\n", timeDisplayTurnsOn.c_str());
  Serial.printf("timeDisplayTurnsOff: %s\n", timeDisplayTurnsOff.c_str());
  Serial.println();

  saveDeviceSettings();

  String html = Save_Settings_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%DEVICESETTINGS%", HTML_TEXT[1]);
  html.replace("%DEVICETEXT%", HTML_TEXT[66]);
  server.send(200, "text/html", html);

  checkDisplay();
  updateData(&display);
}

void handleWorldClockPage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }
  String html = World_Clock_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%WORLDCLOCKSETTINGS%", HTML_TEXT[72]);
  html.replace("%APIPLACE%", HTML_TEXT[5]);
  html.replace("%SHOWPASS%", HTML_TEXT[6]);
  html.replace("%GEOAPIKEYTEXT%", HTML_TEXT[7]);
  html.replace("%SAVETEXT%", HTML_TEXT[31]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[32]);
  html.replace("%GEOALERT2%", HTML_TEXT[34]);
  html.replace("%GEOALERT3%", HTML_TEXT[35]);
  html.replace("%HIDEPASS%", HTML_TEXT[47]);
  html.replace("%GETFROMTEXT%", HTML_TEXT[67]);
  html.replace("%HERETEXT%", HTML_TEXT[68]);
  html.replace("%CITYTEXT%", HTML_TEXT[10]);
  html.replace("%COUNTRYTEXT%", HTML_TEXT[12]);
  html.replace("%TZTEXT%", HTML_TEXT[19]);
  html.replace("%DSTTEXT%", HTML_TEXT[21]);
  html.replace("%TZNAMETEXT%", HTML_TEXT[22]);
  html.replace("%TZNAMESHORTTEXT%", HTML_TEXT[24]);
  html.replace("%FINDTIMES%", HTML_TEXT[74]);
  html.replace("%CITY1LABEL%", HTML_TEXT[75]);
  html.replace("%CITY1PLACE%", HTML_TEXT[76]);
  html.replace("%COUNTRY1PLACE%", HTML_TEXT[77]);
  html.replace("%TZ1PLACE%", HTML_TEXT[78]);
  html.replace("%TZNAME1PLACE%", HTML_TEXT[79]);
  html.replace("%TZNAMESHORT1PLACE%", HTML_TEXT[80]);
  html.replace("%CITY2LABEL%", HTML_TEXT[81]);
  html.replace("%CITY2PLACE%", HTML_TEXT[82]);
  html.replace("%COUNTRY2PLACE%", HTML_TEXT[83]);
  html.replace("%TZ2PLACE%", HTML_TEXT[84]);
  html.replace("%TZNAME2PLACE%", HTML_TEXT[85]);
  html.replace("%TZNAMESHORT2PLACE%", HTML_TEXT[86]);
  html.replace("%CITY3LABEL%", HTML_TEXT[87]);
  html.replace("%CITY3PLACE%", HTML_TEXT[88]);
  html.replace("%COUNTRY3PLACE%", HTML_TEXT[89]);
  html.replace("%TZ3PLACE%", HTML_TEXT[90]);
  html.replace("%TZNAME3PLACE%", HTML_TEXT[91]);
  html.replace("%TZNAMESHORT3PLACE%", HTML_TEXT[92]);
  html.replace("%GEOALERT4%", HTML_TEXT[93]);
  html.replace("%GEOALERT5%", HTML_TEXT[94]);
  html.replace("%GEOAPIKEY%", OPEN_CAGE_ID);
  html.replace("%TZ1%", String(World_Clock1_TZ));
  html.replace("%TZNAME1%", World_Clock1_TZ_NAME);
  html.replace("%TZNAMESHORT1%", World_Clock1_TZ_NAME_SHORT);
  html.replace("%CITY1%", World_Clock1_City_Name);
  html.replace("%COUNTRY1%", World_Clock1_Country_Name);
  html.replace("%IS_DST1%", (World_Clock1_DST_MN == 60) ? "checked" : "");
  html.replace("%TZ2%", String(World_Clock2_TZ));
  html.replace("%TZNAME2%", World_Clock2_TZ_NAME);
  html.replace("%TZNAMESHORT2%", World_Clock2_TZ_NAME_SHORT);
  html.replace("%CITY2%", World_Clock2_City_Name);
  html.replace("%COUNTRY2%", World_Clock2_Country_Name);
  html.replace("%IS_DST2%", (World_Clock2_DST_MN == 60) ? "checked" : "");
  html.replace("%TZ3%", String(World_Clock3_TZ));
  html.replace("%TZNAME3%", World_Clock3_TZ_NAME);
  html.replace("%TZNAMESHORT3%", World_Clock3_TZ_NAME_SHORT);
  html.replace("%CITY3%", World_Clock3_City_Name);
  html.replace("%COUNTRY3%", World_Clock3_Country_Name);
  html.replace("%IS_DST3%", (World_Clock3_DST_MN == 60) ? "checked" : "");
  server.send(200, "text/html", html);
}

void handleSaveWorldClock()
{

  if (server.hasArg("city1"))
  {
    World_Clock1_City_Name = server.arg("city1");
  }

  if (server.hasArg("country1"))
  {
    World_Clock1_Country_Name = server.arg("country1");
  }

  if (server.hasArg("timezoneName1"))
  {
    World_Clock1_TZ_NAME = server.arg("timezoneName1");
  }

  if (server.hasArg("timezoneNameShort1"))
  {
    World_Clock1_TZ_NAME_SHORT = server.arg("timezoneNameShort1");
  }

  if (server.hasArg("timezone1"))
  {
    World_Clock1_TZ = server.arg("timezone1").toInt();
  }

  World_Clock1_DST_MN = server.hasArg("isDST1") ? 60 : 0;

  if (server.hasArg("city2"))
  {
    World_Clock2_City_Name = server.arg("city2");
  }

  if (server.hasArg("country2"))
  {
    World_Clock2_Country_Name = server.arg("country2");
  }

  if (server.hasArg("timezoneName2"))
  {
    World_Clock2_TZ_NAME = server.arg("timezoneName2");
  }

  if (server.hasArg("timezoneNameShort2"))
  {
    World_Clock2_TZ_NAME_SHORT = server.arg("timezoneNameShort2");
  }

  if (server.hasArg("timezone2"))
  {
    World_Clock2_TZ = server.arg("timezone2").toInt();
  }

  World_Clock2_DST_MN = server.hasArg("isDST2") ? 60 : 0;

  if (server.hasArg("city3"))
  {
    World_Clock3_City_Name = server.arg("city3");
  }

  if (server.hasArg("country3"))
  {
    World_Clock3_Country_Name = server.arg("country3");
  }

  if (server.hasArg("timezoneName3"))
  {
    World_Clock3_TZ_NAME = server.arg("timezoneName3");
  }

  if (server.hasArg("timezoneNameShort3"))
  {
    World_Clock3_TZ_NAME_SHORT = server.arg("timezoneNameShort3");
  }

  if (server.hasArg("timezone3"))
  {
    World_Clock3_TZ = server.arg("timezone3").toInt();
  }

  World_Clock3_DST_MN = server.hasArg("isDST3") ? 60 : 0;

  Serial.println();
  Serial.println(CleanText(LOG_TEXT[16]));
  Serial.printf("World_Clock1_TZ: %d, World_Clock1_TZ_NAME: %s World_Clock1_TZ_NAME_SHORT: %s World_Clock1_DST_MN: %d\n", World_Clock1_TZ, World_Clock1_TZ_NAME.c_str(), World_Clock1_TZ_NAME_SHORT.c_str(), World_Clock1_DST_MN);
  Serial.printf("World_Clock1_City_Name: %s\n", World_Clock1_City_Name.c_str());
  Serial.printf("World_Clock1_Country_Name: %s\n", World_Clock1_Country_Name.c_str());
  Serial.println();
  Serial.printf("World_Clock2_TZ: %d, World_Clock2_TZ_NAME: %s World_Clock2_TZ_NAME_SHORT: %s World_Clock2_DST_MN: %d\n", World_Clock2_TZ, World_Clock2_TZ_NAME.c_str(), World_Clock2_TZ_NAME_SHORT.c_str(), World_Clock2_DST_MN);
  Serial.printf("World_Clock2_City_Name: %s\n", World_Clock2_City_Name.c_str());
  Serial.printf("World_Clock2_Country_Name: %s\n", World_Clock2_Country_Name.c_str());
  Serial.println();
  Serial.printf("World_Clock3_TZ: %d, World_Clock3_TZ_NAME: %s World_Clock3_TZ_NAME_SHORT: %s World_Clock3_DST_MN: %d\n", World_Clock3_TZ, World_Clock3_TZ_NAME.c_str(), World_Clock3_TZ_NAME_SHORT.c_str(), World_Clock3_DST_MN);
  Serial.printf("World_Clock3_City_Name: %s\n", World_Clock3_City_Name.c_str());
  Serial.printf("World_Clock3_Country_Name: %s\n", World_Clock3_Country_Name.c_str());
  Serial.println();

  saveWorldClockSettings();

  String html = Save_World_Clock_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%WORLDCLOCKSETTINGS%", HTML_TEXT[72]);
  html.replace("%WORLDCLOCKTEXT%", HTML_TEXT[73]);
  server.send(200, "text/html", html);

  checkDisplay();
}

void handleCurrencyPage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }
  String html = Currency_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%SAVETEXT%", HTML_TEXT[31]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[32]);
  html.replace("%CURRENCYSETTINGS%", HTML_TEXT[95]);
  html.replace("%APIWARNINGTEXT%", HTML_TEXT[96]);
  html.replace("%CURRENCY1TEXT%", HTML_TEXT[97]);
  html.replace("%BASETEXT%", HTML_TEXT[98]);
  html.replace("%TARGETTEXT%", HTML_TEXT[99]);
  html.replace("%CURRENCY2TEXT%", HTML_TEXT[100]);
  html.replace("%CURRENCYALERT%", HTML_TEXT[102]);
  html.replace("%BASE1%", BaseCurrency1);
  html.replace("%TARGET1%", TargetCurrency1);
  html.replace("%BASE2%", BaseCurrency2);
  html.replace("%TARGET2%", TargetCurrency2);
  server.send(200, "text/html", html);
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
  Serial.println(CleanText(LOG_TEXT[17]));
  Serial.printf("BaseCurrency1: %s\n", BaseCurrency1.c_str());
  Serial.printf("TargetCurrency1: %s\n", TargetCurrency1.c_str());
  Serial.printf("BaseCurrency2: %s\n", BaseCurrency2.c_str());
  Serial.printf("TargetCurrency2: %s\n", TargetCurrency2.c_str());
  Serial.println();

  saveCurrencySettings();

  String html = Save_Currency_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%CURRENCYSETTINGS%", HTML_TEXT[95]);
  html.replace("%CURRENCYTEXT%", HTML_TEXT[101]);
  server.send(200, "text/html", html);

  checkDisplay();
  updateData(&display);
}

void handleHomePage()
{
  String html = Home_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%HOMETITLE%", HTML_TEXT[0]);
  html.replace("%DEVICESETTINGS%", HTML_TEXT[1]);
  html.replace("%SECURITYSETTINGS%", HTML_TEXT[3]);
  html.replace("%WORLDCLOCKSETTINGS%", HTML_TEXT[72]);
  html.replace("%CURRENCYSETTINGS%", HTML_TEXT[95]);
  html.replace("%DEFAULTSETTINGS%", HTML_TEXT[45]);
  html.replace("%DEFAULTALERT%", HTML_TEXT[46]);
  html.replace("%OTATEXT%", HTML_TEXT[60]);
  server.send(200, "text/html", html);
}

void handleSecurityPage()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }
  String html = Security_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%SECURITYSETTINGS%", HTML_TEXT[3]);
  html.replace("%SHOWPASS%", HTML_TEXT[6]);
  html.replace("%SAVETEXT%", HTML_TEXT[31]);
  html.replace("%GOHOMETEXT%", HTML_TEXT[32]);
  html.replace("%PASSWORDTEXT%", HTML_TEXT[38]);
  html.replace("%USERTEXT%", HTML_TEXT[40]);
  html.replace("%USERPLACE%", HTML_TEXT[41]);
  html.replace("%PASSWORDPLACE2%", HTML_TEXT[42]);
  html.replace("%HIDEPASS%", HTML_TEXT[47]);
  html.replace("%USER%", SysUser);
  html.replace("%PASSWORD%", SysPass);
  server.send(200, "text/html", html);
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

  Serial.println(CleanText(LOG_TEXT[13]));
  Serial.printf("SysUser: %s\n", SysUser.c_str());
  Serial.printf("SysPass: %s\n", SysPass.c_str());

  saveSecuritySettings();

  String html = Save_Security_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%SECURITYSETTINGS%", HTML_TEXT[3]);
  html.replace("%DEVICETEXT%", HTML_TEXT[65]);
  server.send(200, "text/html", html);

  checkDisplay();
}

void handleResetDeafults()
{
  if (!server.authenticate(SysUser.c_str(), SysPass.c_str()))
  {
    return server.requestAuthentication();
  }

  resetToDefaults();

  String html = Reset_Default_Page;
  html.replace("%LANG%", OPEN_WEATHER_MAP_LANGUAGE);
  html.replace("%DEFAULTSETTINGS%", HTML_TEXT[43]);
  html.replace("%DEFAULTTEXT%", HTML_TEXT[44]);

  server.send(200, "text/html", html);

  delay(2000);
  ESP.restart();
}
