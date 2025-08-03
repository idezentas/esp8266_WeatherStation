#include <Arduino.h>
#pragma once

String OPEN_WEATHER_MAP_LANGUAGE = "tr";

String WDAY_NAMES[] = {"PAZ", "PZT", "SAL", "ÇAR", "PER", "CUM", "CMT"};

String MONTH_NAMES[] = {"OCA", "ŞUB", "MAR", "NİS", "MAY", "HAZ", "TEM", "AĞU", "EYL", "EKİ", "KAS", "ARA"};

String SUN_MOON_TEXT[] = {"GÜNEŞ", "AY", "DOĞUŞ", "BATIŞ"};

String MOON_PHASES[] = {"YENİ AY", "HİLAL", "İLK DÖRDÜN", "ŞİŞKİN AY", "DOLUNAY", "ŞİŞKİN AY", "SON DÖRDÜN", "HİLAL"};

String WIND_DIRECTION[] = {"K", "KD", "D", "GD", "G", "GB", "B", "KB"};

String WIND_DIRECTION_LONG[] = {"KUZEY", "KUZEYDOĞU", "DOĞU", "GÜNEYDOĞU", "GÜNEY", "GÜNEYBATI", "BATI", "KUZEYBATI"};

String AIR_QUALITY_TEXT[] = {"İYİ", "İDARE EDER", "ORTA", "KÖTÜ", "ÇOK KÖTÜ"};

String UV_INDEX_TEXT[] = {"DÜŞÜK", "İDARE EDER", "YÜKSEK", "ÇOK YÜKSEK", "AŞIRI YÜKSEK"};

String PROGRESS_TEXT[] = {"Zaman güncelleniyor...", "Hava güncelleniyor...", "Tahminler güncelleniyor...", "Hava kirliliği güncelleniyor...", "Astronomi güncelleniyor", "Bitti...", "UV indeksi güncelleniyor...", "Kurlar guncelleniyor"};

String HTML_TEXT[] = {"ESP8266 Hava Durumu İstasyonu", "Cihaz Ayarları", "WiFi Ayarları", "Güvenlik Ayarları", "OpenWeatherMap API Anahtarı", "API anahtarınızı girin", "Göster", "OpenCage API Anahtarı", "İlçe Adı", "Örnek: Sarıyer", "Şehir Adı", "Örnek: İstanbul", "Ülke Kodu", "Örnek: TR", "Şehirden Konum/Zaman Bul", "Enlem", "Örnek: 41.1719525", "Boylam", "Örnek: 28.859487", "Zaman Dilimi (GMT)", "Örnek: 3", "Yaz Saatini Kullan", "Zaman Dilimi", "Örnek: Europe/Istanbul", "Zaman Dilimi Kısa Adı", "Örnek: +03", "Birim Seçiniz", "Metrik", "İngiliz", "Güncelleme Sıklığı (Dakika)", "Örnek: 30", "Kaydet", "Anasayfaya Git", "Lütfen ilçe, şehir, ülke ve OpenCage API anahtarını girin.", "Konum bulunamadı.", "API isteği başarısız.", "SSID", "Wifi ağ adını giriniz", "Şifre", "Wifi ağ şifresini giriniz", "Kullanıcı Adı", "Kullanıcı adı giriniz", "Şifre giriniz", "Varsayılan Ayarlar", "Varsayılan ayarlarına dönüldü. Cihaz yeniden başlatılıyor...", "Varsayılan ayarlarlara dön", "Cihaz varsayılan ayarlarına dönecek. Devam edilsin mi?", "Gizle", "JSON çözümleme hatası", "Ayarlar dosyası bulunamadı.", "Ayarları İçe Aktarma", "Ayarlar başarıyla yüklendi. Cihaz yeniden başlatılıyor...", "Dosya oluşturulamadı.", "Dosya açılamadı.", "Geçersiz JSON: ", "Ayarlar dosyası yazılamadı.", "Ayarları Dışa Aktar", "Tüm Ayarları Dışa Aktar (İndir)", "Ayar Dosyası (.json):", "Ayarları Yükle (İçe Aktar)", "OTA Güncellemesi", "Açılma Zamanı", "Örnek: 08:00", "Kapanma Zamanı", "Örnek: 00:00", "Güvenlik bilgileri başarıyla değiştirildi.", "Cihaz ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", "buradan", "alınız", "Wifi Ağını Unut", "Dosya bulunamadı", "Eksik 'file' parametresi", "Dünya Saati Ayarları", "Dünya saati ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", "Tüm Şehirlerin Zaman Dilimlerini Bul", "1. Şehir", "Örnek: Los Angeles", "Örnek: US", "Örnek: -7", "Örnek: America/Los_Angeles", "Örnek: PST", "2. Şehir", "Örnek: Londra", "Örnek: GB", "Örnek: 0","Örnek: Europe/London", "Örnek: GMT", "3. Şehir", "Örnek: Seul", "Örnek: KR", "Örnek: 9", "Örnek: Asia/Seoul","Örnek: KST", "Lütfen OpenCage API anahtarını girin.", "Şehir veya ülke eksik:", "Döviz Kuru Ayarları", "Değerler API tarafından günlük olarak güncellenmektedir.", "1.Döviz Kuru", "Temel Para Birimi", "Hedef Para Birimi", "2.Döviz Kuru", "Döviz Kuru ayarları başarıyla kaydedildi. Ana sayfaya yönlediriliyor...", "Kur listesi yüklenemedi:"};

String NIGHT_MODE_TEXT[] = {"GECE MODU", "BAŞLIYOR", "BİTTİ"};

String LOG_TEXT[] = {"HAVA DURUMU İST", "Sinyal Gücü (RSSI): ", "Bu URL'yi kullanın : ", "readyForUpdate true olarak ayarlandı", "Ekran AÇILDI: ", "Ekran KAPATILDI: ", "Ekranı açma zamanı: ", "Ekranı kapatma zamanı: ", "deserializeJson() başarısız oldu: ", "Yapılandırma moduna girildi", "Lütfen AP'ye bağlanın", "WiFi bağlantısı kurulumu için", "Cihaz Ayarları Güncellendi:", "Güvenlik Ayarları Güncellendi:", "Bu isimde bir dosya yok", "Ayar Dosyaları İndirildi", "Dünya Saati Ayarları Güncellendi:", "Döviz Kuru Ayarları Güncellendi:"};
