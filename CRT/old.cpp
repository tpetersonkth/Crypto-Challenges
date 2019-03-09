//CRT solver
//Author: Thomas Peterson

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>

//#include "sha256.h"

int gcdExtended(int a, int b, int *x, int *y);
bool check(int size, int *a, int*q, int Res);
int evaluate(int a[], int b[], int n);

using namespace std;

int main() {

  int size;
  while (cin >> size) {
    int q[size];
    int a[size];

    //Read input
    for(int i = 0; i < size; i++){
      cin >> q[i];
    }

    for(int i = 0; i < size; i++){
      cin >> a[i];
    }

    int Q = 1;
    for(int i = 0; i < size; i++){
      Q *= q[i];
    }
    //cout << "Q=" << Q << endl;

    //Calculate equation coefficients c
    int c[size];
    for(int i = 0; i < size; i++){
      c[i] = Q/q[i];
      //cout << "c" << c[i] << endl;
    }

    int y[size];
    for(int i = 0; i < size; i++){
      int dummy;
      gcdExtended(q[i], c[i], &dummy, &y[i]);
      //cout << "Y" << i << "->" << dummy << "," << y[i] << endl;
    }

    int Res = 0;
    for(int i = 0; i < size; i++){
      Res += y[i]*c[i]*a[i];
    }

    int r = evaluate(a,q,size);
    cout << r << endl;

    if(check(size,a,q,r)){
      cout << "True" << endl;
    }

    
  }

  return 0;
}

int evaluate(int a[], int b[], int n){
  int Minv[n];
  int q, r, r1, r2, t, t1, t2;

  int total = 1;
  for (int k = 0; k < n; k++)
    total *= b[k];

  for (int k = 0; k < n; k++)
  {
    r1 = b[k];
    r2 = total / b[k];
    t1 = 0;
    t2 = 1;

    while (r2 > 0)
    {
      q = r1 / r2;
      r = r1 - q * r2;
      r1 = r2;
      r2 = r;

      t = t1 - q * t2;
      t1 = t2;
      t2 = t;
    }

    if (r1 == 1)
      Minv[k] = t1;

    if (Minv[k] < 0)
      Minv[k] = Minv[k] + b[k];
  }

  int x = 0;
  for (int k = 0; k < n; k++)
    x += (a[k] * total * Minv[k]) / b[k];

  return x;
}


bool check(int size, int *a, int*q, int Res){
  bool returnVal = true;

  for(int i = 0; i < size; i++) {
     if (a[i]%q[i] != Res%q[i]){
       returnVal = false;
     }
  }

  return returnVal;
}

//Fetched from https://www.geeksforgeeks.org/euclidean-algorithms-basic-and-extended/
// C function for extended Euclidean Algorithm
int gcdExtended(int a, int b, int *x, int *y)
{
  // Base Case
  if (a == 0)
  {
    *x = 0;
    *y = 1;
    return b;
  }

  int x1, y1; // To store results of recursive call
  int gcd = gcdExtended(b%a, a, &x1, &y1);

  // Update x and y using results of recursive
  // call
  *x = y1 - (b/a) * x1;
  *y = x1;

  return gcd;
}
