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
#include "OpenMeteoApi.h"

OpenMeteoApi::OpenMeteoApi() {
}

void OpenMeteoApi::updateWeather(OpenMeteoApiData *data, float lat, float lon) {
  doUpdate(data, buildPath("forecast?", ("latitude=" + String(lat) + "&longitude=" + String(lon))));
}

String OpenMeteoApi::buildPath(String requestType, String locationParameter) {
  return "/v1/" + requestType + locationParameter + "&timezone=auto&timeformat=unixtime&current=uv_index";
}

void OpenMeteoApi::doUpdate(OpenMeteoApiData *data, String path) {
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();
  this->prayItemCounter = 0;
  this->data = data;
  JsonStreamingParser parser;
  parser.setListener(this);
  Serial.printf("[HTTP] Requesting resource at http://%s:%u%s\n", host.c_str(), port, path.c_str());

  WiFiClient client;
#if defined(ESP8266)
  if (client.connect(host, port)) {
#else
  if (client.connect(host.c_str(), port)) {
#endif
    bool isBody = false;
    char c;
    Serial.println("[HTTP] connected, now GETting data");
    client.print("GET " + path + " HTTP/1.1\r\n"
                                 "Host: "
                 + host + "\r\n"
                          "Connection: close\r\n\r\n");

    while (client.connected() || client.available()) {
      if (client.available()) {
        if ((millis() - lost_do) > lostTest) {
          Serial.println("[HTTP] lost in client with a timeout");
          client.stop();
          ESP.restart();
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
    Serial.println("[HTTP] failed to connect to host");
  }
  this->data = nullptr;
}

void OpenMeteoApi::whitespace(char c) {
  Serial.println("whitespace");
}

void OpenMeteoApi::startDocument() {
  Serial.println("start document");
}

void OpenMeteoApi::key(String key) {
  currentKey = String(key);
}

void OpenMeteoApi::value(String value) {  
  // "uv_index": 4.25, float uv_index;
  if (currentKey == "uv_index" && currentParent == "current") {
    this->data->uv_index = value.toFloat();
    Serial.printf("uv_index: %f\n", value.toFloat());
  }
  // "time": "1753625700", uint32_t observationTime;
  if (currentKey == "time" && currentParent == "current") {
    this->data->observationTime = value.toInt();
    Serial.printf("observationTime: %ld\n", value.toInt());
  }
}

void OpenMeteoApi::endArray() {
}


void OpenMeteoApi::startObject() {
  currentParent = currentKey;
}

void OpenMeteoApi::endObject() {
}

void OpenMeteoApi::endDocument() {
}

void OpenMeteoApi::startArray() {
}
