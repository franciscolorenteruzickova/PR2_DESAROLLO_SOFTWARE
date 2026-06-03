// Implemetación de la función del Buffer para insertar elementos
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
// Implementación de la función del Buffer para sacar elementos
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
// Definición de la iterrupción del pulsador
void IRAM_ATTR isr_boton()
{
  boton_flag = true;
}

// Función para encender o apagar el led RGB de la cinta de entrada

static void procesarLED(String msg){

  infoln("Procesando LED: " + msg);

  if(msg == "ENCENDER_LED"){ // Se ha considerado encender led que cambie a color verde
    infoln("LED ON");
    digitalWrite(PIN_LED_1, HIGH); 
    digitalWrite(PIN_LED_2, LOW);
  }

  if(msg == "APAGAR_LED"){ // Se ha considerado apagar led que cambie a color rojo
    infoln("LED OFF");
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, HIGH);
  }
}
// Función para realizar acciones sobre el led de la cinta de salida según los mensajes de MQTT
static void procesarLEDSALIDA(String msg){

  infoln("Procesando LED: " + msg);

  if(msg == "ENCENDER_LED"){
    infoln("LED ON");
    digitalWrite(PIN_LED_SALIDA_3, HIGH);
    digitalWrite(PIN_LED_SALIDA_4, LOW);
  }

  if(msg == "APAGAR_LED"){
    infoln("LED OFF");
    digitalWrite(PIN_LED_SALIDA_3, LOW);
    digitalWrite(PIN_LED_SALIDA_4, HIGH);
  }
}
// Función para mostrar los mensajes recibidos por la LCD
static void procesarLCD(String msg){

  infoln("LCD: " + msg);
  lcd.clear(); // Elimina todo lo que este en la pantalla impreso anteriormente
  lcd.setCursor(0,0); // Posiciona el cursor de la pantalla en la parte superior izquierda
  lcd.print(msg); // Imprime el mensaje recibido como parámetro por la función
}
// Función para reportar los mensajes de MUCHA_LUZ o POCA_LUZ según el valor de la LDR
static String leerLuz(){

  uint32_t valor = analogRead(PIN_LUZ);

  if(valor > 2000) // Si supera este valor imprime este mensaje
  {
    return "MUCHA_LUZ";

  }else if(valor < 1000) // Si es menor a este valor imprime este mensaje
  {
    return "POCA_LUZ";

  }else
  {
  return ""; // Si no se encuentra entre estos valores no cambia de estado
  }
}
// Función para la lectura del ultrasonidos
static String leerUltrasonidos()
{
  float suma = 0;
  int validas = 0;

  for(int i = 0; i < 3; i++) // Se hace una media de las lecturas del ultrasonidos para garantizar una buena medición
  {
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(5);
    // Se envia el pulso del trigger
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGGER, LOW);
    // Se recoge el pulso
    long duracion = pulseIn(PIN_ECHO, HIGH, 30000);

    if(duracion == 0)
    {
      continue;
    }

    float distancia = duracion * 0.0343 / 2; // Se calcula el valor de la distancia

    if(distancia >= 2 && distancia <= 400) // Si la lectura es inconsistente se rechaza
    {
      suma += distancia;
      validas++;
    }

    delay(20); // Se hace una espera entre lecturas para garantizar una buena lectura de valores
  }

  if(validas == 0)
  {
    return "";
  }

  float distancia = suma / validas;

  static String estado = ""; // Si no hay cambio de estado no se tiene en cuenta

  if(distancia < 10) // Si la distancia es menor a 10 cm se devuelve LLENO
  {
    if(estado != "LLENO")
    {
      estado = "LLENO";
      return estado;
    }
  }
  else if(distancia > 15) // Si la distancia es mayor a 15 cm se devuelve VACIO
  {
    if(estado != "VACIO")
    {
      estado = "VACIO";
      return estado;
    }
  }

  return "";
}
// Esta tarea se encarga de gestionar el LED RGB de entrada.
// Lee mensajes del buffer recibido por parámetro y, si hay
// un mensaje disponible, lo procesa mediante procesarLED().
// Si el sistema está parado con boton_stop, la tarea queda en pausa hasta que se vuelva a iniciar.
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
// Esta tarea controla el LED RGB de salida.
// Lee mensajes desde su buffer y los procesa con
// procesarLEDSALIDA(). Se pausa cuando boton_stop está activo.
static void tarea_led_salida(void *pv){

  Buffer_String *buf = (Buffer_String*) pv;
  String msg;
  
  TickType_t xLastWakeTime = xTaskGetTickCount();

  infoln("Tarea LED SALIDA iniciada");
  for(;;){

    while(boton_stop){
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_PAUSA));
    }
    
    if(get_item(buf, &msg)){
        infoln("LED SALIDA msg recibido: " + msg);
        procesarLEDSALIDA(msg);
      }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TIEMPO_ESPERA_LED));

    }
}
// Esta tarea lee periódicamente el sensor ultrasónico.
// Si detecta un cambio de estado, por ejemplo "LLENO"
// o "VACIO", guarda ese mensaje en el buffer para que
// después pueda enviarse por MQTT.
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
// Esta tarea se encarga de actualizar la pantalla LCD.
// Lee mensajes del buffer correspondiente y los muestra
// en la pantalla mediante la función procesarLCD().
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
// Esta tarea lee periódicamente el sensor LDR.
// Si detecta un cambio entre "MUCHA_LUZ" y "POCA_LUZ",
// guarda el mensaje en el buffer para enviarlo después.
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
// Esta tarea se encarga de enviar por MQTT los mensajes
// generados por los sensores. Lee tanto del buffer TX
// como del buffer del sensor ultrasónico y publica los
// datos en el topic definido por TOPIC_PUB.
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