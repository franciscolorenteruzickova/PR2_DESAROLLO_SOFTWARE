#include"KeyBoard.h"
#include"Screen.h"
#include"Sensor.h"
#include"Button.h"
#include"Led.h"

#include<Arduino.h>
#include"Config.h"
#include<WiFi.h>
#ifdef SSL_ROOT_CA
#include <WiFiClientSecure.h>
#endif
#include<PubSubClient.h>

String deviceID = String("giirobpr2-device-") + String(DEVICE_GIIROB_PR2_ID); 

void setup(){

#ifdef LOGGER_ENABLED
  // Inicializamos comunicaciones serial
  Serial.begin(BAUDS);
  delay(1000);
  Serial.println();
#endif

  // Nos conectamos a la wifi
  wifi_connect();

  // Nos conectamos al broker MQTT, indicando un 'client-id'
  mqtt_connect(deviceID);

  // TODO: completar esta función (g_comunicaciones.ino)
  suscribirseATopics();

  // TODO: completar esta función (s_setup.ino)
  on_setup();


}

void loop() {

  // NO QUITAR (jjfons)
  wifi_loop();
  mqtt_loop();

  // TODO: completar esta función (w_loop.ino)
  on_loop();
}