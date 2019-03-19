// Elliptic curve arithmetics
// Author: Thomas Peterson
//

#include <iostream> /* cin and cout */
#include <gmpxx.h> /* mpz_class */
#include <string>

#include "ellCurve.h"


using namespace std;

//mpz_class inv(mpz_class num);
struct point applyOp(struct point P1, struct point P2);
struct point applyOpMul(mpz_class c, struct point P);

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
    temp = temp.substr(2);
    //cout << "loaded:" << temp << endl;
    mpz_class c(temp,16);
    //cout << hex << c << endl;

    //Tests
    struct point g = *(new struct point(Gx,Gy));
    struct point res = applyOpMul(c,g);
    cout << "0x" << hex << res.x << " 0x" << res.y << endl;
  }

}

struct point applyOp(struct point P1, struct point P2){
  struct point R;
  mpz_class x3,y3;

  //Corner cases
  if (P2.x == P1.x && P2.y == -P1.y){
    R.setToIdEl();
    return R;
  }
  else if(P1.idEl){
    return P2;
  }
  else if(P2.idEl){
    return P1;
  }

  //Calculate lambda
  mpz_class lambda;
  if(P1.x == P2.x && P1.y == P2.y){//If P1 == P2

    //lambda = (3(x1)² + a)(2y1)^-1
    mpz_class exp = 2;
    mpz_class temp;
    mpz_powm(temp.get_mpz_t(), P1.x.get_mpz_t(), exp.get_mpz_t(),P.get_mpz_t());

    lambda = 3*temp + A % P;

    mpz_class invVal;
    mpz_class val = 2*P1.y;
    mpz_invert(invVal.get_mpz_t(), val.get_mpz_t(), P.get_mpz_t());

    lambda = lambda * invVal;
  }
  else{

    //lambda = (y2 - y1)(x2 - x1)^-1
    mpz_class invVal;
    mpz_class val = P2.x - P1.x;
    mpz_invert(invVal.get_mpz_t(), val.get_mpz_t(), P.get_mpz_t());
    lambda = (P2.y-P1.y) * invVal;

  }

  //Calculate the point
  mpz_class exp = 2;
  mpz_class temp;
  mpz_powm(temp.get_mpz_t(), lambda.get_mpz_t(), exp.get_mpz_t(), P.get_mpz_t());
  x3 = temp - P1.x - P2.x % P;
  lambda %= P;
  y3 = (lambda*(P1.x-x3)-P1.y) % P;

  // Make sure that x3 and y3 are in the range 0 to P-1
  if(x3 < 0){
    x3 += P;
  }
  if(y3 < 0){
    y3 += P;
  }
  R.set(x3,y3);

  return R;

}

//https://en.wikipedia.org/wiki/Elliptic_curve_point_multiplication#Double-and-add
struct point applyOpMul(mpz_class c, struct point P){
  struct point N = P;
  struct point Q = *(new struct point(0,0));
  Q.setToIdEl();

  while(c > 0){
    if ((c&1) == 1){
      Q = applyOp(Q,N);
    }
    N = applyOp(N,N);
    c = c >> 1;
  }

  return Q;
}


