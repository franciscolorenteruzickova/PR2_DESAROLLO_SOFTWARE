# Type help("robodk.robolink") or help("robodk.robomath") for more information
# Press F5 to run the script
# Documentation: https://robodk.com/doc/en/RoboDK-API.html
# Reference:     https://robodk.com/doc/en/PythonAPI/robodk.html
# Note: It is not required to keep a copy of this file, your Python script is saved with your RDK project

from robodk import robolink

RDK = robolink.Robolink()

MECHANISM_NAME = 'Conveyor Out'

mechanism = RDK.Item(MECHANISM_NAME, robolink.ITEM_TYPE_ROBOT)

# =========================
# 1) reset parámetro
# =========================
RDK.setParam('cinta_salida', 0)
RDK.setParam('group_counter_incorrectos', 0)
RDK.setParam('salida_group_id', 0)
RDK.setParam('salida_groups_on_belt', 0)
RDK.setParam('salida_first_group_id', 0)
RDK.setParam('led_salida', 1)

# =========================
# 2) reset posición física del conveyor
# =========================
if mechanism.Valid():
    mechanism.setJoints([0])

print("Conveyor reseteado a posición inicial")
