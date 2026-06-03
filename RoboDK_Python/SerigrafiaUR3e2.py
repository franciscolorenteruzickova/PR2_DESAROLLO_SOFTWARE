from robodk import robolink, robomath
import time

RDK = robolink.Robolink()
RDK.setRunMode(robolink.RUNMODE_SIMULATE)
RDK.setSimulationSpeed(1)

robot = RDK.Item('UR3e_2')
tool = RDK.Item('Laser2')

if not robot.Valid():
    raise Exception("No existe el robot UR3e_2")

if not tool.Valid():
    raise Exception("No hay tool activa")

robot.setRounding(20)

Z_APPROACH = robomath.transl(0, 0, -60)

palillos = [
    (["T_1_1", "T_1_2", "T_1_3", "T_1_4", "T_1_5"]),
    (["T_2_1", "T_2_2", "T_2_3", "T_2_4", "T_2_5"]),
    (["T_3_1", "T_3_2", "T_3_3", "T_3_4", "T_3_5"]),
    (["T_4_1", "T_4_2", "T_4_3", "T_4_4", "T_4_5"])
]

def esperar_stop():
    while int(RDK.getParam("stop") or 0) == 1:
        time.sleep(0.1)


while int(RDK.getParam("salir") or 0) == 0:

    esperar_stop()

    # Esperar hasta que serigrafia sea 1
    if int(RDK.getParam("serigrafia") or 0) == 0:
        time.sleep(0.1)
        continue

    print("SERIGRAFIA ACTIVADA")

    for i, (target_names) in enumerate(palillos):

        esperar_stop()

        targets = [RDK.Item(name) for name in target_names]

        for t in targets:
            if not t.Valid():
                raise Exception("Falta target: " + t.Name())

        pre_inicio = targets[0].Pose() * Z_APPROACH
        pre_final = targets[-1].Pose() * Z_APPROACH

        robot.setSpeed(80, 40)

        esperar_stop()

        if i == 0:
            robot.MoveJ(pre_inicio)
        else:
            robot.setSpeed(60, 30)
            robot.MoveJ(pre_inicio)

        robot.setSpeed(30, 15)

        esperar_stop()

        robot.MoveL(targets[0])

        for t in targets[1:]:
            esperar_stop()
            robot.MoveL(t)

        esperar_stop()
        robot.MoveL(pre_final)

    inicio = RDK.Item('T_1_1')
    inicio1 = RDK.Item('T_3_5')

    preinicio = inicio.Pose() * Z_APPROACH
    preinicio1 = inicio1.Pose() * Z_APPROACH

    robot.setSpeed(60, 30)
    esperar_stop()
    robot.MoveJ(preinicio1)
    esperar_stop()
    robot.MoveJ(preinicio)

    RDK.setParam("serigrafia", 0)

    print("FIN SERIGRAFIA")
