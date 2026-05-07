#include"Led.h"
#include <Arduino.h>

void suscribirseATopics() {

  mqtt_subscribe(TOPIC_SUB_BALIZAUNO);
  mqtt_subscribe(TOPIC_SUB_BALIZADOS);

}

void alRecibirMensajePorTopic(char* topic, String incomingMessage) {

  // TODO: Controlador que gestiona la recepción de datos

  // A partir de aquí debemos gestionar los mensajes
  //  recibidos por los diferentes topics (canales)
  info("Mensaje recibido: ");
  infoln(incomingMessage);

  if(strcmp(topic, TOPIC_SUB_BALIZAUNO) && incomingMessage == "0"){
    digitalWrite(cinta_uno.green_pin, LOW);
    digitalWrite(cinta_uno.red_pin, HIGH);
  }

  else if(strcmp(topic, TOPIC_SUB_BALIZAUNO) && incomingMessage == "1"){
    digitalWrite(cinta_uno.green_pin, HIGH);
    digitalWrite(cinta_uno.red_pin, LOW);
  }

  else if(strcmp(topic, TOPIC_SUB_BALIZADOS) && incomingMessage == "0"){
    digitalWrite(cinta_dos.green_pin, LOW);
    digitalWrite(cinta_dos.red_pin, HIGH);
  }

  else if(strcmp(topic, TOPIC_SUB_BALIZADOS) && incomingMessage == "1"){
    digitalWrite(cinta_dos.green_pin, HIGH);
    digitalWrite(cinta_dos.red_pin, LOW);
  }

  else {
    
    warnln("Topic no reconocido");
  }

}

void enviarMensajePorTopic(const char* topic, String outgoingMessage) {

  mqtt_publish(topic, outgoingMessage.c_str());

}





