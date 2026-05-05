#include <Wire.h>
#include <LiquidCrystal_I2C.h>
void on_setup() {

    infoln("INICIO SETUP");

    buffer_LED.mux = portMUX_INITIALIZER_UNLOCKED;
    buffer_LCD.mux = portMUX_INITIALIZER_UNLOCKED;
    buffer_TX.mux  = portMUX_INITIALIZER_UNLOCKED;
    // ======================
    // HARDWARE
    // ======================
    infoln("Inicializando hardware");
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_LUZ, INPUT);

    // ======================
    // LCD (si lo usas aquí)
    // ======================
    infoln("Inicializando LCD");
    Wire.begin(PIN_SDA,PIN_SCL);
    lcd.init();
    lcd.backlight();

    // ======================
    // ISR BOTÓN
    // ======================
    infoln("Configurando interrupción botón");
    attachInterrupt(PIN_BOTON, isr_boton, FALLING);

    // ======================
    // ESTADO INICIAL
    // ======================
    PARAR = false;

    // ======================
    // CREACIÓN DE TAREAS
    // ======================
    infoln("Creando tareas FreeRTOS");

    xTaskCreatePinnedToCore(tarea_led, "LED", 4096, &buffer_LED, 1, NULL, 1);

    xTaskCreatePinnedToCore(tarea_lcd, "LCD", 4096, &buffer_LCD, 1, NULL, 1);

    xTaskCreatePinnedToCore(tarea_luz, "LUZ", 4096, &buffer_TX, 1, NULL, 0);

    xTaskCreatePinnedToCore(tarea_mqtt_tx, "MQTT_TX", 4096, &buffer_TX, 1, NULL, 0);

    infoln("SETUP COMPLETADO");
}

