
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
    //cout << "size:" << size << endl;
    for(int i = 0; i < size; i++){
      cin >> q[i];
    }
    for(int i = 0; i < size; i++){
      cin >> a[i];
    }

    //cin.ignore(1000,'\n');

    /*
    //Ensure input a is 0 < a < q[i]
    for(mpz_class i = 0; i < size; i++){
      while(a[i] < 0){
        a[i] += q[i];
      }
      while(a[i] >= q[i]){
        a[i] -= q[i];
      }
    }
    */

    mpz_class r = CRT(a,q,size);
    cout << r << endl;

  }

  return 0;
}

bool check(mpz_class size, mpz_class *a, mpz_class*q, mpz_class Res){
  bool returnVal = true;

  for(int i = 0; i < size; i++) {
    if (a[i]%q[i] != Res%q[i]){
      returnVal = false;
    }
  }

  return returnVal;
}

mpz_class gcd(mpz_class a, mpz_class b){
  if (a == 0)
    return b;
  return gcd(b % a, a);
}

bool possible(mpz_class nArr[], mpz_class size){
  for (int i = 0; i < size; i++){
    for (int j = i+1; j < size; j++){
      if (gcd(nArr[i],nArr[j]) != 1){
        return false;
      }
    }
  }
  return true;
}

//Taken from https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
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

mpz_class CRT(mpz_class aArr[], mpz_class nArr[], mpz_class n){
  mpz_class sArr[n.get_ui()];
  mpz_class quotient, r, prevR, newR, s, prevS, newS;

  //Calculate N = n1*n2*n3*n4..ni
  mpz_class N = 1;
  for (int k = 0; k < n; k++)
    N *= nArr[k];

  //Find inverse
  /*
  for (mpz_class k = 0; k < n; k++){
    prevR = nArr[k];
    newR = N / nArr[k];
    prevS = 0;
    newS = 1;

    //Extended euclidean algorithm
    //See https://brilliant.org/wiki/extended-euclidean-algorithm/
    while (newR != 0)
    {
      quotient = prevR / newR;
      r = prevR - quotient * newR;
      prevR = newR;
      newR = r;

      s = prevS - quotient * newS;
      prevS = newS;
      newS = s;
    }


    if (prevR == 1)
      sArr[k] = prevS;

    // Ensure that each element in sArr is positive
    if (sArr[k] < 0)
      sArr[k] = sArr[k] + nArr[k];
  }
  */

  mpz_class x = 0;
  for (int k = 0; k < n; k++) {

    /*
    //Sanity check
    int c = inv(N / nArr[k], nArr[k]) * (N / nArr[k]);
    for (int i = 0; i < n; i++){
      if (i != k){
        while(c%nArr[i] != 0){
          cout << "Sanity check failed" << endl;
        }
      }
    }
    */

    x += inv(N / nArr[k], nArr[k]) * (N / nArr[k]) * aArr[k];
    //x += sArr[k] * (N / nArr[k]) * aArr[k];
  }


  //Ensure that the answer is in the range 0 to N-1
  while(x >= N){x -= N;}
  while(x < 0){x += N;}

  return x;
}