from robodk import robolink

RDK = robolink.Robolink()

print("RESET GENERAL")

RDK.setParam('salir', 0)
RDK.setParam('stop', 0)
RDK.RunProgram("ResetDelta", True)
RDK.RunProgram("ResetStickConveyor", True)
RDK.RunProgram("ResetStickConveyorDeleteMove", True)
RDK.RunProgram("ResetMainConveyor", True)
RDK.RunProgram("ResetSoldar", True)
RDK.RunProgram("ResetPalillo")

print("RESET COMPLETADO")
