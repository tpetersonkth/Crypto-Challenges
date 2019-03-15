// Calculation of elliptic points
// Author: Thomas Peterson
//

#include <iostream>
#include <math.h>

using namespace std;
bool isSingular(uint64_t a, uint64_t b, uint64_t p);
bool isQuadResidue(uint64_t z, uint64_t p);
uint64_t getNumPoints(uint64_t a, uint64_t b, uint64_t p);

int main(){

  uint64_t p,a,b;
  while(cin >> p){
    cin >> a;
    cin >> b;

    //cout << "Loaded: p:" << p << " a:" << a << " b:" << b << endl;

    if (isSingular(a,b,p)){
      cout << 1;
    }
    else{
      cout << 0;
    }
    cout << " " << getNumPoints(a,b,p) << endl;

  }
}

bool isSingular(uint64_t a, uint64_t b, uint64_t p){
  uint64_t temp = 4*pow(a,3) + 27*pow(b,2);
  temp = temp % p;
  if (temp == 0){
    return true;
  }
  else{
    return false;
  }
}

bool isQuadResidue(uint64_t z, uint64_t p){

  cout << "p:" << p <<" z:" << z << endl;
  uint64_t temp = pow(z,(p-1)/2);
  temp = temp % p;
  //cout << "p:" << p <<" temp:" << temp << endl;

  return temp == 1;
}

//https://en.wikipedia.org/wiki/Counting_points_on_elliptic_curves
uint64_t getNumPoints(uint64_t a, uint64_t b, uint64_t p){
  uint64_t points = 0;

  for(int x = 0; x < p; x++){
    uint64_t temp = pow(x,3) + a*x + b;
    cout << x << "^3" << "+" << a << "*" << x << "+" << b << "=" << temp << endl;
    temp = temp % p;
    cout << temp << " mod " << p << endl;
    //cout << "temp2: " << temp << endl;
    if (isQuadResidue(temp, p)){
      points = points + 2;
    }

    if (temp == 0){
      points++;
    }
  }

  return points;
}

