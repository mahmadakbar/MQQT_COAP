#include <ESP8266WiFi.h>
#include <WiFiClient.h> 

#include <EspMQTTClient.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); 

EspMQTTClient client(
  "A_Redmi",
  "M2yAl244",
  "192.168.43.213",  // MQTT Broker server ip
  "TestClient",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{
  Serial.begin(115200);  
  dht.begin();
  
  // Optionnal functionnalities of EspMQTTClient  : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
  delay(700);
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("dht_sensor/#", [](const String & payload) {
    Serial.println(payload);
  });
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String n;
  String m; 
  n = String(h);
  m = String(t); 
  // Publish a message to "mytopic/test"
  client.publish("dht_sensor/kelembapan", n);
  client.publish("dht_sensor/suhu", m);  
  client.loop();
  delay(3000);
}
