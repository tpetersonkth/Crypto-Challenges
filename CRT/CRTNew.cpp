
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>

bool check(long size, long *a, long*q, long Res);
long gcd(long a, long b);
bool possible(long nArr[], long size);
long CRT(long a[], long q[], long n);

using namespace std;

int main() {
  long size;
  while (cin >> size) {
    long q[size];
    long a[size];
    for(long i = 0; i < size; i++){
      cin >> q[i];
    }
    for(long i = 0; i < size; i++){
      cin >> a[i];
    }

    long r = CRT(a,q,size);
    cout << r << endl;

  }

  return 0;
}

bool check(long size, long *a, long*q, long Res){
  bool returnVal = true;

  for(long i = 0; i < size; i++) {
    if (a[i]%q[i] != Res%q[i]){
      returnVal = false;
    }
  }

  return returnVal;
}

long gcd(long a, long b){
  if (a == 0)
    return b;
  return gcd(b % a, a);
}

bool possible(long nArr[], long size){
  for (long i = 0; i < size; i++){
    for (long j = i+1; j < size; j++){
      if (gcd(nArr[i],nArr[j]) != 1){
        return false;
      }
    }
  }
  return true;
}

//Taken from https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
long inv(long a, long m){
  long m0 = m, t, q;
  long x0 = 0, x1 = 1;

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

long CRT(long aArr[], long nArr[], long n){
  long sArr[n];
  long quotient, r, prevR, newR, s, prevS, newS;

  //Calculate N = n1*n2*n3*n4..ni
  long N = 1;
  for (long k = 0; k < n; k++)
    N *= nArr[k];

  //Find inverse
  for (long k = 0; k < n; k++){
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

  long x = 0;
  for (long k = 0; k < n; k++)
    x += sArr[k] * (N / nArr[k]) * aArr[k];

  //Ensure that the answer is in the range 0 to N-1
  while(x >= N){x -= N;}
  while(x < 0){x += N;}

  return x;
}