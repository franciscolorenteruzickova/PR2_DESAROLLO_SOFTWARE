#include <LiquidCrystal_I2C.h>
// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "08" //"giirobpr2_00"

// WIFI
#define NET_SSID                  "UPV-PSK"
#define NET_PASSWD                "giirob-pr2-2023"

// MQTT
#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
#define MQTT_SERVER_PORT           1883
#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
#define MQTT_PASSWORD             "UPV2024"

#define TOPIC_SUB_LCD               "giirob/pr2/devices/input/lcd"   // topic para leer el texto que le llega a la lcd
#define TOPIC_SUB_LED               "giirob/pr2/devices/input/led"   // topic para leer el texto que le llega a los leds
#define TOPIC_PUB                   "giirob/pr2/devices/output"      //  topic para enviar los datos de la ldr

// MENSAJES

#define ENVIAR_BOTON_PULSADO        "PARAR"
#define ENVIAR_BOTON_LIBRE          "INICIAR"

#define TIEMPO_ESPERA_LED           1000
#define TIEMPO_ESPERA_LCD           1000 
#define TIEMPO_ESPERA_TX            1000
#define TIEMPO_ESPERA_LUZ           1000
#define TIEMPO_PAUSA                1000
#define TIEMPO_ESPERA_ULTRA         1000

#define TAREA_LCD_TASK_PRIORITY     1
#define TAREA_LED_TASK_PRIORITY     1
#define TAREA_LUZ_TASK_PRIORITY     1
#define TAREA_TX_TASK_PRIORITY      1
#define TAREA_ULTRA_TASK_PRIORITY   1

// IO
#define PIN_LED_1               4
#define PIN_LED_2               5
#define PIN_LED_3               16
#define PIN_LED_4               17
#define PIN_BOTON             18
#define PIN_LUZ               10
#define PIN_SDA               21
#define PIN_SCL               20
#define BUFFER_SIZE           10
#define PIN_ECHO              19
#define PIN_TRIGGER           13

extern TaskHandle_t lcd_task_handle;
extern TaskHandle_t led_task_handle;
extern TaskHandle_t tx_task_handle;
extern TaskHandle_t luz_task_handle;
extern TaskHandle_t ultra_task_handle;

LiquidCrystal_I2C lcd(0x27, 16, 2);

struct Buffer_String {
  String buffer[BUFFER_SIZE];
  uint32_t in = 0;
  uint32_t out = 0;
  uint32_t count = 0;
  portMUX_TYPE mux;
};

struct TaskBuffers {
  Buffer_String * buffer_TX;
  Buffer_String * buffer_ULTRA;
};

TaskBuffers buffers_mqtt;

Buffer_String buffer_LED;
Buffer_String buffer_LCD;
Buffer_String buffer_TX;
Buffer_String buffer_ULTRA;

extern volatile bool boton_stop;
extern volatile bool boton_flag;
extern volatile bool boton_state_before;
