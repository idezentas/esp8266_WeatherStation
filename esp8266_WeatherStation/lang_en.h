#include <Arduino.h>
#pragma once

String OPEN_WEATHER_MAP_LANGUAGE = "en";

String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

String SUN_MOON_TEXT[] = {"SUN", "MOON", "RISE", "SET"};

String MOON_PHASES[] = {"NEW MOON", "WAXING CRESCENT", "FIRST QUARTER", "WAXING GIBBOUS", "FULL MOON", "WANING GIBBOUS", "THIRD QUARTER", "WANING CRESCENT"};

String WIND_DIRECTION[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};

String WIND_DIRECTION_LONG[] = {"NORTH", "NORTHEAST", "EAST", "SOUTHEAST", "SOUTH", "SOUTHWEST", "WEST", "NORTHWEST"};

String AIR_QUALITY_TEXT[] = {"GOOD", "MODERATE", "FAIR", "POOR", "VERY POOR"};

String UV_INDEX_TEXT[] = {"GOOD", "MODERATE", "HIGH", "VERY HIGH", "EXTREME"};

String PROGRESS_TEXT[] = {"Updating time...", "Updating weather...", "Updating forecasts...", "Updating air quality...", "Updating astronomy...", "Done...", "Updating UV index...", "Updating currencies..."};

String HTML_TEXT[] = {"ESP8266 Weather Station", "Device Settings", "WiFi Settings", "Security Settings", "OpenWeatherMap API Key", "Enter your API key", "Show", "OpenCage API Key", "District Name", "Example: Sarıyer", "City Name", "Example: Istanbul", "Country Code", "Example: TR", "Find Location/Time from City", "Latitude", "Example: 41.1719525", "Longitude", "Example: 28.859487", "Time Zone (GMT)", "Example: 3", "Use Daylight Saving Time", "Time Zone", "Example: Europe/Istanbul", "Time Zone Abbreviation", "Example: +03", "Select Unit", "Metric", "Imperial", "Update Interval (Minutes)", "Example: 30", "Save", "Go to Homepage", "Please enter district, city, country and OpenCage API key.", "Location not found.", "API request failed.", "SSID", "Enter WiFi network name", "Password", "Enter WiFi password", "Username", "Enter username", "Enter password", "Default Settings", "Restore to default settings. Device is restarting...", "Restore to Default Settings", "The device will be reset to default settings. Continue?", "Hide", "JSON parsing error", "Settings file not found.", "Import Settings", "Settings loaded successfully. Device is restarting...", "Failed to create file.", "Failed to open file.", "Invalid JSON: ", "Failed to write settings file.", "Export Settings", "Export All Settings (Download)", "Settings File (.json):", "Load Settings (Import)", "OTA Update", "Turn On Time", "Example: 08:00", "Turn Off Time", "Example: 00:00", "Security information successfully changed.", "Device settings saved successfully. Redirecting to homepage...", "get from", "here", "Forget WiFi Network", "File not found", "Missing 'file' parameter", "World Clock Settings", "World clock settings saved successfully. Redirecting to homepage...", "Get Time Zones for All Cities", "City 1", "Example: Los Angeles", "Example: US", "Example: -7", "Example: America/Los_Angeles", "Example: PST", "City 2", "Example: London", "Example: GB", "Example: 0", "Example: Europe/London", "Example: GMT", "City 3", "Example: Seoul", "Example: KR", "Example: 9", "Example: Asia/Seoul", "Example: KST", "Please enter the OpenCage API key.", "Missing city or country:", "Currency Rate Settings", "Values are updated daily by the API.", "Currency Rate 1", "Base Currency", "Target Currency", "Currency Rate 2", "Currency exchange settings saved successfully. Redirecting to homepage...", "Failed to load currency list:"};

String NIGHT_MODE_TEXT[] = {"NIGHT MODE", "IS STARTING", "IS FINISHED"};

String LOG_TEXT[] = {"WEATHER STATION", "Signal Strength (RSSI): ", "Use this URL : ", "Setting readyForUpdate to true", "Display was turned ON: ", "Display was turned OFF: ", "Time to turn display on: ", "Time to turn display off: ", "deserializeJson() failed: ", "Entered config mode", "Please connect to AP", "To setup WiFi connection", "Device Settings Updated:", "Security Settings Updated:", "There is no file with that name.", "The configuration files have been downloaded.", "World Clock Settings Updated:", "Currency Rate Settings Updated:"};
