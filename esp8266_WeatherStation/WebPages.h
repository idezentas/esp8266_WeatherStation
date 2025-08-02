#include <Arduino.h>
#pragma once

const char Home_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
    <title>%HOMETITLE%</title>
    <style>
      :root {
        --bg-color: #ffffff;
        --text-color: #333333;
        --input-bg: #f0f0f0;
        --button-bg: #4CAF50;
        --button-text: #ffffff;
        --accent-color: #4CAF50;
        --shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
      }

      @media (prefers-color-scheme: dark) {
        :root {
          --bg-color: #121212;
          --text-color: #eeeeee;
          --input-bg: #2a2a2a;
          --button-bg: #388e3c;
          --button-text: #ffffff;
          --accent-color: #66bb6a;
          --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
        }
      }

      body {
        margin: 0;
        font-family: Arial, sans-serif;
        background: var(--bg-color);
        color: var(--text-color);
        transition: background 0.3s, color 0.3s;
      }

      .container {
        max-width: 400px;
        margin: 0 auto;
        padding: 1rem;
        transition: all 0.3s ease-in-out;
      }

      h1 {
        text-align: center;
        font-size: 1.6rem;
        margin-bottom: 1.5rem;
        color: var(--accent-color);
        text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
      }

      .setting {
        margin-bottom: 1.2rem;
      }

      label {
        display: block;
        margin-bottom: 0.5rem;
        font-weight: bold;
      }

      input[type="text"],
      input[type="number"],
      input[type="password"],
      select,
      button {
        width: 100%;
        box-sizing: border-box;
        padding: 0.75rem;
        background: var(--input-bg);
        border: 2px solid var(--accent-color);
        border-radius: 8px;
        font-size: 1rem;
        color: var(--text-color);
        transition: all 0.3s ease-in-out;
      }

      button {
        background: var(--button-bg);
        color: var(--button-text);
        border: none;
        margin-top: 1rem;
        cursor: pointer;
        font-weight: bold;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      }

      button:hover {
        opacity: 0.95;
        transform: scale(1.01);
      }

      input:focus,
      select:focus {
        outline: none;
        border-color: var(--accent-color);
        box-shadow: 0 0 6px var(--accent-color);
      }

      .buttons {
        margin-top: 1rem;
      }

      input[type="checkbox"] {
        margin-right: 0.5rem;
        transform: scale(1.2);
        vertical-align: middle;
      }

      #status-message {
        display: none;
        padding: 1rem;
        margin-top: 1rem;
        background-color: var(--accent-color);
        color: white;
        border-radius: 8px;
        text-align: center;
        box-shadow: var(--shadow);
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>%HOMETITLE%</h1>
      <div class="buttons">
        <form action="/device" method="GET">
          <button type="submit"><i class="fas fa-cogs"></i> %DEVICESETTINGS%</button>
        </form>
        <form action="/security" method="GET">
          <button type="submit"><i class="fas fa-lock"></i> %SECURITYSETTINGS%</button>
        </form>
        <form action="/worldclock" method="GET">
          <button type="submit"><i class="fas fa-clock"></i> %WORLDCLOCKSETTINGS%</button>
        </form>
        <form action="/currency" method="GET">
          <button type="submit"><i class="fas fa-money-bill"></i> %CURRENCYSETTINGS%</button>
        </form>
        <form action="/resetdefault" method="GET" id="resetForm">
          <button type="submit"><i class="fas fa-undo-alt"></i> %DEFAULTSETTINGS%</button>
        </form>
        <form action="/update" method="GET">
          <button type="submit"><i class="fas fa-upload"></i> %OTATEXT%</button>
        </form>        
      </div>
    </div>
    <script>
      const resetForm = document.getElementById('resetForm');
      const confirmMessage = "%DEFAULTALERT%";
      resetForm.addEventListener('submit', function(e) {
        if (!confirm(confirmMessage)) {
          e.preventDefault();
        }
        const forgetForm = document.getElementById('forgetForm');
      const confirmMessage1 = "%DEFAULTALERT%";
      forgetForm.addEventListener('submit', function(e) {
        if (!confirm(confirmMessage1)) {
          e.preventDefault();
        }
      });
    </script>    
  </body>
</html>
)rawliteral";

const char Device_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
    <title>%DEVICESETTINGS%</title>
    <style>
      :root {
        --bg-color: #ffffff;
        --text-color: #333333;
        --input-bg: #f0f0f0;
        --button-bg: #4CAF50;
        --button-text: #ffffff;
        --accent-color: #4CAF50;
        --shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
      }

      @media (prefers-color-scheme: dark) {
        :root {
          --bg-color: #121212;
          --text-color: #eeeeee;
          --input-bg: #2a2a2a;
          --button-bg: #388e3c;
          --button-text: #ffffff;
          --accent-color: #66bb6a;
          --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
        }
      }

      body {
        margin: 0;
        font-family: Arial, sans-serif;
        background: var(--bg-color);
        color: var(--text-color);
        transition: background 0.3s, color 0.3s;
      }

      .container {
        max-width: 400px;
        margin: 0 auto;
        padding: 1rem;
        transition: all 0.3s ease-in-out;
      }

      h1 {
        text-align: center;
        font-size: 1.6rem;
        margin-bottom: 1.5rem;
        color: var(--accent-color);
        text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
      }

      .setting {
        margin-bottom: 1.2rem;
      }

      label {
        display: block;
        margin-bottom: 0.5rem;
        font-weight: bold;
      }

      input[type="text"],
      input[type="number"],
      input[type="password"],
      select,
      button {
        width: 100%;
        box-sizing: border-box;
        padding: 0.75rem;
        background: var(--input-bg);
        border: 2px solid var(--accent-color);
        border-radius: 8px;
        font-size: 1rem;
        color: var(--text-color);
        transition: all 0.3s ease-in-out;
      }

      button {
        background: var(--button-bg);
        color: var(--button-text);
        border: none;
        margin-top: 1rem;
        cursor: pointer;
        font-weight: bold;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      }

      button:hover {
        opacity: 0.95;
        transform: scale(1.01);
      }

      input:focus,
      select:focus {
        outline: none;
        border-color: var(--accent-color);
        box-shadow: 0 0 6px var(--accent-color);
      }

      .buttons {
        margin-top: 1rem;
      }

      input[type="checkbox"] {
        margin-right: 0.5rem;
        transform: scale(1.2);
        vertical-align: middle;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>%DEVICESETTINGS%</h1>
      <form action="/savedevice" method="GET">
        <div class="setting">
          <label for="OWMApiKey">%OWMAPIKEYTEXT% (%GETFROMTEXT% <a href='https://openweathermap.org/' target='_BLANK'>%HERETEXT%</a>)</label>
          <input type="password" id="OWMApiKey" name="OWMApiKey" required placeholder="%APIPLACE%" value="%OWMAPIKEY%">
          <button type="button" id="toggleOWMPassword" onclick="toggleApiKey()">
            <i class='fas fa-eye'></i> %SHOWPASS% </button>
        </div>
        <div class="setting">
          <label for="geoApiKey">%GEOAPIKEYTEXT% (%GETFROMTEXT% <a href='https://opencagedata.com/' target='_BLANK'>%HERETEXT%</a>)</label>
          <input type="password" id="geoApiKey" name="geoApiKey" required placeholder="%APIPLACE%" value="%GEOAPIKEY%">
          <button type="button" id="toggleGEOPassword" onclick="toggleGeoApiKey()"><i class='fas fa-eye'></i> %SHOWPASS% </button>
        </div>
        <div class="setting">
          <label for="district">%DISTRICTTEXT%</label>
          <input type="text" id="district" name="district" required placeholder="%DISTRICTPLACE%" value="%DISTRICT%">
        </div>
        <div class="setting">
          <label for="city">%CITYTEXT%</label>
          <input type="text" id="city" name="city" required placeholder="%CITYPLACE%" value="%CITY%">
        </div>
        <div class="setting">
          <label for="country">%COUNTRYTEXT%</label>
          <input type="text" id="country" name="country" required placeholder="%COUNTRYPLACE%" value="%COUNTRY%">
          <button type="button" onclick="geocodeFromCity()"><i class="fas fa-search"></i> %FINDGEOTIME%</button>
        </div>
        <div class="setting">
          <label for="latitude">%LATTEXT%</label>
          <input type="text" id="latitude" name="latitude" required placeholder="%LATPLACE%" value="%LAT%" inputmode="decimal">
        </div>
        <div class="setting">
          <label for="longitude">%LONTEXT%</label>
          <input type="text" id="longitude" name="longitude" required placeholder="%LONPLACE%" value="%LON%" inputmode="decimal">
        </div>
        <div class="setting">
          <label for="timezone">%TZTEXT%</label>
          <input type="text" id="timezone" name="timezone" required placeholder="%TZPLACE%" value="%TZ%" inputmode="numeric">
        </div>
        <div class="setting">
          <label>
            <input type="checkbox" id="isDST" name="isDST" %IS_DST%> %DSTTEXT% </label>
        </div>
        <div class="setting">
          <label for="timezoneName">%TZNAMETEXT%</label>
          <input type="text" id="timezoneName" name="timezoneName" required placeholder="%TZNAMEPLACE%" value="%TZNAME%">
        </div>
        <div class="setting">
          <label for="timezoneNameShort">%TZNAMESHORTTEXT%</label>
          <input type="text" id="timezoneNameShort" name="timezoneNameShort" required placeholder="%TZNAMESHORTPLACE%" value="%TZNAMESHORT%">
        </div>
        <div class="setting">
          <label for="units">%UNITTEXT%</label>
          <select id="units" name="units">
            <option value="metric" %METRIC%>%METRICTEXT%</option>
            <option value="imperial" %IMPERIAL%>%IMPERIALTEXT%</option>
          </select>
        </div>
        <div class="setting">
          <label for="updateInterval">%INTERVALTEXT%</label>
          <input type="text" id="updateInterval" name="updateInterval" required placeholder="%INTERVALPLACE%" value="%INTERVAL%" inputmode="numeric">
        </div>
        <div class="setting">
          <label for="turnOnTime">%TIMEONTEXT%</label>
          <input type="text" id="turnOnTime" name="turnOnTime" required placeholder="%TIMEONPLACE%" value="%TIMEON%">
        </div>
        <div class="setting">
          <label for="turnOffTime">%TIMEOFFTEXT%</label>
          <input type="text" id="turnOffTime" name="turnOffTime" required placeholder="%TIMEOFFPLACE%" value="%TIMEOFF%">
        </div>
        <div class="buttons">
          <button type="submit"><i class="fas fa-floppy-disk"></i> %SAVETEXT%</button>
        </div>
      </form>
      <form action="/" method="GET">
        <div class="buttons">
          <button type="submit"><i class="fas fa-home"></i> %GOHOMETEXT%</button>
        </div>
      </form>
    </div>
    <script>
      function toggleApiKey() {
        const input = document.getElementById("OWMApiKey");
        input.type = input.type === "password" ? "text" : "password";
        const toggleBtn = document.getElementById('toggleOWMPassword');
        const icon = toggleBtn.querySelector('i');
        icon.classList.toggle('fa-eye');
        icon.classList.toggle('fa-eye-slash');
        toggleBtn.textContent = (input.type === "password") ? " %SHOWPASS%" : " %HIDEPASS%";
        toggleBtn.prepend(icon);
      }

      function toggleGeoApiKey() {
        const input = document.getElementById("geoApiKey");
        input.type = input.type === "password" ? "text" : "password";
        const toggleBtn = document.getElementById('toggleGEOPassword');
        const icon = toggleBtn.querySelector('i');
        icon.classList.toggle('fa-eye');
        icon.classList.toggle('fa-eye-slash');
        toggleBtn.textContent = (input.type === "password") ? " %SHOWPASS%" : " %HIDEPASS%";
        toggleBtn.prepend(icon);        
      }

      function geocodeFromCity() {
        const city = document.getElementById('city').value.trim();
        const district = document.getElementById('district').value.trim();
        const country = document.getElementById('country').value.trim().toUpperCase();
        const lang = document.getElementById('country').value.trim().toLowerCase();
        const geoApiKey = document.getElementById('geoApiKey').value;
        if (!district || !city || !country || !geoApiKey) {
          alert("%GEOALERT%");
          return;
        }
        const url = `https://api.opencagedata.com/geocode/v1/json?q=${encodeURIComponent(district)},${encodeURIComponent(city)},${encodeURIComponent(country)}&key=${geoApiKey}&language=${lang}&pretty=1`;
        fetch(url).then(response => response.json()).then(data => {
          if (data.results && data.results.length > 0) {
            const geometry = data.results[0].geometry;
            const timezone = data.results[0].annotations.timezone;
            const roadinfo = data.results[0].annotations.roadinfo;
            const lat = geometry.lat;
            const lng = geometry.lng;
            const now_in_dst = timezone.now_in_dst;
            const gmtOffset = timezone.offset_sec / 3600;
            const name = timezone.name;
            const short_name = timezone.short_name;
            const speed_in = roadinfo.speed_in;
            if (gmtOffset) {
              document.getElementById('timezone').value = gmtOffset;
            }
            if (name) {
              document.getElementById('timezoneName').value = name;
            }
            if (lat) {
              document.getElementById('latitude').value = lat;
            }
            if (lng) {
              document.getElementById('longitude').value = lng;
            }
            if (short_name) {
              document.getElementById('timezoneNameShort').value = short_name;
            }
            const isDST = document.getElementById('isDST');
            if (now_in_dst === "1" || now_in_dst === 1) {
              isDST.checked = true;
            } else {
              isDST.checked = false;
            }
            var selectUnit = document.getElementById("units");
            if (speed_in.toLowerCase() === "km/h") {
              selectUnit.value = "metric";
            } else {
              selectUnit.value = "imperial";
            }
          } else {
            alert("%GEOALERT2%");
          }
        }).catch(err => {
          console.error(err);
          alert("%GEOALERT3%");
        });
      }
    </script>
  </body>
</html>
)rawliteral";

const char World_Clock_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
    <title>%WORLDCLOCKSETTINGS%</title>
    <style>
        :root {
            --bg-color: #ffffff;
            --text-color: #333333;
            --input-bg: #f0f0f0;
            --button-bg: #4CAF50;
            --button-text: #ffffff;
            --accent-color: #4CAF50;
            --shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
        }

        @media (prefers-color-scheme: dark) {
            :root {
                --bg-color: #121212;
                --text-color: #eeeeee;
                --input-bg: #2a2a2a;
                --button-bg: #388e3c;
                --button-text: #ffffff;
                --accent-color: #66bb6a;
                --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
            }
        }

        body {
            margin: 0;
            font-family: Arial, sans-serif;
            background: var(--bg-color);
            color: var(--text-color);
            transition: background 0.3s, color 0.3s;
        }

        .container {
            max-width: 400px;
            margin: 0 auto;
            padding: 1rem;
            transition: all 0.3s ease-in-out;
        }

        h1 {
            text-align: center;
            font-size: 1.6rem;
            margin-bottom: 1.5rem;
            color: var(--accent-color);
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
        }

        .setting {
            margin-bottom: 1.2rem;
        }

        label {
            display: block;
            margin-bottom: 0.5rem;
            font-weight: bold;
        }

        input[type="text"],
        input[type="number"],
        input[type="password"],
        select,
        button {
            width: 100%;
            box-sizing: border-box;
            padding: 0.75rem;
            background: var(--input-bg);
            border: 2px solid var(--accent-color);
            border-radius: 8px;
            font-size: 1rem;
            color: var(--text-color);
            transition: all 0.3s ease-in-out;
        }

        button {
            background: var(--button-bg);
            color: var(--button-text);
            border: none;
            margin-top: 1rem;
            cursor: pointer;
            font-weight: bold;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }

        button:hover {
            opacity: 0.95;
            transform: scale(1.01);
        }

        input:focus,
        select:focus {
            outline: none;
            border-color: var(--accent-color);
            box-shadow: 0 0 6px var(--accent-color);
        }

        .buttons {
            margin-top: 1rem;
        }

        input[type="checkbox"] {
            margin-right: 0.5rem;
            transform: scale(1.2);
            vertical-align: middle;
        }

        .centered-text {
            text-align: center;
            font-weight: bold;
            margin-bottom: 0.8rem;
            font-size: 1.3rem;
            color: var(--accent-color);
            margin-top: 2rem;
        }
    </style>
</head>

<body>
    <div class="container">
        <h1>%WORLDCLOCKSETTINGS%</h1>
        <form action="/saveworldclock" method="GET">
            <div class="setting">
                <label for="geoApiKey">%GEOAPIKEYTEXT% (%GETFROMTEXT% <a href='https://opencagedata.com/'
                        target='_BLANK'>%HERETEXT%</a>)</label>
                <input type="password" id="geoApiKey" name="geoApiKey" required placeholder="%APIPLACE%"
                    value="%GEOAPIKEY%">
                <button type="button" id="toggleGEOPassword" onclick="toggleGeoApiKey()"><i class='fas fa-eye'></i>
                    %SHOWPASS% </button>
            </div>

            <div class="centered-text">%CITY1LABEL%</div>
            <div class="setting">
                <label for="city1">%CITYTEXT%</label>
                <input type="text" id="city1" name="city1" required placeholder="%CITY1PLACE%" value="%CITY1%">
            </div>
            <div class="setting">
                <label for="country1">%COUNTRYTEXT%</label>
                <input type="text" id="country1" name="country1" required placeholder="%COUNTRY1PLACE%"
                    value="%COUNTRY1%">
            </div>
            <div class="setting">
                <label for="timezone1">%TZTEXT%</label>
                <input type="text" id="timezone1" name="timezone1" required placeholder="%TZ1PLACE%" value="%TZ1%"
                    inputmode="numeric">
            </div>
            <div class="setting">
                <label>
                    <input type="checkbox" id="isDST1" name="isDST1" %IS_DST1%> %DSTTEXT% </label>
            </div>
            <div class="setting">
                <label for="timezoneName1">%TZNAMETEXT%</label>
                <input type="text" id="timezoneName1" name="timezoneName1" required placeholder="%TZNAME1PLACE%"
                    value="%TZNAME1%">
            </div>
            <div class="setting">
                <label for="timezoneNameShort1">%TZNAMESHORTTEXT%</label>
                <input type="text" id="timezoneNameShort1" name="timezoneNameShort1" required
                    placeholder="%TZNAMESHORT1PLACE%" value="%TZNAMESHORT1%">
            </div>

            <div class="centered-text">%CITY2LABEL%</div>
            <div class="setting">
                <label for="city2">%CITYTEXT%</label>
                <input type="text" id="city2" name="city2" required placeholder="%CITY2PLACE%" value="%CITY2%">
            </div>
            <div class="setting">
                <label for="country2">%COUNTRYTEXT%</label>
                <input type="text" id="country2" name="country2" required placeholder="%COUNTRY2PLACE%"
                    value="%COUNTRY2%">
            </div>
            <div class="setting">
                <label for="timezone2">%TZTEXT%</label>
                <input type="text" id="timezone2" name="timezone2" required placeholder="%TZ2PLACE%" value="%TZ2%"
                    inputmode="numeric">
            </div>
            <div class="setting">
                <label>
                    <input type="checkbox" id="isDST2" name="isDST2" %IS_DST2%> %DSTTEXT% </label>
            </div>
            <div class="setting">
                <label for="timezoneName2">%TZNAMETEXT%</label>
                <input type="text" id="timezoneName2" name="timezoneName2" required placeholder="%TZNAME2PLACE%"
                    value="%TZNAME2%">
            </div>
            <div class="setting">
                <label for="timezoneNameShort2">%TZNAMESHORTTEXT%</label>
                <input type="text" id="timezoneNameShort2" name="timezoneNameShort2" required
                    placeholder="%TZNAMESHORT2PLACE%" value="%TZNAMESHORT2%">
            </div>

            <div class="centered-text">%CITY3LABEL%</div>
            <div class="setting">
                <label for="city3">%CITYTEXT%</label>
                <input type="text" id="city3" name="city3" required placeholder="%CITY3PLACE%" value="%CITY3%">
            </div>
            <div class="setting">
                <label for="country3">%COUNTRYTEXT%</label>
                <input type="text" id="country3" name="country3" required placeholder="%COUNTRY3PLACE%"
                    value="%COUNTRY3%">
            </div>
            <div class="setting">
                <label for="timezone3">%TZTEXT%</label>
                <input type="text" id="timezone3" name="timezone3" required placeholder="%TZ3PLACE%" value="%TZ3%"
                    inputmode="numeric">
            </div>
            <div class="setting">
                <label>
                    <input type="checkbox" id="isDST3" name="isDST3" %IS_DST3%> %DSTTEXT% </label>
            </div>
            <div class="setting">
                <label for="timezoneName3">%TZNAMETEXT%</label>
                <input type="text" id="timezoneName3" name="timezoneName3" required placeholder="%TZNAME3PLACE%"
                    value="%TZNAME3%">
            </div>
            <div class="setting">
                <label for="timezoneNameShort3">%TZNAMESHORTTEXT%</label>
                <input type="text" id="timezoneNameShort3" name="timezoneNameShort3" required
                    placeholder="%TZNAMESHORT3PLACE%" value="%TZNAMESHORT3%">
            </div>

            <button type="button" onclick="geocodeAllCities()"><i class="fas fa-search"></i> %FINDTIMES%</button>

            <div class="buttons">
                <button type="submit"><i class="fas fa-floppy-disk"></i> %SAVETEXT%</button>
            </div>
        </form>
        <form action="/" method="GET">
            <div class="buttons">
                <button type="submit"><i class="fas fa-home"></i> %GOHOMETEXT%</button>
            </div>
        </form>
    </div>
    <script>
        function toggleGeoApiKey() {
            const input = document.getElementById("geoApiKey");
            input.type = input.type === "password" ? "text" : "password";
            const toggleBtn = document.getElementById('toggleGEOPassword');
            const icon = toggleBtn.querySelector('i');
            icon.classList.toggle('fa-eye');
            icon.classList.toggle('fa-eye-slash');
            toggleBtn.textContent = (input.type === "password") ? " %SHOWPASS%" : " %HIDEPASS%";
            toggleBtn.prepend(icon);
        }

        function geocodeAllCities() {
            const geoApiKey = document.getElementById('geoApiKey').value;
            if (!geoApiKey) {
                alert("%GEOALERT4%");
                return;
            }

            const cityInputs = document.querySelectorAll('[id^="city"]');

            cityInputs.forEach(cityInput => {
                const idSuffix = cityInput.id.replace('city', '');
                const city = cityInput.value.trim();
                const countryInput = document.getElementById(`country${idSuffix}`);
                const country = countryInput ? countryInput.value.trim().toUpperCase() : "";
                const lang = country.toLowerCase();

                if (!city || !country) {
                    console.warn(`%GEOALERT5% city${idSuffix}`);
                    return;
                }

                const url = `https://api.opencagedata.com/geocode/v1/json?q=${encodeURIComponent(city)},${encodeURIComponent(country)}&key=${geoApiKey}&language=${lang}&pretty=1`;

                fetch(url)
                    .then(response => response.json())
                    .then(data => {
                        if (data.results && data.results.length > 0) {
                            const timezone = data.results[0].annotations.timezone;
                            const now_in_dst = timezone.now_in_dst;
                            const gmtOffset = timezone.offset_sec / 3600;
                            const name = timezone.name;
                            const short_name = timezone.short_name;

                            const tzOffsetEl = document.getElementById(`timezone${idSuffix}`);
                            const tzNameEl = document.getElementById(`timezoneName${idSuffix}`);
                            const tzShortEl = document.getElementById(`timezoneNameShort${idSuffix}`);
                            const dstCheckbox = document.getElementById(`isDST${idSuffix}`);

                            if (tzOffsetEl) tzOffsetEl.value = gmtOffset;
                            if (tzNameEl) tzNameEl.value = name;
                            if (tzShortEl) tzShortEl.value = short_name;
                            if (dstCheckbox) dstCheckbox.checked = now_in_dst === "1" || now_in_dst === 1;
                        } else {
                            alert(`%GEOALERT2%: city${idSuffix}`);
                        }
                    })
                    .catch(err => {
                        console.error(err);
                        alert(`%GEOALERT3%: city${idSuffix}`);
                    });
            });
        }
    </script>
</body>

</html>
)rawliteral";

const char Currency_Page[] PROGMEM = R"rawliteral(
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
    <title>%CURRENCYSETTINGS%</title>
    <style>
      :root {
        --bg-color: #ffffff;
        --text-color: #333333;
        --input-bg: #f0f0f0;
        --button-bg: #4CAF50;
        --button-text: #ffffff;
        --accent-color: #4CAF50;
        --shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
      }

      @media (prefers-color-scheme: dark) {
        :root {
          --bg-color: #121212;
          --text-color: #eeeeee;
          --input-bg: #2a2a2a;
          --button-bg: #388e3c;
          --button-text: #ffffff;
          --accent-color: #66bb6a;
          --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
        }
      }

      body {
        margin: 0;
        font-family: Arial, sans-serif;
        background: var(--bg-color);
        color: var(--text-color);
        transition: background 0.3s, color 0.3s;
      }

      .container {
        max-width: 400px;
        margin: 0 auto;
        padding: 1rem;
        transition: all 0.3s ease-in-out;
      }

      h1 {
        text-align: center;
        font-size: 1.6rem;
        margin-bottom: 1.5rem;
        color: var(--accent-color);
        text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
      }

      .setting {
        margin-bottom: 1.2rem;
      }

      label {
        display: block;
        margin-bottom: 0.5rem;
        font-weight: bold;
      }

      input[type="text"],
      input[type="number"],
      input[type="password"],
      select,
      button {
        width: 100%;
        box-sizing: border-box;
        padding: 0.75rem;
        background: var(--input-bg);
        border: 2px solid var(--accent-color);
        border-radius: 8px;
        font-size: 1rem;
        color: var(--text-color);
        transition: all 0.3s ease-in-out;
      }

      button {
        background: var(--button-bg);
        color: var(--button-text);
        border: none;
        margin-top: 1rem;
        cursor: pointer;
        font-weight: bold;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      }

      button:hover {
        opacity: 0.95;
        transform: scale(1.01);
      }

      input:focus,
      select:focus {
        outline: none;
        border-color: var(--accent-color);
        box-shadow: 0 0 6px var(--accent-color);
      }

      .buttons {
        margin-top: 1rem;
      }

      input[type="checkbox"] {
        margin-right: 0.5rem;
        transform: scale(1.2);
        vertical-align: middle;
      }

      .centered-text {
        text-align: center;
        font-weight: bold;
        margin-bottom: 0.8rem;
        font-size: 1.3rem;
        color: var(--accent-color);
        margin-top: 2rem;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>%CURRENCYSETTINGS%</h1>
      <form action="/savecurrency" method="GET">
        <div class="centered-text">%APIWARNINGTEXT%</div>
        <div class="centered-text">%CURRENCY1TEXT%</div>
        <div class="setting">
          <label for="base1text">%BASETEXT%</label>
          <select id="base1" name="base1"></select>
        </div>
        <div class="setting">
          <label for="target1text">%TARGETTEXT%</label>
          <select id="target1" name="target1"></select>
        </div>
        <div class="centered-text">%CURRENCY2TEXT%</div>
        <div class="setting">
          <label for="base2text">%BASETEXT%</label>
          <select id="base2" name="base2"></select>
        </div>
        <div class="setting">
          <label for="target2text">%TARGETTEXT%</label>
          <select id="target2" name="target2"></select>
        </div>
        <div class="buttons">
          <button type="submit">
            <i class="fas fa-floppy-disk"></i> %SAVETEXT% </button>
        </div>
      </form>
      <form action="/" method="GET">
        <div class="buttons">
          <button type="submit">
            <i class="fas fa-home"></i> %GOHOMETEXT% </button>
        </div>
      </form>
    </div>
    <script>
      let lang = "%LANG%".toLowerCase();
      if (lang != "tr" && lang != "en") {
        lang = "en";
      }
      const githubJsonUrl = `https://raw.githubusercontent.com/idezentas/esp8266_WeatherStation/refs/heads/main/currency-${lang}.json`;
      const selections = {
        base1: "%BASE1%",
        target1: "%TARGET1%",
        base2: "%BASE2%",
        target2: "%TARGET2%"
      };
      fetch(githubJsonUrl).then(res => res.json()).then(data => {
        const codes = Object.keys(data);
        function fillSelect(id, selected) {
          const sel = document.getElementById(id);
          sel.innerHTML = "";
          codes.forEach(code => {
            const option = document.createElement("option");
            option.value = code;
            option.textContent = `${code.toUpperCase()} - ${data[code].name} (${data[code].symbol})`;
            if (code.toUpperCase() === selected.toUpperCase()) option.selected = true;
            sel.appendChild(option);
          });
        }
        fillSelect("base1", selections.base1);
        fillSelect("target1", selections.target1);
        fillSelect("base2", selections.base2);
        fillSelect("target2", selections.target2);
      }).catch(err => {
        console.error("%CURRENCYALERT%", err);
      });
    </script>
  </body>
</html>
)rawliteral";

const char Security_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
    <title>%SECURITYSETTINGS%</title>
    <style>
      :root {
        --bg-color: #ffffff;
        --text-color: #333333;
        --input-bg: #f0f0f0;
        --button-bg: #4CAF50;
        --button-text: #ffffff;
        --accent-color: #4CAF50;
        --shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
      }

      @media (prefers-color-scheme: dark) {
        :root {
          --bg-color: #121212;
          --text-color: #eeeeee;
          --input-bg: #2a2a2a;
          --button-bg: #388e3c;
          --button-text: #ffffff;
          --accent-color: #66bb6a;
          --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
        }
      }

      body {
        margin: 0;
        font-family: Arial, sans-serif;
        background: var(--bg-color);
        color: var(--text-color);
        transition: background 0.3s, color 0.3s;
      }

      .container {
        max-width: 400px;
        margin: 0 auto;
        padding: 1rem;
        transition: all 0.3s ease-in-out;
      }

      h1 {
        text-align: center;
        font-size: 1.6rem;
        margin-bottom: 1.5rem;
        color: var(--accent-color);
        text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.1);
      }

      .setting {
        margin-bottom: 1.2rem;
      }

      label {
        display: block;
        margin-bottom: 0.5rem;
        font-weight: bold;
      }

      input[type="text"],
      input[type="number"],
      input[type="password"],
      select,
      button {
        width: 100%;
        box-sizing: border-box;
        padding: 0.75rem;
        background: var(--input-bg);
        border: 2px solid var(--accent-color);
        border-radius: 8px;
        font-size: 1rem;
        color: var(--text-color);
        transition: all 0.3s ease-in-out;
      }

      button {
        background: var(--button-bg);
        color: var(--button-text);
        border: none;
        margin-top: 1rem;
        cursor: pointer;
        font-weight: bold;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      }

      button:hover {
        opacity: 0.95;
        transform: scale(1.01);
      }

      input:focus,
      select:focus {
        outline: none;
        border-color: var(--accent-color);
        box-shadow: 0 0 6px var(--accent-color);
      }

      .buttons {
        margin-top: 1rem;
      }

      input[type="checkbox"] {
        margin-right: 0.5rem;
        transform: scale(1.2);
        vertical-align: middle;
      }

    </style>
  </head>
  <body>
    <div class="container">
      <h1>%SECURITYSETTINGS%</h1>
      <form action="/savesecurity" method="GET">
        <div class="setting">
          <label for="user">%USERTEXT%</label>
          <input type="text" id="user" name="user" required placeholder="%USERPLACE%" value="%USER%">
        </div>
        <div class="setting">
          <label for="password">%PASSWORDTEXT%</label>
          <input type="password" id="password" name="password" required placeholder="%PASSWORDPLACE2%" value="%PASSWORD%">
          <button type="button" id="toggleSecurityPassword" onclick="toggleSecurityPass()"><i class='fas fa-eye'></i> %SHOWPASS%</button>
        </div>
        <div class="buttons">
          <button type="submit"><i class="fas fa-floppy-disk"></i> %SAVETEXT%</button>
        </div>
      </form>
      <form action="/" method="GET">
        <button type="submit"><i class="fas fa-home"></i> %GOHOMETEXT%</button>
      </form>
    </div>
    <script>
      function toggleSecurityPass() {
        const input = document.getElementById("password");
        input.type = input.type === "password" ? "text" : "password";
        const toggleBtn = document.getElementById('toggleSecurityPassword');
        const icon = toggleBtn.querySelector('i');
        icon.classList.toggle('fa-eye');
        icon.classList.toggle('fa-eye-slash');
        toggleBtn.textContent = (input.type === "password") ? " %SHOWPASS%" : " %HIDEPASS%";
        toggleBtn.prepend(icon);        
      }
    </script>
  </body>
</html>
)rawliteral";

const char Reset_Default_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%DEFAULTSETTINGS%</title>
  </head>
  <body>
    <h3>%DEFAULTTEXT%</h3>
    <script>
      setTimeout(() => location.reload(), 3000);
    </script>"
  </body>
</html>
)rawliteral";

const char Save_Settings_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%DEVICESETTINGS%</title>
  </head>
  <body>
    <h3>%DEVICETEXT%</h3>
    <script>
      setTimeout(()=>location.href='/', 3000);
    </script>
  </body>
</html>
)rawliteral";

const char Save_World_Clock_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%WORLDCLOCKSETTINGS%</title>
  </head>
  <body>
    <h3>%WORLDCLOCKTEXT%</h3>
    <script>
      setTimeout(()=>location.href='/', 3000);
    </script>
  </body>
</html>
)rawliteral";

const char Save_Currency_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%CURRENCYSETTINGS%</title>
  </head>
  <body>
    <h3>%CURRENCYTEXT%</h3>
    <script>
      setTimeout(()=>location.href='/', 3000);
    </script>
  </body>
</html>
)rawliteral";

const char Save_Security_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%SECURITYSETTINGS%</title>
  </head>
  <body>
    <h3>%DEVICETEXT%</h3>
    <script>
      setTimeout(()=>location.href='/', 3000);
    </script>
  </body>
</html>
)rawliteral";

const char File_Not_Found_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />  
    <title>%FILENOTFOUNDTEXT%</title>
  </head>
  <body>
    <h3>%FILENOTFOUNDTEXT%</h3>
  </body>
</html>
)rawliteral";


const char Missing_Parameter_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />  
    <title>%MISSINGPARTEXT%</title>
  </head>
  <body>
    <h3>%MISSINGPARTEXT%</h3>
  </body>
</html>
)rawliteral";

