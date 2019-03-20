// Calculation of elliptic points
// Author: Thomas Peterson

//The term "Crypto book" referes to cryptography theory and practice third edition by Douglas R Stinson

#include <iostream> /* cin and cout */
#include <gmpxx.h> /* mpz_class */

using namespace std;
int isSingular(mpz_class a, mpz_class b, mpz_class p);
bool isQuadResidue(mpz_class z, mpz_class p);
mpz_class getNumPoints(mpz_class a, mpz_class b, mpz_class p);

int main(){

  mpz_class p,a,b;
  while(cin >> p){
    cin >> a;
    cin >> b;

    cout << isSingular(a,b,p) << " " << getNumPoints(a,b,p) << endl;

  }
}

/*
 * Checks if an elliptic curve is singular using its determinant
 * Params:
 *  a - Elliptic curve parameter a
 *  b - Elliptic curve parameter b
 *  p - Modulus
 * Returns: True if the elliptic curve is singular, False if not
 * More info: crypto book page 255
*/
int isSingular(mpz_class a, mpz_class b, mpz_class p){
  //Calculate 4*a^3 + 27*b^2 and check if equal to 0
  mpz_class exp1 = 3;
  mpz_class exp2 = 2;
  mpz_class t1,t2;
  mpz_powm(t1.get_mpz_t(), a.get_mpz_t(),exp1.get_mpz_t(),p.get_mpz_t());
  mpz_powm(t2.get_mpz_t(), b.get_mpz_t(),exp2.get_mpz_t(),p.get_mpz_t());

  mpz_class temp = (4*t1 + 27*t2) % p;
  return temp == 0 ? 1 : 0;

}

/*
 * Uses euclidean criterion to calculate if z is a quadratic residue modulo p or not
 * Params:
 *  z - integer to determine if quadric residue or not
 *  p - Modulus
 * Returns: True if z is a quadratic residue modulo p
 */
bool isQuadResidue(mpz_class z, mpz_class p){
  //z^(p-1)/2 mod p
  mpz_class temp;
  mpz_class exp = (p-1)/2;
  mpz_powm(temp.get_mpz_t(),z.get_mpz_t(),exp.get_mpz_t(),p.get_mpz_t());

  return temp == 1;
}


/*
 * Returns the number of affine points of an elliptic curve
 * Params:
 *  a - Elliptic curve parameter a
 *  b - Elliptic curve parameter b
 *  p - Modulus
 * Returns: Number of affine points of the elliptic curve specified by a,b and p
 * More info: More info: https://en.wikipedia.org/wiki/Counting_points_on_elliptic_curves
 */
mpz_class getNumPoints(mpz_class a, mpz_class b, mpz_class p){
  mpz_class points = 0;

  for(mpz_class x = 0; x < p; x++){
    mpz_class temp;
    mpz_class exp = 3;
    mpz_powm(temp.get_mpz_t(), x.get_mpz_t(),exp.get_mpz_t(),p.get_mpz_t());
    temp = (temp + a*x + b) % p;

    if (isQuadResidue(temp, p)){
      points = points + 2;
    }

    if (temp == 0){
      points++;
    }
  }

  return points;
}

