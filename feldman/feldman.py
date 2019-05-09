import sys

def lineToVars(line):
    line = line.split()

    #Get p, g and d
    p = int(line[0])
    g = int(line[1])
    d = int(line[2])
    line = line[3:]

    #Get A
    A = []
    for i in range(0,d+1):
        A.append(int(line[i]))
    line = line[d+1:]

    #Get k and the s
    k = int(line[0])
    s = []
    for x in range(1,k+1):
        s.append([x,int(line[x])])

    return (p,g,d,A,k,s)

#Check that the shares are actually valid shares
#(I discovered that apparently some of the provided shares aren't valid..)
#Check that res1 = product_{\forall i}(g^a_i^{x^i}) = g^a(x) mod p = res2
def discardInvalidShares(shares, A, p):
    s = []

    for share in shares:

        #Calculate res for this share
        res = 1
        for i in range(0,len(A)):
            res *= modExp(A[i],share[0]**i,p)
        res %= p

        #Calculate res2
        res2 = modExp(g,share[1], p)

        #Should be equal for valid share
        if res == res2:
            s.append(share)

    return s

#Get secret key using formula derived from lagrange interpolation formula for polynomials
#See: Course litterature page 484
def getSecretKey(shares, q):
    t = len(shares)

    K = 0
    for j in range(0,t):
        b = 1
        for k in range(0,t):
            if (k!=j):
                sub = shares[k][0]-shares[j][0]
                inv = inverse(sub,q)
                #Assert that inverse is correct
                test = inv*sub % q;
                #print(str(sub)+"*"+str(inv)+"="+str(test)+" mod "+str(q))
                if (test != 1):
                    #Bad inverse, exit
                    print("bad inverse")
                    sys.exit(1)


                b = b * shares[k][0]*inv
                b = b % q

            b = b % q

        K += b*shares[j][1] % q

    return int(K%q)

''' Calculates the multiplicative inverse of a number a modulo m using the extended euclidean algorithm
 * Params:
 *  a - The number
 *  n - The modulo
 * Returns: The multiplicative inverse of a modulo m if exists, otherwise -1
Uses the Extended euclidean algorithm
See: https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
'''
def inverse(a, n):
    #Ensure that a is positive
    a = a %n
    a = (a+n) % n

    #Get the inverse
    t = 0
    newt = 1
    r = n
    newr = a
    while newr != 0:
        quotient = int(r / newr)
        (t, newt) = (newt, t - quotient * newt)
        (r, newr) = (newr, r - quotient * newr)
    if r > 1:
        print("Warning -1")
        return -1
    if t < 0:
        t = t + n
    return t

'''
From book Applied Cryptography
See: https://en.wikipedia.org/wiki/Modular_exponentiation

def modExp(base, exponent, modulus):

    if modulus == 1:
        return 0

    result = 1
    base = base % modulus
    while exponent > 0:
        if (exponent % 2 == 1):
            result = (result * base) % modulus
        exponent = exponent >> 1
        base = (base * base) % modulus
    return result
'''

def modExp(base, exponent, modulus):
    b = '{:b}'.format(exponent)
    out = 1

    for i in range(len(b)):
        out = out * out % modulus
        if b[i] == '1':
            out = out * base % modulus

    return out


#If this is the main script
if __name__ == "__main__":
    for line in sys.stdin:
        (p,g,d,A,k,s) = lineToVars(line)
        q = (p-1)/2
        s = discardInvalidShares(s, A, p)
        K = getSecretKey(s,q)
        print(K)
