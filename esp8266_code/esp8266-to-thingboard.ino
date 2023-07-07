/*
the following code is reading sensor data from ardunio nano 33 ble sense using UART and then submitting to ECU Thing board
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>


const char* ssid = "LAB01";
const char* password = "Room23340";
#define TOKEN "BJHf6ggVFbyIdBhxGzfJ" //Access token of device Display
char ThingsboardHost[] = "10.5.64.84";
SoftwareSerial mySerial(5, 4); //Rx, Tx pins
String storedData = "";


WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;

void setup()
{
  mySerial.begin(9600); // it will get data from Ardunio board
  Serial.begin(9600); // to print on IDE serial monitor
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("connected to");
  Serial.println(ssid);
  client.setServer( ThingsboardHost, 1883 );
}

void loop()
{
  if ( !client.connected() ) 
  {
      reconnect();
  }

  if (mySerial.available()) {
    // read the incoming data as string
    storedData = mySerial.readString();
    Serial.println(storedData);
  }
  // #send data every 10 seconds
  getAndSendTemperatureAndHumidityData();
  delay(1000);
}

void getAndSendTemperatureAndHumidityData()
{
  /*
 
  // Prepare a JSON payload string
  String payload = "{";
 payload += "\"Humidity\":";payload += 60; payload += ",";
 payload += "\"Temperature\":";payload += 25; 
  payload += "}";

  char attributes[1000];
  payload.toCharArray( attributes, 1000 );
  client.publish( "v1/devices/me/telemetry",attributes);
  Serial.println( attributes );
  */

  char jsonobject1[300];
  storedData.toCharArray(jsonobject1, 300);

  client.publish( "v1/devices/me/telemetry",jsonobject1);
  // Serial.println( jsonobject1 );
   
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Drifted_ESP8266_V2", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 500 );
    }
  }
}
