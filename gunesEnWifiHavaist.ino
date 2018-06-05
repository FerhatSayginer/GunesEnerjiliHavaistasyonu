/*Bu kodlar Ferhat SAYGINER Tarafından Cumhuriyet Üniversitesi 
Bilgisayar Mühendisliği Bitirme Projesi Olan Güneş Enerjili Wifi Hava
İstasyonunda Kullanılmak Üzere Yazılmıştır.*/

#include <Adafruit_Sensor.h> //DHT22 Sensörümüz için gerekli kütüphane.
#include <DHT.h> //DHT22 Sensörümüz için gerekli kütüphane.
#include <DHT_U.h> //DHT22 Sensörümüz için gerekli kütüphane.
#include "BMP280.h" //BMP280 Sensörümüz için gerekli kütüphane.
#include "Wire.h" //Arduino I2C haberleşmesi için gerekli kütüphane.
#define P0 1013.25 //BMP280 sensörümüzün yükseklik ölçümünde 
//kullanacağı bir sabit.
BMP280 bmp; //BMP280 sensörümüzün kodumuz içindeki temsili adı artık "bmp".
#include <ESP8266WiFi.h> //Devre Kartımız olan ESP8266'nın kütüphanesi.
#include <BlynkSimpleEsp8266.h> //Blynk Uygulamamız için gerekli olan kütüphane.
#define DHTPIN    2 //DHT22'nin hangi pinden okunacağı bildirildi.
#define DHTTYPE   DHT22 //Hangi DHT'yi kullandığımızı bildirdik.
DHT_Unified dht(DHTPIN, DHTTYPE); //DHTPIN ve DHTTYPE bildirildi.
int nem; //nem ölçümünde kullanacağımız değişkeni tanımladık.
char authToken[] = "92496ecb73f44e18ac5e634593091172";//Blynk Uyulaması 
//içerisinde Proje Ayarları sekmesinden almış oluduğumuz doğrulama şifresi.
char kullaniciAdi[] = "G6";
char sifre[] = "131213ff"; //ESP8266'nın bağlanacağı 
//WİFİ Kullanıcı Adı ve Şifre bilgileri.
void setup()
{
  Serial.begin(9600); // Seri iletişimi başlattık.
  Blynk.begin(authToken, kullaniciAdi, sifre); // Blynk Uygulamasına bağlandık.
  dht.begin(); //DHT22 Sensörümüz ile iletişimi başlattık.
  sensor_t sensor; //DHT22 Sensör işlemleri.
  dht.humidity().getSensor(&sensor); //DHT22 Sensör ayrıntıları.
  if(!bmp.begin()){ //Eğer Bağlı bir BMP280 Sensörü Bulunamazsa Uyarı verdirdik.
    Serial.println("Geçerli Bir BMP280 Sensörü Bulunamadı!");
    while(1);
  }
    bmp.setOversampling(4); //BMP Sensör ayrıntıları.
  }
void loop()
{  
   Blynk.begin(authToken, kullaniciAdi, sifre); //Projemiz 10 dk. da bir veri 
   //okuyacaktır bu sebeple yeniden bağlanmaya ihtiyacı olabilir.
   Blynk.run(); // Blynk uygulama iletişimi başlasın.
   sensors_event_t event; //DHT22 Sensör ayrıntıları. 
   dht.humidity().getEvent(&event); //DHT22 Sensör ayrıntıları. 
   if (isnan(event.relative_humidity)) { //DHT22 Sensöründen okuma 
   //yapılamadığında yazdırdık.
      Serial.println("Nem Okunurken Hata!");
   }
   else {//hata yoksa veriyi okuduk.
      Serial.print("\nNem= \t ");
      Serial.print(event.relative_humidity);
      Serial.println(" %");
      nem=event.relative_humidity; //nem değişkenine atamamazı yaptık.
      Blynk.virtualWrite(1, nem); // Blynk Uygulamamızda Sanal pin 1'e nem 
      //değeremizi gönderdik.
    }
  double sicaklik, basinc, yukseklik; //BMP280 Sensörü için değişkenleri tanımladık.
  char result = bmp.startMeasurment();
  if(result!=0){
    delay(result);
    result = bmp.getTemperatureAndPressure(sicaklik,basinc);
      if(result!=0)
      {
        yukseklik = bmp.altitude(basinc,P0);
        Serial.print("Yükseklik = \t");Serial.print(yukseklik); Serial.println(" m");
        //Seri port ekranına yaz.
        Blynk.virtualWrite(3, yukseklik); 
        // Blynk Uygulamamızda Sanal pin 3'e yükselik değeremizi gönderdik.
        Serial.print("Sıcaklık = \t");Serial.print(sicaklik); Serial.println(" C");
        //Seri port ekranına yaz.
         Blynk.virtualWrite(0, sicaklik); 
         // Blynk Uygulamamızda Sanal pin 0'a sıcaklık değeremizi gönderdik.
        Serial.print("Basınç = \t");Serial.print(basinc); Serial.println(" mBar"); 
        //Seri port ekranına yaz.
        Blynk.virtualWrite(2, basinc); 
        // Blynk Uygulamamızda Sanal pin 2'ye basıç değeremizi gönderdik.    
      }
      else {
        Serial.println("Hata!");//işlem yapılamazsa hata ver.
      }
  }
  else {
    Serial.println("Hata!");//işlem yapılamazsa hata ver.
  }
      delay(600000);//10 dk bekleterek okumalarımızı yapmaktayız.
}
