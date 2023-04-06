// import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include "DHT.h"

// define dht11 pins
#define DHTPIN D7
#define DHTTYPE DHT11

// define the port to listen for incoming TCP connections
#define LISTEN_PORT 80


DHT dht(DHTPIN, DHTTYPE, 15); // initialize dht sensor
aREST rest = aREST(); // instance aREST
WiFiServer server(LISTEN_PORT); // create instance of server

// define variable to exposed to api
int temperature;
int humidity;

// wifi parameters
const char* ssid  = "Ridho";
const char* password = "katakunci2023";

// define host dweet io
const char* host = "dweet.io";

void setup() {
  Serial.begin(9600); // start serial
  dht.begin(); // init sensor dht

  // connect to wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  // reading connection dweet io
  Serial.print("Connecting to ");
  Serial.println(host);

  // reading temperature dan humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // use wifi client class to create tcp connections
  WiFiClient client;
  const int httPort = 80;
  if (!client.connect(host, httPort)) {
    Serial.println("connection failed");
    return;
  }

  // this will send teh request to the server dweet io
  client.print(String("GET /dweet/for/go.dht11?temperature=") + String(temperature) + "&humidity=" + String(humidity) + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection close\r\n\r\n");
  delay(10);
  Serial.println("closing connection");
  delay(10000); 
}
