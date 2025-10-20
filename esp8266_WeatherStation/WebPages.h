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
      --bg-color: #121212;
      --text-color: #eeeeee;
      --input-bg: #2a2a2a;
      --button-bg: #388e3c;
      --button-text: #ffffff;
      --accent-color: #66bb6a;
      --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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
      margin-bottom: 1.2rem;
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
            --bg-color: #121212;
            --text-color: #eeeeee;
            --input-bg: #2a2a2a;
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
        }

        html,
        body {
            height: 100%;
            margin: 0;
            padding: 0;
        }

        body {
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

        .overlay {
            height: 100%;
            width: 0;
            position: fixed;
            z-index: 1;
            top: 0;
            left: 0;
            background-color: rgba(0, 0, 0, 0.9);
            overflow-x: hidden;
            transition: 0.5s;
        }

        .overlay-content {
            position: relative;
            top: 10%;
            width: 100%;
            margin-top: 0;
            text-align: center;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        .overlay-content a {
            display: block;
            padding: 8px 16px;
            text-decoration: none;
            font-size: 36px;
            color: #818181;
            margin: 5px 0;
            transition: 0.3s;
            white-space: normal;
            word-wrap: break-word;
        }

        .overlay a:hover,
        .overlay a:focus {
            color: #f1f1f1;
        }

        .overlay .closebtn {
            position: absolute;
            top: 20px;
            right: 45px;
            font-size: 60px;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        @media screen and (max-height: 450px) {
            .overlay a {
                font-size: 20px
            }

            .overlay .closebtn {
                font-size: 40px;
                top: 15px;
                right: 35px;
            }
        }

        .menu-icon {
            font-size: 30px;
            cursor: pointer;
            position: fixed;
            top: 20px;
            right: 20px;
            z-index: 2;
        }

        @media screen and (max-width: 600px) {
            .menu-icon {
                top: 15px;
                right: 15px;
            }

            h1 {
                padding-top: 60px;
            }
        }
    </style>
</head>

<body>
    <div id="toast" role="alert" aria-live="assertive" aria-atomic="true"></div>

    <main class="container" role="main">
        <h1>%DEVICESETTINGS%</h1>
        <form action="/savedevice" method="GET" id="deviceForm">
            <div class="setting">
                <label for="city">%CITYTEXT%</label>
                <input type="text" id="city" name="city" required placeholder="%CITYPLACE%" value="%CITY%"
                    autocomplete="address-level2" autocorrect="on" spellcheck="true">
            </div>
            <div class="setting">
                <label for="country">%COUNTRYTEXT%</label>
                <input type="text" id="country" name="country" required placeholder="%COUNTRYPLACE%" value="%COUNTRY%"
                    autocomplete="country" autocorrect="on" spellcheck="true">
                <button type="button" onclick="geocodeFromCity()">
                    <i class="fas fa-search"></i> %FINDGEOTIME%
                </button>
            </div>
            <div class="setting">
                <label for="latitude">%LATTEXT%</label>
                <input type="text" id="latitude" name="latitude" required placeholder="%LATPLACE%" value="%LAT%"
                    inputmode="decimal">
            </div>
            <div class="setting">
                <label for="longitude">%LONTEXT%</label>
                <input type="text" id="longitude" name="longitude" required placeholder="%LONPLACE%" value="%LON%"
                    inputmode="decimal">
            </div>
            <div class="setting">
                <button type="button" onclick="openMap()">
                    <i class='fas fa-map-marker' style='color:red'></i> %SHOWMAPTEXT%
                </button>
            </div>
            <div class="setting">
                <label for="timezoneName">%TZNAMETEXT%</label>
                <input type="text" id="timezoneName" name="timezoneName" placeholder="%TZNAMEPLACE%" value="%TZNAME%" readonly />
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
                    <option value="up30" %UP30%>30</option>
                    <option value="up60" %UP60%>60</option>
                </select>
            </div>
            <div class="buttons">
                <button type="submit" id="submitBtn">
                    <i class="fas fa-floppy-disk"></i> %SAVETEXT%
                </button>
            </div>
        </form>
    </main>

    <div id="myNav" class="overlay">
        <a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
        <div class="overlay-content">
            <a href="/"><i class="fas fa-home"></i> %HOMETEXT%</a>
            <a href="/apikey"><i class="fas fa-key"></i> %APIKEYSETTINGS%</a>
            <a href="/display"><i class="fas fa-desktop"></i> %DISPLAYSETTINGS%</a>
            <a href="/security"><i class="fas fa-lock"></i> %SECURITYSETTINGS%</a>
            <a href="/update"><i class="fas fa-upload"></i> %OTATEXT%</a>
            <a href="https://github.com/idezentas"><i class="fas fa-question-circle"></i> %ABOUTTEXT%</a>
        </div>
    </div>
    <span id="menuIcon" class="menu-icon" onclick="openNav()">&#9776;</span>

    <script>
        const ZONES_URL = 'https://raw.githubusercontent.com/idezentas/esp8266_WeatherStation/refs/heads/main/zones.json';
        let zonesObj = null;

        fetch(ZONES_URL)
            .then(resp => {
                if (!resp.ok) throw new Error('Timezone data could not be obtained: ' + resp.status);
                return resp.json();
            })
            .then(data => {
                zonesObj = data;
                updatePosix();
            })
            .catch(err => console.error(err));

        document.getElementById('timezoneName').addEventListener('input', updatePosix);

        function updatePosix() {
            const tzName = document.getElementById('timezoneName').value.trim();
            if (zonesObj && tzName in zonesObj) {
                document.getElementById('posixInput').value = zonesObj[tzName];
            } else {
                document.getElementById('posixInput').value = '';
            }
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

        function geocodeFromCity() {
            const city = document.getElementById('city').value.trim();
            const country = document.getElementById('country').value.trim();
            const geoApiKey = "%GEOAPIKEY%";
            let lang = "%LANG%".toLowerCase();
            if (lang != "tr" && lang != "en") {lang = "en";}

            if (!city || !country || !geoApiKey) {
                showToast("%GEOALERT%");
                return;
            }

            const url = `https://api.opencagedata.com/geocode/v1/json?q=${encodeURIComponent(city)},${encodeURIComponent(country)}&key=${geoApiKey}&language=${lang}&pretty=1&limit=1`;
            fetch(url)
                .then(resp => resp.json())
                .then(data => {
                    if (data.results && data.results.length > 0) {
                        const geometry = data.results[0].geometry;
                        const tzname = data.results[0].annotations.timezone.name;
                        const speed_in = data.results[0].annotations.roadinfo.speed_in;

                        if (geometry.lat) { document.getElementById('latitude').value = geometry.lat; }
                        if (geometry.lng) { document.getElementById('longitude').value = geometry.lng; }

                        if (tzname) {
                            document.getElementById('timezoneName').value = tzname;
                            updatePosix();
                        }

                        document.getElementById("units").value = (speed_in.toLowerCase() === "km/h") ? "metric" : "imperial";
                    } else {
                        showToast("%GEOALERT2%");
                    }
                })
                .catch(err => {
                    console.error(err);
                    showToast("%GEOALERT3%");
                });
        }

        function openNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            overlay.style.width = "100%";
            document.getElementById("menuIcon").style.display = "none";

            setTimeout(() => {
                content.style.opacity = "1";
                closeBtn.style.opacity = "1";
            }, 0);
        }

        function closeNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            content.style.opacity = "0";
            closeBtn.style.opacity = "0";

            setTimeout(() => {
                overlay.style.width = "0%";
                document.getElementById("menuIcon").style.display = "block";
            }, 0);
        }

        function openMap() {
            const latitudeInput = document.getElementById('latitude');
            const longitudeInput = document.getElementById('longitude');
            const link = `https://www.google.com/maps/@${encodeURIComponent(latitudeInput.value)},${encodeURIComponent(longitudeInput.value)},2532m/data=!3m1!1e3`;
            if (link) {
                window.open(link, '_blank');
            } else {
                showToast('Lütfen geçerli bir link girin!');
            }
        }
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
            --bg-color: #121212;
            --text-color: #eeeeee;
            --input-bg: #2a2a2a;
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

        .overlay {
            height: 100%;
            width: 0;
            position: fixed;
            z-index: 1;
            top: 0;
            left: 0;
            background-color: rgba(0, 0, 0, 0.9);
            overflow-x: hidden;
            transition: 0.5s;
        }

        .overlay-content {
            position: relative;
            top: 10%;
            width: 100%;
            margin-top: 0;
            text-align: center;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        .overlay-content a {
            display: block;
            padding: 8px 16px;
            text-decoration: none;
            font-size: 36px;
            color: #818181;
            margin: 5px 0;
            transition: 0.3s;
            white-space: normal;
            word-wrap: break-word;
        }

        .overlay a:hover,
        .overlay a:focus {
            color: #f1f1f1;
        }

        .overlay .closebtn {
            position: absolute;
            top: 20px;
            right: 45px;
            font-size: 60px;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        @media screen and (max-height: 450px) {
            .overlay a {
                font-size: 20px
            }

            .overlay .closebtn {
                font-size: 40px;
                top: 15px;
                right: 35px;
            }
        }

        .menu-icon {
            font-size: 30px;
            cursor: pointer;
            position: fixed;
            top: 20px;
            right: 20px;
            z-index: 2;
        }

        @media screen and (max-width: 600px) {
            .menu-icon {
                top: 15px;
                right: 15px;
            }

            h1 {
                padding-top: 60px;
            }
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
    </main>
    <div id="myNav" class="overlay">
        <a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
        <div class="overlay-content">
            <a href="/"><i class="fas fa-home"></i> %HOMETEXT%</a>
            <a href="/apikey"><i class="fas fa-key"></i> %APIKEYSETTINGS%</a>
            <a href="/device"><i class="fas fa-cogs"></i> %DEVICESETTINGS%</a>
            <a href="/security"><i class="fas fa-lock"></i> %SECURITYSETTINGS%</a>
            <a href="/update"><i class="fas fa-upload"></i> %OTATEXT%</a>
            <a href="https://github.com/idezentas"><i class="fas fa-question-circle"></i> %ABOUTTEXT%</a>
        </div>
    </div>
    <span id="menuIcon" class="menu-icon" onclick="openNav()">&#9776;</span>
    <script>
        const form = document.getElementById('deviceForm');
        const submitBtn = document.getElementById('submitBtn');

        form.addEventListener('submit', (e) => {
            submitBtn.disabled = true;
            showToast("%FORMSENDING%");
        });

        function openNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            overlay.style.width = "100%";
            document.getElementById("menuIcon").style.display = "none";

            setTimeout(() => {
                content.style.opacity = "1";
                closeBtn.style.opacity = "1";
            }, 0);
        }

        function closeNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            content.style.opacity = "0";
            closeBtn.style.opacity = "0";

            setTimeout(() => {
                overlay.style.width = "0%";
                document.getElementById("menuIcon").style.display = "block";
            }, 0);
        }
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
            --bg-color: #121212;
            --text-color: #eeeeee;
            --input-bg: #2a2a2a;
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

        .overlay {
            height: 100%;
            width: 0;
            position: fixed;
            z-index: 1;
            top: 0;
            left: 0;
            background-color: rgba(0, 0, 0, 0.9);
            overflow-x: hidden;
            transition: 0.5s;
        }

        .overlay-content {
            position: relative;
            top: 10%;
            width: 100%;
            margin-top: 0;
            text-align: center;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        .overlay-content a {
            display: block;
            padding: 8px 16px;
            text-decoration: none;
            font-size: 36px;
            color: #818181;
            margin: 5px 0;
            transition: 0.3s;
            white-space: normal;
            word-wrap: break-word;
        }

        .overlay a:hover,
        .overlay a:focus {
            color: #f1f1f1;
        }

        .overlay .closebtn {
            position: absolute;
            top: 20px;
            right: 45px;
            font-size: 60px;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        @media screen and (max-height: 450px) {
            .overlay a {
                font-size: 20px
            }

            .overlay .closebtn {
                font-size: 40px;
                top: 15px;
                right: 35px;
            }
        }

        .menu-icon {
            font-size: 30px;
            cursor: pointer;
            position: fixed;
            top: 20px;
            right: 20px;
            z-index: 2;
        }

        @media screen and (max-width: 600px) {
            .menu-icon {
                top: 15px;
                right: 15px;
            }

            h1 {
                padding-top: 60px;
            }
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
    </main>
    <div id="myNav" class="overlay">
        <a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
        <div class="overlay-content">
            <a href="/"><i class="fas fa-home"></i> %HOMETEXT%</a>
            <a href="/apikey"><i class="fas fa-key"></i> %APIKEYSETTINGS%</a>
            <a href="/device"><i class="fas fa-cogs"></i> %DEVICESETTINGS%</a>
            <a href="/display"><i class="fas fa-desktop"></i> %DISPLAYSETTINGS%</a>
            <a href="/update"><i class="fas fa-upload"></i> %OTATEXT%</a>
            <a href="https://github.com/idezentas"><i class="fas fa-question-circle"></i> %ABOUTTEXT%</a>
        </div>
    </div>
    <span id="menuIcon" class="menu-icon" onclick="openNav()">&#9776;</span>
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

        function openNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            overlay.style.width = "100%";
            document.getElementById("menuIcon").style.display = "none";

            setTimeout(() => {
                content.style.opacity = "1";
                closeBtn.style.opacity = "1";
            }, 0);
        }

        function closeNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            content.style.opacity = "0";
            closeBtn.style.opacity = "0";

            setTimeout(() => {
                overlay.style.width = "0%";
                document.getElementById("menuIcon").style.display = "block";
            }, 0);
        }
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
            --bg-color: #121212;
            --text-color: #eeeeee;
            --input-bg: #2a2a2a;
            --button-bg: #388e3c;
            --button-text: #ffffff;
            --accent-color: #66bb6a;
            --shadow: 0 2px 12px rgba(0, 0, 0, 0.5);
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

        .overlay {
            height: 100%;
            width: 0;
            position: fixed;
            z-index: 1;
            top: 0;
            left: 0;
            background-color: rgba(0, 0, 0, 0.9);
            overflow-x: hidden;
            transition: 0.5s;
        }

        .overlay-content {
            position: relative;
            top: 10%;
            width: 100%;
            margin-top: 0;
            text-align: center;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        .overlay-content a {
            display: block;
            padding: 8px 16px;
            text-decoration: none;
            font-size: 36px;
            color: #818181;
            margin: 5px 0;
            transition: 0.3s;
            white-space: normal;
            word-wrap: break-word;
        }

        .overlay a:hover,
        .overlay a:focus {
            color: #f1f1f1;
        }

        .overlay .closebtn {
            position: absolute;
            top: 20px;
            right: 45px;
            font-size: 60px;
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        @media screen and (max-height: 450px) {
            .overlay a {
                font-size: 20px
            }

            .overlay .closebtn {
                font-size: 40px;
                top: 15px;
                right: 35px;
            }
        }

        .menu-icon {
            font-size: 30px;
            cursor: pointer;
            position: fixed;
            top: 20px;
            right: 20px;
            z-index: 2;
        }

        @media screen and (max-width: 600px) {
            .menu-icon {
                top: 15px;
                right: 15px;
            }

            h1 {
                padding-top: 60px;
            }
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
    </main>
    <div id="myNav" class="overlay">
        <a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
        <div class="overlay-content">
            <a href="/"><i class="fas fa-home"></i> %HOMETEXT%</a>
            <a href="/device"><i class="fas fa-cogs"></i> %DEVICESETTINGS%</a>
            <a href="/display"><i class="fas fa-desktop"></i> %DISPLAYSETTINGS%</a>
            <a href="/security"><i class="fas fa-lock"></i> %SECURITYSETTINGS%</a>
            <a href="/update"><i class="fas fa-upload"></i> %OTATEXT%</a>
            <a href="https://github.com/idezentas"><i class="fas fa-question-circle"></i> %ABOUTTEXT%</a>
        </div>
    </div>
    <span id="menuIcon" class="menu-icon" onclick="openNav()">&#9776;</span>
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

        function openNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            overlay.style.width = "100%";
            document.getElementById("menuIcon").style.display = "none";

            setTimeout(() => {
                content.style.opacity = "1";
                closeBtn.style.opacity = "1";
            }, 0);
        }

        function closeNav() {
            const overlay = document.getElementById("myNav");
            const content = document.querySelector(".overlay-content");
            const closeBtn = document.querySelector(".closebtn");

            content.style.opacity = "0";
            closeBtn.style.opacity = "0";

            setTimeout(() => {
                overlay.style.width = "0%";
                document.getElementById("menuIcon").style.display = "block";
            }, 0);
        }
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

static const char Save_Security_Page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="%LANG%">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>%SECURITYSETTINGS%</title>
  </head>
  <body>
    <h3>%SECURITYTEXT%</h3>
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
