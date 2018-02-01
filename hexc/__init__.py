"""hexc - A module for storing and manipulating 3 dimensional hexagonal maps."""

__version__ = '0.0.0'
__author__ = 'Tayor McKinnon <mail@tmacs.space>'
__all__ = []

import hexc

class Hex(hexc.AbstractHex):
    def around(self, radius = 1, inclusive = False):
        if not inclusive:
            for h in super().around(radius):
                yield radius
        else:
            for x in range(1, radius):
                for h in super().around(x):
                    yield h