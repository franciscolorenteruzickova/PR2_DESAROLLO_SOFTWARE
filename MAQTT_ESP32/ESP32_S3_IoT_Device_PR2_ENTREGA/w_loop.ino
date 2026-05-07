volatile bool boton_stop = false;
volatile bool boton_flag = false;
volatile bool boton_state_before = false;

void on_loop() {
  // =========================

  static uint32_t last_time = 0;
  // PARADA DE EMERGENCIA
  // =========================
  // Detectar cambio de estado (flanco)
  if (boton_flag)
  {
    boton_flag = false;

    if((millis() - last_time) > 1000)
    {
      boton_stop = !boton_stop;
      last_time = millis();
    }
  }

  if (boton_stop != boton_state_before)
  {
    if(boton_stop)
    {
      enviarMensajePorTopic(TOPIC_PUB, ENVIAR_BOTON_PULSADO);
      infoln("PARADA DE EMERGENCIA");
    } else 
    {
      enviarMensajePorTopic(TOPIC_PUB, ENVIAR_BOTON_LIBRE);
      infoln("REANUDADO");
    }
    boton_state_before = boton_stop;
  }
}

