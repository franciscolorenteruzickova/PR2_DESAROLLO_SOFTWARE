import sys
sys.path.append(r"C:\Users\Fran\Downloads\ProyectoPrueba")
from MainConveyor import avanzar_y_crear_bandeja, inicializar_bandejas, borrar_bandeja, MAX_BANDEJAS
from robodk import robolink
from robodk.robomath import transl
import time

RDK = robolink.Robolink()

robot = RDK.Item('ABB IRB 360-1/800 4D', robolink.ITEM_TYPE_ROBOT)
ref_salida_mal = RDK.Item('DeleteStickConveyorRef', robolink.ITEM_TYPE_FRAME)
ref_salida_bien = RDK.Item('MainConveyorRef', robolink.ITEM_TYPE_FRAME)
tool = RDK.Item('RobotiQ EPick Vacuum Gripper (1 Cup)')
home = RDK.Item('HOME', robolink.ITEM_TYPE_TARGET)

robot.setSpeed(800, 600)

Z_APPROACH = transl(0, 0, -75)


pick = []
for i in range(1, 4):
    t = RDK.Item(f'Pick_{i}', robolink.ITEM_TYPE_TARGET)
    if not t.Valid():
        print(f"ERROR: Pick_{i} no existe")
    pick.append(t)

correct = [RDK.Item(f'Correct_{i}') for i in range(1, 10)]

incorrect = [RDK.Item(f'Incorrect_{i}') for i in range(1, 5)]

# -------------------------
# INDICES
# -------------------------
correct_index = 0
incorrect_index = 0
processed_correct = 0

def esperar_stop():
    while int(RDK.getParam("stop") or 0) == 1:
        time.sleep(0.1)

inicializar_bandejas()

def esperar_cintas_paradas():
    while True:
        esperar_stop()
        
        cinta_entrada = int(RDK.getParam('cinta_entrada') or 0)
        cinta_salida = int(RDK.getParam('cinta_salida') or 0)
        cinta_central = int(RDK.getParam('cinta_central') or 0)

        if cinta_entrada == 0 and cinta_salida == 0 and cinta_central == 0:
            time.sleep(0.2)
            break

        time.sleep(0.1)


def borrar_grupo_correctos(group_id):
    for i in range(9):
        obj = RDK.Item(f"correct_out_{group_id}_{i}")
        if obj.Valid():
            print("Eliminando correcto:", obj.Name())
            obj.Delete()


def esperar_serigrafia_terminada():
    while True:
        esperar_stop()
        
        serigrafia1 = int(RDK.getParam('serigrafia1') or 0)
        serigrafia = int(RDK.getParam('serigrafia') or 0)

        if serigrafia1 == 0 and serigrafia == 0:
            break
        
        time.sleep(0.1)


# -------------------------
# HELPERS
# -------------------------
def pre(target):
    esperar_stop()
    esperar_cintas_paradas()
    robot.MoveJ(target.Pose() * Z_APPROACH)

def pre_place_incorrecto(target):
    esperar_stop()
    esperar_cintas_paradas()
    robot.MoveJ(target.Pose() * Z_APPROACH)

def go_place_incorrecto(target):
    esperar_stop()
    esperar_cintas_paradas()
    robot.MoveL(target.Pose())

def go(target):
    esperar_stop()
    esperar_cintas_paradas()
    robot.MoveL(target.Pose())


# -------------------------
# LOOP PRINCIPAL
# -------------------------
while int(RDK.getParam("salir") or 0) == 0:

    esperar_stop()
    
    groups_to_process = int(RDK.getParam('groups_to_process') or 0)
    groups_created = int(RDK.getParam('groups_created') or 0)
    correct_group_id = int(RDK.getParam('correct_group_id') or 0)

    esperar_cintas_paradas()

    if groups_to_process >= groups_created:
        time.sleep(0.1)
        continue

    group_id = groups_to_process

    all_items = RDK.ItemList()

    sticks = []

    for item in all_items:
        name = item.Name()

        if not name:
            continue

        parts = name.split("_")

        if len(parts) != 3:
            continue

        tipo, grupo, indice = parts

        if tipo not in ["correcto", "incorrecto"]:
            continue

        if not grupo.isdigit() or not indice.isdigit():
            continue

        if int(grupo) != group_id:
            continue

        sticks.append(item)

    if len(sticks) < 3:
        time.sleep(0.1)
        continue

    sticks.sort(key=lambda x: int(x.Name().split("_")[2]))

    

    # CICLO 3 PALILLOS
    for i in range(3):
        esperar_stop()
        
        esperar_cintas_paradas()

        stick = sticks[i]

        tipo = stick.Name().split("_")[0]

        esperar_stop()
        # PICK
        pre(pick[i])
        go(pick[i])

        tool.AttachClosest()
        
        pre(pick[i])

        if tipo == "correcto":
            pre(correct[correct_index])
            go(correct[correct_index])

            tool.DetachAll(ref_salida_bien)

            stick.setName(f"correct_out_{correct_group_id}_{correct_index}")
            
            pre(correct[correct_index])

            correct_index += 1
            processed_correct +=1
            
            if processed_correct >= 9:
                
                print("GRUPO DE 9 CORRECTOS COMPLETADO")

                esperar_stop()
                
                robot.MoveJ(home)
                time.sleep(0.3)

                esperar_serigrafia_terminada()
                
                esperar_stop()

                correct_groups_on_belt = int(RDK.getParam('correct_groups_on_belt') or 0)
                correct_first_group_id = int(RDK.getParam('correct_first_group_id') or 0)
                groups_on_belt = int(RDK.getParam('main_groups_on_belt') or 0)
                first_group_id = int(RDK.getParam('main_first_group_id') or 0)

                if correct_groups_on_belt >= 2:
                    
                    borrar_grupo_correctos(correct_first_group_id)
                    
                    correct_first_group_id += 1
                    correct_groups_on_belt -= 1

                if groups_on_belt >= MAX_BANDEJAS:
                    borrar_bandeja(first_group_id)
                    first_group_id += 1
                    groups_on_belt -= 1

                RDK.setParam('main_groups_on_belt', groups_on_belt)
                RDK.setParam('main_first_group_id', first_group_id)

                avanzar_y_crear_bandeja()
                
                correct_groups_on_belt += 1

                RDK.setParam('correct_groups_on_belt', correct_groups_on_belt)
                RDK.setParam('correct_first_group_id', correct_first_group_id)

                correct_group_id += 1
                RDK.setParam('correct_group_id', correct_group_id)

                RDK.setParam("serigrafia", 1)
                RDK.setParam("serigrafia1", 1)

                processed_correct = 0
                correct_index = 0

                print("SERIGRAFÍA")

        elif tipo == "incorrecto":
            esperar_stop()
            
            robot.MoveJ(home)
            pre_place_incorrecto(incorrect[incorrect_index])
            go_place_incorrecto(incorrect[incorrect_index])

            tool.DetachAll(ref_salida_mal)
            
            salida_group_id = int(RDK.getParam('salida_group_id') or 0)
            stick.setName(f"salida_{salida_group_id}_{incorrect_index}")
            pre_place_incorrecto(incorrect[incorrect_index])
            esperar_stop()
            robot.MoveJ(home)

            incorrect_index += 1


            c = int(RDK.getParam('group_counter_incorrectos') or 0)
            c += 1
            RDK.setParam('group_counter_incorrectos', c)

            if incorrect_index >= 4:

                incorrect_index = 0
                salida_group_id += 1
                RDK.setParam('salida_group_id', salida_group_id)

    # -------------------------
    # FIN DE GRUPO
    # -------------------------
    groups_to_process += 1
    RDK.setParam('groups_to_process', groups_to_process)

    time.sleep(0.1)
