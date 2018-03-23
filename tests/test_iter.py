import hexc
import pytest
import ref

@pytest.fixture
def Hexes():
    return [hexc.AbstractHex(x, x + 1, -x - (x + 1)) for x in range(50)]

def test_around_order(Hexes):
    for h in Hexes:
        ref_ring = ref.hex_ring(h, 5)
        for j in h.around(5):
            rh = ref_ring.pop(0)
            assert rh.q == j.q and rh.r == j.r and rh.s == j.s


def test_within_order(Hexes):
    for h in Hexes:
        ref_spiral = ref.hex_spiral(h, 5)
        imp_spiral = list(h.within(5))
        print(len(ref_spiral), len(imp_spiral))
        for i in range(len(ref_spiral)):
            rh = ref_spiral[i]
            ih = imp_spiral[i]
            if rh.q == ih.q or rh.r == ih.r or rh.s == ih.s:
                assert rh.q == ih.q or rh.r == ih.r or rh.s == ih.s
            else:
                begin = i - 5 if i > 5 else 0
                for x, y in zip(ref_spiral[begin:i], imp_spiral[begin:i]):
                    print(x, y)
                print('')
                print(rh, ih, end = '')
                print('        <---- %i'%i)
                print('')
                for x, y in zip(ref_spiral[i+1:i+5], imp_spiral[i+1:i+5]):
                    print(x, y)
                assert rh.q == ih.q or rh.r == ih.r or rh.s == ih.s


if __name__ == '__main__':
    test_within_order(Hexes())