
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <gmpxx.h>
#include <sstream>
#include <assert.h>

mpz_class inv(mpz_class a, mpz_class m);
bool check(mpz_class size, mpz_class *a, mpz_class*q, mpz_class Res);
mpz_class gcd(mpz_class a, mpz_class b);
bool possible(mpz_class nArr[], mpz_class size);
mpz_class CRT(mpz_class a[], mpz_class q[], mpz_class n);

using namespace std;

int main() {
  mpz_class size;
  while (cin >> size) {
    mpz_class q[size.get_ui()];
    mpz_class a[size.get_ui()];

    for(int i = 0; i < size; i++){
      cin >> q[i];
    }
    for(int i = 0; i < size; i++){
      cin >> a[i];
    }

    mpz_class r = CRT(a,q,size);
    cout << r << endl;

  }

  return 0;
}

/*
 * Calculates the greatest common divisor of a and b using the euclidean algorithm
 * Params:
 *  a - The first number
 *  b - The second number
 * Returns: The greatest common divisor of a and b
 */
mpz_class gcd(mpz_class a, mpz_class b){
  if (a == 0)
    return b;
  return gcd(b % a, a);
}

/* Calculates the multiplicative inverse of a number a modulo m
 * Params:
 *  a - The number
 *  m - The modulo
 * Returns: The multiplicative inverse of a modulo m
 * Taken from https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
 */
mpz_class inv(mpz_class a, mpz_class m){
  mpz_class m0 = m, t, q;
  mpz_class x0 = 0, x1 = 1;

  if (m == 1)
    return 0;

  // Apply extended Euclid Algorithm
  while (a > 1)
  {
    // q is quotient
    q = a / m;

    t = m;

    // m is remainder now, process same as
    // euclid's algo
    m = a % m, a = t;

    t = x0;

    x0 = x1 - q * x0;

    x1 = t;
  }

  // Make x1 positive
  if (x1 < 0)
    x1 += m0;

  return x1;
}

/*
 * Calculates the answer variable x to a set of modular equations on the form x = ai mod ni, using the chinese remainder theorem
 * Params:
 *  aArr - The array containing the values of each ai
 *  nArr - The array containing the values of each ni
 *  n - The number of equations to satisify (= len of nArr and aArr)
 * Returns: The variable x that fulfills all the equations
 */
mpz_class CRT(mpz_class aArr[], mpz_class nArr[], mpz_class n){
  mpz_class sArr[n.get_ui()];
  mpz_class quotient, r, prevR, newR, s, prevS, newS;

  //Calculate N = n1*n2*n3*n4..ni
  mpz_class N = 1;
  for (int k = 0; k < n; k++)
    N *= nArr[k];

  //Calculate x
  mpz_class x = 0;
  for (int k = 0; k < n; k++) {
    x += inv(N / nArr[k], nArr[k]) * (N / nArr[k]) * aArr[k];
  }

  //Ensure that the answer is in the range 0 to N-1
  while(x >= N){x -= N;}
  while(x < 0){x += N;}

  return x;
}