volatile bool boton_stop = false; // Estado inicial de las varibales del pulsador
volatile bool boton_flag = false;
volatile bool boton_state_before = false;

// Esta función se ejecuta continuamente.
// Se encarga de:
//  - Gestionar la parada de emergencia.
//  - Aplicar antirrebote al pulsador.
//  - Notificar por MQTT los cambios de estado del sistema.

void on_loop() {

  static uint32_t last_time = 0;
  // PARADA DE EMERGENCIA
  // Detectar cambio de estado
  if (boton_flag)
  {
    boton_flag = false;

    if((millis() - last_time) > 1000) // Ignorar pulsaciones durante 1 segundo
    {
      boton_stop = !boton_stop;
      last_time = millis();
    }
  }

  if (boton_stop != boton_state_before)
  {
    if(boton_stop)
    {
      enviarMensajePorTopic(TOPIC_PUB, ENVIAR_BOTON_PULSADO);// Pulblicación del mensaje mediante MQTT
      infoln("PARADA DE EMERGENCIA");
    } else 
    {
      enviarMensajePorTopic(TOPIC_PUB, ENVIAR_BOTON_LIBRE);
      infoln("REANUDADO");
    }
    boton_state_before = boton_stop;
  }
}

