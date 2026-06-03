from robodk import robolink
import re

RDK = robolink.Robolink()

# -------------------------
# RESET PARÁMETROS
# -------------------------
RDK.setParam('main_conv_pos', 0)
RDK.setParam('main_group_id', 0)
RDK.setParam('main_groups_on_belt', 0)
RDK.setParam('main_first_group_id', 0)
RDK.setParam('cinta_central', 0)
RDK.setParam('correct_group_id', 0)
RDK.setParam('correct_groups_on_belt', 0)
RDK.setParam('correct_first_group_id', 0)

# -------------------------
# BUSCAR NOMBRES A ELIMINAR
# -------------------------
nombres_borrar = []

for item in RDK.ItemList():

    try:
        name = item.Name()
    except:
        continue

    if not name:
        continue

    if re.match(r"^bandeja_\d+_rack$", name) or re.match(r"^bandeja_\d+_plantilla$", name):
        nombres_borrar.append(name)

# -------------------------
# ELIMINAR OBJETOS
# -------------------------
for name in nombres_borrar:

    obj = RDK.Item(name)

    if obj.Valid():
        print("Eliminando:", name)
        obj.Delete()

# -------------------------
# DEVOLVER CINTA A ORIGEN
# -------------------------
mechanism = RDK.Item('MainConveyor', robolink.ITEM_TYPE_ROBOT)

if mechanism.Valid():
    joints = mechanism.Joints()
    joints[0, 0] = 0
    mechanism.setJoints(joints)

print("RESET CINTA CENTRAL COMPLETADO")
