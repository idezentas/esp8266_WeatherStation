#ifndef TZ_DATABASE_H
#define TZ_DATABASE_H

#include <Arduino.h>

class TZDatabase {
public:
  TZDatabase();
  String getPosix(String region);
};

#endif