#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DHT.h>

// Update these with values suitable for your network.

const char* ssid = "ESP32_AP";
const char* password = "M2yAl244";
const char* mqtt_server = "192.168.4.2";

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <elapsedMillis.h>

elapsedMillis timeElapsed;
unsigned int interval_t = 3000;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  IPAddress ip(192,168,4,3);   
  IPAddress gateway(192,168,4,1);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String MQTT_DATA = "";
  for (int i = 0; i < length; i++) {
    MQTT_DATA += (char)payload[i];
  }

  // Switch on the LED if an 1 was received as first character
  if (MQTT_DATA == "OFF") {
    Serial.println("i wanna sleep");  
    
    while(timeElapsed >= interval_t){
      ESP.deepSleep(20e6);
    }
    
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "DHT_sensor";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("dht_sensor/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 3 seconds
const long interval = 3000; 

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;

    // Read Humidity
    float h = dht.readHumidity();
  
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
  
    // if temperature read failed, don't change t & h value
    if (isnan(t) || isnan(h)) {
        Serial.println("Failed to read from DHT sensor!");
    } else {
      char n[8];
      char m[9];
      dtostrf(h, 1, 2, n);
      dtostrf(t, 1, 2, m);
      Serial.println(n);
      Serial.println(m);
      client.publish("dht_sensor/kelembapan", n);
      client.publish("dht_sensor/suhu", m);
    }
  }
  client.loop();
  //delay(1000);
}
