import MqttListener as mqtt_listener
import time
import threading

mqtt_listener.conectar_mqtt()

mqtt_listener.iniciar_loop_mqtt()

time.sleep(1)  # Espera un momento para asegurarse de que la conexión MQTT esté establecida

while True:
    time.sleep(1)  # Mantén el programa en ejecución para seguir recibiendo mensajes MQTT
    mqtt_listener.encender_led()
    mqtt_listener.publicar_lcd("¡INICIO DE LA ESTACIÓN!")

    time.sleep(5)

    mqtt_listener.apagar_led()

threading.Event().wait()  # Mantén el programa en ejecución para seguir recibiendo mensajes MQTT