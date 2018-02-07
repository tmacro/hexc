import hexc
import time

center = hexc.AbstractHex(0,0,0)

g =  hexc.HexRangeGenerator(center, 100)

for x in center.within(3):
    print(x)