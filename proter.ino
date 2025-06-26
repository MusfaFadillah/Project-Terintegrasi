unsigned long currentMillis;

// Deklarasi GPS ------------------------------------------------------------------------------------------------------------------------------------
#include <TinyGPSPlus.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

unsigned long previousMillisGps = 0; // waktu terakhir sensor dibaca
const long intervalGps = 10000;       // jeda waktu (10 detik = 10000 ms)

double latitude, longitude;

// The TinyGPS++ object
TinyGPSPlus gps;

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

// GPS Loop ------------------------------------------------------------------------------------------------------------------------------------
void gpsLoop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) {

      // Cek apakah waktu jeda sudah tercapai
      if (currentMillis - previousMillisGps >= intervalGps) {
        previousMillisGps = currentMillis;

        // Baca sensor
        displayInfo();
      }
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print("Sensor GPS :\n");
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  |  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    int hour = gps.time.hour() + 7;
    if (hour < 10) Serial.print(F("0"));
    Serial.print(hour);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  |  Satellites: ")); 
  if (gps.satellites.isValid())
  {
    Serial.print(gps.satellites.value());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
  Serial.println();
}

// Deklarasi Telegram ------------------------------------------------------------------------------------------------------------------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>

#define ssid "XxX"
#define password "12345678"

#define token "7188460217:AAFE5WGB32C7tT-7bzPpY662kfyC6psOocY"
#define CHAT_ID "6383860034"

WiFiClientSecure client;
UniversalTelegramBot bot(token, client);

int interval = 1000;
unsigned long waktu_terakhir;

// void handleNewMessages(int numNewMessages) {
//   Serial.println("handleNewMessages");
//   Serial.println(String(numNewMessages));

//   for (int i=0; i<numNewMessages; i++) {
    
//     // Cek Chat ID
//     String chat_id = String(bot.messages[i].chat_id);
//     if (chat_id != CHAT_ID) {
//       bot.sendMessage(chat_id, "Unauthorized user", "");
//       continue;
//     }
    
//     // Terima pesan dari telegram
//     String text = bot.messages[i].text;
//     Serial.println(text);

//     String from_name = bot.messages[i].from_name;
//     if (from_name == "") {
//       from_name = "Guest";
//     }

//     if (text == "/start") {
//       String welcome = "Selamat datang di sistem helm pintar otomatis";
//       bot.sendMessage(CHAT_ID, welcome);
//       Serial.println("Mengirim pesan : " + String(welcome));
//     }

//     if (text == "/maps") {
//       while (ss.available() > 0) {
//         if (gps.encode(ss.read()))
//           displayInfo();
//       }

//       latitude = gps.location.lat();
//       longitude = gps.location.lng();
      
//       if ((latitude == 0) || (longitude == 0))
//       {
//         latitude = -6.354758804041;
//         longitude = 106.775315014374;
//       }
//       String koordinat = "Lokasi saat ini : " + String(latitude, 12) + ", " + String(longitude, 12);
//       koordinat += "\nLink lokasi saat ini : https://www.google.com/maps/@" + String(latitude, 12) + "," + String(longitude, 12) + ",21z?entry=ttu";
//       bot.sendMessage(CHAT_ID, koordinat);
//       Serial.println("Mengirim pesan : \n" + String(koordinat));
//     }
//   }
// }

// void cek_pesan() {
//   if (millis() > waktu_terakhir + interval){
//     int banyakPesan = bot.getUpdates(bot.last_message_received + 1);
  
//     while(banyakPesan) {
//       Serial.println("\ngot response");
//       handleNewMessages(banyakPesan);
//       banyakPesan = bot.getUpdates(bot.last_message_received + 1);
//     }
//     waktu_terakhir = millis();
//   }
// }

// Setup Telegram ------------------------------------------------------------------------------------------------------------------------------------
void telegramSetup()
{
  // coba connect ke wifi
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nBerhasil terhubung ke WiFi : " + String(ssid) + "\n");

  // IP Address
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.print("Atur Waktu: ");
  configTime(0, 0, "pool.ntp.org"); 
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println("\n" + String(now) + "\n");
}

// Deklarasi MPU6050 ------------------------------------------------------------------------------------------------------------------------
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

unsigned long previousMillisMpu = 0; // waktu terakhir sensor dibaca
const long intervalMpu = 10000;       // jeda waktu (10 detik = 10000 ms)

float ax, ay, az;

// Setup MPU6050 ------------------------------------------------------------------------------------------------------------------------
void mpuSetup()
{
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) 
  {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) 
  {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) 
  {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}

// Loop MPU6050 ------------------------------------------------------------------------------------------------------------------------
void mpuLoop()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  ax = a.acceleration.x;
  ay = a.acceleration.y;
  az = a.acceleration.z;

  if (currentMillis - previousMillisMpu >= intervalMpu) {
    previousMillisMpu = currentMillis;

    // Print nilai sensor
    Serial.print("Sensor MPU6050 :\n");
    Serial.print("ax = " + String(ax) + "  |  ay = " + String(ay) + "  |  az = " + String(az) + "\n");
    Serial.println();
  }
}

// Deklarasi SW420 ------------------------------------------------------------------------------------------------------------------------
#define swPin 12

int getar;

// Setup SW420 ------------------------------------------------------------------------------------------------------------------------
void swSetup()
{
  pinMode(swPin, INPUT);
}

// Loop SW420 ------------------------------------------------------------------------------------------------------------------------
void swLoop()
{
  getar = digitalRead(swPin);
  if (getar == 1) {
    Serial.print("Nilai sensor Sw-420 : " + String(getar) + "\n");
  }
}

// Deteksi jatuh ------------------------------------------------------------------------------------------------------------------------
unsigned long fallStartTime = 0;
const unsigned long fallThresholdTime = 60000; // 60 detik
bool fallDetected = false;

void cekJatuh()
{
  // Cek kondisi helm miring selama lebih dari 1 menit
  bool isFallen =
    // (abs(ax) > 8.5 && abs(ay) < 3 && abs(az) < 3) ||
    // (abs(ay) > 8.5 && abs(ax) < 3 && abs(az) < 3) ||
    // (abs(ay) > 5 && abs(ax) < 3 && abs(az) > 5) ||
    // (abs(ax) > 5 && abs(ay) < 3 && abs(az) > 5)
    az < 5;

  // Jatuh 1 jika helm pengguna terus menerus miring selama 1 menit
  if (isFallen && getar == 1) 
  {
    fallDetected = true;
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    if ((latitude == 0) || (longitude == 0))
    {
      latitude = -6.354758804041;
      longitude = 106.775315014374;
    }
    String pesan1 = "🚨 Bahaya kecelakaan terdeteksi! Orientasi abnormal dan hantaman terdeteksi";
    pesan1 += "\nLokasi saat ini : " + String(latitude, 12) + ", " + String(longitude, 12);
    pesan1 += "\nLink lokasi saat ini : https://www.google.com/maps/@" + String(latitude, 12) + "," + String(longitude, 12) + ",21z?entry=ttu";
    // bot.sendMessage(CHAT_ID, pesan1);

    bool response = bot.sendMessage(CHAT_ID, pesan1);

    // Cek apakah respons dari server Telegram valid
    if (response == true) {
      Serial.println("✅ Pesan berhasil dikirim ke Telegram.");
    } else {
      Serial.println("❌ Gagal mengirim pesan.");
      Serial.println("Respons Telegram:");
      Serial.println(response);
    }

    Serial.println("Mengirim pesan : \n" + String(pesan1));

    Serial.println("🚨 Bahaya kecelakaan terdeteksi! Orientasi abnormal dan hantaman terdeteksi");
    Serial.println();
    // Tambahkan aksi di sini: notifikasi, buzzer, dll
    fallStartTime = millis();
    fallDetected = false;
  } 
  else 
  {
    fallStartTime = 0; // reset waktu jika orientasi kembali normal
    fallDetected = false;
  }

  // Jatuh 2 jika pengguna mengalami benturan dari sisi sampinng kanan kiri atau depan belakang, serta mengalami percepatan mendadak
  if 
  (/*abs(az) > 8.5 && (abs(ax) > 9.0 || abs(ay) > 9.0)*/ 
    (abs(ax) > 10 && abs(ay) > 10) || (abs(ax) > 10 && abs(az) > 10) || (abs(az) > 10 && abs(ay) > 10) ||
    (abs(ax) > 15) || (abs(ay) > 15) || (abs(az) > 15)
  ) 
  {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    if ((latitude == 0) || (longitude == 0))
    {
      latitude = -6.354758804041;
      longitude = 106.775315014374;
    }
    String pesan2 = "🚨 Bahaya kecelakaan terdeteksi! Helm tegak, ada benturan dari samping/depan";
    pesan2 += "\nLokasi saat ini : " + String(latitude, 12) + ", " + String(longitude, 12);
    pesan2 += "\nLink lokasi saat ini : https://www.google.com/maps/@" + String(latitude, 12) + "," + String(longitude, 12) + ",21z?entry=ttu";
    bot.sendMessage(CHAT_ID, pesan2);
    Serial.println("Mengirim pesan : \n" + String(pesan2));

    Serial.println("🚨 Bahaya kecelakaan terdeteksi! Helm tegak, ada benturan dari samping/depan");
    // Tambahkan aksi tambahan seperti buzzer, notifikasi, dll
  } 
  else {}
}

// --------------------------------------------

void setup()
{
  Serial.begin(115200);
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  telegramSetup();
  mpuSetup();
  swSetup();
}

// --------------------------------------------

void loop()
{
  currentMillis = millis();
  gpsLoop();
  // cek_pesan();
  mpuLoop();
  swLoop();
  cekJatuh();
  delay(100);
}
