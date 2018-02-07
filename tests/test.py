import hexc
import time

center = hexc.AbstractHex(0,0,0)

# for x in center.aroud
start = time.time()
z = 0
# for x in range(500):
#     for y in center.around(radius = x):
#         z +=1
# print(z)
for hex in hexc.HexRangeGenerator(center, 5):
    z += 1
    # print(z)
print(time.time() - start)