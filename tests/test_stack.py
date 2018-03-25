import hexc
import pytest

@pytest.fixture()
def faux_slice():
    return [(h, i) for i, h in enumerate(list(hexc.AbstractHex(0,0).within(10)))]

@pytest.fixture()
def faux_stack():
    return [faux_slice() for s in range(5)]

@pytest.fixture()
def empty_stack():
    return hexc.Stack(radius = 10, height = 5)

def test_stack_storage(faux_slice, empty_stack):
    for k, v in faux_slice:
        empty_stack.set(k,v)
    for k, v in faux_slice:
        got = empty_stack.get(k)
        assert got == v
    

if __name__ == '__main__':
    test_stack_storage(faux_slice(), empty_stack())