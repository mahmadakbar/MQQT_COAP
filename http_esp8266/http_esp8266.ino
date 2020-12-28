#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <DHT.h>

/* Set these to your desired credentials. */
const char *ssid = "A_Redmi";  //ENTER YOUR WIFI SETTINGS
const char *password = "M2yAl244";

/* Set sensor type */
#define DHTTYPE DHT22 // type DHT 22
const int DHTPin = 4; // DHT Sensor pada pin 5 di NodeMCU
DHT dht(DHTPin, DHTTYPE); //inisialisasi library DHTpin

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 3000; 


//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(115200);

  dht.begin();
  
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);

    // Read Humidity
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    
    // if temperature read failed, don't change t & h value

    if (isnan(newT) || isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      h = newH;
      
      //Post Data
      String postData = String("kelembapan=") + h + String("&suhu_c=") + t;

      http.begin("http://192.168.43.213/esp8266/input.php");              //Specify request destination
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
    
      int httpCode = http.POST(postData);   //Send the request
      String payload = http.getString();    //Get the response payload
    
      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload
    
      http.end();  //Close connection
      
      
    }
  }
  //delay(5000);  //Post Data at every 5 seconds
}
//=======================================================================
