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
#include <WiFiClientSecure.h>
#include "FloatRates.h"

FloatRates::FloatRates() {
}

void FloatRates::updateCurrency(FloatRatesData *data, String myBaseCurrency, String myTargetCurrency) {
  doUpdate(data, buildPath(myBaseCurrency), myTargetCurrency);
}

String FloatRates::buildPath(String myBaseCurrency) {
  return "/daily/" + myBaseCurrency + ".json";
}

void FloatRates::doUpdate(FloatRatesData *data, String path, String myTargetCurrency) {
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();
  this->data = data;
  this->data->targetName = myTargetCurrency;
  JsonStreamingParser parser;
  parser.setListener(this);
  Serial.printf("[HTTP] Requesting resource at http://%s:%u%s\n", host, port, path.c_str());

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

void FloatRates::whitespace(char c) {
  Serial.println("whitespace");
}

void FloatRates::startDocument() {
  Serial.println("start document");
}

void FloatRates::key(String key) {
  currentKey = String(key);
}

void FloatRates::value(String value) {
  // "date": "Fri, 1 Aug 2025 23:59:00 GMT",
  if (currentParent == this->data->targetName && currentKey == "date") {
    this->data->date = value;
    Serial.printf("date: %s\n", value.c_str());
  }
  // Ex: "rate": 1.1452916771944,
  if (currentParent == this->data->targetName && currentKey == "rate") {
    this->data->rate = value.toFloat();
    Serial.printf("target (%s): %f\n", this->data->targetName.c_str(), value.toFloat());
  }
  // Ex: "inverseRate": 0.87314002180623
  if (currentParent == this->data->targetName && currentKey == "inverseRate") {
    this->data->inverseRate = value.toFloat();
    Serial.printf("target (%s) (inverse): %f\n", this->data->targetName.c_str(), value.toFloat());
  }
}

void FloatRates::endArray() {
}


void FloatRates::startObject() {
  currentParent = currentKey;
}

void FloatRates::endObject() {
}

void FloatRates::endDocument() {
}

void FloatRates::startArray() {
}
