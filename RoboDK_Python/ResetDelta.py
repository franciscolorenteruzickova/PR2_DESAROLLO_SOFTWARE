from robodk import robolink
from robodk.robomath import transl
import time

RDK = robolink.Robolink()

robot = RDK.Item('ABB IRB 360-1/800 4D', robolink.ITEM_TYPE_ROBOT)
home = RDK.Item('HOME', robolink.ITEM_TYPE_TARGET)

robot.setSpeed(800, 600)

robot.MoveJ(home)

