Buffer_String buffer_LED;
Buffer_String buffer_LCD;
Buffer_String buffer_TX;

volatile bool PARAR = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool put_item(Buffer_String *b, String item){
  portENTER_CRITICAL(&b->mux);

  if(b->count == BUFFER_SIZE){
    portEXIT_CRITICAL(&b->mux);
    return false;
  }

  b->buffer[b->in] = item;
  b->in = (b->in + 1) % BUFFER_SIZE;
  b->count++;

  portEXIT_CRITICAL(&b->mux);
  return true;
}

bool get_item(Buffer_String *b, String *item){
  portENTER_CRITICAL(&b->mux);

  if(b->count == 0){
    portEXIT_CRITICAL(&b->mux);
    return false;
  }

  *item = b->buffer[b->out];
  b->out = (b->out + 1) % BUFFER_SIZE;
  b->count--;

  portEXIT_CRITICAL(&b->mux);
  return true;
}

void IRAM_ATTR isr_boton(){
  PARAR = true;
}

void procesarLED(String msg){

  infoln("Procesando LED: " + msg);

  if(msg == "ENCENDER_LED"){
    infoln("LED ON");
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
  }

  if(msg == "APAGAR_LED"){
    infoln("LED OFF");
    digitalWrite(PIN_LED_3, LOW);
    digitalWrite(PIN_LED_4, HIGH);
  }
}

void procesarLCD(String msg){

  infoln("LCD: " + msg);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);
}

String leerLuz(){

  int valor = analogRead(PIN_LUZ);

  if(valor > 2000){
    infoln("MUCHA LUZ");
    return "MUCHA_LUZ";
  }

  if(valor < 1000){
    infoln("POCA LUZ");
    return "POCA_LUZ";
  }

  return "";
}

void tarea_led(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;
  String msg;
  
  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea LED iniciada");

  while(!PARAR){

    if(get_item(buf, &msg)){
      infoln("LED msg recibido: " + msg);
      procesarLED(msg);
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
  }

  infoln("Tarea LED finalizada");
  vTaskDelete(NULL);
}

void tarea_lcd(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;
  String msg;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea LCD iniciada");

  while(!PARAR){

    if(get_item(buf, &msg)){
      infoln("LCD msg recibido: " + msg);
      procesarLCD(msg);
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
  }
  infoln("Tarea LCD finalizada");
  vTaskDelete(NULL);
}

void tarea_luz(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea LUZ iniciada");

  while(!PARAR){

    String msg = leerLuz();

    if(msg != ""){
      infoln("Sensor luz: " + msg);

      put_item(buf, msg);
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(4000));
  }
  infoln("Tarea LUZ finalizada");
  vTaskDelete(NULL);
}

void tarea_mqtt_tx(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;
  String msg;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea MQTT TX iniciada");

  while(!PARAR){

    if(get_item(buf, &msg)){
      infoln("MQTT TX: " + msg);
      enviarMensajePorTopic(TOPIC_PUB, msg);
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
  }

  infoln("Tarea MQTT TX finalizada");
  vTaskDelete(NULL);
}