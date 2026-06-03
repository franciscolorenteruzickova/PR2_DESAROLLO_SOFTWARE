from robodk import robolink, robomath
import time

RDK = robolink.Robolink()
RDK.setRunMode(robolink.RUNMODE_SIMULATE)
RDK.setSimulationSpeed(1)

robot = RDK.Item('UR3e_1')
tool = RDK.Item('Laser1')

if not robot.Valid():
    raise Exception("No existe el robot UR3e_1")

# Suavizado
robot.setRounding(20)

robot.setTool(tool)

# Offset de aproximación
Z_APPROACH = robomath.transl(0, 0, -60)

palillos = [
    (["T_9_1", "T_9_2", "T_9_3", "T_9_4", "T_9_5"]),
    (["T_8_1", "T_8_2", "T_8_3", "T_8_4", "T_8_5"]),
    (["T_7_1", "T_7_2", "T_7_3", "T_7_4", "T_7_5"]),
    (["T_6_1", "T_6_2", "T_6_3", "T_6_4", "T_6_5"]),
    (["T_5_1", "T_5_2", "T_5_3", "T_5_4", "T_5_5"]),
]

def esperar_stop():
    while int(RDK.getParam("stop") or 0) == 1:
        time.sleep(0.1)


while int(RDK.getParam("salir") or 0) == 0:

    esperar_stop()
    # Esperar activación
    if int(RDK.getParam("serigrafia1") or 0) == 0:
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

        # -------------------------
        # MOVIMIENTO RÁPIDO
        # -------------------------
        robot.setSpeed(80, 40)

        esperar_stop()

        if i == 0:
            robot.MoveJ(pre_inicio)
        else:
            robot.setSpeed(60, 30)
            robot.MoveJ(pre_inicio)

        # -------------------------
        # MOVIMIENTO SUAVE
        # -------------------------
        robot.setSpeed(30, 15)

        esperar_stop()

        robot.MoveL(targets[0])

        RDK.RunProgram("WeldOn(ON)")
        
        for t in targets[1:]:
            esperar_stop()
            robot.MoveL(t)

        RDK.RunProgram("WeldOn(OFF)")
        
        esperar_stop()
        robot.MoveL(pre_final)

    # -------------------------
    # VOLVER A INICIO
    # -------------------------
    inicio = RDK.Item('T_9_1')
    preinicio = inicio.Pose() * Z_APPROACH

    if not inicio.Valid():
        raise Exception("No existe T_9_1")

    robot.setSpeed(60, 30)

    esperar_stop()
    
    robot.MoveJ(preinicio)

    # Reset del parámetro
    RDK.setParam("serigrafia1", 0)

    print("FIN SERIGRAFIA")
