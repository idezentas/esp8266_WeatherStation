#include <Arduino.h>

String OPEN_WEATHER_MAP_LANGUAGE = "tr";

const char *DIFF_TEXT = "FARK";

const char *HOUR_TEXT = "SA";

const char *WDAY_NAMES[] = {"PAZ", "PZT", "SAL", "ÇAR", "PER", "CUM", "CMT"};

const char *MONTH_NAMES[] = {"OCA", "ŞUB", "MAR", "NİS", "MAY", "HAZ", "TEM", "AĞU", "EYL", "EKİ", "KAS", "ARA"};

const char *SUN_MOON_TEXT[] = {"GÜNEŞ", "AY"};

const char *MOON_PHASES[] = {"YENİ AY", "HİLAL", "İLK DÖRDÜN", "ŞİŞKİN AY", "DOLUNAY", "ŞİŞKİN AY", "SON DÖRDÜN", "HİLAL"};

const char *WIND_DIRECTION[] = {"K", "KD", "D", "GD", "G", "GB", "B", "KB"};

const char *WIND_DIRECTION_LONG[] = {"KUZEY", "KUZEYDOĞU", "DOĞU", "GÜNEYDOĞU", "GÜNEY", "GÜNEYBATI", "BATI", "KUZEYBATI"};

const char *AIR_QUALITY_TEXT[] = {"İYİ", "İDARE EDER", "ORTA", "KÖTÜ", "ÇOK KÖTÜ"};

const char *UV_INDEX_TEXT[] = {"DÜŞÜK", "İDARE EDER", "YÜKSEK", "ÇOK YÜKSEK", "AŞIRI YÜKSEK"};

const char *PROGRESS_TEXT[] = {"Saatler güncelleniyor...", "OWM güncelleniyor...", "Astronomi güncelleniyor...", "OpenMeteo güncelleniyor...", "FloatRates güncelleniyor...", "Bitti..."};

const char *TIME_UPDATE_TEXT[] = {"SONRAKİ", "GÜNCELLEME"};

const char *NIGHT_MODE_TEXT[] = {"GECE MODU", "BAŞLIYOR", "BİTTİ"};

const char *OTA_TEXT[] = {"OTA", "BAŞLIYOR", "BİTTİ", "YÜKLENİYOR", "HATA"};

const char *HTML_TEXT[] = {"ESP8266 Hava Durumu İstasyonu", "Cihaz Ayarları", "Güvenlik Ayarları", "OpenWeatherMap API Anahtarı", "API anahtarınızı girin", "Göster", "OpenCage API Anahtarı", "Şehir Adı", "Örnek: İstanbul", "Ülke Adı", "Örnek: Türkiye", "Şehirden Konum/Zaman Dilimi Bul", "Enlem", "Örnek: 41.17", "Boylam", "Örnek: 28.86", "Zaman Dilimi", "Birim Seçiniz", "Metrik", "İngiliz", "Güncelleme Sıklığı (Dakika)", "Kaydet", "Anasayfaya Git", "Lütfen şehir, ülke ve OpenCage API anahtarını girin.", "Konum bulunamadı.", "API isteği başarısız.", "Hakkında", "Şifre", "Kullanıcı Adı", "Kullanıcı adı giriniz", "Şifre giriniz", "Varsayılan Ayarlar", "Varsayılan ayarlarına dönüldü. Cihaz yeniden başlatılıyor...", "Varsayılan ayarlarlara dön", "Cihaz varsayılan ayarlarına dönecek. Devam edilsin mi?", "Gizle", "OTA Güncellemesi", "Açılma Zamanı", "Kapanma Zamanı", "Güvenlik bilgileri başarıyla değiştirildi.", "Cihaz ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", "buradan", "alınız", "Döviz Kuru Ayarları", "Değerler API tarafından günlük olarak güncellenmektedir.", "1.Döviz Kuru", "Temel Para Birimi", "Hedef Para Birimi", "2.Döviz Kuru", "Döviz Kuru ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", "Kur listesi yüklenemedi:", "Ekran yönünü çevir", "Kaydediliyor...", "Temel Para Birimi ile Hedef Para Birimi aynı olamaz", "Döviz Kuru", "Ekran Ayarları", "Ekran ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", "API Anahtar Ayarları", "API Anahtarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", "Zaman dilimi bulunamadı", "Zaman dilimleri alınamadı: ", "Zaman dilimleri yüklenemedi.", "Ekran Parlaklığı", "Saat Formatı", "Verileri Güncelle", "24 Saat", "12 Saat", "Ay Gösterim Formatı", "Sayı Olarak", "Yazı Olarak", "Lütfen açılma/kapanma saatini 24 saat formatında giriniz."};

const char *LOG_TEXT[] = {"HAVA DURUMU İST", "Sinyal Gücü (RSSI): ", "Bu URL'yi kullanın : ", "readyForUpdate true olarak ayarlandı", "Ekran Açıldı: ", "Ekran Kapatıldı: ", "Ekranı açma zamanı: ", "Ekranı kapatma zamanı: ", "deserializeJson() başarısız oldu: ", "Yapılandırma moduna girildi", "Lütfen AP'ye bağlanın", "WiFi bağlantısı kurulumu için", "Cihaz Ayarları Güncellendi:", "Güvenlik Ayarları Güncellendi:", "Bu isimde bir dosya yok", "Döviz Kuru Ayarları Güncellendi:", "OTA güncellemesi başladı!", "OTA İlerleme Durumu", "Şuan", "Final", "OTA güncellemesi başarıyla tamamlandı!", "OTA güncellemesi sırasında bir hata oluştu!", "API Anahtarları Güncellendi:", "NTP zamanı bekleniyor...", "Zaman senkronize edildi."};
