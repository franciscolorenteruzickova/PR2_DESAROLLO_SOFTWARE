from robodk import robolink
import re

RDK = robolink.Robolink()

all_items = RDK.ItemList()

for item in all_items:

    name = item.Name()

    if not name:
        continue

    # correcto_0_1 / incorrecto_3_2
    patron_entrada = r'^(correcto|incorrecto)_\d+_\d+$'

    # salida_0_1
    patron_salida = r'^salida_\d+_\d+$'

    patron_correcto_salida = r'^correct_out_\d+_\d+$'

    if re.match(patron_entrada, name) or re.match(patron_salida, name) or re.match(patron_correcto_salida, name):

        print("Eliminando:", name)

        item.Delete()

print("Limpieza completada")
