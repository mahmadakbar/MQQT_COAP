/*
ESP-COAP Client
*/
#include <ESP8266WiFi.h>
#include "coap_client.h"

#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//instance for coapclient
coapClient coap;

//WiFi connection info
const char* ssid = "A_Redmi";
const char* password = "M2yAl244";
//const char* password = "*******";

//ip address and default port of coap server in which your interested in
IPAddress ip(192,168,43,213);//take ETH Zurich or coap.me server to run and check client 
int port =5683;

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port);

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
    char p[packet.payloadlen + 1];
    memcpy(p, packet.payload, packet.payloadlen);
    p[packet.payloadlen] = NULL;

    //response from coap server
 if(packet.type==3 && packet.code==0){
      Serial.println("ping ok");
    }

    Serial.println(p);
}

void setup() {
   
    Serial.begin(115200);

    dht.begin();

    WiFi.begin(ssid, password);
    Serial.println(" ");

    // Connection info to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    yield();
    Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address of client
    Serial.println(WiFi.localIP());

    // client response callback.
    // this endpoint is single callback.
    coap.response(callback_response);

    // start coap client
    coap.start();

    //get request to server (arguments ip adrress of server,default port,resource(uri))
    //int msgid = coap.get(ip,port,"light");

    //observe request (arguments ip adrress of server,deafult port,resource name,interger(0) ) 
    //int msgid= coap.observe(ip,port,"light",0);

    //reset observe cancel
    //int msgid=coap.observecancel(ip,port,"resoucename");
    
}
//int i=0;

//// Generally, you should use "unsigned long" for variables that hold time
//// The value will quickly become too large for an int to store
unsigned long pMillis = 0 ;   // will store last time DHT was updated
//
//// Updates DHT readings every 3 seconds
const long interval = 3000; 

void loop() {
    bool state;

    unsigned long currentMillis = millis();
    if (currentMillis - pMillis >= interval) {
      // save the last time you updated the DHT values
      pMillis = currentMillis;
      // Read Humidity
      float h = dht.readHumidity();
  
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
  
      if (isnan(t) || isnan(h)) {
          Serial.println("Failed to read from DHT sensor!");
      } else {
        char n[8];
        char m[9];
        dtostrf(h, 1, 2, n);
        dtostrf(t, 1, 2, m);
        Serial.println(n);
        Serial.println(m);

        //post request
        //arguments server ip address,default port,resource name, payload,payloadlength
        int msgid =coap.post(ip,port,"CoAP",n,strlen(n));
        int msgid2 =coap.post(ip,port,"CoAP",m,strlen(m));

        //put request 
        //arguments server ip address,default port,resource name, payload,payloadlength
//        int msgid =coap.put(ip,port,"CoAPkelembapan",n,strlen(n));
//        int msgid2 =coap.put(ip,port,"CoAPsuhu",m,strlen(m));
      }
    }

    state = coap.loop();

    //delay(1000);
}
