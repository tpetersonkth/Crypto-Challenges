// Elliptic curve arithmetics
// Author: Thomas Peterson
//

#include <iostream> /* cin and cout */
#include <gmpxx.h> /* mpz_class */
#include <string>

#include "ellCurve.h"


using namespace std;

struct point applyOp(struct point P1, struct point P2);

//Elliptic curve parameters
mpz_class P("fffffffffffffffffffffffffffffffeffffffffffffffff",16);
mpz_class A("fffffffffffffffffffffffffffffffefffffffffffffffc",16);
mpz_class B("22123dc2395a05caa7423daeccc94760a7d462256bd56916",16);
mpz_class Gx("7d29778100c65a1da1783716588dce2b8b4aee8e228f1896",16);
mpz_class Gy("38a90f22637337334b49dcb66a6dc8f9978aca7648a943b0",16);
mpz_class q("ffffffffffffffffffffffff7a62d031c83f4294f640ec13",16);



int main(){
  string temp;


  while(cin >> hex >> temp){

    cout << hex << P << endl;
    cout << hex << B << endl;

    //Tests
    struct point R = applyOp(*(new struct point(17,10)), *(new struct point(95,31)));
    cout << "R -> " << R.x << ":" << R.y << endl;
  }

}

//Applies the group operation for the elliptic curve group
//See appendix F1 of https://tools.ietf.org/html/rfc6090
struct point applyOp(struct point P1, struct point P2){
  struct point R;
  mpz_class x3,y3;

  P1.print();
  P2.print();


  if (P1.idEl){
    R.set(P2.x,P2.y);
  }
  else if (P2.idEl){
    R.set(P1.x,P2.y);
  }
  else if(P1.y != P2.y && P1.x == P2.x){
    R.setToIdEl();
  }
  else if(P1.y != P2.y && P1.x != P2.x){
    mpz_class temp = (P2.y-P1.y);
    mpz_class temp2 = (P2.x-P1.x);
    //mpz_class temp = ((P2.y-P1.y)/(P2.x-P1.x))^2;
    cout << "temp: " << temp << ":" << temp2 << endl;
    x3 = temp - P1.x - P2.x % P;
    y3 = (P1.x-x3)*(P2.y-P1.y)/(P2.x-P1.x) - P1.y % P;
    R.set(x3,y3);
  }
  else if (P1.x == P2.x && P1.y == P2.y && P1.y == 0){
    R.setToIdEl();
  }
  else{
    x3 = ((3*P1.x^2 + A)/(2*P1.y))^2 - 2*P1.x % P;
    y3 = (P1.x-x3)*(3*P1.x^2 + A)/(2*P1.y) - P1.y % P;
    R.set(x3,y3);
  }

  return R;

}

//https://en.wikipedia.org/wiki/Elliptic_curve_point_multiplication#Double-and-add
struct point applyOpMul(struct point P1, struct point P2){

}
