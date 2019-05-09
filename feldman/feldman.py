import sys

'''
Extracts the variables required to retrieve a secret key from a set of shares
Params:
    Line - a line containing all the variables values
Returns: (p,g,d,A,k,s),a tuple of the parsed variables
'''
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

'''
Check that the shares are actually valid shares
Params: 
    shares - THe set o shares to check
    A - The set of valuese g^{a_i} for all 0 <= i <= d
    p - The modulus of the group Z_{p}^*
Returns: The subset of shares only containing valid shares
The check is done by checking that res1 = product_{\forall i}(g^a_i^{x^i}) = g^a(x) mod p = res2 (Latex math notation)
'''
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

        #Ensure that these are equal
        if res == res2:
            s.append(share)

    return s

'''
Extracts the secret key from a set of shares
Params:
    shares - The set of shares
    q - The modulus of the group G_q
Returns: The secret key K
The secret key is extracted from the shares using a formula derived from lagrange interpolation formula for polynomials
See: Course litterature page 484
'''
def getSecretKey(shares, q):
    t = len(shares)

    K = 0
    for j in range(0,t):
        b = 1
        for k in range(0,t):
            if (k!=j):
                sub = shares[k][0]-shares[j][0]
                inv = inverse(sub,q)

                b = b * shares[k][0]*inv
                b = b % q

        K += b*shares[j][1] % q

    return int(K%q)

''' 
Calculates the multiplicative inverse of a number a modulo m using the extended euclidean algorithm
 * Params:
 *  a - The number whose inverse is desired
 *  m - The modulo
 * Returns: The multiplicative inverse of a modulo m if exists, otherwise -1
Uses the Extended euclidean algorithm
See: https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
'''
def inverse(a, m):
    #Ensure that 0 <= a < n
    a = a %m

    #Calculate the inverse of a mod m
    t = 0
    newt = 1
    r = m
    newr = a
    while newr != 0:
        quotient = int(r / newr)
        (t, newt) = (newt, t - quotient * newt)
        (r, newr) = (newr, r - quotient * newr)
    if r > 1:
        print("Warning -1")
        return -1
    if t < 0:
        t = t + m
    return t

'''
Caluclates base^exponent mod modulus
Params:
    base - The base of the expression
    exponent - The exponent of the expression
    modulus - The modulus of the expression
Returns: base^exponent mod modulus

From the book 'Applied Cryptography' by Bruce Schneier
See: https://en.wikipedia.org/wiki/Modular_exponentiation
'''
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

#If this is the main script
if __name__ == "__main__":
    for line in sys.stdin:
        #Get all variables from the current input line
        (p,g,d,A,k,s) = lineToVars(line)
        q = (p-1)/2

        #Filter out invalid shares
        s = discardInvalidShares(s, A, p)

        #Calculate and output the secret key using the valid shares
        K = getSecretKey(s,q)
        print(K)
