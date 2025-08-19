#include<ESP8266WiFi.h>
#include<FirebaseESP8266.h>
#include "DHT.h"
//wifi credentials
#define WIFI_SSID "MSB"
#define WIFI_PASSWORD "onetwothreefour"
 //Firebase credentials
#define FIREBASE_HOST "https://led-project-440f9-default-rtdb.asia-southeast1.firebasedatabase.app/"   // Without "https://"
#define FIREBASE_AUTH "AIzaSyDpBuWYpMtX0zwLLFunsi4nrpVksObUpjY"  
// DHT setup
#define DHTPIN D4        // GPIO2
#define DHTTYPE DHT11    // Change to DHT22 if using that
//create objects
DHT dht(DHTPIN, DHTTYPE);
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;
void setup()
{
   Serial.begin(115200);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\Connected to WiFi");
  //set firebase config
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  // No authentication needed with legacy token
  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
  dht.begin();
}
void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("°C Humidity: ");
  Serial.print(h);
  Serial.println("%");
  if (Firebase.setFloat(firebaseData,"/Temperature", t))
  {
    Serial.println("Temperature sent ");
  } else {
    Serial.println(firebaseData.errorReason());
  }
  if (Firebase.setFloat(firebaseData,"/Humidity", h))
  {
    Serial.println("Humidity sent");
  } else {
    Serial.println(firebaseData.errorReason());
  }
  delay(5000);

}