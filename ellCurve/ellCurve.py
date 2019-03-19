# Elliptic curve arithmetics
# Author: Thomas Peterson

import sys

P =  0xfffffffffffffffffffffffffffffffeffffffffffffffff
A =  0xfffffffffffffffffffffffffffffffefffffffffffffffc
B =  0x22123dc2395a05caa7423daeccc94760a7d462256bd56916
Gx = 0x7d29778100c65a1da1783716588dce2b8b4aee8e228f1896
Gy = 0x38a90f22637337334b49dcb66a6dc8f9978aca7648a943b0
Q = 0xffffffffffffffffffffffff7a62d031c83f4294f640ec13

class Point():
    def __init__(self, x, y):
        self.x = x
        self.y = y

g = Point(Gx,Gy)
ID = -1#Identity element of the elliptic group

#Taken from https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python
def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

#Taken from https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python
def modinv(a, m):
    a = a % m
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

def applyOp(P1, P2):
  x3 = 0
  y3 = 0

  #Corner cases
  if(P1 == ID):
    return P2
  elif(P2 == ID):
      return P1
  elif (P2.x == P1.x and P2.y == -P1.y):
    return ID

  #Calculate lambda
  if(P1.x == P2.x and P1.y == P2.y):
    #lambda = (3(x1)² + a)(2y1)^-1
    l = (3*(P1.x**2) + A) * modinv(2*P1.y, P)
    l = l % P
  else:
    #lambda = (y2 - y1)(x2 - x1)^-1
    l = (P2.y-P1.y) * modinv(P2.x-P1.x, P)
    l = l % P

  #Calculate the point
  temp = l**2 % P
  x3 = (temp - P1.x - P2.x) % P
  y3 = (l*(P1.x-x3)-P1.y) % P

  # Make sure that x3 and y3 are in the range 0 to P-1
  if(x3 < 0):
    x3 += P
  if(y3 < 0):
    y3 += P

  return Point(x3,y3)



#https://en.wikipedia.org/wiki/Elliptic_curve_point_multiplication#Double-and-add
def applyOpMul(c, P):
  N = P
  Q = ID

  while(c > 0):
    if ((c&1) == 1):
      Q = applyOp(Q,N)
    
    N = applyOp(N,N)
    c = c >> 1
  

  return Q



if __name__=="__main__":
    for i in sys.stdin:
        num = int(i, 0)
        ans = applyOpMul(num,g)
        print(str(hex(ans.x)) + " " + str(hex(ans.y)))