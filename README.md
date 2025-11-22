Türkçe README için [tıklayınız](https://github.com/idezentas/esp8266_WeatherStation?tab=readme-ov-file#esp8266-hava-durumu-i%CC%87stasyonu)

# ESP8266 Weather Station

This project is based on the [ThingPulse - WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo) example and has been significantly extended to include real-time data, a mobile-friendly interface, and many advanced features.

## Features

- Synchronous Web Server with a user-friendly web interface
- Settings Pages (in order)
  - API Keys
  - Device
  - Display
  - Security
  - World Clock
  - Update Data
  - Restore to Default Settings
  - OTA Update
  - About
- OTA Updates: Remote firmware updates with ElegantOTA v3
- WiFiManager: No hardcoded Wi-Fi credentials required.
- Live Weather, Air Quality, Forecasts: From OpenWeatherMap API
- UV Index: Real-time UV data from Open-Meteo API
- Time & Date Display: Synced using NTP servers. Automatic daylight saving time compatibility is available.
- World Clock: View current time in multiple cities
- Night Mode: OLED display turns off/on automatically based on configured time.
- Display Rotation: Flip the OLED display vertically.
- Mobile-Friendly Interface: Fully responsive design
- Multi-language Support: 
  - English
  - Turkish
  - Italian
  - Spanish
- Indoor: Temperature and humidity data were obtained using a DHT22 sensor.
- Pages on the OLED Screen (in order)
  - Time and date
  - City name
  - Temperature
  - Humidity
  - Indoor temperature
  - Indoor humidity
  - Wind speed and direction
  - Air quality
  - UV index
  - Atmospheric pressure
  - Daily Weather forecasts (2 pages)
  - Sunrise/sunset
  - Moonrise/moonset and phase
  - World Clock (2 pages for each city)
  - Wi-Fi network name
  - IP address
  - Next update

## Web Interface Screenshots

Home Page

<img src="images/home-en.png" alt="Home Page" width="50%"/>

API Key Settings

<img src="images/apikeys-en.png" alt="API Key Settings" width="50%"/>

Device Settings

<img src="images/device-en.png" alt="Device Settings" width="50%"/>

Display Settings

<img src="images/display-en.png" alt="Display Settings" width="50%"/>

Security Settings

<img src="images/security-en.png" alt="Security Settings" width="50%"/>

World Clock Settings

<img src="images/worldclock-en.png" alt="World Clock Settings" width="50%"/>

Menu Example

<img src="images/menu_example-en.png" alt="Menu Example" width="100%"/>

## Hardware Requirements

- ESP8266 (NodeMCU, Wemos D1 Mini or similar)  
- 0.96" I2C OLED Display (SSD1306) 
- DHT22
- 10K ohm resistor 
- Wi-Fi connection  
- 5V USB power supply

### NodeMCU (ESP8266) Wiring Table

| OLED Pin Name | NodeMCU (ESP8266) Pin |
| :-----------: | :-------------------: |
|      GND      |          GND          |
|      VCC      |          3V3          |
|      SCL      |      D1 (GPIO5)       |
|      SDA      |      D2 (GPIO4)       |

| DHT22 Pin Name | NodeMCU (ESP8266) Pin |
| :------------: | :-------------------: |
|      GND       |          GND          |
|      VCC       |          3V3          |
|      DATA      |      D5 (GPIO14)      |

One lead of the 10K ohm resistor will be connected to the DATA pin, and the other lead will be connected to VCC.

<img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/oled-temperature-humidity-esp8266.png?quality=100&strip=all&ssl=1" alt="Wiring Diagram" width="60%"/>

Wiring Diagram Source: [Random Nerd Tutorials](https://randomnerdtutorials.com/esp32-esp8266-dht-temperature-and-humidity-oled-display/)

## Setup Instructions

- Create a free account on [OpenWeatherMap](https://openweathermap.org) and [OpenCage](https://opencagedata.com) and obtain API keys.

- Open the project using Arduino IDE or PlatformIO.
- If you are opening the project using PlatformIO, first open `platformio.ini` and wait for the libraries to download in the terminal window. It will automatically download all the necessary libraries.
- If you are opening the project using Arduino IDE, first install the ESP8266 board. Then install the following libraries:
  - [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)
  - [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)
  - [WiFiManager](https://github.com/tzapu/WiFiManager)
  - [Json Streaming Parser](https://github.com/squix78/json-streaming-parser)
  - [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
  - [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)

- Then upload the code to the NodeMCU (ESP8266) device.

- If your WiFi network is already configured, it will connect automatically. You do not need to configure the WiFiManager settings.

- When the device is turned on, if WiFiManager is active, connect to the WiFi network displayed on the OLED display to open the settings portal.
  - Click the **Configure WiFi** button.
  - Select the WiFi network from the list above.
  - Enter your WiFi network password.
  - Click the **Save** button.
  - The system will then automatically connect to the WiFi network you selected.

- After connecting to the WiFi network, the device's IP address will be displayed on the OLED display and serial monitor. Go to that IP address via your browser. If you can't see it, don't worry. The page displaying the IP address is among the pages that will appear during normal use. You can also find the IP address there.

- After going to the IP address, you will be greeted by the main page of the web interface.

- When you want to go to the settings pages, you will be asked for a username and password. The default username is `admin` and the default password is `esp8266`. You can change them from the **Security Settings** page if you wish.

- First, go to the **API Key Settings** page via the web interface:
  - Enter the `OpenWeatherMap`, `OpenCage` and `TimeZoneDB` API keys.
  - Click the **Save** button. After clicking, it will save the values and then automatically redirect you to the main page. At the same time, the data will be updated with the new values on the OLED screen.

- Then, go to the **Device Settings** page via the web interface:
  - Select the language you want to use. (Default language is `English`)
  - Select the theme you want to use. (Default theme is the `dark` theme)
  - Select the search method. (Address or Latitude and Longitude)
    - If you are searching by address, enter the address according to one of the search types and click the **Search** button. It will automatically make the other necessary settings for you.
    - If you are searching by latitude and longitude, enter the latitude and longitude values in the appropriate fields and then click the **Search** button. It will automatically make the other necessary settings for you.
  - To check the latitude and longitude values entered, click the **Show on Map** button. (The location corresponding to the values entered will be displayed on Google Maps in the new tab.)
  - Select the update interval in minutes.
  - Click the **Save** button. After clicking, it will save the values and then automatically redirect you to the main page. At the same time, the data will be updated with the new values on the OLED screen.

- Then, go to the **World Clock Settings** page via the web interface:
  - Select the search method. (Address or Latitude and Longitude)
    - If you are searching by address, enter the address according to one of the search types and click on the tab for the city whose information you want to change, then click the **Search** button. It will automatically make the other necessary settings for you.
    - If you are searching by latitude and longitude, enter the latitude and longitude values in the relevant fields and click on the tab for the city whose information you want to change, then click the **Search** button. It will automatically make the other necessary settings for you.
  - To check the entered latitude and longitude values, click the **Show on Map** button. (The location corresponding to the entered values will be displayed in Google Maps in a new tab.)
  - Click the **Save** button. After clicking, it will save the values and then automatically redirect you to the home page. At the same time, the data will be updated with the new values on the OLED screen.

- Then, go to the **Display Settings** page via the web interface:
  - Select the display orientation, time format, and month display format in that order.
  - Enter the turn on and turn off times for night mode scheduling.  
  - Click the **Save** button. After clicking, it will save the values and then automatically redirect you to the main page. At the same time, the data will be updated with the new values on the OLED screen.

- After checking the values you have selected on the OLED display, you can start using it with peace of mind.

- To change the settings you have selected at a later time, you can go to the relevant page on the web interface and make the necessary changes.

## Credits

- Demo Base: [ThingPulse / WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo)  
- OLED Display Library: [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)  
- OTA Library: [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)  
- Weather Data: [OpenWeatherMap](https://openweathermap.org)  
- UV Index: [Open-Meteo](https://open-meteo.com)  
- Time Sync: [NTP Servers](https://pool.ntp.org) and [TimeZoneDB](https://timezonedb.com/) 
- WiFi Config Portal: [WiFiManager](https://github.com/tzapu/WiFiManager)
- Night Mode and Display Rotation Feature: [Qrome / PrinterMonitor](https://github.com/Qrome/printer-monitor)
- JSON Library: [Json Streaming Parser](https://github.com/squix78/json-streaming-parser)
- Timezones: [posix_tz_db](https://github.com/nayarsystems/posix_tz_db)
- DHT22 Libraries: [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor) and [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- Language Translation Structure: [rob040/LEDmatrixClock](https://github.com/rob040/LEDmatrixClock)

# ESP8266 Hava Durumu İstasyonu

Bu proje, [ThingPulse - WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo) örneği temel alınarak geliştirilmiştir. ESP8266 kullanılarak gerçek zamanlı veri sağlayan, mobil uyumlu ve kullanıcı dostu bir hava durumu istasyonu oluşturulmuştur.

## Özellikler

- Senkron Web Sunucusu ile kullanıcı dostu web arayüzü
- Ayarlar Sayfaları: 
  - API Anahtarları
  - Cihaz
  - Ekran
  - Güvenlik
  - Dünya Saati
  - Verileri Güncelle
  - Varsayılan ayarlara sıfırlama
  - OTA Güncellemesi
  - Hakkında
- OTA Güncelleme: ElegantOTA ile uzaktan yazılım güncellemeleri
- WiFiManager: Gömülü Wi-Fi adı/şifresi gerekmeden yapılandırma
- Anlık Hava Durumu, Hava Kalitesi, Hava Tahminleri: OpenWeatherMap üzerinden alınmaktadır.
- UV İndeksi: Open-Meteo API'den gerçek zamanlı UV bilgisi
- Saat & Tarih Gösterimi: NTP sunucusu üzerinden alınmaktadır. Otomatik yaz saati uyumluluğu bulunmaktadır.
- Dünya Saati: Birden fazla şehirlerin saatlerini gösterme
- Gece Modu : Belirtilen saatlerde OLED ekran otomatik açılır/kapanır.
- Ekran Döndürme: OLED ekranı dikey olarak döndürme
- Mobil Uyumlu Arayüz: Tüm cihazlarda uyumlu tasarım
- Çoklu Dil Desteği: 
  - İngilizce
  - Türkçe
  - İtalyanca
  - İspanyolca
- İç Mekan: DHT22 sensörü ile sıcaklık ve nem bilgileri elde edildi.
- OLED Ekrandaki Sayfalar (Sırasıyla)
  - Saat ve tarih
  - Şehir adı
  - Hava sıcaklığı
  - Nem
  - İç mekan sıcaklığı
  - İç mekan nemi
  - Rüzgar hızı ve yönü
  - Hava kalitesi
  - UV indeksi
  - Atmosferik basınç
  - Günlük Hava tahminleri (2 Sayfa)
  - Güneşin doğuşu/batışı
  - Ayın doğuşu/batışı ve evresi
  - Dünya Saati (Her şehir için 2 sayfa)
  - Wifi ağ adı
  - IP adresi
  - Sonraki güncelleme

## Web Arayüzü Ekran görüntüleri

Ana Sayfa

<img src="images/home-tr.png" alt="Ana Sayfa" width="50%"/>

API Anahtar Ayarları

<img src="images/apikeys-tr.png" alt="API Anahtar Ayarları" width="50%"/>

Cihaz Ayarları

<img src="images/device-tr.png" alt="Cihaz Ayarları" width="50%"/>

Ekran Ayarları

<img src="images/display-tr.png" alt="Ekran Ayarları" width="50%"/>

Güvenlik Ayarları

<img src="images/security-tr.png" alt="Güvenlik Ayarları" width="50%"/>

Dünya Saati Ayarları

<img src="images/worldclock-tr.png" alt="Dünya Saati Ayarları" width="50%"/>

Menü Örneği

<img src="images/menu_example-tr.png" alt="Menü Örneği" width="100%"/>

## Donanım Gereksinimleri

- ESP8266 (NodeMCU, Wemos D1 Mini veya benzeri)  
- 0.96" I2C OLED ekran (SSD1306)
- DHT22
- 10K ohm direnç
- Wi-Fi bağlantısı  
- 5V USB güç kaynağı

### NodeMCU (ESP8266) Bağlantı Tablosu

| OLED Pin Adı | NodeMCU (ESP8266) Pin |
| :----------: | :-------------------: |
|     GND      |          GND          |
|     VCC      |          3V3          |
|     SCL      |      D1 (GPIO5)       |
|     SDA      |      D2 (GPIO4)       |

| DHT22 Pin Adı | NodeMCU (ESP8266) Pin |
| :-----------: | :-------------------: |
|      GND      |          GND          |
|      VCC      |          3V3          |
|     DATA      |      D5 (GPIO14)      |

10K ohm direncin bir ucu DATA pinine, diğer ucu ise VCC'ye bağlanacaktır.

<img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/oled-temperature-humidity-esp8266.png?quality=100&strip=all&ssl=1" alt="Bağlantı Şeması" width="60%"/>

Bağlantı Şeması Kaynağı: [Random Nerd Tutorials](https://randomnerdtutorials.com/esp32-esp8266-dht-temperature-and-humidity-oled-display/)

## Kurulum aşamaları

- [OpenWeatherMap](https://openweathermap.org) ve [OpenCage](https://opencagedata.com) sitelerinden ücretsiz üyelik oluşturup API anahtarlarını alınız.

- Arduino IDE veya PlatformIO ile projeyi açın
- Eğer PlatformIO ile projeyi açacaksanız önce `platformio.ini` açıp terminal ekranında kütüphanelerin inmesi bekleyiniz. Gerekli bütün kütüphaneleri kendi otomatik olarak indirecektir.
- Eğer Arduino IDE ile projeyi açacaksanız önce ESP8266 kartını yükleyiniz. Daha sonra aşağıdaki kütüphaneleri yükleyiniz:
  - [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)  
  - [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)
  - [WiFiManager](https://github.com/tzapu/WiFiManager)
  - [Json Streaming Parser](https://github.com/squix78/json-streaming-parser)
  - [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
  - [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)

- Daha sonra kodu NodeMCU(ESP8266) cihaza yükleyin.

- Eğer WiFi ağınız daha önceden tanımlı ise kendisi otomatik olarak bağlanacaktır. WiFiManager ayarını yapmanıza gerek yoktur.

- Cihaz açıldığında WiFiManager aktifse OLED ekranda yazan WiFi ağına bağlanarak ayar portalını açınız.
  - **Configure WiFi** butona tıklayınız.
  - Yukarıdaki listeden WiFi ağını seçiniz.
  - Wifi ağı şifrenizi giriniz.
  - **Save** butonuna tıklayınız.
  - Sonra sistem kendiliğinden seçtiğiniz WiFi ağına bağlanacaktır.

- WiFi ağına bağlandıktan sonra cihazın IP adresi OLED ekranda ve seri monitörde yazacaktır. Tarayıcı üzerinden o IP adresine gidiniz. Eğer göremediyseniz merak etmeyin. Normal kullanım esnasında gözükecek sayfalar arasında IP adresini yazan sayfa bulunmaktadır. Oradan da IP adresini öğrenebilirsiniz.

- IP adrese gittikten sonra sizi web arayüzünün ana sayfası karşılacaktır.

- Ayar sayfalarına gitmek istediğinizde sizden kullanıcı adı ve şifre isteyecektir. Varsayılan kullanıcı adı: `admin` ve varsayılan şifre: `esp8266` olarak belirlenmiştir. Değiştirmek istediğiniz zaman **Güvenlik Ayarları** sayfasından değiştirebilirsiniz.

- Web arayüzü üzerinden önce **API Anahtar Ayarları** sayfasına giderek:
  - `OpenWeatherMap`, `OpenCage` ve `TimeZoneDB` API anahtarlarını giriniz.
  - **Kaydet** butona tıklayınız. Tıkladıktan sonra değerleri kaydedip daha sonra sizi otomatik olarak ana sayfaya yönlendirecektir. Aynı zamanda OLED ekranda yeni değerler ile veriler güncellenecektir.

- Daha sonra web arayüzü üzerinden önce **Cihaz Ayarları** sayfasına giderek:
  - Kullanmak istediğiniz dili seçiniz. (Varsayılan dil `İngilizce`'dir)
  - Kullanmak istediğiniz temayı seçiniz. (Varsayılan tema `karanlık` temadır)
  - Sorgulama yöntemini seçiniz. (Adres yada Enlem ve Boylam)
    - Eğer adres ile arama yapacaksanız arama türlerinden birine göre adresi girip **Ara** butonuna tıklayınız. Size otomatik olarak diğer gerekli ayarları yapacaktır.
    - Eğer enlem ve boylam ile arama yapacaksanız enlem ve boylam değerlerini ilgili yerlere girdikten sonra **Ara** butonuna tıklayınız. Size otomatik olarak diğer gerekli ayarları yapacaktır.
  - Girilen enlem ve boylam değerlerini kontrol etmek için **Haritada Göster** butonuna tıklayınız. (Girilen değerlere karşılık gelen konum, yeni sekmedeki Google Haritalar'da görüntülenecektir.)
  - Güncelleme sıklığı seçiniz.
  - **Kaydet** butona tıklayınız. Tıkladıktan sonra değerleri kaydedip daha sonra sizi otomatik olarak ana sayfaya yönlendirecektir. Aynı zamanda OLED ekranda yeni değerler ile veriler güncellenecektir.

- Daha sonra web arayüzü üzerinden önce **Dünya Saati Ayarları** sayfasına giderek:
  - Sorgulama yöntemini seçiniz. (Adres yada Enlem ve Boylam)
    - Eğer adres ile arama yapacaksanız arama türlerinden birine göre adresi girin ve hangi şehir için bilgileri değiştirmek istiyorsanız o şehrin sekmesine tıklayıp **Ara** butonuna tıklayınız. Size otomatik olarak diğer gerekli ayarları yapacaktır.
    - Eğer enlem ve boylam ile arama yapacaksanız enlem ve boylam değerlerini ilgili yerlere girin ve hangi şehir için bilgileri değiştirmek istiyorsanız o şehrin sekmesine tıklayıp **Ara** butonuna tıklayınız. Size otomatik olarak diğer gerekli ayarları yapacaktır.
  - Girilen enlem ve boylam değerlerini kontrol etmek için **Haritada Göster** butonuna tıklayınız. (Girilen değerlere karşılık gelen konum, yeni sekmedeki Google Haritalar'da görüntülenecektir.)
  - **Kaydet** butona tıklayınız. Tıkladıktan sonra değerleri kaydedip daha sonra sizi otomatik olarak ana sayfaya yönlendirecektir. Aynı zamanda OLED ekranda yeni değerler ile veriler güncellenecektir.

- Daha sonra web arayüzü üzerinden önce **Ekran Ayarları** sayfasına giderek:
  - Sırasıyla ekran yönünü, saat formatını ve ay gösterim formatını seçiniz.
  - Gece modu zamanlaması için açılış ve kapanış zamanlarını giriniz.
  - **Kaydet** butona tıklayınız. Tıkladıktan sonra değerleri kaydedip daha sonra sizi otomatik olarak ana sayfaya yönlendirecektir. Aynı zamanda OLED ekranda yeni değerler ile veriler güncellenecektir.

- OLED ekran üzerinden seçtiğiniz değerleri kontrol ettikten sonra gönül rahatlığıyla kullanmaya başlayabilirsiniz.

- Başka bir zaman seçtiğiniz ayarı değiştirmek için web arayüzünde ilgili sayfaya giderek gerekli değişiklikleri yapabilirsiniz.

## Teşekkürler

- Demo Tabanı: [ThingPulse / WeatherStationDemo](https://github.com/ThingPulse/esp8266-weather-station/tree/master/examples/WeatherStationDemo)  
- OLED Ekran Kütüphanesi: [ESP8266 and ESP32 OLED driver for SSD1306 displays](https://github.com/ThingPulse/esp8266-oled-ssd1306)  
- OTA Kütüphanesi: [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA)  
- Hava Durumu Verisi: [OpenWeatherMap](https://openweathermap.org)  
- UV İndeksi Verisi: [Open-Meteo](https://open-meteo.com)  
- Saat: [NTP Sunucuları](https://pool.ntp.org) ve [TimeZoneDB](https://timezonedb.com/)
- WiFi Kurulumu: [WiFiManager](https://github.com/tzapu/WiFiManager)  
- Gece Modu ve Ekran Döndürme Özelliği: [Qrome / PrinterMonitor](https://github.com/Qrome/printer-monitor)
- JSON Kütüphanesi: [Json Streaming Parser](https://github.com/squix78/json-streaming-parser)
- Zaman Dilimleri: [posix_tz_db](https://github.com/nayarsystems/posix_tz_db)
- DHT22 Kütüphaneleri: [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor) ve [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- Dil Çeviri Yapısı: [rob040/LEDmatrixClock](https://github.com/rob040/LEDmatrixClock)
