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

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiClient.h>
#include "OpenWeatherMapAir.h"

OpenWeatherMapAir::OpenWeatherMapAir()
{
}
void OpenWeatherMapAir::updateCurrent(OpenWeatherMapAirData *data, String appId, float lat, float lon)
{
  doUpdate(data, buildPath(appId, "lat=" + String(lat) + "&lon=" + String(lon)));
}

String OpenWeatherMapAir::buildPath(String appId, String locationParameter)
{
  return "/data/2.5/air_pollution?" + locationParameter + "&appid=" + appId;
}

void OpenWeatherMapAir::doUpdate(OpenWeatherMapAirData *data, String path)
{
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();
  this->weatherItemCounter = 0;
  this->data = data;
  JsonStreamingParser parser;
  parser.setListener(this);
  Serial.printf_P(PSTR("[HTTP] Requesting resource at http://%s:%u%s\n"), host.c_str(), port, path.c_str());

  WiFiClient client;
#if defined(ESP8266)
  if (client.connect(host, port))
  {
#else
  if (client.connect(host.c_str(), port))
  {
#endif
    bool isBody = false;
    char c;
    Serial.println(F("[HTTP] connected, now GETting data"));
    client.print("GET " + path + " HTTP/1.1\r\n"
                                 "Host: " +
                 host + "\r\n"
                        "Connection: close\r\n\r\n");

    while (client.connected() || client.available())
    {
      if (client.available())
      {
        if ((millis() - lost_do) > lostTest)
        {
          Serial.println(F("[HTTP] lost in client with a timeout"));
          client.stop();
        }
        c = client.read();
        if (c == '{' || c == '[')
        {
          isBody = true;
        }
        if (isBody)
        {
          parser.parse(c);
        }
      }
      // give WiFi and TCP/IP libraries a chance to handle pending events
      yield();
    }
    client.stop();
  }
  else
  {
    Serial.println(F("[HTTP] failed to connect to host"));
  }
  this->data = nullptr;
}

void OpenWeatherMapAir::whitespace(char c)
{
  Serial.println(F("whitespace"));
}

void OpenWeatherMapAir::startDocument()
{
  Serial.println(F("start document"));
  Serial.println();
}

void OpenWeatherMapAir::key(String key)
{
  currentKey = String(key);
}

void OpenWeatherMapAir::value(String value)
{
  // "aqi": 2, uint16_t aqi;
  if (currentParent == "main" && currentKey == "aqi")
  {
    this->data->aqi = value.toInt();
    Serial.printf_P(PSTR("aqi: %ld\n"), value.toInt());
  }
  // "dt": 1751288421, uint32_t observationTime;
  if (currentKey == "dt")
  {
    this->data->observationTime = value.toInt();
    Serial.printf_P(PSTR("observationTime: %ld\n"), value.toInt());
  }
}

void OpenWeatherMapAir::endArray()
{
}

void OpenWeatherMapAir::startObject()
{
  currentParent = currentKey;
}

void OpenWeatherMapAir::endObject()
{
  if (currentParent == "weather")
  {
    weatherItemCounter++;
  }
  currentParent = "";
}

void OpenWeatherMapAir::endDocument()
{
}

void OpenWeatherMapAir::startArray()
{
}
