/**
 * @file TranslationStrings.h
 * @author  idezentas
 * @brief Language definitions X-macros
 * @copyright Copyright (c) 2025
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 *
 * Thanks to rob040 for this template.
 *
 * This file contains the language definitions for the supported languages.
 *
 */

// Supported language list
// X(XX, "xx", "LanguageName")
// XX is the language code,
// "xx" is the language code string used in the Weather URL and in the language selection dropdown list
// LanguageName is the name of the language in that language (it is used in the language selection dropdown list)
#ifndef LANGUAGELIST
#define LANGUAGELIST       \
   X(EN, "en", "English")  \
   X(TR, "tr", "Türkçe")   \
   X(IT, "it", "Italiano") \
   X(ES, "es", "Español")

#endif // LANGUAGELIST

#ifdef TR

// OLED Strings
TR(TR_MONTHNAMESHORT,
   "JAN,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC", // English
   "OCA,ŞUB,MAR,NİS,MAY,HAZ,TEM,AĞU,EYL,EKİ,KAS,ARA", // Turkish
   "GEN,FEB,MAR,APR,MAG,GIU,LUG,AGO,SET,OTT,NOV,DIC", // Italian
   "ENE,FEB,MAR,ABR,MAY,JUN,JUL,AGO,SEP,OCT,NOV,DIC") // Spanish

TR(TR_DAYNAMESHORT,
   "SUN,MON,TUE,WED,THU,FRI,SAT", // English
   "PAZ,PZT,SAL,ÇAR,PER,CUM,CMT", // Turkish
   "DOM,LUN,MAR,MER,GIO,VEN,SAB", // Italian
   "DOM,LUN,MAR,MIÉ,JUE,VIE,SÁB") // Spanish

TR(TR_WINDDIRECTIONS,
   "N,NE,E,SE,S,SW,W,NW", // English
   "K,KD,D,GD,G,GB,B,KB", // Turkish
   "N,NE,E,SE,S,SO,O,NO", // Italian
   "N,NE,E,SE,S,SO,O,NO") // Spanish

TR(TR_WINDDIRECTIONSLONG,
   "NORTH,NORTHEAST,EAST,SOUTHEAST,SOUTH,SOUTHWEST,WEST,NORTHWEST", // English
   "KUZEY,KUZEYDOĞU,DOĞU,GÜNEYDOĞU,GÜNEY,GÜNEYBATI,BATI,KUZEYBATI", // Turkish
   "NORD,NORDEST,EST,SUDEST,SUD,SUDOVEST,OVEST,NORDOVEST",          // Italian
   "NORTE,NORESTE,ESTE,SURESTE,SUR,SUROESTE,OESTE,NOROESTE")        // Spanish

TR(TR_MOONPHASES,
   "NEW MOON,WAXING CRESCENT,FIRST QUARTER,WAXING GIBBOUS,FULL MOON,WANING GIBBOUS,THIRD QUARTER,WANING CRESCENT",            // English
   "YENİ AY,HİLAL,İLK DÖRDÜN,ŞİŞKİN AY,DOLUNAY,ŞİŞKİN AY,SON DÖRDÜN,HİLAL",                                                   // Turkish
   "LUNA NUOVA,LUNA CRESCENTE,PRIMO QUARTO,GIBBOSA CRESCENTE,LUNA PIENA,GIBBOSA CALANTE,ULTIMO QUARTO,LUNA CALANTE",          // Italian
   "LUNA NUEVA,LUNA CRECIENTE,CUARTO CRECIENTE,GIBOSA CRECIENTE,LUNA LLENA,GIBOSA MENGUANTE,CUARTO MENGUANTE,LUNA MENGUANTE") // Spanish

TR(TR_AIRQUALITYS,
   "GOOD,MODERATE,FAIR,POOR,VERY POOR",           // English
   "İYİ,İDARE EDER,ORTA,KÖTÜ,ÇOK KÖTÜ",           // Turkish
   "BUONA,MODERATA,DISCRETA,SCARSA,MOLTO SCARSA", // Italian
   "BUENA,MODERADA,ACEPTABLE,MALA,MUY MALA")      // Spanish

TR(TR_UVINDEXES,
   "GOOD,MODERATE,HIGH,VERY HIGH,EXTREME",            // English
   "DÜŞÜK,İDARE EDER,YÜKSEK,ÇOK YÜKSEK,AŞIRI YÜKSEK", // Turkish
   "BUONO,MODERATO,ALTO,MOLTO ALTO,ESTREMO",          // Italian
   "BUENO,MODERADO,ALTO,MUY ALTO,EXTREMO")            // Spanish

TR(TR_PROGRESSES,
   "Updating time,Updating TimeZoneDB,Updating OWM,Updating astronomy,Updating OpenMeteo,Done",                           // English
   "Saat güncelleniyor,TimeZoneDB güncelleniyor,OWM güncelleniyor,Astronomi güncelleniyor,OpenMeteo güncelleniyor,Bitti", // Turkish
   "Aggiornamento ora,Aggiornamento TimeZoneDB,Aggiornamento OWM,Aggiornamento astronomia,Aggiornamento OpenMeteo,Fatto", // Italian
   "Actualizando hora,Actualizando TimeZoneDB,Actualizando OWM,Actualizando astronomía,Actualizando OpenMeteo,Hecho")     // Spanish

TR(TR_SUN,
   "SUN",   // English
   "GÜNEŞ", // Turkish
   "SOLE",  // Italian
   "SOL")   // Spanish

TR(TR_MOON,
   "MOON", // English
   "AY",   // Turkish
   "LUNA", // Italian
   "LUNA") // Spanish

TR(TR_NIGHTMODESTART,
   "NIGHT MODE IS STARTING",     // English
   "GECE MODU BAŞLIYOR",         // Turkish
   "MODALITÀ NOTTURNA IN AVVIO", // Italian
   "INICIANDO MODO NOCTURNO")    // Spanish

TR(TR_NIGHTMODEFINISH,
   "NIGHT MODE IS FINISHED",      // English
   "GECE MODU BİTTİ",             // Turkish
   "MODALITÀ NOTTURNA TERMINATA", // Italian
   "MODO NOCTURNO FINALIZADO")    // Spanish

TR(TR_OTASTART,
   "OTA IS STARTING", // English
   "OTA BAŞLIYOR",    // Turkish
   "AVVIO OTA...",    // Italian
   "INICIANDO OTA")   // Spanish

TR(TR_OTAPROGRESS,
   "OTA IS UPLOADING",   // English
   "OTA YÜKLENİYOR",     // Turkish
   "CARICAMENTO OTA...", // Italian
   "CARGANDO OTA")       // Spanish

TR(TR_OTAFINISH,
   "OTA IS FINISHED", // English
   "OTA BİTTİ",       // Turkish
   "OTA TERMINATO",   // Italian
   "OTA FINALIZADO")  // Spanish

TR(TR_OTAERROR,
   "OTA ERROR",    // English
   "OTA HATA",     // Turkish
   "ERRORE OTA",   // Italian
   "ERROR DE OTA") // Spanish

TR(TR_NEXTUPDATE,
   "NEXT UPDATE",            // English
   "SONRAKİ GÜNCELLEME",     // Turkish
   "PROSSIMO AGGIORNAMENTO", // Italian
   "PRÓXIMA ACTUALIZACIÓN")  // Spanish

TR(TR_WEATHERSTATION,
   "WEATHER STATION",        // English
   "HAVA DURUMU İST",        // Turkish
   "STAZIONE METEO",         // Italian
   "ESTACIÓN METEOROLÓGICA") // Spanish

// HTML Strings
TR(TR_HTMLTITLE,
   "ESP8266 Weather Station",        // English
   "ESP8266 Hava Durumu İstasyonu",  // Turkish
   "Stazione Meteo ESP8266",         // Italian
   "Estación Meteorológica ESP8266") // Spanish

TR(TR_HTMLDEVICESETTINGS,
   "Device Settings",          // English
   "Cihaz Ayarları",           // Turkish
   "Impostazioni Dispositivo", // Italian
   "Ajustes del Dispositivo")  // Spanish

TR(TR_HTMLSECURITYSETTINGS,
   "Security Settings",      // English
   "Güvenlik Ayarları",      // Turkish
   "Impostazioni Sicurezza", // Italian
   "Ajustes de Seguridad")   // Spanish

TR(TR_HTMLOWMAPIKEY,
   "OpenWeatherMap API Key",      // English
   "OpenWeatherMap API Anahtarı", // Turkish
   "Chiave API OpenWeatherMap",   // Italian
   "Clave API de OpenWeatherMap") // Spanish

TR(TR_HTMLENTERAPIKEY,
   "Enter your API key",          // English
   "API anahtarınızı girin",      // Turkish
   "Inserisci la tua chiave API", // Italian
   "Introduce tu clave API")      // Spanish

TR(TR_HTMLSHOW,
   "Show",    // English
   "Göster",  // Turkish
   "Mostra",  // Italian
   "Mostrar") // Spanish

TR(TR_HTMLOCAPIKEY,
   "OpenCage API Key",      // English
   "OpenCage API Anahtarı", // Turkish
   "Chiave API OpenCage",   // Italian
   "Clave API de OpenCage") // Spanish

TR(TR_HTMLCITYNAME,
   "City Name",           // English
   "Şehir Adı",           // Turkish
   "Nome Città",          // Italian
   "Nombre de la Ciudad") // Spanish

TR(TR_HTMLCOUNTRYNAME,
   "Country Name",    // English
   "Ülke Adı",        // Turkish
   "Nome Nazione",    // Italian
   "Nombre del País") // Spanish

TR(TR_HTMLFINDLOCATION,
   "Search", // English
   "Ara",    // Turkish
   "Cerca",  // Italian
   "Buscar") // Spanish

TR(TR_HTMLLATITUDE,
   "Latitude",   // English
   "Enlem",      // Turkish
   "Latitudine", // Italian
   "Latitud")    // Spanish

TR(TR_HTMLLONGITUDE,
   "Longitude",   // English
   "Boylam",      // Turkish
   "Longitudine", // Italian
   "Longitud")    // Spanish

TR(TR_HTMLTIMEZONE,
   "Time Zone",    // English
   "Zaman Dilimi", // Turkish
   "Fuso Orario",  // Italian
   "Zona Horaria") // Spanish

TR(TR_HTMLUNITSELECT,
   "Select Unit",        // English
   "Birim Seçiniz",      // Turkish
   "Seleziona Unità",    // Italian
   "Seleccionar Unidad") // Spanish

TR(TR_HTMLUNITMETRIC,
   "Metric",  // English
   "Metrik",  // Turkish
   "Metrico", // Italian
   "Métrico") // Spanish

TR(TR_HTMLUNITIMPERIAL,
   "Imperial",  // English
   "İngiliz",   // Turkish
   "Imperiale", // Italian
   "Imperial")  // Spanish

TR(TR_HTMLUPDATEINTERVAL,
   "Update Interval (Minutes)",            // English
   "Güncelleme Sıklığı (Dakika)",          // Turkish
   "Intervallo Aggiornamento (Minuti)",    // Italian
   "Intervalo de Actualización (Minutos)") // Spanish

TR(TR_HTMLSAVE,
   "Save",    // English
   "Kaydet",  // Turkish
   "Salva",   // Italian
   "Guardar") // Spanish

TR(TR_HTMLHOME,
   "Homepage",         // English
   "Anasayfa",         // Turkish
   "Homepage",         // Italian
   "Página de Inicio") // Spanish

TR(TR_HTMLMISSINGINPUT,
   "Please enter city, country and OpenCage API key.",     // English
   "Lütfen şehir, ülke ve OpenCage API anahtarını girin.", // Turkish
   "Inserisci città, nazione e chiave API OpenCage.",      // Italian
   "Introduce ciudad, país y clave API de OpenCage.")      // Spanish

TR(TR_HTMLLOCATIONNOTFOUND,
   "Location not found.",      // English
   "Konum bulunamadı.",        // Turkish
   "Posizione non trovata.",   // Italian
   "Ubicación no encontrada.") // Spanish

TR(TR_HTMLAPIFAIL,
   "API request failed.",        // English
   "API isteği başarısız.",      // Turkish
   "Richiesta API fallita.",     // Italian
   "Fallo en la solicitud API.") // Spanish

TR(TR_HTMLABOUT,
   "About",        // English
   "Hakkında",     // Turkish
   "Informazioni", // Italian
   "Acerca de")    // Spanish

TR(TR_HTMLPASSWORD,
   "Password",   // English
   "Şifre",      // Turkish
   "Password",   // Italian
   "Contraseña") // Spanish

TR(TR_HTMLUSERNAME,
   "Username",      // English
   "Kullanıcı Adı", // Turkish
   "Nome Utente",   // Italian
   "Usuario")       // Spanish

TR(TR_HTMLENTERUSERNAME,
   "Enter username",        // English
   "Kullanıcı adı giriniz", // Turkish
   "Inserisci nome utente", // Italian
   "Introduce usuario")     // Spanish

TR(TR_HTMLENTERPASSWORD,
   "Enter password",       // English
   "Şifre giriniz",        // Turkish
   "Inserisci password",   // Italian
   "Introduce contraseña") // Spanish

TR(TR_HTMLDEFAULTSETTINGS,
   "Default Settings",         // English
   "Varsayılan Ayarlar",       // Turkish
   "Impostazioni Predefinite", // Italian
   "Ajustes Predeterminados")  // Spanish

TR(TR_HTMLDEFAULTRESTORED,
   "Restore to default settings. The device is restarting...",        // English
   "Varsayılan ayarlarına dönüldü. Cihaz yeniden başlatılıyor...",    // Turkish
   "Ripristino impostazioni predefinite. Riavvio del dispositivo...", // Italian
   "Restaurando ajustes predeterminados. Reiniciando dispositivo...") // Spanish

TR(TR_HTMLDEFAULTRESET,
   "Restore to Default Settings",         // English
   "Varsayılan ayarlarlara dön",          // Turkish
   "Ripristina Impostazioni Predefinite", // Italian
   "Restaurar Ajustes Predeterminados")   // Spanish

TR(TR_HTMLDEFAULTCONFIRM,
   "The device will be reset to default settings. Continue?",                 // English
   "Cihaz varsayılan ayarlarına dönecek. Devam edilsin mi?",                  // Turkish
   "Il dispositivo sarà ripristinato. Continuare?",                           // Italian
   "El dispositivo se reiniciará a los ajustes predeterminados. ¿Continuar?") // Spanish

TR(TR_HTMLHIDE,
   "Hide",     // English
   "Gizle",    // Turkish
   "Nascondi", // Italian
   "Ocultar")  // Spanish

TR(TR_HTMLOTAUPDATE,
   "OTA Update",        // English
   "OTA Güncellemesi",  // Turkish
   "Aggiornamento OTA", // Italian
   "Actualización OTA") // Spanish

TR(TR_HTMLONTIME,
   "Turn On Time",      // English
   "Açılma Zamanı",     // Turkish
   "Ora Accensione",    // Italian
   "Hora de Encendido") // Spanish

TR(TR_HTMLOFFTIME,
   "Turn Off Time",   // English
   "Kapanma Zamanı",  // Turkish
   "Ora Spegnimento", // Italian
   "Hora de Apagado") // Spanish

TR(TR_HTMLSECURITYCHANGED,
   "Security information successfully changed.",   // English
   "Güvenlik bilgileri başarıyla değiştirildi.",   // Turkish
   "Informazioni di sicurezza modificate.",        // Italian
   "Información de seguridad cambiada con éxito.") // Spanish

TR(TR_HTMLDEVICESAVED,
   "Device settings saved successfully. Redirecting to homepage...",     // English
   "Cihaz ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", // Turkish
   "Impostazioni salvate. Reindirizzamento alla homepage...",            // Italian
   "Ajustes guardados. Redirigiendo a la página de inicio...")           // Spanish

TR(TR_HTMLGETFROM,
   "Get from",   // English
   "Buradan",    // Turkish
   "Ottieni da", // Italian
   "Obtener de") // Spanish

TR(TR_HTMLHERE,
   "here",   // English
   "alınız", // Turkish
   "qui",    // Italian
   "aquí")   // Spanish

TR(TR_HTMLROTATEDISPLAY,
   "Flip display orientation",         // English
   "Ekran yönünü çevir",               // Turkish
   "Capovolgi Orientamento Display",   // Italian
   "Invertir orientación de pantalla") // Spanish

TR(TR_HTMLSAVING,
   "Saving...",       // English
   "Kaydediliyor...", // Turkish
   "Salvataggio...",  // Italian
   "Guardando...")    // Spanish

TR(TR_HTMLDISPLAYSETTINGS,
   "Display Settings",     // English
   "Ekran Ayarları",       // Turkish
   "Impostazioni Display", // Italian
   "Ajustes de Pantalla")  // Spanish

TR(TR_HTMLDISPLAYSAVED,
   "Display settings saved successfully. Redirecting to homepage...",      // English
   "Ekran ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...",   // Turkish
   "Impostazioni display salvate. Reindirizzamento alla homepage...",      // Italian
   "Ajustes de pantalla guardados. Redirigiendo a la página de inicio...") // Spanish

TR(TR_HTMLAPISETTINGS,
   "API Key Settings",        // English
   "API Anahtar Ayarları",    // Turkish
   "Impostazioni Chiavi API", // Italian
   "Ajustes de Claves API")   // Spanish

TR(TR_HTMLAPISAVED,
   "API keys saved successfully. Redirecting to homepage...",             // English
   "API Anahtarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", // Turkish
   "Chiavi API salvate. Reindirizzamento alla homepage...",               // Italian
   "Claves API guardadas. Redirigiendo a la página de inicio...")         // Spanish

TR(TR_HTMLBRIGHTNESS,
   "Display Contrast",      // English
   "Ekran Parlaklığı",      // Turkish
   "Contrasto Display",     // Italian
   "Contraste de Pantalla") // Spanish

TR(TR_HTMLTIMEFORMAT,
   "Time Format",     // English
   "Saat Formatı",    // Turkish
   "Formato Ora",     // Italian
   "Formato de Hora") // Spanish

TR(TR_HTMLREFRESHDATA,
   "Update Data",       // English
   "Verileri Güncelle", // Turkish
   "Aggiorna Dati",     // Italian
   "Actualizar Datos")  // Spanish

TR(TR_HTML24H,
   "24-hour",  // English
   "24 Saat",  // Turkish
   "24 ore",   // Italian
   "24 horas") // Spanish

TR(TR_HTML12H,
   "12-hour",  // English
   "12 Saat",  // Turkish
   "12 ore",   // Italian
   "12 horas") // Spanish

TR(TR_HTMLMONTHFORMAT,
   "Month Display Format", // English
   "Ay Gösterim Formatı",  // Turkish
   "Formato Mese",         // Italian
   "Formato de Mes")       // Spanish

TR(TR_HTMLMONTHNUMERIC,
   "As Number (Day/Month)",     // English
   "Sayı Olarak (Gün/Ay)",      // Turkish
   "Come Numero (Giorno/Mese)", // Italian
   "Como Número (Día/Mes)")     // Spanish

TR(TR_HTMLMONTH,
   "As Name",     // English
   "Yazı Olarak", // Turkish
   "Come Nome",   // Italian
   "Como Nombre") // Spanish

TR(TR_HTMLENTERTIME,
   "Please enter the on/off time in 24-hour format.",              // English
   "Lütfen açılma/kapanma saatini 24 saat formatında giriniz.",    // Turkish
   "Inserisci l'ora di accensione/spegnimento in formato 24 ore.", // Italian
   "Introduce la hora de encendido/apagado en formato 24 horas.")  // Spanish

TR(TR_HTMLSHOWONMAP,
   "Show on Map",     // English
   "Haritada Göster", // Turkish
   "Mostra su Mappa", // Italian
   "Mostrar en Mapa") // Spanish

TR(TR_HTMLSECURITYSAVED,
   "Security settings saved successfully. Redirecting to homepage...",      // English
   "Güvenlik ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", // Turkish
   "Impostazioni sicurezza salvate. Reindirizzamento alla homepage...",     // Italian
   "Ajustes de seguridad guardados. Redirigiendo a la página de inicio...") // Spanish

TR(TR_HTMLSYSTEMLANG,
   "System Language",    // English
   "Sistem Dili",        // Turkish
   "Lingua di Sistema",  // Italian
   "Idioma del Sistema") // Spanish

TR(TR_HTMLWORLDCLOCKSAVED,
   "World clock settings saved successfully. Redirecting to homepage...",       // English
   "Dünya saati ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...",  // Turkish
   "Impostazioni orologio mondiale salvate. Reindirizzamento alla homepage...", // Italian
   "Ajustes de reloj mundial guardados. Redirigiendo a la página de inicio...") // Spanish

TR(TR_HTMLWORLDCLOCKSETTINGS,
   "World Clock Settings",           // English
   "Dünya Saati Ayarları",           // Turkish
   "Impostazioni Orologio Mondiale", // Italian
   "Ajustes del Reloj Mundial")      // Spanish

TR(TR_HTMLWORLDCLOCKCITY1,
   "City 1",   // English
   "1.Şehir",  // Turkish
   "Città 1",  // Italian
   "Ciudad 1") // Spanish

TR(TR_HTMLWORLDCLOCKCITY2,
   "City 2",   // English
   "2.Şehir",  // Turkish
   "Città 2",  // Italian
   "Ciudad 2") // Spanish

TR(TR_HTMLWORLDCLOCKCITY3,
   "City 3",   // English
   "3.Şehir",  // Turkish
   "Città 3",  // Italian
   "Ciudad 3") // Spanish

TR(TR_HTMLQUERYOPTION,
   "Select the search method:",         // English
   "Sorgulama yöntemi seçin:",          // Turkish
   "Seleziona metodo di ricerca:",      // Italian
   "Selecciona el método de búsqueda:") // Spanish

TR(TR_HTMLQUERYADDRESS,
   "Search by entering an address", // English
   "Adres girerek sorgulama",       // Turkish
   "Cerca inserendo un indirizzo",  // Italian
   "Buscar por dirección")          // Spanish

TR(TR_HTMLQUERYLATLON,
   "Search by entering latitude and longitude", // English
   "Enlem ve Boylam girerek sorgulama",         // Turkish
   "Cerca inserendo latitudine e longitudine",  // Italian
   "Buscar por latitud y longitud")             // Spanish

TR(TR_HTMLQUERYADDRESSPLACE,
   "Enter address (example: Istanbul, Turkey)",   // English
   "Adres yazın (örnek: İstanbul, Türkiye)",      // Turkish
   "Inserisci indirizzo (es: Istanbul, Turchia)", // Italian
   "Introduce dirección (ej: Estambul, Turquía)") // Spanish

TR(TR_HTMLQUERYLATPLACE,
   "Latitude (example: 41.01)", // English
   "Enlem (örnek: 41.01)",      // Turkish
   "Latitudine (es: 41.01)",    // Italian
   "Latitud (ej: 41.01)")       // Spanish

TR(TR_HTMLQUERYLONPLACE,
   "Longitude (example: 28.98)", // English
   "Boylam (örnek: 28.98)",      // Turkish
   "Longitudine (es: 28.98)",    // Italian
   "Longitud (ej: 28.98)")       // Spanish

TR(TR_HTMLLOCATIONEDITOR,
   "Location Editor",     // English
   "Konum Düzenleyici",   // Turkish
   "Editor Posizione",    // Italian
   "Editor de Ubicación") // Spanish

TR(TR_HTMLCITYSETTINGS,
   "Settings",     // English
   "Ayarları",     // Turkish
   "Impostazioni", // Italian
   "Ajustes")      // Spanish

TR(TR_HTMLADDRESSERROR,
   "Enter address!",            // English
   "Adres girin!",              // Turkish
   "Inserisci indirizzo!",      // Italian
   "¡Introduce una dirección!") // Spanish

TR(TR_HTMLADDRESSNOTFOUND,
   "Address not found!",        // English
   "Adres bulunamadı!",         // Turkish
   "Indirizzo non trovato!",    // Italian
   "¡Dirección no encontrada!") // Spanish

TR(TR_HTMLADDRESSNOVALID,
   "Please enter a valid value!", // English
   "Geçerli bir değer girin!",    // Turkish
   "Inserisci un valore valido!", // Italian
   "¡Introduce un valor válido!") // Spanish

TR(TR_HTMLNOACTIVETAB,
   "Error: No active tab found!",                // English
   "Hata: Aktif sekme bulunamadı!",              // Turkish
   "Errore: Nessuna scheda attiva trovata!",     // Italian
   "¡Error: No se encontró una pestaña activa!") // Spanish

TR(TR_HTMLYES,
   "Yes",  // English
   "Evet", // Turkish
   "Sì",   // Italian
   "Sí")   // Spanish

TR(TR_HTMLNO,
   "No",    // English
   "Hayır", // Turkish
   "No",    // Italian
   "No")    // Spanish

TR(TR_HTMLSEARCHINFO,
   "Search Types: <br>- City Name, Country Name <br>- Postal Code, City Name",                               // English
   "Arama Türleri: <br>- Şehir Adı, Ülke Adı <br>- Posta Kodu, Şehir Adı",                                   // Turkish
   "Tipi di Ricerca: <br>- Nome della Città, Nome della Nazione <br>- Codice Postale, Nome della Città",     // Italian
   "Tipos de Búsqueda: <br>- Nombre de la Ciudad, Nombre del País <br>- Código Postal, Nombre de la Ciudad") // Spanish

TR(TR_HTMLTIMEAPIKEY,
   "TimeZoneDB API Key",      // English
   "TimeZoneDB API Anahtarı", // Turkish
   "Chiave API TimeZoneDB",   // Italian
   "Clave API de TimeZoneDB") // Spanish

// Serial Monitor Strings
TR(TR_LOGRSSI,
   "Signal Strength (RSSI)",      // English
   "Sinyal Gücü (RSSI)",          // Turkish
   "Potenza Segnale (RSSI)",      // Italian
   "Intensidad de Señal (RSSI):") // Spanish

TR(TR_LOGUSETHISURL,
   "Use this URL:",       // English
   "Bu URL'yi kullanın:", // Turkish
   "Usa questo URL:",     // Italian
   "Usa esta URL:")       // Spanish

TR(TR_LOGREADYFORUPDATE,
   "Setting readyForUpdate to true",       // English
   "readyForUpdate true olarak ayarlandı", // Turkish
   "Impostazione readyForUpdate a true",   // Italian
   "Estableciendo readyForUpdate a true")  // Spanish

TR(TR_LOGSCREENOPENED,
   "Display was turned on:", // English
   "Ekran Açıldı:",          // Turkish
   "Display acceso:",        // Italian
   "Pantalla encendida:")    // Spanish

TR(TR_LOGSCREENCLOSED,
   "Display was turned off:", // English
   "Ekran Kapatıldı:",        // Turkish
   "Display spento:",         // Italian
   "Pantalla apagada:")       // Spanish

TR(TR_LOGSCREENOPENTIME,
   "Time to turn display on:",       // English
   "Ekranı açma zamanı:",            // Turkish
   "Ora accensione display:",        // Italian
   "Hora de encendido de pantalla:") // Spanish

TR(TR_LOGSCREENCLOSETIME,
   "Time to turn display off:",    // English
   "Ekranı kapatma zamanı:",       // Turkish
   "Ora spegnimento display:",     // Italian
   "Hora de apagado de pantalla:") // Spanish

TR(TR_LOGENTERCONFIGMODE,
   "Entered config mode",                // English
   "Yapılandırma moduna girildi",        // Turkish
   "Entrato in modalità configurazione", // Italian
   "Entrando en modo configuración")     // Spanish

TR(TR_LOGCONNECTTOAP,
   "Please connect to AP",       // English
   "Lütfen AP'ye bağlanın",      // Turkish
   "Connettersi all'AP",         // Italian
   "Por favor, conéctese al AP") // Spanish

TR(TR_LOGWIFISETUP,
   "To setup WiFi connection",            // English
   "WiFi bağlantısı kurulumu için",       // Turkish
   "Per configurare la connessione WiFi", // Italian
   "Para configurar la conexión WiFi")    // Spanish

TR(TR_LOGDEVICESETTINGSUPDATED,
   "Device Settings Updated:",              // English
   "Cihaz Ayarları Güncellendi:",           // Turkish
   "Impostazioni Dispositivo Aggiornate:",  // Italian
   "Ajustes del Dispositivo Actualizados:") // Spanish

TR(TR_LOGSECURITYSETTINGSUPDATED,
   "Security Settings Updated:",         // English
   "Güvenlik Ayarları Güncellendi:",     // Turkish
   "Impostazioni Sicurezza Aggiornate:", // Italian
   "Ajustes de Seguridad Actualizados:") // Spanish

TR(TR_LOGFILENOTFOUND,
   "There is no file with that name.",     // English
   "Bu isimde bir dosya yok",              // Turkish
   "Nessun file con questo nome.",         // Italian
   "No existe un archivo con ese nombre.") // Spanish

TR(TR_LOGOTASTARTED,
   "OTA update started!",          // English
   "OTA güncellemesi başladı!",    // Turkish
   "Aggiornamento OTA avviato!",   // Italian
   "¡Actualización OTA iniciada!") // Spanish

TR(TR_LOGOTAPROGRESS,
   "OTA Progress",        // English
   "OTA İlerleme Durumu", // Turkish
   "Progresso OTA",       // Italian
   "Progreso OTA")        // Spanish

TR(TR_LOGNOW,
   "Current", // English
   "Şu an",   // Turkish
   "Attuale", // Italian
   "Actual")  // Spanish

TR(TR_LOGFINAL,
   "Final",  // English
   "Final",  // Turkish
   "Finale", // Italian
   "Final")  // Spanish

TR(TR_LOGOTASUCCESS,
   "OTA update finished successfully!",         // English
   "OTA güncellemesi başarıyla tamamlandı!",    // Turkish
   "Aggiornamento OTA terminato con successo!", // Italian
   "¡Actualización OTA finalizada con éxito!")  // Spanish

TR(TR_LOGOTAERROR,
   "There was an error during OTA update!",        // English
   "OTA güncellemesi sırasında bir hata oluştu!",  // Turkish
   "Errore durante l'aggiornamento OTA!",          // Italian
   "¡Hubo un error durante la actualización OTA!") // Spanish

TR(TR_LOGAPIKEYSUPDATED,
   "API Keys Updated:",            // English
   "API Anahtarları Güncellendi:", // Turkish
   "Chiavi API Aggiornate:",       // Italian
   "Claves API Actualizadas:")     // Spanish

TR(TR_LOGWAITNTP,
   "Waiting for NTP time...",   // English
   "NTP zamanı bekleniyor...",  // Turkish
   "In attesa dell'ora NTP...", // Italian
   "Esperando hora NTP...")     // Spanish

TR(TR_LOGNTPSYNCED,
   "Time synchronized.",       // English
   "Zaman senkronize edildi.", // Turkish
   "Ora sincronizzata.",       // Italian
   "Hora sincronizada.")       // Spanish

TR(TR_LOGDISPLAYSETTINGSUPDATED,
   "Display Settings Updated:",         // English
   "Ekran Ayarları Güncellendi:",       // Turkish
   "Impostazioni Display Aggiornate:",  // Italian
   "Ajustes de Pantalla Actualizados:") // Spanish

TR(TR_LOGFILEOPENFAIL,
   "File open failed!",           // English
   "Dosya açma başarısız!",       // Turkish
   "Apertura file fallita!",      // Italian
   "¡Error al abrir el archivo!") // Spanish

TR(TR_LOGSAVINGSETTINGS,
   "Saving settings now...",      // English
   "Ayarlar kaydediliyor...",     // Turkish
   "Salvataggio impostazioni...", // Italian
   "Guardando ajustes...")        // Spanish

TR(TR_LOGWORLDCLOCKSETTINGSUPDATED,
   "World Clock Settings Updated:",              // English
   "Dünya Saati Ayarları Güncellendi:",          // Turkish
   "Impostazioni Orologio Mondiale Aggiornate:", // Italian
   "Ajustes de Reloj Mundial Actualizados:")     // Spanish

#endif