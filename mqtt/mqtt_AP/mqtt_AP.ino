#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#include <PubSubClient.h>

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "ESP32_AP";
const char *password = "M2yAl244";

WiFiServer server(80);

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 22;

void reconnectmqttserver() {
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
     clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
    Serial.println("connected");
      client.subscribe("dht_sensor/LED");
    } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String MQTT_DATA = "";
  for (int i=0;i<length;i++) {
   MQTT_DATA += (char)payload[i];}
  if (MQTT_DATA == "ON") {
    digitalWrite(ledPin, HIGH);

  }
  if (MQTT_DATA == "OFF") {
    digitalWrite(ledPin, LOW);

  }

}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  client.setServer("192.168.4.2", 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);

  Serial.println("Server started");
}

void loop() {
 if (!client.connected()) {
    reconnectmqttserver();
 }
 client.loop();
}
