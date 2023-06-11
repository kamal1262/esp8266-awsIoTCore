#include <pgmspace.h>
 
#define SECRET
 
const char WIFI_SSID[] = "Vodafone-1A82D";               //TAMIM2.4G
const char WIFI_PASSWORD[] = "n9dSxHdefTf984c9";           //0544287380
 
#define THINGNAME "ESP8266"
 
int8_t TIME_ZONE = -5; //NYC(USA): -5 UTC
 
const char MQTT_HOST[] = "a2aamxm5eja0es-ats.iot.ap-southeast-2.amazonaws.com";
 
 
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
 
 
// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
 
)KEY";
 
 
// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
 
)KEY";
