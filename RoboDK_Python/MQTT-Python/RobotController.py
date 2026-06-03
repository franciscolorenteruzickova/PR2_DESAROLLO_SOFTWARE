from robodk import robolink # RoboDK API
from robodk import robomath # Robot toolbox
import MqttListener as mqtt_listener
RDK = robolink.Robolink()

def handle_message(mqttc, topic, payload):
    if topic == "giirob/pr2/devices/output":
        print(f"Payload recibido: {payload}")
        if payload == "PARAR":
            print ("Iniciando parada de emergencia...")
            RDK.setParam("stop", 1)   # Detiene la estación técnica

        elif payload == "INICIAR":
            print ("Reanudando operaciones...")
            RDK.setParam("stop", 0)   # Reanuda la estación técnica

        elif payload == "VACIO":
            print("Contenedor de palillos vacío")
            mqtt_listener.publicar_lcd("Contenedor vacio")

        elif payload == "LLENO":
            print("Contenedor de palillos lleno")
            mqtt_listener.publicar_lcd("Contenedor lleno")

        elif payload == "POCA_LUZ":
            print("Aumentar la iluminación")
            mqtt_listener.publicar_lcd("Aumentar luz")

        elif payload == "MUCHA_LUZ":
            print("Disminuir la iluminación")
            mqtt_listener.publicar_lcd("Disminuir luz")
            
        else:
            print ("Comando no reconocido.")
            mqtt_listener.publicar_lcd("Incorrect.")


