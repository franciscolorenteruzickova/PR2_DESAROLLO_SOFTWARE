
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

void IRAM_ATTR isr_boton()
{
  boton_flag = true;
}



static void procesarLED(String msg){

  infoln("Procesando LED: " + msg);

  if(msg == "ENCENDER_LED"){
    infoln("LED ON");
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
    digitalWrite(PIN_LED_3, LOW);
    digitalWrite(PIN_LED_4, LOW);
  }

  if(msg == "APAGAR_LED"){
    infoln("LED OFF");
    digitalWrite(PIN_LED_3, HIGH);
    digitalWrite(PIN_LED_4, HIGH);
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);
  }
}

static void procesarLCD(String msg){

  infoln("LCD: " + msg);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);
}

static String leerLuz(){

  uint32_t valor = analogRead(PIN_LUZ);

  if(valor > 2000)
  {
    return "MUCHA_LUZ";

  }else if(valor < 1000)
  {
    return "POCA_LUZ";

  }else
  {
  return "";
  }
}

static String leerUltrasonidos()
{
  long duracion;
  float distancia;

  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER, LOW);

  duracion = pulseIn(PIN_ECHO, HIGH, 30000);

  if(duracion == 0)
  {
    return "";
  }

  distancia = duracion * 0.034 / 2;

  infoln("Valor Distancia: " + String(distancia));

  static String estado = "";

  if(distancia < 10)
  {
    if(estado != "LLENO")
    {
      estado = "LLENO";
      return estado;
    }
  }
  else if (distancia > 15)
  {
    if(estado != "VACIO")
    {
      estado = "VACIO";
      return estado;
    }
  }

  return "";
}

static void tarea_led(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;
  String msg;
  
  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea LED iniciada");
  for(;;){

    while(boton_stop){
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_PAUSA));
    }
    
    if(get_item(buf, &msg)){
        infoln("LED msg recibido: " + msg);
        procesarLED(msg);
      }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_ESPERA_LED));

    }
}

static void tarea_ultra(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  static String last_msg = "";

  infoln("Tarea ULTRA iniciada");

  for(;;)
  {
    while(boton_stop)
    {
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_PAUSA));
    }

    String msg = leerUltrasonidos();

    if(msg != "" && msg != last_msg)
    {
      infoln("Sensor ultrasonidos: " + msg);

      put_item(buf, msg);

      last_msg = msg;
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_ESPERA_ULTRA));
  }
}
static void tarea_lcd(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;
  String msg;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea LCD iniciada");

  for(;;){

    while(boton_stop)
    {
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_PAUSA));
    }
    if(get_item(buf, &msg))
    {
    infoln("LCD msg recibido: " + msg);
    procesarLCD(msg);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_ESPERA_LCD));
    }
  }
}

static void tarea_luz(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  static String last_msg = "";

  infoln("Tarea LUZ iniciada");

  for(;;)
  {
    while(boton_stop)
    {
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_PAUSA));
    }
    String msg = leerLuz();
    if(msg != "" && msg != last_msg)
    {
      infoln("Sensor luz: " + msg);
      put_item(buf, msg);
      last_msg = msg;
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_ESPERA_LUZ));
  }
}

static void tarea_mqtt_tx(void *pv){

  TaskBuffers *bufs = (TaskBuffers*) pv;
  String msg;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea MQTT TX iniciada");

  for(;;)
  {
    while(boton_stop)
    {
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_PAUSA));
    }
    if(get_item(bufs->buffer_TX, &msg))
    {
      infoln("MQTT TX: " + msg);
      enviarMensajePorTopic(TOPIC_PUB, msg);
    }
    if(get_item(bufs->buffer_ULTRA, &msg))
    {
      infoln("MQTT TX: " + msg);
      enviarMensajePorTopic(TOPIC_PUB, msg);
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_ESPERA_LUZ));
  }
}