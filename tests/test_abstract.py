import hexc
import pytest
import ref

@pytest.fixture
def Hexes():
    return [hexc.AbstractHex(x, x + 1, -x - (x + 1)) for x in range(50)]

@pytest.fixture
def EqualHexes():
    hexes = [hexc.AbstractHex(x, x + 1, -x - (x + 1)) for x in range(50)]
    return list(zip(hexes, hexes))

@pytest.fixture
def UnEqualHexes():
    hexes = [hexc.AbstractHex(x, x + 1, -x - (x + 1)) for x in range(50)]
    return list(zip(hexes, reversed(hexes)))    

def test_type_exist():
    assert hexc.AbstractHex

def test_hex_addition(Hexes):
    for hex in Hexes:
        refAns = ref.add(hex, hex)
        implAns = hex + hex
        assert ref.equals(refAns, implAns)

def test_hex_subtraction(Hexes):
    for hex in Hexes:
        refAns = ref.subtract(hex, hex)
        implAns = hex - hex
        assert ref.equals(refAns, implAns)

def test_hex_scale(Hexes):
    for idx, hex in enumerate(Hexes):
        refAns = ref.scale(hex, idx)
        implAns = hex * idx
        assert ref.equals(refAns, implAns)

def test_hex_equals(EqualHexes):
    for a, b in EqualHexes:
        refAns = ref.equals(a, b)
        implAns = a == b
        assert refAns == implAns
    
def test_hex_not_equals(UnEqualHexes):
    for a, b in UnEqualHexes:
        refAns = ref.equals(a, b)
        implAns = a == b
        assert refAns == implAns

def test_hex_length(Hexes):
    for hex in Hexes:
        refAns = ref.length(hex)
        implAns = hex.length()
        assert refAns == implAns

def test_hex_distance(UnEqualHexes):
    for a, b in UnEqualHexes:
        refAns = ref.distance(a, b)
        implAns = a.distance(b)
        assert refAns == implAns

def test_hex_direction(Hexes):
    for hex in Hexes:
        for d in range(6):
            refAns = ref.direction(d)
            implAns = hex.direction(d)
            assert ref.equals(refAns, implAns)

def test_hex_neighbor(Hexes):
    for hex in Hexes:
        for d in range(6):
            refAns = ref.neighbor(hex, d)
            implAns = hex.neighbor(d)
            assert ref.equals(refAns, implAns)