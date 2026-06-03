from robodk import robolink
from robodk.robomath import transl, rotz
import time

RDK = robolink.Robolink()

FRAME = RDK.Item('MainConveyorRef', robolink.ITEM_TYPE_FRAME)

rack_base = RDK.Item('AssemblyRack', robolink.ITEM_TYPE_OBJECT)

STEP_GROUP = -970

MAX_BANDEJAS = 4

RDK.setParam('cinta_central', 0)

def esperar_stop():
    while int(RDK.getParam("stop") or 0) == 1:
        time.sleep(0.1)


def move_cinta_central(step=STEP_GROUP):

    esperar_stop()
    
    mechanism = RDK.Item('MainConveyor', robolink.ITEM_TYPE_ROBOT)

    if mechanism.Valid():
        j = mechanism.Joints()
        current = j[0, 0]
        mechanism.setJoints([current + float(step)])


def borrar_bandeja(group_id):

    esperar_stop()
    
    nombres = [
        f"bandeja_{group_id}_rack",
    ]

    for nombre in nombres:
        obj = RDK.Item(nombre)
        if obj.Valid():
            print("Eliminando:", nombre)
            obj.Delete()


def crear_bandeja(group_id, conveyor_position):

    esperar_stop()

    
    rack_base.Copy()
    rack = RDK.Paste()

    if rack.Valid():
        rack.setParent(FRAME)
        rack.setPose(transl(0, (-conveyor_position), 0))
        rack.setName(f"bandeja_{group_id}_rack")
    else:
        print("ERROR: No se pudo crear AssemblyRack")
        return False

    return True


def inicializar_bandejas():
    groups_on_belt = int(RDK.getParam('main_groups_on_belt') or 0)

    while groups_on_belt < MAX_BANDEJAS:
        esperar_stop()
        
        avanzar_y_crear_bandeja()
        groups_on_belt = int(RDK.getParam('main_groups_on_belt') or 0)

def avanzar_y_crear_bandeja():
    esperar_stop()
    
    conveyor_position = float(RDK.getParam('main_conv_pos') or 0)
    group_id = int(RDK.getParam('main_group_id') or 0)
    groups_on_belt = int(RDK.getParam('main_groups_on_belt') or 0)
    first_group_id = int(RDK.getParam('main_first_group_id') or 0)

    RDK.Render(False)

    ok = crear_bandeja(group_id, conveyor_position)

    if not ok:
        RDK.Render(True)
        return False

    conveyor_position -= STEP_GROUP
    RDK.setParam('main_conv_pos', conveyor_position)

    RDK.Render(True)

    RDK.setParam('cinta_central', 1)

    try:
        esperar_stop()
        move_cinta_central()
        time.sleep(0.5)
    finally:
        RDK.setParam('cinta_central', 0)

    group_id += 1
    groups_on_belt +=1

    RDK.setParam('main_group_id', group_id)
    RDK.setParam('main_groups_on_belt', groups_on_belt)
    RDK.setParam('main_first_group_id', first_group_id)

    RDK.Update()

    return True
