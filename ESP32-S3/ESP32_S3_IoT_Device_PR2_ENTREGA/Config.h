#include <LiquidCrystal_I2C.h>
// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "08" //"giirobpr2_00"8

// WIFI
#define NET_SSID                  "iPhone" // Definir el nombre de la red Wifi a usar
#define NET_PASSWD                "ghiasrvghg" // Contraseña del la red wifi

// MQTT
#define MQTT_SERVER_IP            "broker.emqx.io" // definir broker al que se va a conectar
#define MQTT_SERVER_PORT           1883 // puerto del broker por defecto
//#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
//#define MQTT_PASSWORD             "UPV2024"
#define TOPIC_SUB_LCD               "giirob/pr2/devices/input/lcd"   // topic para leer el texto que le llega a la lcd
#define TOPIC_SUB_LED               "giirob/pr2/devices/input/led"   // topic para leer el texto que le llega a los leds
#define TOPIC_SUB_LED_SALIDA         "giirob/pr2/devices/input/led2"  // topic para leer el texto que le llega a el led de salida
#define TOPIC_PUB                   "giirob/pr2/devices/output"      //  topic para enviar los datos de la ldr

// MENSAJES

#define ENVIAR_BOTON_PULSADO        "PARAR" // Mensaje que se envia para parar la estación en RoboDK
#define ENVIAR_BOTON_LIBRE          "INICIAR" // Mensaje que se envia para iniciar la estación en RoboDK

#define TIEMPO_ESPERA_LED           1000 // Periodo de actualización de la tarea LED
#define TIEMPO_ESPERA_LCD           1000 // Periodo de actualización de la LCD
#define TIEMPO_ESPERA_TX            1000 // Periodo de transmisión MQTT
#define TIEMPO_ESPERA_LUZ           1000 // Periodo de lectura del sensor LDR
#define TIEMPO_PAUSA                1000 // Tiempo de pausa de las tareas
#define TIEMPO_ESPERA_ULTRA         1000 // Periodo de lectura del ultrasonidos

#define TAREA_LCD_TASK_PRIORITY     1 // Prioridades de las tareas
#define TAREA_LED_TASK_PRIORITY     1
#define TAREA_LED_SALIDA_TASK_PRIORITY 1
#define TAREA_LUZ_TASK_PRIORITY     1
#define TAREA_TX_TASK_PRIORITY      1
#define TAREA_ULTRA_TASK_PRIORITY   1

// IO
#define PIN_LED_1               4 // Pin LED de cinta de entrada verde
#define PIN_LED_2               5 // Pin LED de cinta de entrada rojo
#define PIN_LED_SALIDA_3               16 // Pin LED de cinta de salida verde
#define PIN_LED_SALIDA_4               17 // Pin LED de cinta de salida rojo
#define PIN_BOTON             18 // Pin del botón de interrupción de las tareas
#define PIN_LUZ               10 // Pin de la LDR
#define PIN_SDA               21 // Pin SDA de la pantalla LCD
#define PIN_SCL               20 // Pin SCL de la pantalla LCD
#define BUFFER_SIZE           10 // Tamaño del buffer
#define PIN_ECHO              19 // Pin ECHO del sensor de ultrasonidos
#define PIN_TRIGGER           13 // Pin TRIGGER del sensor de ultrasonidos

extern TaskHandle_t lcd_task_handle;
extern TaskHandle_t led_task_handle;
extern TaskHandle_t led_salida_task_handle;
extern TaskHandle_t tx_task_handle;
extern TaskHandle_t luz_task_handle;
extern TaskHandle_t ultra_task_handle;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Creación de la pantalla LCD

struct Buffer_String {
  String buffer[BUFFER_SIZE];
  uint32_t in = 0; // Posición de escritura
  uint32_t out = 0; // Posición de lectura
  uint32_t count = 0; // Número de elementos almacenados
  portMUX_TYPE mux; // Mutex para proteger los accesos concurrentes
};

struct TaskBuffers {
  Buffer_String * buffer_TX; // Buffer para transmitir datos MQTT
  Buffer_String * buffer_ULTRA; // Buffer para transmitir datos del ultrasonidos
};

TaskBuffers buffers_mqtt; // Estructura de buffers para la transmisión en MQTT

Buffer_String buffer_LED; // Buffer para el LED de cinta entrada
Buffer_String buffer_LED_SALIDA; // Buffer para el LED de cinta entrada
Buffer_String buffer_LCD; // Buffer para la LCD
Buffer_String buffer_TX; // Buffer para la transmisión en MQTT
Buffer_String buffer_ULTRA; // Buffer para la transmisión en MQTT

extern volatile bool boton_stop; // Variable para almacenar el estado del pulsador
extern volatile bool boton_flag; // Variable para detectar solo una pulsación
extern volatile bool boton_state_before; // Variable para determinar si se ha detectado un cambio de estado en el pulsador
