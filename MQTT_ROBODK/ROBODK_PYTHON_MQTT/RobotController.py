from robodk import robolink # RoboDK API
from robodk import robomath # Robot toolbox
import MqttListener as mqtt_listener
RDK = robolink.Robolink()

def handle_message(mqttc, topic, payload):
    if topic == "giirob/pr2/devices/output":
        print(f"Payload recibido: {payload}")
        if payload == "POCA_LUZ":
            print ("Ejecutando acción para poca luz...")
            mqtt_listener.publicar_lcd("Poca luz detectada.")

        elif payload == "VACIO":
            print ("Reemplazar contenedor...")
            mqtt_listener.publicar_lcd("Contenedor vacío.")
        
        elif payload == "LLENO":
            print ("Contenedor lleno...")
            mqtt_listener.publicar_lcd("Contenedor lleno.")

        elif payload == "PARAR":
            print ("Iniciando parada de emergencia...")
            RDK.setParam("STOP", 1)   # Detiene la estación técnica

        elif payload == "INICIAR":
            print ("Reanudando operaciones...")
            RDK.setParam("STOP", 0)   # Reanuda la estación técnica
        else:
            print ("Comando no reconocido.")
            mqtt_listener.publicar_lcd("Comando no reconocido.")


