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
float temperature;
float humidity;

// wifi parameters
const char* ssid  = "user";
const char* password = "password";

void setup() {
  Serial.begin(9600); // start serial
  dht.begin(); // init sensor dht

  // init variable and expose them to REST api
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // connect to wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  // start server
  server.begin();
  Serial.println("Server Started");
}

void loop() {
  // reading temperature dan humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // hadnle rest calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while(!client.available()) {
    delay(1);    
  }

  rest.handle(client);
}
