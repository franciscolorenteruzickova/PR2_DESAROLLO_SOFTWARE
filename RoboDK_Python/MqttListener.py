import paho.mqtt.client as mqtt
import RobotController as rc
from robodk import robolink
from robodk import robomath


RDK = robolink.Robolink()

broker="broker.emqx.io"
port=1883
base_topic_input="giirob/pr2/devices/input"
base_topic_output="giirob/pr2/devices/output"
station_input_topic_led=base_topic_input + "/led"
station_input_topic_led_2=base_topic_input + "/led2"
station_input_topic_lcd=base_topic_input + "/lcd"
station_output_topic=base_topic_output

def on_message(mqttc, obj, msg):
 payload = msg.payload.decode('utf-8')
 topic = msg.topic
 qos = msg.qos
 rc.handle_message(mqttc, topic, payload)

mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_message = on_message

def conectar_mqtt():
    mqttc.connect(broker, port, 60)
    mqttc.subscribe(station_output_topic, 0)
    print(f"INICIO DE LA ESTACIÓN")

def iniciar_loop_mqtt():
    mqttc.loop_start()

def encender_led():
    mqttc.publish(station_input_topic_led, "ENCENDER_LED")

def apagar_led():
    mqttc.publish(station_input_topic_led, "APAGAR_LED")

def encender_led_2():
    mqttc.publish(station_input_topic_led_2, "ENCENDER_LED")

def apagar_led_2():
    mqttc.publish(station_input_topic_led_2, "APAGAR_LED")

def publicar_lcd(mensaje):
    mqttc.publish(station_input_topic_lcd, mensaje)
