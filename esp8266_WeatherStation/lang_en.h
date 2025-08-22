#include <Arduino.h>

String OPEN_WEATHER_MAP_LANGUAGE = "en";

String SYSTEM_LANG = "en";

const char* DIFF_TEXT = "DIFF";

const char* HOUR_TEXT = "HR";

const char* INDOOR_TEMP_TEXT = "INDOOR TEMPERATURE";

const char* INDOOR_HUM_TEXT = "INDOOR HUMIDITY";

const char* WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

const char* MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

const char* SUN_MOON_TEXT[] = {"SUN", "MOON"};

const char* MOON_PHASES[] = {"NEW MOON", "WAXING CRESCENT", "FIRST QUARTER", "WAXING GIBBOUS", "FULL MOON", "WANING GIBBOUS", "THIRD QUARTER", "WANING CRESCENT"};

const char* WIND_DIRECTION[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};

const char* WIND_DIRECTION_LONG[] = {"NORTH", "NORTHEAST", "EAST", "SOUTHEAST", "SOUTH", "SOUTHWEST", "WEST", "NORTHWEST"};

const char* AIR_QUALITY_TEXT[] = {"GOOD", "MODERATE", "FAIR", "POOR", "VERY POOR"};

const char* UV_INDEX_TEXT[] = {"GOOD", "MODERATE", "HIGH", "VERY HIGH", "EXTREME"};

const char* PROGRESS_TEXT[] = {"Updating times...", "Updating OWM...", "Updating astronomy...", "Updating OpenMeteo...", "Updating FloatRates...","Done..."};

const char *TIME_UPDATE_TEXT[] = {"NEXT", "UPDATE"};

const char* NIGHT_MODE_TEXT[] = {"NIGHT MODE", "IS STARTING", "IS FINISHED"};

const char* OTA_TEXT[] = {"OTA", "IS STARTING", "IS FINISHED", "IS UPLOADING", "ERROR"};

const char* HTML_TEXT[] = {"ESP8266 Weather Station ", "Device Settings", "Security Settings", "OpenWeatherMap API Key", "Enter your API key", "Show", "OpenCage API Key", "City Name", "Example: Istanbul", "Country Name", "Example: Turkey", "Find Location/Timezone from City", "Latitude", "Example: 41.17", "Longitude", "Example: 28.86", "Time Zone", "Select Unit", "Metric", "Imperial", "Update Interval (Minutes)", "Save", "Homepage", "Please enter city, country and OpenCage API key.", "Location not found.", "API request failed.", "About", "Password", "Username", "Enter username", "Enter password", "Default Settings", "Restore to default settings. The device is restarting...", "Restore to Default Settings", "The device will be reset to default settings. Continue?", "Hide", "OTA Update", "Turn On Time", "Turn Off Time", "Security information successfully changed.", "Device settings saved successfully. Redirecting to homepage...", "get from", "here", "Currency Rate Settings", "Values are updated daily by the API.", "Currency Rate 1", "Base Currency", "Target Currency", "Currency Rate 2", "Currency exchange settings saved successfully. Redirecting to homepage...", "Failed to load currency list:", "Flip display orientation", "Saving...", "Base Currency and Target Currency cannot be the same", "Currency Rate", "Display Settings", "Display settings saved successfully. Redirecting to homepage...", "API Key Settings", "API keys saved successfully. Redirecting to homepage...", "Time zone not found", "Time zones could not be retrieved: ", "Time zones could not be loaded.", "Display Contrast", "Time Format", "Update Data", "24-hour", "12-hour", "Month Display Format", "As Number", "As Name", "Please enter the on/off time in 24-hour format.", "Show on Map"};

const char* LOG_TEXT[] = {"WEATHER STATION", "Signal Strength (RSSI): ", "Use this URL : ", "Setting readyForUpdate to true", "Display was turned on: ", "Display was turned off: ", "Time to turn display on: ", "Time to turn display off: ", "deserializeJson() failed: ", "Entered config mode", "Please connect to AP", "To setup WiFi connection", "Device Settings Updated:", "Security Settings Updated:", "There is no file with that name.", "Currency Rate Settings Updated:", "OTA update started!", "OTA Progress", "Current", "Final", "OTA update finished successfully!", "There was an error during OTA update!", "API Keys Updated:", "Waiting for NTP time...", "Time synchronized."};
