void on_loop() {
  // =========================
  // PARADA DE EMERGENCIA
  // =========================
  if (PARAR) {

    enviarMensajePorTopic(TOPIC_PUB, "PARAR");

    infoln("PARADA DE EMERGENCIA");

    delay(100);

    exit(0);
  }
}

