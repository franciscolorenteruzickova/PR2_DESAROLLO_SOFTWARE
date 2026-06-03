import sys
sys.path.append(r"C:\Users\Fran\Downloads\ProyectoPrueba")
import MqttListener as mqtt_listener
from ConexionBasedeDatos import crear_proceso_y_palillo
from robodk import robolink
from robodk.robomath import transl
from robodk.robodialogs import mbox
import random
import time

RDK = robolink.Robolink()

FRAME = RDK.Item('StickConveyorRef', robolink.ITEM_TYPE_FRAME)
stick_ok = RDK.Item('StickCorrecto', robolink.ITEM_TYPE_OBJECT)
stick_no = RDK.Item('StickIncorrecto', robolink.ITEM_TYPE_OBJECT)
led_verde = RDK.Item('Light (Green)2', robolink.ITEM_TYPE_OBJECT)
led_rojo = RDK.Item('Light (Red)2', robolink.ITEM_TYPE_OBJECT)

mqtt_listener.conectar_mqtt()
mqtt_listener.iniciar_loop_mqtt()
time.sleep(1)

nombre = mbox("Introduce el nombre:", entry="")

mqtt_listener.publicar_lcd(nombre)

STEP_GROUP = 400
OFFSET_Y = 100
MAX_GROUPS = 5

def esperar_stop():
        while int(RDK.getParam("stop") or 0) == 1:
                time.sleep(0.1)


def led_verde_on():
        global ultimo_led_entrada
        
        led_verde.setVisible(True)
        led_rojo.setVisible(False)
        if ultimo_led_entrada != 1:
                mqtt_listener.encender_led()
                print("MQTT entrada: ENCENDER_LED")
                ultimo_led_entrada = 1
        
        RDK.Update()

def led_rojo_on():
        global ultimo_led_entrada
        
        led_verde.setVisible(False)
        led_rojo.setVisible(True)
        if ultimo_led_entrada != 0:
                mqtt_listener.apagar_led()
                print("MQTT entrada: APAGAR_LED")
                ultimo_led_entrada = 0
        RDK.Update()

def move_cinta(step=STEP_GROUP):
    mechanism = RDK.Item('StickConveyor', robolink.ITEM_TYPE_ROBOT)
    if mechanism.Valid():
        j = mechanism.Joints()
        current = j[0, 0]
        mechanism.setJoints([current + float(step)])

RDK.Render(True)

ultimo_led_salida = None
ultimo_led_entrada = None

conveyor_position = float(RDK.getParam('conv_pos') or 0)
fin_notificado = False

while int(RDK.getParam("salir") or 0) == 0:

    esperar_stop()
    
    groups_created = int(RDK.getParam('groups_created') or 0)
    groups_to_process = int(RDK.getParam('groups_to_process') or 0)

    led_salida_param = RDK.getParam('led_salida')
                     
    if led_salida_param is None or led_salida_param == '':
            led_salida = 1
    else:
            led_salida = int(led_salida_param)

    if led_salida != ultimo_led_salida:

            if led_salida == 1:
                mqtt_listener.encender_led_2()
                print("MQTT salida: ENCENDER_LED")
            else:
                mqtt_listener.apagar_led_2()
                print("MQTT salida: APAGAR_LED")

            ultimo_led_salida = led_salida

    pending = groups_created - groups_to_process

    if pending < MAX_GROUPS:
            
        fin_notificado = False

        group_id = groups_created

        print("Creando grupo", group_id)

        RDK.Render(False)

        for i in range(3):

            selected = random.choice([stick_ok, stick_ok, stick_ok, stick_ok, stick_no])

            selected.Copy()
            obj = RDK.Paste()

            if not obj.Valid():
                print("Paste falló")
                continue

            obj.setParent(FRAME)

            y_offset = (i - 1) * OFFSET_Y

            obj.setPose(transl(conveyor_position,y_offset,0))

            es_correcto = selected.Name() == stick_ok.Name()

            obj.setName(
                f"{'correcto' if es_correcto else 'incorrecto'}_{group_id}_{i}"
            )
            if es_correcto:
                    crear_proceso_y_palillo()

        conveyor_position += STEP_GROUP
        RDK.setParam('conv_pos', str(conveyor_position))

        RDK.Render(True)
        
        led_rojo_on()
        time.sleep(0.1)

        esperar_stop()

        RDK.setParam('cinta_entrada', 1)
        
        try:
            move_cinta()
            time.sleep(0.2)
        finally:
            RDK.setParam('cinta_entrada', 0)

        led_verde_on()
        
        groups_created += 1

        RDK.setParam('groups_created', groups_created)

        RDK.Render(True)
        RDK.Update()

        time.sleep(0.5)

    else:
            if not fin_notificado:
                    RDK.Render(True)
                    led_verde_on()
                    fin_notificado = True
            time.sleep(0.1)
