'''
This module contains known good implementations of the funtions and methods for testing.
Code adapted from https://www.redblobgames.com/grids/hexagons/codegen/output/lib.py
'''

import collections

RefHex = collections.namedtuple("RefHex", ["q", "r", "s"])


def add(a, b):
    return RefHex(a.q + b.q, a.r + b.r, a.s + b.s)

def subtract(a, b):
    return RefHex(a.q - b.q, a.r - b.r, a.s - b.s)

def scale(a, k):
    return RefHex(a.q * k, a.r * k, a.s * k)

def equals(a, b):
    return a.q == b.q and a.r == b.r and a.s == b.s

def length(hex):
    return (abs(hex.q) + abs(hex.r) + abs(hex.s)) // 2

def distance(a, b):
    return length(subtract(a, b))

hex_directions = [RefHex(1, -1, 0), RefHex(1, 0, -1), RefHex(0, 1, -1), RefHex(-1, 1, 0), RefHex(-1, 0, 1), RefHex(0, -1, 1)]
def direction(dir):
    return hex_directions[dir]

def neighbor(hex, dir):
    return add(hex, direction(dir))

def hex_ring(center, radius):
    results = []
    # this code doesn't work for radius == 0; can you see why?
    cube = add(center, scale(direction(4), radius))
    for i in range(6):
        for j in range(radius):
            results.append(cube)
            cube = neighbor(cube, i)
    return results

def hex_spiral(center, radius):
    results = [center]
    for k in range(radius + 1):
        results = results + hex_ring(center, k)
    return results
