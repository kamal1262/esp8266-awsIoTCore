

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 4); //Rx, Tx pins

// #define DHTPIN 4        // Digital pin connected to the DHT sensor
// #define DHTTYPE DHT11   // DHT 11

// DHT dht(DHTPIN, DHTTYPE);

float h;
float t;
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;
String storedData = "";
// Variable to hold current epoch timestamp
unsigned long Epoch_Time;

#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"

WiFiClientSecure net;

BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);

time_t now;
time_t nowish = 1510592825;


void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

// Get_Epoch_Time() Function that gets current epoch time
unsigned long Get_Epoch_Time() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}


void messageReceived(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);


  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}


void publishMessage()
{
  /*
  StaticJsonDocument<250> doc;
  doc["time"] = millis();
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  */
  // StaticJsonDocument<250> jsonBuffer;
  // JsonObject& root = jsonBuffer(storedData);

  // serializeJson(jsonBuffer, storedData);

  //  DeserializationError error = deserializeJson(jsonBuffer, storedData);

  //  if (error) {
  //     Serial.print(F("deserializeJson() failed: "));
  //     Serial.println(error.f_str());
  //     return;
  //  }

  // //  const char* sensor = jsonBuffer["sensor"];
  // //  long time = jsonBuffer["time"];

  // //  Serial.println(sensor);
  char jsonobject1[300];

  storedData.toCharArray(jsonobject1, 300);
  // JSONVar myObject = JSON.parse(jsonobject1);


  //==========================timefrom board=======================================
  // if we need the timestamp from the board then use following code  ( AW IoT core)
  //-----------------------------------------------------------------


  /*
  StaticJsonDocument<250> doc;
  doc["time"] = Epoch_Time;
  doc["data"] = jsonobject1;
  // doc["data"] = storedData;

  char jsonBuffer[300];
  serializeJson(doc, jsonBuffer); // print to client

  // Serial.println(jsonobject1);
  Serial.println(jsonBuffer);
  // publish to MQTT server
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  */


  //=================================================================
  // if we dont need the timestamp from the board then use following code  ( AW IoT core)
  //-----------------------------------------------------------------


  // publish to MQTT server
  Serial.println(jsonobject1);
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonobject1);



}


void setup()
{
  mySerial.begin(9600);  // it will get data from Ardunio board
  Serial.begin(9600);  // to print on IDE serial monitor
  connectAWS();
  // dht.begin();
}


void loop()
{
  Epoch_Time = Get_Epoch_Time();
  // Serial.print("Epoch Time: ");
  // Serial.println(Epoch_Time);


  if (mySerial.available()) {
    // read the incoming data as string
    storedData = mySerial.readString();
    // Serial.println(storedData);
  }

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.println(F("°C "));
  // delay(1000);

  now = time(nullptr);

  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 5000)
    {
      lastMillis = millis();
      publishMessage();
    }
  }
}




