/**The MIT License (MIT)

 Copyright (c) 2025 by idezentas
 Thanks to ThingPulse Ltd., https://thingpulse.com

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
#include "TimeZoneDB.h"

TimeZoneDB::TimeZoneDB()
{
}

void TimeZoneDB::updateCurrent(TimeZoneDBData *data, String appId, float lat, float lon)
{
  doUpdate(data, buildPath(appId, "lat=" + String(lat, 7) + "&lng=" + String(lon, 7)));
}

String TimeZoneDB::buildPath(String appId, String locationParameter)
{
  return "/v2.1/get-time-zone?key=" + appId + "&format=json&by=position&" + locationParameter;
}

void TimeZoneDB::doUpdate(TimeZoneDBData *data, String path)
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

void TimeZoneDB::whitespace(char c)
{
  Serial.println(F("whitespace"));
}

void TimeZoneDB::startDocument()
{
  Serial.println(F("start document"));
  Serial.println();
}

void TimeZoneDB::key(String key)
{
  currentKey = String(key);
}

void TimeZoneDB::value(String value)
{
  // "zoneName": "America/Los_Angeles", String zoneName;
  if (currentKey == "zoneName")
  {
    this->data->zoneName = value.c_str();
    Serial.printf_P(PSTR("zoneName: %s\n"), value.c_str());
  }
  // "abbreviation": "PST", String abbreviation;
  if (currentKey == "abbreviation")
  {
    this->data->abbreviation = value.c_str();
    Serial.printf_P(PSTR("abbreviation: %s\n"), value.c_str());
  }
  // "gmtOffset": -28800, int16_t gmtOffset;
  if (currentKey == "gmtOffset")
  {
    this->data->gmtOffset = value.toInt();
    Serial.printf_P(PSTR("gmtOffset: %ld\n"), value.toInt());
  }
  // "dst": "0", uint8_t dst;
  if (currentKey == "dst")
  {
    this->data->dst = value.toInt();
    Serial.printf_P(PSTR("dst: %ld\n"), value.toInt());
  }
}

void TimeZoneDB::endArray()
{
}

void TimeZoneDB::startObject()
{
  currentParent = currentKey;
}

void TimeZoneDB::endObject()
{
  if (currentParent == "weather")
  {
    weatherItemCounter++;
  }
  currentParent = "";
}

void TimeZoneDB::endDocument()
{
  Serial.println();
  Serial.println(F("end document"));
  Serial.println();
}

void TimeZoneDB::startArray()
{
}
