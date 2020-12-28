#include <WiFi.h>
#include <PubSubClient.h>

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
      client.subscribe("test");
    } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
   WiFi.begin("A_Redmi","M2yAl244");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));
  client.setServer("192.168.43.213", 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
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


void loop()
{
  if (!client.connected()) {
    reconnectmqttserver();
    }
    client.loop();

}
