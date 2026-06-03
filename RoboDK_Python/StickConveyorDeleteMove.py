import sys
sys.path.insert(0, r"C:\Users\Fran\Downloads\ProyectoPrueba")
from robodk import robolink
from robodk.robomath import transl
import random
import time

RDK = robolink.Robolink()

FRAME = RDK.Item('DeleteStickConveyorRef', robolink.ITEM_TYPE_FRAME)
led_verde = RDK.Item('Light (Green)1', robolink.ITEM_TYPE_OBJECT)
led_rojo = RDK.Item('Light (Red)1', robolink.ITEM_TYPE_OBJECT)


time.sleep(1)

STEP_GROUP = 400
MAX_GROUPS = 4
MAX_GROUPS_SALIDA = 4

def esperar_stop():
        while int(RDK.getParam("stop") or 0) == 1:
                time.sleep(0.1)

def led_verde_on():
        led_verde.setVisible(True)
        led_rojo.setVisible(False)
        RDK.setParam('led_salida', 1)
        RDK.Update()

def led_rojo_on():
        led_verde.setVisible(False)
        led_rojo.setVisible(True)
        RDK.setParam('led_salida', 0)
        RDK.Update()


def move_cinta(step=STEP_GROUP):
    mechanism = RDK.Item('Conveyor Out', robolink.ITEM_TYPE_ROBOT)
    if mechanism.Valid():
        j = mechanism.Joints()
        current = j[0, 0]
        mechanism.setJoints([current + float(step)])

def borrar_grupo_salida(group_id):
        for i in range(4):
                obj = RDK.Item(f"salida_{group_id}_{i}")
                if obj.Valid():
                        obj.Delete()

RDK.Render(True)

led_verde_on()

while int(RDK.getParam("salir") or 0) == 0:

    esperar_stop()
    
    counter_stick = int(RDK.getParam('group_counter_incorrectos') or 0)

    print("group_counter_incorrectos =", counter_stick)

    if counter_stick  >= MAX_GROUPS:

        esperar_stop()

        led_rojo_on()
        RDK.setParam('cinta_salida', 1)

        try:
                move_cinta()
                time.sleep(0.2)
        finally:
                RDK.setParam('cinta_salida', 0)

        # REINICIAR CONTADOR
        counter_stick = int(RDK.getParam('group_counter_incorrectos') or 0)
        RDK.setParam('group_counter_incorrectos', max(0, counter_stick - 4))

        groups_on_belt = int(RDK.getParam('salida_groups_on_belt') or 0)
        first_group_id = int(RDK.getParam('salida_first_group_id') or 0)

        groups_on_belt +=1

        if groups_on_belt > MAX_GROUPS_SALIDA:
                borrar_grupo_salida(first_group_id)
                first_group_id += 1
                groups_on_belt -= 1
        RDK.setParam('salida_groups_on_belt', groups_on_belt)
        RDK.setParam('salida_first_group_id', first_group_id)

        led_verde_on()

    time.sleep(0.1)
