// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "08" //"giirobpr2_00"

// WIFI
#define NET_SSID                  "iPhone"
#define NET_PASSWD                "ghiasrvghg"

// MQTT
#define MQTT_SERVER_IP            "broker.emqx.io"
#define MQTT_SERVER_PORT           1883
//#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
//#define MQTT_PASSWORD             "UPV2024"

#define TOPIC_SUB_LCD               "giirob/pr2/devices/input/lcd"    // TODO: topic ejemplo para ejercicio inicial de saludo de los dispositivos
#define TOPIC_SUB_LED               "giirob/pr2/devices/input/led"
#define TOPIC_PUB                   "giirob/pr2/devices/output"


// IO
#define PIN_LED_1               4
#define PIN_LED_2               5
#define PIN_LED_3               6
#define PIN_LED_4               7
#define PIN_BOTON             15
#define PIN_LUZ               10
#define PIN_SDA               21
#define PIN_SCL               20
#define BUFFER_SIZE           10

struct Buffer_String {
  String buffer[BUFFER_SIZE];
  int in = 0;
  int out = 0;
  int count = 0;
  portMUX_TYPE mux;
};
