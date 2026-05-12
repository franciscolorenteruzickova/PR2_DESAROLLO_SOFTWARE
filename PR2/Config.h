// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
//#define DEVICE_ESP_ID             "54CE0361421"   // ESP32 ID
#define DEVICE_GIIROB_PR2_ID      "08" //"giirobpr2_00"

// WIFI
#define NET_SSID                  "Tu Africano Favorito"
#define NET_PASSWD                "20062006"

// MQTT
#define MQTT_SERVER_IP            "broker.emqx.io"
#define MQTT_SERVER_PORT           1883
//#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
//#define MQTT_PASSWORD             "UPV2024"

#define TOPIC_SUB_BALIZAUNO               "giirob/pr2/devices/balizauno"    // TODO: topic ejemplo para ejercicio inicial de saludo de los dispositivos
#define TOPIC_SUB_BALIZADOS               "giirob/pr2/devices/balizados"
#define TOPIC_PUB                   "giirob/pr2/estadosimulacion"
