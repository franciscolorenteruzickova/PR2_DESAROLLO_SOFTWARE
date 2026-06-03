from robodk import robolink
import time

RDK = robolink.Robolink()

boton = RDK.Item('BotonStop')

if not boton.Valid():
    raise Exception("No existe BotonStop")

while int(RDK.getParam("salir") or 0) == 0:

    seleccion = RDK.Selection()

    if boton in seleccion:

        estado_actual = int(RDK.getParam('stop') or 0)

        if estado_actual == 0:
            RDK.setParam("stop", 1)
            print("STOP ACTIVADO")
        else:
            RDK.setParam("stop", 0)
            print("STOP DESACTIVADO")

        # Deseleccionar para que no se repita en bucle
        RDK.setSelection([])

    time.sleep(0.1)
