#include <Wire.h>
#include <LiquidCrystal_I2C.h>

TaskHandle_t lcd_task_handle = NULL;
TaskHandle_t led_task_handle = NULL;
TaskHandle_t led_salida_task_handle = NULL;
TaskHandle_t tx_task_handle = NULL;
TaskHandle_t luz_task_handle = NULL;
TaskHandle_t ultra_task_handle = NULL;

void on_setup() {

    infoln("INICIO SETUP");
    // ======================
    // HARDWARE
    // ======================
    infoln("Inicializando hardware");
    pinMode(PIN_LED_1, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);
    pinMode(PIN_LED_SALIDA_3, OUTPUT);
    pinMode(PIN_LED_SALIDA_4, OUTPUT);
    pinMode(PIN_LUZ, INPUT);
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    // ======================
    // LCD (si lo usas aquí)
    // ======================
    infoln("Inicializando LCD");
    Wire.begin(PIN_SDA,PIN_SCL);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Inicio Configuración");
    delay(3000);
    lcd.clear();

    // ======================
    // ISR BOTÓN
    // ======================
    infoln("Configurando interrupción botón");
    pinMode(PIN_BOTON, INPUT_PULLUP);
    attachInterrupt(PIN_BOTON, isr_boton, FALLING);

    buffer_LED.mux = portMUX_INITIALIZER_UNLOCKED;
    buffer_LCD.mux = portMUX_INITIALIZER_UNLOCKED;
    buffer_LED_SALIDA.mux = portMUX_INITIALIZER_UNLOCKED;
    buffer_TX.mux  = portMUX_INITIALIZER_UNLOCKED;
    buffer_ULTRA.mux = portMUX_INITIALIZER_UNLOCKED;

    buffers_mqtt.buffer_ULTRA = &buffer_ULTRA;
    buffers_mqtt.buffer_TX = &buffer_TX;

    infoln("Creando tareas FreeRTOS");

    xTaskCreatePinnedToCore(tarea_led, "LED", 6144, &buffer_LED, TAREA_LED_TASK_PRIORITY, &led_task_handle, 1);
    
    xTaskCreatePinnedToCore(tarea_led_salida, "LED SALIDA", 6144, &buffer_LED_SALIDA, TAREA_LED_SALIDA_TASK_PRIORITY, &led_salida_task_handle, 0);

    xTaskCreatePinnedToCore(tarea_lcd, "LCD", 6144, &buffer_LCD, TAREA_LCD_TASK_PRIORITY, &lcd_task_handle, 1);

    xTaskCreatePinnedToCore(tarea_luz, "LUZ", 6144, &buffer_TX, TAREA_LUZ_TASK_PRIORITY, &luz_task_handle, 0);

    xTaskCreatePinnedToCore(tarea_mqtt_tx, "MQTT_TX", 6144, &buffers_mqtt, TAREA_TX_TASK_PRIORITY, &tx_task_handle, 0);

    xTaskCreatePinnedToCore(tarea_ultra, "ULTRA", 6144, &buffer_ULTRA, TAREA_ULTRA_TASK_PRIORITY, &ultra_task_handle, 1);

    infoln("SETUP COMPLETADO");
}

