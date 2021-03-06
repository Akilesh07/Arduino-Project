#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "realme 3i"
#define WLAN_PASS       "Akilesh7"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "Akilesh"
#define AIO_KEY         "aio_xWWE92mkVL038nliCGYDFSgAmTIx"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe light_button = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/akisusan");

void MQTT_connect();

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  mqtt.subscribe(&light_button);
}
void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &light_button) {
      Serial.println((char *)light_button.lastread);
      String vue =(char *)light_button.lastread;
    if(vue == "10"){
      digitalWrite(LED_BUILTIN,HIGH);
    }else
    {
      digitalWrite(LED_BUILTIN,LOW);
    }
    }  
  }
    if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}
