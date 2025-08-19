# ESP8266 Weather Station

This project is based on the [ThingPulse - WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo) example and has been significantly extended to include real-time data, a mobile-friendly interface, and many advanced features.

## Features

- Synchronous Web Server with a user-friendly web interface
- Settings Pages: Device, Security, API Keys, Display, Currency Rates, and Restore to Default Settings
- OTA Updates: Remote firmware updates with ElegantOTA v3
- WiFiManager: No hardcoded Wi-Fi credentials required
- Live Weather & Air Quality (AQI): From OpenWeatherMap
- UV Index: Real-time UV data from Open-Meteo API
- Currency Rates: From Floatrates API
- Time & Date Display: Synced using NTP servers. Automatic daylight saving time compatibility is available.
- Night Mode: OLED screen turns off/on automatically based on configured time
- Display Rotation: Flip the OLED screen vertically
- Theme Support: Light and dark theme options in the web interface (The theme selection will automatically be the same as the theme used in the system.)
- Mobile-Friendly Interface: Fully responsive design
- Multi-language Support: Currently supports **Turkish** and **English**

## Web Interface Screenshots

Home Page

<img src="images/home-en.png" alt="Home Page" width="300"/>

API Key Settings

<img src="images/apikeys-en.png" alt="API Key Settings" width="300"/>

Device Settings

<img src="images/device-en.png" alt="Device Settings" width="300"/>

Display Settings

<img src="images/display-en.png" alt="Display Settings" width="300"/>

Security Settings

<img src="images/security-en.png" alt="Security Settings" width="300"/>

Currency Settings

<img src="images/currency-en.png" alt="Currency Settings" width="300"/>

## Setup Instructions

- Create a free account on [OpenWeatherMap](https://openweathermap.org) and [OpenCage](https://opencagedata.com) and obtain API keys.

- Open the project using Arduino IDE or PlatformIO.
- If you are opening the project using PlatformIO, first open `platformio.ini` and wait for the libraries to download in the terminal window. It will automatically download all the necessary libraries.
- If you are opening the project using Arduino IDE, first install the ESP8266 board. Then install the following libraries:
  - [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)
  - [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)
  - [WiFiManager](https://github.com/tzapu/WiFiManager)
  - [Json Streaming Parser](https://github.com/squix78/json-streaming-parser)
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

- Then open the `esp8266_WeatherStation.ino` file:
  - If you want to use Turkish, change `#include "lang_tr.h"`, if you want to use English, change `#include "lang_en.h"`. This selection specifies the language of the text on the OLED and web page.

- Upload the software to the NodeMCU (ESP8266) device.

- If your WiFi network is already configured, it will connect automatically. You do not need to configure the WiFiManager settings.

- When the device is turned on, if WiFiManager is active, connect to the WiFi network displayed on the OLED screen to open the settings portal.
  - Click the **Configure WiFi** button.
  - Select the WiFi network from the list above.
  - Enter your WiFi network password.
  - Click the **Save** button.
  - The system will then automatically connect to the WiFi network you selected.

- After connecting to the WiFi network, the device's IP address will be displayed on the OLED screen. Go to that IP address via your browser. If you can't see it, don't worry. The page displaying the IP address is among the pages that will appear during normal use. You can also find the IP address there.

- After going to the IP address, you will be greeted by the main page of the web interface.

- When you want to go to the settings pages, you will be asked for a username and password. The default username is `admin` and the default password is `esp8266`. You can change them from the **Security Settings** page if you wish.

- First, go to the **API Key Settings** page via the web interface:
  - Enter the OpenWeatherMap and OpenCage API keys.
  - Click the **Save** button. After clicking, you will be automatically redirected to the home page.

- Then, go to the **Device Settings** page via the web interface:
  - Enter the city, country names then click the **Find Location/Time from City** button. It will automatically configure the other necessary settings for you.
  - Enter the update frequency in minutes.
  - Click the **Save** button. After clicking, you will be automatically redirected to the main page. At the same time, the data will be updated with new values on the OLED screen.

- Then, go to the **Display Settings** page via the web interface:
  - Select the display orientation, time format, and month display format in that order.
  - Enter the turn on and turn off times for night mode scheduling.  
  - Click the **Save** button. After clicking, you will be automatically redirected to the home page. At the same time, the data will be updated with the new values on the OLED screen.

- Then, go to the **Currency Rate Settings** page via the web interface:
  - Select the base currency and target currency for 2 currency exchange rates.
  - Click the **Save** button. After clicking, you will be automatically redirected to the main page. At the same time, the data will be updated with the new values on the OLED screen.

- After checking the values you have selected on the OLED screen, you can start using it with peace of mind.

- To change the settings you have selected at a later time, you can go to the relevant page on the web interface and make the necessary changes.

## Hardware Requirements

- ESP8266 (NodeMCU, Wemos D1 Mini or similar)  
- 0.96" I2C OLED Display (SSD1306)  
- Wi-Fi connectivity  
- 5V USB power supply

### OLED - NodeMCU (ESP8266) Wiring Table

| OLED Pin Name | NodeMCU (ESP8266) Pin |
|---------------|------------------------|
| GND           | GND                    |
| VCC           | 3V3                    |
| SCL           | D1 (GPIO5)             |
| SDA           | D2 (GPIO4)             |

<img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1" alt="Wiring Diagram" width="500"/>

Wiring Diagram Source: [Random Nerd Tutorials](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)

## Credits

- Demo Base: [ThingPulse / WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo)  
- OLED Display Library: [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)  
- OTA: [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)  
- Weather Data: [OpenWeatherMap](https://openweathermap.org)  
- UV Index: [Open-Meteo](https://open-meteo.com)  
- Time Sync: [NTP Servers](https://pool.ntp.org)  
- WiFi Config Portal: [WiFiManager](https://github.com/tzapu/WiFiManager)  
- Currency Data: [Floatrates](https://www.floatrates.com)  
- Night Mode and Display Rotation Feature: [Qrome / PrinterMonitor](https://github.com/Qrome/printer-monitor)
- JSON: [Json Streaming Parser](https://github.com/squix78/json-streaming-parser) and [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- Timezones [posix_tz_db](https://github.com/nayarsystems/posix_tz_db)

# ESP8266 Hava Durumu İstasyonu

Bu proje, [ThingPulse - WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo) örneği temel alınarak geliştirilmiştir. ESP8266 kullanılarak gerçek zamanlı veri sağlayan, mobil uyumlu ve kullanıcı dostu bir hava durumu istasyonu oluşturulmuştur.

## Özellikler

- Senkron Web Sunucusu ile kullanıcı dostu web arayüzü
- Ayarlar Sayfaları: Cihaz, Güvenlik, API Anahtarları, Ekran, Döviz Kurları ve Varsayılan ayarlara sıfırlama
- OTA Güncelleme: ElegantOTA ile uzaktan yazılım güncellemeleri
- WiFiManager: Gömülü Wi-Fi adı/şifresi gerekmeden yapılandırma
- Anlık Hava Durumu & Hava Kalitesi (AQI): OpenWeatherMap üzerinden alınmaktadır.
- UV İndeksi: Open-Meteo API'den gerçek zamanlı UV bilgisi
- Döviz Kurları: Floatrates API üzerinden alınmaktadır.
- Saat & Tarih Gösterimi: NTP sunucusu üzerinden alınmaktadır. Otomatik yaz saati uyumluluğu bulunmaktadır.
- Gece Modu : Belirtilen saatlerde OLED ekran otomatik açılır/kapanır.
- Ekran Döndürme: OLED ekranı dikey olarak döndürme
- Tema Desteği: Web arayüzünde açık ve koyu tema seçeneği (Tema seçimi otomatik olarak sistemde kullanan tema aynı olacaktır.)
- Mobil Uyumlu Arayüz: Tüm cihazlarda uyumlu tasarım
- Çoklu Dil Desteği: Şu anda **Türkçe** ve **İngilizce** arayüz seçeneği

## Web Arayüzü Ekran görüntüleri

Ana Sayfa

<img src="images/home-tr.png" alt="Ana Sayfa" width="300"/>

API Anahtar Ayarları

<img src="images/apikeys-tr.png" alt="API Anahtar Ayarları" width="300"/>

Cihaz Ayarları

<img src="images/device-tr.png" alt="Cihaz Ayarları" width="300"/>

Ekran Ayarları

<img src="images/display-tr.png" alt="Ekran Ayarları" width="300"/>

Güvenlik Ayarları

<img src="images/security-tr.png" alt="Güvenlik Ayarları" width="300"/>

Döviz Kuru Ayarları

<img src="images/currency-tr.png" alt="Döviz Kuru Ayarları" width="300"/>

## Kurulum aşamaları

- [OpenWeatherMap](https://openweathermap.org) ve [OpenCage](https://opencagedata.com) sitelerinden ücretsiz üyelik oluşturup API anahtarlarını alınız.

- Arduino IDE veya PlatformIO ile projeyi açın
- Eğer PlatformIO ile projeyi açacaksanız önce `platformio.ini` açıp terminal ekranında kütüphanelerin inmesi bekleyiniz. Gerekli bütün kütüphaneleri kendi otomatik olarak indirecektir.
- Eğer Arduino IDE ile projeyi açacaksanız önce ESP8266 kartını yükleyiniz. Daha sonra aşağıdaki kütüphaneleri yükleyiniz:
  - [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)  
  - [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)
  - [WiFiManager](https://github.com/tzapu/WiFiManager)
  - [Json Streaming Parser](https://github.com/squix78/json-streaming-parser)
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

- Daha sonra `esp8266_WeatherStation.ino` dosyasını açarak:
  - Eğer kullanmak istediğiniz dil Türkçe ise `#include "lang_tr.h"`, Eğer kullanmak istediğiniz dil İngilizce ise `#include "lang_en.h"` olarak değiştiriniz. Bu seçiminiz OLED ve web sayfası üzerinde bulunan yazılarının dili belirtmektedir.

- Yazılımı NodeMCU(ESP8266) cihaza yükleyin.

- Eğer WiFi ağınız daha önceden tanımlı ise kendisi otomatik olarak bağlanacaktır. WiFiManager ayarını yapmanıza gerek yoktur.

- Cihaz açıldığında WiFiManager aktifse OLED ekranda yazan WiFi ağına bağlanarak ayar portalını açınız.
  - **Configure WiFi** butona tıklayınız.
  - Yukarıdaki listeden WiFi ağını seçiniz.
  - Wifi ağı şifrenizi giriniz.
  - **Save** butonuna tıklayınız.
  - Sonra sistem kendiliğinden seçtiğiniz WiFi ağına bağlanacaktır.

- WiFi ağına bağlandıktan sonra cihazın IP adresi OLED ekranda yazacaktır. Tarayıcı üzerinden o IP adresine gidiniz. Eğer göremediyseniz merak etmeyin. Normal kullanım esnasında gözükecek sayfalar arasında IP adresini yazan sayfa bulunmaktadır. Oradan da IP adresini öğrenebilirsiniz.

- IP adrese gittikten sonra sizi web arayüzünün ana sayfası karşılacaktır.

- Ayar sayfalarına gitmek istediğinizde sizden kullanıcı adı ve şifre isteyecektir. Varsayılan kullanıcı adı: `admin` ve varsayılan şifre: `esp8266` olarak belirlenmiştir. Değiştirmek istediğiniz zaman **Güvenlik Ayarları** sayfasından değiştirebilirsiniz.

- Web arayüzü üzerinden önce **API Anahtar Ayarları** sayfasına giderek:
  - OpenWeatherMap ve OpenCage API anahtarlarını giriniz.
  - **Kaydet** butona basınız. Bastıktan sonra sizi otomatik olarak ana sayfaya yönlendirecektir.

- Daha sonra web arayüzü üzerinden önce **Cihaz Ayarları** sayfasına giderek:
  - Şehir ve Ülke adlarını girip **Şehirden Konum/Zaman Bul** butonuna tıklayınız. Size otomatik olarak diğer gerekli ayarları yapacaktır.
  - Güncelleme sıklığı seçiniz.
  - **Kaydet** butona basınız. Bastıktan sonra sizi otomatik olarak ana sayfaya yönlendirecektir. Aynı zamanda OLED ekranda yeni değerler ile veriler güncellenecektir.

- Daha sonra web arayüzü üzerinden önce **Ekran Ayarları** sayfasına giderek:
  - Sırasıyla ekran yönünü, saat formatını ve ay gösterim formatını seçiniz.
  - Gece modu zamanlaması için açılış ve kapanış zamanlarını giriniz.
  - **Kaydet** butona basınız. Bastıktan sonra sizi otomatik olarak ana sayfaya yönlendirecektir. Aynı zamanda OLED ekranda yeni değerler ile veriler güncellenecektir.

- Daha sonra web arayüzü üzerinden önce **Döviz Kurları Ayarları** sayfasına giderek:
  - 2 tane döviz kuru için temel para birimi ve hedef para birimi seçiniz.
  - **Kaydet** butona basınız. Bastıktan sonra sizi otomatik olarak ana sayfaya yönlendirecektir. Aynı zamanda OLED ekranda yeni değerler ile veriler güncellenecektir.

- OLED ekran üzerinden seçtiğiniz değerleri kontrol ettikten sonra gönül rahatlığıyla kullanmaya başlayabilirsiniz.

- Başka bir zaman seçtiğiniz ayarı değiştirmek için web arayüzünde ilgili sayfaya giderek gerekli değişiklikleri yapabilirsiniz.

## Donanım Gereksinimleri

- ESP8266 (NodeMCU, Wemos D1 Mini veya benzeri)  
- 0.96" I2C OLED ekran (SSD1306)
- Wi-Fi bağlantısı  
- 5V USB güç kaynağı

### OLED - NodeMCU (ESP8266) Bağlantı Tablosu

| OLED Pin Adı | NodeMCU (ESP8266) Pin |
|--------------|-----------------------|
| GND          | GND                   |
| VCC          | 3V3                   |
| SCL          | D1 (GPIO5)            |
| SDA          | D2 (GPIO4)            |

<img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1" alt="Bağlantı Şeması" width="500"/>

Bağlantı Şeması Kaynağı: [Random Nerd Tutorials](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)

## Teşekkürler

- Demo Tabanı: [ThingPulse / WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo)  
- OLED Ekran Kütüphanesi: [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)  
- OTA: [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)  
- Hava Verisi: [OpenWeatherMap](https://openweathermap.org)  
- UV Verisi: [Open-Meteo](https://open-meteo.com)  
- Saat: [NTP Sunucuları](https://pool.ntp.org)  
- WiFi Kurulumu: [WiFiManager](https://github.com/tzapu/WiFiManager)  
- Döviz Verisi: [Floatrates](https://www.floatrates.com)  
- Gece Modu ve Ekran Döndürme Özelliği: [Qrome / PrinterMonitor](https://github.com/Qrome/printer-monitor)
- JSON: [Json Streaming Parser](https://github.com/squix78/json-streaming-parser) ve [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- Zaman Dilimleri [posix_tz_db](https://github.com/nayarsystems/posix_tz_db)
