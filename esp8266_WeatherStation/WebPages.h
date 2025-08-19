static const char Home_Page[] PROGMEM = R"rawliteral(
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
      --button-bg: #388e3c;
      --button-text: #ffffff;
      --accent-color: #66bb6a;
      --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

    .buttons {
      margin-top: 1rem;
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
    <h1>%VERSION%</h1>
    <div class="buttons">
      <form action="/apikey" method="GET">
        <button type="submit">
          <i class="fas fa-key"></i> %APIKEYSETTINGS% </button>
      </form>
      <form action="/device" method="GET">
        <button type="submit">
          <i class="fas fa-cogs"></i> %DEVICESETTINGS% </button>
      </form>
      <form action="/display" method="GET">
        <button type="submit">
          <i class="fas fa-desktop"></i> %DISPLAYSETTINGS% </button>
      </form>
      <form action="/security" method="GET">
        <button type="submit">
          <i class="fas fa-lock"></i> %SECURITYSETTINGS% </button>
      </form>
      <form action="/currency" method="GET">
        <button type="submit">
          <i class="fas fa-money-bill"></i> %CURRENCYSETTINGS% </button>
      </form>
      <form action="/updatedata" method="GET">
        <button type="submit">
          <i class="fas fa-cloud-download-alt"></i> %UPDATEDATATEXT% </button>
      </form>
      <form action="/resetdefault" method="GET" id="resetForm">
        <button type="submit">
          <i class="fas fa-undo-alt"></i> %DEFAULTSETTINGS% </button>
      </form>
      <form action="/update" method="GET">
        <button type="submit">
          <i class="fas fa-upload"></i> %OTATEXT% </button>
      </form>
      <form action="https://github.com/idezentas" method="GET" target="_blank">
        <button type="submit">
          <i class="fas fa-question-circle"></i> %ABOUTTEXT% </button>
      </form>
    </div>
    <div id="status-message" role="alert" aria-live="assertive"></div>
  </div>
  <script>
    const resetForm = document.getElementById('resetForm');
    const statusMessage = document.getElementById('status-message');

    function disableButton(form) {
      const btn = form.querySelector('button[type="submit"]');
      btn.disabled = true;
      btn.style.opacity = 0.6;
      btn.style.cursor = 'not-allowed';
    }
    resetForm.addEventListener('submit', function (e) {
      if (!confirm("%DEFAULTALERT%")) {
        e.preventDefault();
        return;
      }
      disableButton(resetForm);
      statusMessage.textContent = "%DEFAULTSETTINGS%";
      statusMessage.style.display = "block";
    });
  </script>
</body>

</html>
)rawliteral";

static const char Device_Page[] PROGMEM = R"rawliteral(
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
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

        html,
        body {
            height: 100%;
            margin: 0;
            padding: 0;
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

        button:hover:not(:disabled) {
            opacity: 0.95;
            transform: scale(1.01);
        }

        button:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
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

        .centered-text {
            text-align: center;
            font-weight: bold;
            margin-bottom: 0.8rem;
            font-size: 1.3rem;
            color: var(--accent-color);
            margin-top: 2rem;
        }

        #toast {
            visibility: hidden;
            min-width: 250px;
            margin-left: -125px;
            background-color: var(--accent-color);
            color: white;
            text-align: center;
            border-radius: 8px;
            padding: 1rem;
            position: fixed;
            top: 1rem;
            left: 50%;
            z-index: 1100;
            font-size: 1rem;
            box-shadow: var(--shadow);
            transition: visibility 0s, opacity 0.5s ease-in-out;
            opacity: 0;
        }

        #toast.show {
            visibility: visible;
            opacity: 1;
        }
    </style>
</head>

<body>
    <div id="toast" role="alert" aria-live="assertive" aria-atomic="true"></div>

    <main class="container" role="main">
        <h1>%DEVICESETTINGS%</h1>
        <form action="/savedevice" method="GET" id="deviceForm">
            <div class="setting">
                <div class="setting">
                    <label for="city">%CITYTEXT%</label>
                    <input type="text" id="city" name="city" required placeholder="%CITYPLACE%" value="%CITY%"
                        autocomplete="address-level2" autocorrect="on" spellcheck="true">
                </div>
                <div class="setting">
                    <label for="country">%COUNTRYTEXT%</label>
                    <input type="text" id="country" name="country" required placeholder="%COUNTRYPLACE%"
                        value="%COUNTRY%" autocomplete="country" autocorrect="on" spellcheck="true">
                    <button type="button" onclick="geocodeFromCity()">
                        <i class="fas fa-search"></i> %FINDGEOTIME%
                    </button>
                </div>
                <div class="setting">
                    <label for="latitude">%LATTEXT%</label>
                    <input type="number" id="latitude" name="latitude" required placeholder="%LATPLACE%" value="%LAT%"
                        inputmode="decimal">
                </div>
                <div class="setting">
                    <label for="longitude">%LONTEXT%</label>
                    <input type="number" id="longitude" name="longitude" required placeholder="%LONPLACE%" value="%LON%"
                        inputmode="decimal">
                </div>
                <div class="setting">
                    <label for="timezoneName">%TZNAMETEXT%</label>
                    <select id="timezoneName">
                        <option>...</option>
                    </select>
                </div>
                <div class="setting">
                    <input type="hidden" id="posixInput" name="posixInput" readonly />
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
                    <select id="updateInterval" name="updateInterval">
                        <option value="up10" %UP10%>10</option>
                        <option value="up15" %UP15%>15</option>
                        <option value="up20" %UP20%>20</option>
                        <option value="up25" %UP25%>25</option>
                        <option value="up30" %UP30%>30</option>
                        <option value="up45" %UP45%>45</option>
                        <option value="up60" %UP60%>60</option>
                    </select>
                </div>
                <div class="buttons">
                    <button type="submit" id="submitBtn">
                        <i class="fas fa-floppy-disk"></i> %SAVETEXT%
                    </button>
                </div>
        </form>
        <form action="/" method="GET">
            <div class="buttons">
                <button type="submit">
                    <i class="fas fa-home"></i> %GOHOMETEXT%
                </button>
            </div>
        </form>
    </main>

    <script>
        const ZONES_URL = 'https://raw.githubusercontent.com/idezentas/esp8266_WeatherStation/refs/heads/main/zones.json';
        const selectTz = document.getElementById('timezoneName');
        const posixInput = document.getElementById('posixInput');
        let DEFAULT_ZONE = "%TZNAME%";
        let zonesObj = null;

        function geocodeFromCity() {
            const city = document.getElementById('city').value.trim();
            const country = document.getElementById('country').value.trim().toUpperCase();
            let geoApiKey = "%GEOAPIKEY%";

            let lang = "%LANG%".toLowerCase();
            if (lang != "tr" && lang != "en") {
                lang = "en";
            }

            if (!city || !country || !geoApiKey) {
                showToast("%GEOALERT%");
                return;
            }
            const url = `https://api.opencagedata.com/geocode/v1/json?q=${encodeURIComponent(city)},${encodeURIComponent(country)}&key=${geoApiKey}&language=${lang}&pretty=1&limit=1`;
            fetch(url).then(response => response.json()).then(data => {
                if (data.results && data.results.length > 0) {
                    const geometry = data.results[0].geometry;
                    const tzname = data.results[0].annotations.timezone.name;
                    const speed_in = data.results[0].annotations.roadinfo.speed_in;
                    const lat = geometry.lat;
                    const lng = geometry.lng;

                    if (tzname) {
                        for (let i = 0; i < selectTz.options.length; i++) {
                            if (selectTz.options[i].value === tzname) {
                                selectTz.selectedIndex = i;
                                updateInput();
                            }
                        }
                    }
                    if (lat) {
                        document.getElementById('latitude').value = lat;
                    }
                    if (lng) {
                        document.getElementById('longitude').value = lng;
                    }
                    var selectUnit = document.getElementById("units");
                    (speed_in.toLowerCase() === "km/h") ? selectUnit.value = "metric" : selectUnit.value = "imperial";

                } else {
                    showToast("%GEOALERT2%");
                }
            }).catch(err => {
                console.error(err);
                showToast("%GEOALERT3%");
            });
        }

        const form = document.getElementById('deviceForm');
        const submitBtn = document.getElementById('submitBtn');
        const toast = document.getElementById('toast');

        function showToast(message) {
            toast.textContent = message;
            toast.classList.add('show');
            setTimeout(() => {
                toast.classList.remove('show');
            }, 3000);
        }

        form.addEventListener('submit', (e) => {
            submitBtn.disabled = true;
            showToast("%FORMSENDING%");
        });

        function populateSelect(zones) {
            zonesObj = zones;
            selectTz.innerHTML = '';
            const entries = Object.entries(zones).sort((a, b) => a[0].localeCompare(b[0], undefined, {
                sensitivity: 'base'
            }));
            if (entries.length === 0) {
                const opt = document.createElement('option');
                opt.textContent = '%TZALERT1%';
                opt.disabled = true;
                selectTz.appendChild(opt);
                posixInput.value = '';
                return;
            }
            let defaultSelectedIndex = -1;
            entries.forEach(([iana, posix], i) => {
                const opt = document.createElement('option');
                opt.value = iana;
                opt.textContent = `${iana}`;
                opt.dataset.posix = posix;
                if (iana === DEFAULT_ZONE) {
                    opt.selected = true;
                    defaultSelectedIndex = i;
                }
                selectTz.appendChild(opt);
            });
            if (defaultSelectedIndex === -1 && selectTz.options.length > 0) {
                selectTz.selectedIndex = 0;
            }
        }

        function updateInput() {
            const opt = selectTz.selectedOptions[0];
            if (!opt) {
                posixInput.value = '';
                return;
            }
            posixInput.value = opt.dataset.posix;
        }
        selectTz.addEventListener('change', updateInput);
        fetch(ZONES_URL).then(resp => {
            if (!resp.ok) throw new Error('%TZALERT2%' + resp.status);
            return resp.json();
        }).then(zones => {
            populateSelect(zones);
            updateInput();
        }).catch(err => {
            console.error(err);
            selectTz.innerHTML = '';
            const opt = document.createElement('option');
            opt.textContent = '%TZALERT3%';
            opt.disabled = true;
            selectTz.appendChild(opt);
            posixInput.value = '';
        });
    </script>
</body>

</html>
)rawliteral";

static const char Currency_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
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
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

        html,
        body {
            height: 100%;
            margin: 0;
            padding: 0;
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

        button:hover:not(:disabled) {
            opacity: 0.95;
            transform: scale(1.01);
        }

        button:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
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

        .centered-text {
            text-align: center;
            font-weight: bold;
            margin-bottom: 0.8rem;
            font-size: 1.3rem;
            color: var(--accent-color);
            margin-top: 2rem;
        }

        #toast {
            visibility: hidden;
            min-width: 250px;
            margin-left: -125px;
            background-color: var(--accent-color);
            color: white;
            text-align: center;
            border-radius: 8px;
            padding: 1rem;
            position: fixed;
            top: 1rem;
            left: 50%;
            z-index: 1100;
            font-size: 1rem;
            box-shadow: var(--shadow);
            transition: visibility 0s, opacity 0.5s ease-in-out;
            opacity: 0;
        }

        #toast.show {
            visibility: visible;
            opacity: 1;
        }
    </style>
</head>

<body>
    <div id="toast" role="alert" aria-live="assertive" aria-atomic="true"></div>

    <main class="container" role="main">
        <h1>%CURRENCYSETTINGS%</h1>
        <form id="deviceForm" action="/savecurrency" method="GET">
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
                <button type="submit" id="submitBtn">
                    <i class="fas fa-floppy-disk"></i> %SAVETEXT% </button>
            </div>
        </form>
        <form action="/" method="GET">
            <div class="buttons">
                <button type="submit">
                    <i class="fas fa-home"></i> %GOHOMETEXT% </button>
            </div>
        </form>
    </main>

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

        const form = document.getElementById('deviceForm');
        const submitBtn = document.getElementById('submitBtn');
        const toast = document.getElementById('toast');

        function showToast(message) {
            toast.textContent = message;
            toast.classList.add('show');
            setTimeout(() => {
                toast.classList.remove('show');
            }, 3000);
        }

        form.addEventListener("submit", function (e) {
            const b1 = document.getElementById("base1").value;
            const t1 = document.getElementById("target1").value;
            const b2 = document.getElementById("base2").value;
            const t2 = document.getElementById("target2").value;

            if ((b1 === t1 && b1 !== "") || (b2 === t2 && b2 !== "")) {
                e.preventDefault();
                showToast("%CURRENCYALERT2%");
                submitBtn.disabled = false;
            } else {
                submitBtn.disabled = true;
                showToast("%FORMSENDING%");
            }
        });
    </script>
</body>

</html>
)rawliteral";

static const char Display_Settings_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
    <title>%DISPLAYSETTINGS%</title>
    <style>
        :root {
            --bg-color: #ffffff;
            --text-color: #333333;
            --input-bg: #f0f0f0;
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

        html,
        body {
            height: 100%;
            margin: 0;
            padding: 0;
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

        input[type="time"],
        input[type="range"],
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

        button:hover:not(:disabled) {
            opacity: 0.95;
            transform: scale(1.01);
        }

        button:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
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
            margin-top: 1rem;
        }
    </style>
</head>

<body>
    <main class="container" role="main">
        <h1>%DISPLAYSETTINGS%</h1>
        <form action="/savedisplay" method="GET">
            <div class="setting">
                <label>
                    <input type="checkbox" id="invDisp" name="invDisp" %IS_INVDISP_CHECKED%> %INVDISPTEXT%
                </label>
            </div>
            <div class="setting">
                <label for="setConstrast">%DISPLAYCONTRAST%</label>
                <input type="range" id="setConstrast" name="setConstrast" required value="%SETCONTRAST%" min=0 max=255>
            </div>
            <div class="setting">
                <label for="hours">%HOURTEXT%</label>
                <select id="hours" name="hours">
                    <option value="hour24" %HOUR24%>%HOUR24TEXT%</option>
                    <option value="hour12" %HOUR12%>%HOUR12TEXT%</option>
                </select>
            </div>
            <div class="setting">
                <label for="months">%MONTHTEXT%</label>
                <select id="months" name="months">
                    <option value="asName" %ASNAME%>%ASNAMETEXT%</option>
                    <option value="asNumber" %ASNUMBER%>%ASNUMBERTEXT%</option>
                </select>
            </div>
            <div class="setting">
                <div class="centered-text">%TIMEWARNINGTEXT%</div>
                <label for="turnOnTime">%TIMEONTEXT%</label>
                <input type="time" id="turnOnTime" name="turnOnTime" required value="%TIMEON%">
            </div>
            <div class="setting">
                <label for="turnOffTime">%TIMEOFFTEXT%</label>
                <input type="time" id="turnOffTime" name="turnOffTime" required value="%TIMEOFF%">
            </div>
            <div class="buttons">
                <button type="submit" id="submitBtn">
                    <i class="fas fa-floppy-disk"></i> %SAVETEXT% </button>
            </div>
        </form>
        <form action="/" method="GET">
            <button type="submit"><i class="fas fa-home"></i> %GOHOMETEXT%</button>
            </div>
        </form>
    </main>

    <script>
        const form = document.getElementById('deviceForm');
        const submitBtn = document.getElementById('submitBtn');

        form.addEventListener('submit', (e) => {
            submitBtn.disabled = true;
            showToast("%FORMSENDING%");
        });
    </script>
</body>

</html>
)rawliteral";

static const char Security_Page[] PROGMEM = R"rawliteral(
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
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 8px rgba(0, 0, 0, 0.5);
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

        html,
        body {
            height: 100%;
            margin: 0;
            padding: 0;
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
        input[type="password"],
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

        button:hover:not(:disabled) {
            opacity: 0.95;
            transform: scale(1.01);
        }

        button:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
        }

        input:focus {
            outline: none;
            border-color: var(--accent-color);
            box-shadow: 0 0 6px var(--accent-color);
        }

        .buttons {
            margin-top: 1rem;
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
    <main class="container" role="main">
        <h1>%SECURITYSETTINGS%</h1>
        <form action="/savesecurity" method="GET">
            <div class="setting">
                <label for="user">%USERTEXT%</label>
                <input type="text" id="user" name="user" required minlength="5" placeholder="%USERPLACE%"
                    value="%USER%">
            </div>
            <div class="setting">
                <label for="password">%PASSWORDTEXT%</label>
                <input type="password" id="password" name="password" required minlength="6"
                    placeholder="%PASSWORDPLACE2%" value="%PASSWORD%">
                <button type="button" id="toggleSecurityPassword" onclick="toggleSecurityPass()"><i
                        class='fas fa-eye'></i> %SHOWPASS%</button>
            </div>
            <div class="buttons">
                <button type="submit" id="submitBtn">
                    <i class="fas fa-floppy-disk"></i> %SAVETEXT% </button>
            </div>
        </form>
        <form action="/" method="GET">
            <button type="submit"><i class="fas fa-home"></i> %GOHOMETEXT%</button>
            </div>
        </form>
    </main>

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

        const form = document.getElementById('deviceForm');
        const submitBtn = document.getElementById('submitBtn');

        form.addEventListener('submit', (e) => {
            submitBtn.disabled = true;
            showToast("%FORMSENDING%");
        });
    </script>
</body>

</html>
)rawliteral";

static const char ApiKey_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
    <title>%APIKEYSETTINGS%</title>
    <style>
        :root {
            --bg-color: #ffffff;
            --text-color: #333333;
            --input-bg: #f0f0f0;
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

        html,
        body {
            height: 100%;
            margin: 0;
            padding: 0;
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

        button:hover:not(:disabled) {
            opacity: 0.95;
            transform: scale(1.01);
        }

        button:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
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

        .centered-text {
            text-align: center;
            font-weight: bold;
            margin-bottom: 0.8rem;
            font-size: 1.3rem;
            color: var(--accent-color);
            margin-top: 2rem;
        }

        #toast {
            visibility: hidden;
            min-width: 250px;
            margin-left: -125px;
            background-color: var(--accent-color);
            color: white;
            text-align: center;
            border-radius: 8px;
            padding: 1rem;
            position: fixed;
            top: 1rem;
            left: 50%;
            z-index: 1100;
            font-size: 1rem;
            box-shadow: var(--shadow);
            transition: visibility 0s, opacity 0.5s ease-in-out;
            opacity: 0;
        }

        #toast.show {
            visibility: visible;
            opacity: 1;
        }
    </style>
</head>

<body>
    <div id="toast" role="alert" aria-live="assertive" aria-atomic="true"></div>

    <main class="container" role="main">
        <h1>%APIKEYSETTINGS%</h1>
        <form action="/saveapikey" method="GET" id="deviceForm">
            <div class="setting">
                <label for="OWMApiKey">%OWMAPIKEYTEXT% (%GETFROMTEXT% <a href='https://openweathermap.org/'
                        target='_BLANK'>%HERETEXT%</a>) </label>
                <input type="password" id="OWMApiKey" name="OWMApiKey" required placeholder="%APIPLACE%"
                    value="%OWMAPIKEY%">
                <button type="button" id="toggleOWMPassword" onclick="toggleApiKey()">
                    <i class='fas fa-eye'></i> %SHOWPASS%
                </button>
            </div>
            <div class="setting">
                <label for="geoApiKey">%GEOAPIKEYTEXT% (%GETFROMTEXT% <a href='https://opencagedata.com/'
                        target='_BLANK'>%HERETEXT%</a>) </label>
                <input type="password" id="geoApiKey" name="geoApiKey" required placeholder="%APIPLACE%"
                    value="%GEOAPIKEY%">
                <button type="button" id="toggleGEOPassword" onclick="toggleGeoApiKey()">
                    <i class='fas fa-eye'></i> %SHOWPASS%
                </button>
            </div>
            <div class="buttons">
                <button type="submit" id="submitBtn">
                    <i class="fas fa-floppy-disk"></i> %SAVETEXT%
                </button>
            </div>
        </form>
        <form action="/" method="GET">
            <div class="buttons">
                <button type="submit">
                    <i class="fas fa-home"></i> %GOHOMETEXT%
                </button>
            </div>
        </form>
    </main>

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

        const form = document.getElementById('deviceForm');
        const submitBtn = document.getElementById('submitBtn');
        const toast = document.getElementById('toast');

        function showToast(message) {
            toast.textContent = message;
            toast.classList.add('show');
            setTimeout(() => {
                toast.classList.remove('show');
            }, 3000);
        }

        form.addEventListener('submit', (e) => {
            submitBtn.disabled = true;
            showToast("%FORMSENDING%");
        });
    </script>
</body>

</html>
)rawliteral";

static const char Reset_Default_Page[] PROGMEM = R"rawliteral(
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

static const char Save_Settings_Page[] PROGMEM = R"rawliteral(
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

static const char Save_Currency_Page[] PROGMEM = R"rawliteral(
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

static const char Save_Security_Page[] PROGMEM = R"rawliteral(
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

static const char Save_Display_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%DISPLAYSETTINGS%</title>
  </head>
  <body>
    <h3>%DISPLAYTEXT%</h3>
    <script>
      setTimeout(()=>location.href='/', 3000);
    </script>
  </body>
</html>
)rawliteral";

static const char Save_ApiKey_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%APIKEYSETTINGS%</title>
  </head>
  <body>
    <h3>%APIKEYTEXT%</h3>
    <script>
      setTimeout(()=>location.href='/', 3000);
    </script>
  </body>
</html>
)rawliteral";
