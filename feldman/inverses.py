
def modInverse(num, base):
    number0 = num % base
    base0 = base
    t0 = 0
    t = 1
    q = base0 // number0
    r = base0 - q * number0

    while r > 0:
        temp = (t0 - q * t) % base
        t0 = t
        t = temp
        base0 = number0
        number0 = r
        q = base0 // number0
        r = base0 - q * number0

    if number0 != 1:
        return -1
    else:
        return t


def mulinv(a, b):
    """return x such that (x * a) % b == 1"""
    g, x, _ = gcdExtendedRec(a, b)
    if g == 1:
        return x % b
    return -1
def getInverse(num, base):#a,b
    (g,x,y) = gcdExtendedRec(num,base)
    if (g != 1):
        return -1
    else:
        return (x + base) % base#Add base to avoid negative x

'''

'''
def gcdExtendedRec(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, x, y = gcdExtendedRec(b % a, a)
        return (g, y - (b // a) * x, x)
