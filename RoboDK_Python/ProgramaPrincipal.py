from robodk import robolink
import time

RDK = robolink.Robolink()

print("CINTA ENTRADA")
RDK.RunProgram("StickConveyorMove")

# ESPERAR SOLO AL INICIO
while int(RDK.getParam("salir") or 0) == 0:

    groups_created = int(RDK.getParam("groups_created") or 0)

    print("Esperando grupos iniciales:", groups_created)

    if groups_created >= 5:
        break

    time.sleep(0.2)

print("CINTA SALIDA")
RDK.RunProgram("StickConveyorDeleteMove")

time.sleep(2)

print("SERIGRAFIA LASER")
RDK.RunProgram("SerigrafiaUR3e2")

time.sleep(1)

print("SERIGRAFIA LASER 1")
RDK.RunProgram("SerigrafiaUR3e1")

time.sleep(1)

print("BOTON STOP")
RDK.RunProgram("StopBoton")

time.sleep(2)

print("DELTA")
RDK.RunProgram("Delta")

print("SISTEMA INICIADO")
