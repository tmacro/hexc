import hexc

center = hexc.Hex(0,0,0)

for x in center.around(radius = 10):
    print(x)

for x in center.around(radius = 10, inclusive = True):
    print(x)