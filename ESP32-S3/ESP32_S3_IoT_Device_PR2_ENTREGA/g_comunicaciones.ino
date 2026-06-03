void suscribirseATopics() {
  
  // TODO: añadir suscripciones a los topics MQTT ...
  mqtt_subscribe(TOPIC_SUB_LED);
  mqtt_subscribe(TOPIC_SUB_LED_SALIDA);
  mqtt_subscribe(TOPIC_SUB_LCD);


}

void alRecibirMensajePorTopic(char* topic, String incomingMessage) {

  // TODO: Controlador que gestiona la recepción de datos

  // A partir de aquí debemos gestionar los mensajes
  //  recibidos por los diferentes topics (canales)
  info("Mensaje recibido: ");
  infoln(incomingMessage);

  // Filtramos por topic (buena práctica)
  if (strcmp(topic, TOPIC_SUB_LED) == 0) {
    // Enviamos a los buffers (arquitectura concurrente)
    put_item(&buffer_LED, incomingMessage);
  } 
  else if (strcmp(topic, TOPIC_SUB_LED_SALIDA) == 0) { // Topic de la cinta de salida
    put_item(&buffer_LED_SALIDA, incomingMessage);
  }
  else if (strcmp(topic, TOPIC_SUB_LCD) == 0) { // Topic de la pantalla LCD
    put_item(&buffer_LCD, incomingMessage);

  } else {
    
    warnln("Topic no reconocido");
  }

}

void enviarMensajePorTopic(const char* topic, String outgoingMessage) {

  mqtt_publish(topic, outgoingMessage.c_str());

}





