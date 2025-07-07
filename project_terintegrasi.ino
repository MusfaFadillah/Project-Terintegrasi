#include <Arduino.h>
unsigned long currentMillis;
int statusKecelakaan = 0;

// Deklarasi GPS --------------------------------------------------
#include <TinyGPSPlus.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

unsigned long prevMillisPrintGps = 0, prevMillisReadGps = 0; // waktu terakhir sensor dibaca
const unsigned long intervalPrintGps = 5000, intervalReadGps = 1000; // jeda waktu (10 detik = 10000 ms)

double latitude, longitude;
int jam, menit, detik, tanggal, bulan, tahun, satelit;

// The TinyGPS++ object
TinyGPSPlus gps;

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

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
    if (hour > 24) {
      hour = hour - 24;
    }
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

// GPS Setup --------------------------------------------------
void gpsSetup()
{
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
}

// GPS Loop --------------------------------------------------
void gpsLoop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) {

      // Cek apakah waktu jeda sudah tercapai
      if (currentMillis - prevMillisReadGps >= intervalReadGps) {
        prevMillisReadGps = currentMillis;

        // Baca data sensor
        satelit = gps.satellites.value();
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        jam = gps.time.hour();
        if (jam > 24) {
          jam = jam - 24;
        }
        menit = gps.time.minute();
        detik = gps.time.second();
        tanggal = gps.date.day();
        bulan = gps.date.month();
        tahun = gps.date.year();
      }

      // Cek apakah waktu jeda sudah tercapai
      if (currentMillis - prevMillisPrintGps >= intervalPrintGps) {
        prevMillisPrintGps = currentMillis;

        // Tampilkan data sensor
        displayInfo();
      }
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

// Deklarasi MPU6050 --------------------------------------------------
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

unsigned long prevMillisPrintMpu = 0; // waktu terakhir sensor dibaca
const unsigned long intervalPrintMpu = 5000;       // jeda waktu (10 detik = 10000 ms)

float ax, ay, az;

// MPU6050 Setup --------------------------------------------------
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

// MPU6050 Loop --------------------------------------------------
void mpuLoop()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  ax = a.acceleration.x;
  ay = a.acceleration.y;
  az = a.acceleration.z;

  if (currentMillis - prevMillisPrintMpu >= intervalPrintMpu) {
    prevMillisPrintMpu = currentMillis;

    // Print nilai sensor
    Serial.print("Sensor MPU6050 :\n");
    Serial.print("ax = ");
    Serial.print(String(ax));
    Serial.print("  |  ay = ");
    Serial.print(String(ay));
    Serial.print("  |  az = ");
    Serial.print(String(az));
    Serial.print("\n");
    Serial.println();
  }
}

// Deklarasi SW420 --------------------------------------------------
#define swPin 12

int getar = 0, statusGetarNow, statusGetarPrev = 0;

// SW420 Setup --------------------------------------------------
void swSetup()
{
  pinMode(swPin, INPUT);
}

// SW420 Loop --------------------------------------------------
void swLoop()
{
  statusGetarNow = digitalRead(swPin);
  if (statusGetarNow != statusGetarPrev) {
    getar = statusGetarNow;
    statusGetarPrev = statusGetarNow;
    Serial.print("Nilai sensor Sw-420 : ");
    Serial.print(String(getar));
    Serial.print("\n");
  }
}

// Deklarasi Telegram --------------------------------------------------
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>

#define ssid_wifi "XxX"
#define password_wifi "12345678"

#define token_telegram "7188460217:AAFE5WGB32C7tT-7bzPpY662kfyC6psOocY"
#define CHAT_ID "6383860034"

WiFiClientSecure client;
UniversalTelegramBot bot(token_telegram, client);

// Telegram Setup --------------------------------------------------
void telegramSetup()
{
  // coba connect ke wifi
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(ssid_wifi);
  WiFi.begin(ssid_wifi, password_wifi);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nBerhasil terhubung ke WiFi : ");
  Serial.print(String(ssid_wifi));
  Serial.print("\n");

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
  Serial.print("\n");
  Serial.print(String(now));
  Serial.println("\n");
}

// Deklarasi MQTT --------------------------------------------------
#include <PubSubClient.h>

const char* mqtt_server = "broker.hivemq.com";

unsigned long prevMillisMqtt = 0;
const unsigned long intervalMqtt = 3000;

WiFiClient espClient;
PubSubClient mqttclient(espClient);
#define MSG_BUFFER_SIZE  (50)

String nilaiJam = "0", nilaiMenit = "0", nilaiDetik = "0";

void reconnect() {
  // Loop until we're reconnected
  while (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttclient.connect(clientId.c_str())) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
      mqttclient.publish("musfa/mqtt", "musfa");
      // ... and resubscribe
      mqttclient.subscribe("musfa/mqtt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// MQTT Setup --------------------------------------------------
void mqttSetup() {
  mqttclient.setServer(mqtt_server, 1883);
}

// MQTT Loop --------------------------------------------------
void mqttLoop() {
  if (!mqttclient.connected()) {
    reconnect();
  }
  mqttclient.loop();

  if (currentMillis - prevMillisMqtt > intervalMqtt) {
    prevMillisMqtt = currentMillis;
    
    // Buat String nilai
    int nilaiJam1 = gps.time.hour() + 7;
    if (nilaiJam1 > 24) {
      nilaiJam1 = nilaiJam1 - 24;
    }
    if (nilaiJam1 < 10) {
      nilaiJam = "0";
      nilaiJam += String(nilaiJam1);
    }
    else {
      nilaiJam = String(nilaiJam1);
    }
    // ---
    int nilaiMenit1 = gps.time.minute();
    if (nilaiMenit1 < 10) {
      nilaiMenit = "0";
      nilaiMenit += String(nilaiMenit1);
    }
    else {
      nilaiMenit = String(nilaiMenit1);
    }
    // ---
    int nilaiDetik1 = gps.time.second();
    if (nilaiDetik1 < 10) {
      nilaiDetik = "0";
      nilaiDetik += String(nilaiDetik1);
    }
    else {
      nilaiDetik = String(nilaiDetik1);
    }

    // Buat JSON string
    String payload = "{";

    payload += "\"ax\":\"";
    payload += String(ax, 2);
    payload += "\",";

    payload += "\"ay\":\"";
    payload += String(ay, 2);
    payload += "\",";

    payload += "\"az\":\"";
    payload += String(az, 2);
    payload += "\",";

    payload += "\"getar\":\"";
    payload += String(getar);
    payload += "\",";

    payload += "\"satelit\":\"";
    payload += String(satelit);
    payload += "\",";

    payload += "\"latitude\":\"";
    payload += String(latitude, 6);
    payload += "\",";

    payload += "\"longitude\":\"";
    payload += String(longitude, 6);
    payload += "\",";

    payload += "\"tanggal\":\"";
    payload += String(tanggal);
    payload += "\",";

    payload += "\"bulan\":\"";
    payload += String(bulan);
    payload += "\",";

    payload += "\"tahun\":\"";
    payload += String(tahun);
    payload += "\",";

    payload += "\"jam\":\"";
    payload += String(nilaiJam);
    payload += "\",";

    payload += "\"menit\":\"";
    payload += String(nilaiMenit);
    payload += "\",";

    payload += "\"detik\":\"";
    payload += String(nilaiDetik);
    payload += "\",";

    payload += "\"status\":\"";
    payload += String(statusKecelakaan);
    payload += "\"";

    payload += "}";

    Serial.println(payload);

    mqttclient.publish("musfa/proter", payload.c_str());

    Serial.println();
  }
}

// Deteksi jatuh --------------------------------------------------
bool isFallen = false;
unsigned long fallStartTime = 0;
const unsigned long intervalFall = 60000;

void mqttKecelakaan()
{
  if (!mqttclient.connected()) {
    reconnect();
  }
  mqttclient.loop();

  // Buat String nilai
  int nilaiJam1 = gps.time.hour() + 7;
  if (nilaiJam1 > 24) {
    nilaiJam1 = nilaiJam1 - 24;
  }
  if (nilaiJam1 < 10) {
    nilaiJam = "0";
    nilaiJam += String(nilaiJam1);
  }
  else {
    nilaiJam = String(nilaiJam1);
  }
  // ---
  int nilaiMenit1 = gps.time.minute();
  if (nilaiMenit1 < 10) {
    nilaiMenit = "0";
    nilaiMenit += String(nilaiMenit1);
  }
  else {
    nilaiMenit = String(nilaiMenit1);
  }
  // ---
  int nilaiDetik1 = gps.time.second();
  if (nilaiDetik1 < 10) {
    nilaiDetik = "0";
    nilaiDetik += String(nilaiDetik1);
  }
  else {
    nilaiDetik = String(nilaiDetik1);
  }

  // Buat JSON string
  String payload = "{";

  payload += "\"ax\":\"";
  payload += String(ax, 2);
  payload += "\",";

  payload += "\"ay\":\"";
  payload += String(ay, 2);
  payload += "\",";

  payload += "\"az\":\"";
  payload += String(az, 2);
  payload += "\",";

  payload += "\"getar\":\"";
  payload += String(getar);
  payload += "\",";

  payload += "\"satelit\":\"";
  payload += String(satelit);
  payload += "\",";

  payload += "\"latitude\":\"";
  payload += String(latitude, 6);
  payload += "\",";

  payload += "\"longitude\":\"";
  payload += String(longitude, 6);
  payload += "\",";

  payload += "\"tanggal\":\"";
  payload += String(tanggal);
  payload += "\",";

  payload += "\"bulan\":\"";
  payload += String(bulan);
  payload += "\",";

  payload += "\"tahun\":\"";
  payload += String(tahun);
  payload += "\",";

  payload += "\"jam\":\"";
  payload += String(nilaiJam);
  payload += "\",";

  payload += "\"menit\":\"";
  payload += String(nilaiMenit);
  payload += "\",";

  payload += "\"detik\":\"";
  payload += String(nilaiDetik);
  payload += "\",";

  payload += "\"status\":\"";
  payload += String(statusKecelakaan);
  payload += "\"";

  payload += "}";

  Serial.println(payload);

  mqttclient.publish("musfa/proter", payload.c_str());

  Serial.println();
}

void cekJatuh()
{
  // Cek kondisi helm pengguna
  statusKecelakaan = 0;

  // Jatuh 1 jika helm miring dan ada getaran
  if (az < 5 && getar == 1) {
    statusKecelakaan = 1;

    // if (latitude == 0 && longitude == 0) {
    //   latitude = -6.354806;
    //   longitude = 106.775043;
    // }

    String pesan1 = "🚨 Bahaya kecelakaan terdeteksi! Orientasi abnormal dan hantaman terdeteksi";
    pesan1 += "\nLokasi saat ini : ";
    pesan1 += String(latitude, 6);
    pesan1 += ", ";
    pesan1 += String(longitude, 6);
    pesan1 += "\nLink lokasi saat ini : https://www.google.com/maps/?q=";
    pesan1 += String(latitude, 6);
    pesan1 += ",";
    pesan1 += String(longitude, 6);

    bool response1 = bot.sendMessage(CHAT_ID, pesan1);

    Serial.println("Mengirim pesan1");

    // Cek apakah respons dari server Telegram valid
    if (response1 == true) {
      Serial.println("✅ Pesan berhasil dikirim ke Telegram.");
    } 
    else {
      Serial.println("❌ Gagal mengirim pesan.");
      Serial.print("Respons Telegram: ");
      Serial.println(response1);
    }

    mqttKecelakaan();
  } 
  else {
    statusKecelakaan = 0;
  }

  // Jatuh 2 jika pengguna mengalami benturan dari sisi sampinng kanan kiri atau depan belakang, serta mengalami percepatan mendadak
  if ((abs(ax) > 10 && abs(ay) > 10) || (abs(ax) > 10 && abs(az) > 10) || (abs(az) > 10 && abs(ay) > 10) ||
      (abs(ax) > 15) || (abs(ay) > 15) || (abs(az) > 15)) {
    statusKecelakaan = 1;

    // if (latitude == 0 && longitude == 0) {
    //   latitude = -6.354806;
    //   longitude = 106.775043;
    // }

    String pesan2 = "🚨 Bahaya kecelakaan terdeteksi! Helm tegak, ada benturan dari samping/depan";
    pesan2 += "\nLokasi saat ini : ";
    pesan2 += String(latitude, 6);
    pesan2 += ", ";
    pesan2 += String(longitude, 6);
    pesan2 += "\nLink lokasi saat ini : https://www.google.com/maps/?q=";
    pesan2 += String(latitude, 6);
    pesan2 += ",";
    pesan2 += String(longitude, 6);

    bool response2 = bot.sendMessage(CHAT_ID, pesan2);

    Serial.println("Mengirim pesan2");

    // Cek apakah respons dari server Telegram valid
    if (response2 == true) {
      Serial.println("✅ Pesan berhasil dikirim ke Telegram.");
    } else {
      Serial.println("❌ Gagal mengirim pesan.");
      Serial.print("Respons Telegram: ");
      Serial.println(response2);
    }

    mqttKecelakaan();
  } 
  else {
    statusKecelakaan = 0;
  }

  // Jatuh 3 jika helm miring selama 1 menit
  // Deteksi jatuh berdasarkan akselerasi (misal az < 5 dianggap jatuh)
  if (az < 5) {
    if (fallStartTime == 0) {
      fallStartTime = currentMillis;
      Serial.println("Mulai menghitung jatuh");
    }
    
    if (currentMillis - fallStartTime > intervalFall) {
      statusKecelakaan = 1;
      
      // if (latitude == 0 && longitude == 0) {
      //   latitude = -6.354806;
      //   longitude = 106.775043;
      // }

      String pesan3 = "🚨 Bahaya kecelakaan terdeteksi! Helm miring selama lebih dari 1 menit";
      pesan3 += "\nLokasi saat ini : ";
      pesan3 += String(latitude, 6);
      pesan3 += ", ";
      pesan3 += String(longitude, 6);
      pesan3 += "\nLink lokasi saat ini : https://www.google.com/maps/?q=";
      pesan3 += String(latitude, 6);
      pesan3 += ",";
      pesan3 += String(longitude, 6);

      bool response3 = bot.sendMessage(CHAT_ID, pesan3);

      Serial.println("Mengirim pesan3");

      // Cek apakah respons dari server Telegram valid
      if (response3 == true) {
        Serial.println("✅ Pesan berhasil dikirim ke Telegram.");
      } else {
        Serial.println("❌ Gagal mengirim pesan.");
        Serial.print("Respons Telegram: ");
        Serial.println(response3);
      }

      mqttKecelakaan();

      fallStartTime = 0;
    }
  }
  else {
    statusKecelakaan = 0;
    fallStartTime = 0;
  }

  // if (az < 5) {
  //   if (!isFallen) {
  //     isFallen = true;
  //     fallStartTime = millis();
  //     Serial.println("Terdeteksi jatuh!");
  //   }
  // } else {
  //   // Jika sudah bangun sebelum 1 menit
  //   if (isFallen) {
  //     isFallen = false;
  //     fallStartTime = 0;
  //     Serial.println("Bangkit kembali, aman.");
  //   }
  // }

  // // Cek apakah sudah jatuh lebih dari 1 menit
  // if (isFallen && (currentMillis - fallStartTime > intervalFall)) {
  //   statusKecelakaan = 1;

  //   // if (latitude == 0 && longitude == 0) {
  //   //   latitude = -6.354806;
  //   //   longitude = 106.775043;
  //   // }

  //   String pesan3 = "🚨 Bahaya kecelakaan terdeteksi! Helm miring selama lebih dari 1 menit";
  //   pesan3 += "\nLokasi saat ini : ";
  //   pesan3 += String(latitude, 6);
  //   pesan3 += ", ";
  //   pesan3 += String(longitude, 6);
  //   pesan3 += "\nLink lokasi saat ini : https://www.google.com/maps/?q=";
  //   pesan3 += String(latitude, 6);
  //   pesan3 += ",";
  //   pesan3 += String(longitude, 6);

  //   bool response3 = bot.sendMessage(CHAT_ID, pesan3);

  //   Serial.print("Mengirim pesan3");

  //   // Cek apakah respons dari server Telegram valid
  //   if (response3 == true) {
  //     Serial.println("✅ Pesan berhasil dikirim ke Telegram.");
  //   } else {
  //     Serial.println("❌ Gagal mengirim pesan.");
  //     Serial.print("Respons Telegram: ");
  //     Serial.println(response3);
  //   }

  //   mqttKecelakaan();

  //   // Reset status agar tidak mengulang-ulang aksi
  //   isFallen = false;
  //   fallStartTime = 0;
  // }
  // else {
  //   statusKecelakaan = 0;
  // }
}

// --------------------------------------------------
void setup()
{
  Serial.begin(115200);

  telegramSetup();
  mqttSetup();
  gpsSetup();
  mpuSetup();
  swSetup();
}

//  --------------------------------------------------
void loop()
{
  currentMillis = millis();

  gpsLoop();
  mpuLoop();
  swLoop();
  mqttLoop();
  cekJatuh();
  
  delay(50);
}