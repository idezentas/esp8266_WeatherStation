/**The MIT License (MIT)
 
 Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com
 
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

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiClient.h>
#include "OpenWeatherMapCurrent.h"

OpenWeatherMapCurrent::OpenWeatherMapCurrent() {

}

void OpenWeatherMapCurrent::updateCurrent(OpenWeatherMapCurrentData *data, String appId, String location) {
  doUpdate(data, buildPath(appId, "q=" + location));
}
void OpenWeatherMapCurrent::updateCurrent(OpenWeatherMapCurrentData *data, String appId, float lat, float lon) {
  doUpdate(data, buildPath(appId, "lat=" + String(lat) + "&lon=" + String(lon)));
}

void OpenWeatherMapCurrent::updateCurrentById(OpenWeatherMapCurrentData *data, String appId, String locationId) {
  doUpdate(data, buildPath(appId, "id=" + locationId));
}

String OpenWeatherMapCurrent::buildPath(String appId, String locationParameter) {
  String units = metric ? "metric" : "imperial";
  return "/data/2.5/weather?" + locationParameter + "&appid=" + appId + "&units=" + units + "&lang=" + language;
}

void OpenWeatherMapCurrent::doUpdate(OpenWeatherMapCurrentData *data, String path) {
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();
  this->weatherItemCounter = 0;
  this->data = data;
  JsonStreamingParser parser;
  parser.setListener(this);
  Serial.printf_P(PSTR("[HTTP] Requesting resource at http://%s:%u%s\n"), host.c_str(), port, path.c_str());

  WiFiClient client;
  #if defined(ESP8266)
  if (client.connect(host, port)) {
  #else
  if (client.connect(host.c_str(), port)) {
  #endif
    bool isBody = false;
    char c;
    Serial.println(F("[HTTP] connected, now GETting data"));
    client.print("GET " + path + " HTTP/1.1\r\n"
                 "Host: " + host + "\r\n"
                 "Connection: close\r\n\r\n");

    while (client.connected() || client.available()) {
      if (client.available()) {
        if ((millis() - lost_do) > lostTest) {
          Serial.println(F("[HTTP] lost in client with a timeout"));
          client.stop();
        }
        c = client.read();
        if (c == '{' || c == '[') {
          isBody = true;
        }
        if (isBody) {
          parser.parse(c);
        }
      }
      // give WiFi and TCP/IP libraries a chance to handle pending events
      yield();
    }
    client.stop();
  } else {
    Serial.println(F("[HTTP] failed to connect to host"));
  }
  this->data = nullptr;
}

void OpenWeatherMapCurrent::whitespace(char c) {
  Serial.println(F("whitespace"));
}

void OpenWeatherMapCurrent::startDocument() {
  Serial.println(F("start document"));
}

void OpenWeatherMapCurrent::key(String key) {
  currentKey = String(key);
}

void OpenWeatherMapCurrent::value(String value) {
  // "lon": 8.54, float lon;
  if (currentKey == "lon") {
    this->data->lon = value.toFloat();
    Serial.printf_P(PSTR("lon: %.2f\n"), value.toFloat());
  }
  // "lat": 47.37 float lat;
  if (currentKey == "lat") {
    this->data->lat = value.toFloat();
    Serial.printf_P(PSTR("lat: %.2f\n"), value.toFloat());
  }
  // weatherItemCounter: only get the first item if more than one is available
  if (currentParent == "weather" && weatherItemCounter == 0) {
    // "id": 521, weatherId weatherId;
    if (currentKey == "id") {
      this->data->weatherId = value.toInt();
      Serial.printf_P(PSTR("weatherId: %ld\n"), value.toInt());
    }
    // "main": "Rain", String main;
    if (currentKey == "main") {
      this->data->main = value;
      Serial.printf_P(PSTR("main: %s\n"), value.c_str());
    }
    // "description": "shower rain", String description;
    if (currentKey == "description") {
      this->data->description = value;
      this->data->descriptionClean = UpperText(CleanText(value).c_str());
      Serial.printf_P(PSTR("description: %s\n"), value.c_str());
      Serial.printf_P(PSTR("descriptionClean: %s\n"), UpperText(CleanText(value)).c_str());
    }
    // "icon": "09d" String icon;
   //String iconMeteoCon;
    if (currentKey == "icon") {
      this->data->icon = value;
      this->data->iconMeteoCon = getMeteoconIcon(value);
      Serial.printf_P(PSTR("icon: %s\n"), value.c_str());
      Serial.printf_P(PSTR("iconMeteoCon: %s\n"), getMeteoconIcon(value).c_str());
    }

  }

  // "temp": 290.56, float temp;
  if (currentKey == "temp") {
    this->data->temp = value.toFloat();
    Serial.printf_P(PSTR("temp: %.2f\n"), value.toFloat());
  }
  // "feels_like": 290.87, float feelsLike;
  if (currentKey == "feels_like") {
    this->data->feelsLike = value.toFloat();
    Serial.printf_P(PSTR("feelsLike: %.2f\n"), value.toFloat());
  }
  // "pressure": 1013, uint16_t pressure;
  if (currentKey == "pressure") {
    this->data->pressure = value.toInt();
    Serial.printf_P(PSTR("pressure: %ld\n"), value.toInt());
  }
  // "humidity": 87, uint8_t humidity;
  if (currentKey == "humidity") {
    this->data->humidity = value.toInt();
    Serial.printf_P(PSTR("humidity: %ld\n"), value.toInt());
  }
  // "temp_min": 289.15, float tempMin;
  if (currentKey == "temp_min") {
    this->data->tempMin = value.toFloat();
    Serial.printf_P(PSTR("tempMin: %.2f\n"), value.toFloat());
  }
  // "temp_max": 292.15 float tempMax;
  if (currentKey == "temp_max") {
    this->data->tempMax = value.toFloat();
    Serial.printf_P(PSTR("tempMax: %.2f\n"), value.toFloat());
  }
  // "wind": {"speed": 1.5}, float windSpeed;
  if (currentKey == "speed") {
    this->data->windSpeed = value.toFloat();
    Serial.printf_P(PSTR("windSpeed: %.2f\n"), value.toFloat());
  }
  // "wind": {deg: 226.505}, float windDeg;
  if (currentKey == "deg") {
    this->data->windDeg = value.toFloat();
    Serial.printf_P(PSTR("windDeg: %.2f\n"), value.toFloat());
  }
  // "clouds": {"all": 90}, uint8_t clouds;
  if (currentKey == "all") {
    this->data->clouds = value.toInt();
    Serial.printf_P(PSTR("clouds: %ld\n"), value.toInt());
  }
  // "dt": 1527015000, uint64_t observationTime;
  if (currentKey == "dt") {
    this->data->observationTime = value.toInt();
    Serial.printf_P(PSTR("observationTime: %ld\n"), value.toInt());
  }
  // "sunrise": 1526960448, uint32_t sunrise;
  if (currentKey == "sunrise") {
    this->data->sunrise = value.toInt();
    Serial.printf_P(PSTR("sunrise: %ld\n"), value.toInt());
  }
  // "sunset": 1527015901 uint32_t sunset;
  if (currentKey == "sunset") {
    this->data->sunset = value.toInt();
    Serial.printf_P(PSTR("sunset: %ld\n"), value.toInt());
  }
}

void OpenWeatherMapCurrent::endArray() {

}


void OpenWeatherMapCurrent::startObject() {
  currentParent = currentKey;
}

void OpenWeatherMapCurrent::endObject() {
  if (currentParent == "weather") {
    weatherItemCounter++;
  }
  currentParent = "";
}

void OpenWeatherMapCurrent::endDocument() {

}

void OpenWeatherMapCurrent::startArray() {

}


String OpenWeatherMapCurrent::UpperText(String text)
{
  text.toUpperCase();
  return text;
}

String OpenWeatherMapCurrent::CleanText(String text)
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

String OpenWeatherMapCurrent::getMeteoconIcon(String icon) {
 	// clear sky
  // 01d
  if (icon == "01d") 	{
    return "B";
  }
  // 01n
  if (icon == "01n") 	{
    return "C";
  }
  // few clouds
  // 02d
  if (icon == "02d") 	{
    return "H";
  }
  // 02n
  if (icon == "02n") 	{
    return "4";
  }
  // scattered clouds
  // 03d
  if (icon == "03d") 	{
    return "N";
  }
  // 03n
  if (icon == "03n") 	{
    return "5";
  }
  // broken clouds
  // 04d
  if (icon == "04d") 	{
    return "Y";
  }
  // 04n
  if (icon == "04n") 	{
    return "%";
  }
  // shower rain
  // 09d
  if (icon == "09d") 	{
    return "R";
  }
  // 09n
  if (icon == "09n") 	{
    return "8";
  }
  // rain
  // 10d
  if (icon == "10d") 	{
    return "Q";
  }
  // 10n
  if (icon == "10n") 	{
    return "7";
  }
  // thunderstorm
  // 11d
  if (icon == "11d") 	{
    return "P";
  }
  // 11n
  if (icon == "11n") 	{
    return "6";
  }
  // snow
  // 13d
  if (icon == "13d") 	{
    return "W";
  }
  // 13n
  if (icon == "13n") 	{
    return "#";
  }
  // mist
  // 50d
  if (icon == "50d") 	{
    return "M";
  }
  // 50n
  if (icon == "50n") 	{
    return "M";
  }
  // Nothing matched: N/A
  return ")";

}
