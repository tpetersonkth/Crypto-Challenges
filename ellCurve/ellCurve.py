# -*- coding: utf-8 -*-
# Elliptic curve arithmetics
# Author: Thomas Peterson

import sys

#Elliptic curve parameters
P =  0xfffffffffffffffffffffffffffffffeffffffffffffffff
A =  0xfffffffffffffffffffffffffffffffefffffffffffffffc
B =  0x22123dc2395a05caa7423daeccc94760a7d462256bd56916
Gx = 0x7d29778100c65a1da1783716588dce2b8b4aee8e228f1896
Gy = 0x38a90f22637337334b49dcb66a6dc8f9978aca7648a943b0
Q = 0xffffffffffffffffffffffff7a62d031c83f4294f640ec13

#Class for holding a point
class Point():
    def __init__(self, x, y):
        self.x = x
        self.y = y

#Global variables
g = Point(Gx,Gy) #Generator point
ID = -1 #Identity element of the elliptic group

#Taken from https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python
#Extended ecludian algorithm - calculates the coefficients of Bézout's identity
#Params:
#   a - The first number
#   b - The second number
#Returns: (g, x, y) such that a*x + b*y = g = gcd(a, b)
def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

#Taken from https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python
#Calculates and returns the modular inverse of a modulus m
#Params:
#   a - Number to find inverse for
#   m - Modulus
#Returns: Inverse of a in modulus m
def modinv(a, m):
    a = a % m
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

#Applies elliptic curve point addition on two points
#Params:
#   P1 - The first point
#   P2 - The second point
#Returns: P1 + P2
def add(P1, P2):
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


#Applies elliptic curve point multiplication using the double-and-add algorithm
#Params:
#   f - A factor
#   P - A point
#Returns: fP
#More info: https://en.wikipedia.org/wiki/Elliptic_curve_point_multiplication#Double-and-add
def multiply(f, P):
  N = P
  Q = ID

  while(f > 0):
    if ((f&1) == 1):
      Q = add(Q,N)
    
    N = add(N,N)
    f = f >> 1

  return Q


if __name__=="__main__":
    for i in sys.stdin:
        num = int(i, 0)
        ans = multiply(num,g)
        print(str(hex(ans.x)) + " " + str(hex(ans.y)))