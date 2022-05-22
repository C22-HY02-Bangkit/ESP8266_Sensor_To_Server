#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Pin for sensor
#define soilPin 0
#define DHTPIN D4

// Declare Additional Libraries
#include <DHT.h>
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float humidity, temperature, heat_index;

const char* ssid = "PARAMITA";
const char* password = "76602223";
String serverName = "http://tranquil-tor-91089.herokuapp.com/api/monitor";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  heat_index = dht.computeHeatIndex(temperature, humidity);
  Serial.print("Humidity : " + String(humidity));
  Serial.print("  | Temperature : " + String(temperature));
  Serial.print("  | Heat index : " + String(heat_index) + "\n");


  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + "?text=" + String(temperature);;
      Serial.print(serverPath);
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    delay(1000);
}
