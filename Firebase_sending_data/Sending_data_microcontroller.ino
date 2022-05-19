#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>



#define FIREBASE_HOST "hy-02-database-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "CJ9CIQJQDPwQxzOey5TGfT1vXMFlUPJRSd9NJ40g"
#define WIFI_SSID "QWERTYUIOP"
#define WIFI_PASSWORD "ohhyeahh"

#define soil_pin A0    // Connect Data pin of DHT to D2


//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;


void setup()
{

  Serial.begin(9600);
  pinMode(A0, INPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void sensorUpdate(){

  float soilMosture = analogRead(A0);
  if (isnan(soilMosture)) {
    Serial.println(F("Tidak bisa membaca!"));
    return;
  }

  Serial.println("Nilai soil: " + String(soilMosture));

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/soilMosture", soilMosture))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  
}
void loop() {
  sensorUpdate();
}
