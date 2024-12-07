#include <FirebaseESP32.h>
#include <WiFi.h>
#include <TinyGPS++.h> // ใช้สำหรับจัดการข้อมูล GPS

// การกำหนดค่าพินและ Serial Interface
#define WIFI_SSID "bbs007 2.4G"
#define WIFI_PASSWORD "Ballsunbeer"

/* Firebase Configuration */
#define API_KEY "AIzaSyBiBXvhX4YenKelpFUA30_R5p_OVkbHy8o"
#define DATABASE_URL "https://mybike02-ec694-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "sunny179ss@gmail.com"
#define USER_PASSWORD "password"

// โมดูล A7670E
#define MODEM_RX_PIN 27
#define MODEM_TX_PIN 26
#define SerialAT Serial1

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// GPS object
TinyGPSPlus gps; // ออบเจ็กต์ GPS

unsigned long sendDataPrevMillis = 0;

void setup()
{
  Serial.begin(115200); // Serial Monitor
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN); // Serial สำหรับโมดูล A7670E

  // เชื่อมต่อ Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // เริ่มต้น Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);

  // ตรวจสอบการเชื่อมต่อ Firebase
  if (!Firebase.ready())
  {
    Serial.println("Failed to initialize Firebase.");
    while (true)
    {
      delay(1000);
    }
  }
  Serial.println("Firebase initialized.");
}

void loop()
{
  // อ่านข้อมูลจากโมดูล GPS ผ่าน SerialAT
  while (SerialAT.available() > 0)
  {
    char c = SerialAT.read();
    gps.encode(c); // ถอดรหัสข้อมูล NMEA

    if (gps.location.isUpdated())
    {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();

      // แสดงข้อมูล GPS บน Serial Monitor
      Serial.print("Latitude: ");
      Serial.println(latitude, 6);
      Serial.print("Longitude: ");
      Serial.println(longitude, 6);

      // ส่งข้อมูลไปยัง Firebase ทุก 5 วินาที
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0))
      {
        sendDataPrevMillis = millis();

        // สร้าง JSON สำหรับข้อมูล GPS
        FirebaseJson json;
        json.set("latitude", latitude);
        json.set("longitude", longitude);

        if (Firebase.updateNode(fbdo, F("/gps_data"), json))
        {
          Serial.println("GPS data sent to Firebase successfully.");
        }
        else
        {
          Serial.print("Failed to send GPS data: ");
          Serial.println(fbdo.errorReason());
        }
      }
    }
  }
}
