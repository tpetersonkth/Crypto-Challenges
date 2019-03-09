//CRT solver
//Author: Thomas Peterson

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <math.h>

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

    float fRes = float(Res);
    float fQ = float(Q);
    if(Res < 0){
      int factor = ceil(abs(fRes/fQ));
      //cout << fRes << " " << fQ << " " << fRes/fQ << " " << factor << endl;
      Res += Q*factor;
    }

    if(Res >= Q){
      int factor = floor(fRes/fQ);
      //cout << fRes << " " << fQ << " " << fRes/fQ << " " << factor << endl;
      Res -= Q*factor;
    }


    cout << float(Res) << endl;

  }

  return 0;
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
