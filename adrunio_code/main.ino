#include <ArduinoJson.h>
#include <Arduino_HS300x.h>   // temperature and humidity sensor
#include <Arduino_LPS22HB.h>  // Pressure sensor

#include <Arduino_LSM9DS1.h>  // IMU

#include <Arduino_APDS9960.h>  // Color sensor

float old_temp = 0;
float old_hum = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);    // set LED pin as output
  digitalWrite(LED_BUILTIN, LOW);  // switch off LED pin

  Serial.begin(9600);   // initialize serial communication at 9600 bits per second:
  Serial1.begin(9600);  // initialize UART with baud rate of 9600

  if (!HS300x.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1)
      ;
  }
  // Initiate the pressure sensor
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1)
      ;
  }
  // // initiate IMU
  // if (!IMU.begin()) {
  //   Serial.println("IMU init failed");
  //   while (1) {}
  // }
  // // Initiate Color sensor
  // if (!APDS.begin()) {
  //   Serial.println("Failed to initialize colour sensor!");
  // }
}



void loop() {

/*
  Serial.println("hi there");
  delay(2000);

  Serial1.println("sending this msg from ardunio BLE nano 33");
  delay(2000);
*/
  delay(1000);
  StaticJsonDocument<250> json_doc;
  // json_doc["sensor"] = "gps";
  // json_doc["time"] = 1351824120;


  // Add an array.
  // JsonArray data = json_doc.createNestedArray("data");
  // data.add(48.756080);
  // data.add(2.302038);



  // read all the sensor values
  float temperature = HS300x.readTemperature();
  float humidity = HS300x.readHumidity();

  float pressure = BARO.readPressure();
  float x, y, z;  // IMU
  int r, g, b;    // Color sensor

  // // Pressure sensor test
  // Serial.print("Pressure = ");
  // Serial.print(pressure);
  // Serial.println(" kPa");

  json_doc["temperature"] = temperature;
  json_doc["humidity"] = humidity;
  json_doc["pressure"] = pressure;


  // Color sensor Test
  if (APDS.colorAvailable()) {

    // Read Color
    APDS.readColor(r, g, b);

    // // print the values
    // Serial.print("r = ");
    // Serial.println(r);
    // Serial.print("g = ");
    // Serial.println(g);
    // Serial.print("b = ");
    // Serial.println(b);
    // Serial.println();
  }
    // Add an array for color sensor
  JsonArray color = json_doc.createNestedArray("color");
  color.add(r);
  color.add(g);
  color.add(b);

  // IMU test
  if (IMU.accelerationAvailable()) {

    // Read Accelerometer
    IMU.readAcceleration(x, y, z);


    // Serial.print("x");
    // Serial.print('\t');
    // Serial.print("y");
    // Serial.print('\t');
    // Serial.print("z");
    // Serial.print('\n');
    // Serial.print(x);
    // Serial.print('\t');
    // Serial.print(y);
    // Serial.print('\t');
    // Serial.println(z);
  }

  // Add an array for IMU sensor
  JsonArray IMU = json_doc.createNestedArray("IMU");
  IMU.add(x);
  IMU.add(y);
  IMU.add(z);


  // Generate the minified JSON and send it to the Serial port.
  //
  serializeJson(json_doc, Serial1);  //sending on UART
  serializeJson(json_doc, Serial);   // print it on serial onitor
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  // Start a new line
  Serial.println();   //on serial monitor
  // Serial1.println();  // on UART write

  // Generate the prettified JSON and send it to the Serial port.
  // serializeJsonPretty(json_doc, Serial1); // UART to esp8266
  // serializeJsonPretty(json_doc, Serial); // on serial monitor
}
