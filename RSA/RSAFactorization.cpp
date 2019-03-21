//Factorization of RSA
//Author: Thomas Peterson

#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <algorithm>    /* gcd */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <gmpxx.h>      /* mpz_class */

using namespace std;

void recoverPrimeFactors(mpz_class n, mpz_class e, mpz_class d, mpz_class *outp, mpz_class *outq);
void calculateAns(mpz_class y, mpz_class n, mpz_class *outp, mpz_class *outq);
void outputAns(mpz_class p, mpz_class q);

int main(){
  srand(time(NULL));

  mpz_class n,e,d;
  mpz_class p = 0;
  mpz_class q = 0;
  while(cin >> n){
    //Read the input
    cin >> e;
    cin >> d;

    //Recover the prime factors
    recoverPrimeFactors(n, e, d, &p, &q);

    //Output the answer
    outputAns(p,q);
  }

}

/*
 * Recovers the primae factors p and q from the private exponenent, public exponent and the product pq
 * Params:
 *  n - The product pq
 *  e - The public exponent
 *  d - The private exponent
 *  outp - The location to put the first prime factor
 *  outq - The location to put the second prime factor
 *
 * Pseudo code: Appendix C of https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-56Br1.pdf
 * Theory: http://www.ams.org/notices/199902/boneh.pdf
 */

void recoverPrimeFactors(mpz_class n, mpz_class e, mpz_class d, mpz_class *outp, mpz_class *outq) {
  mpz_class p,q;
  mpz_class k = (d*e)-1;
  mpz_class r,t;

  if((k&0x1) == 1){//k must be odd
    cout << "Can not compute primefactors for " << k << endl;
    return;
  }

  t = 0;
  while(true){
    k = k/2;
    t++;
    if((k&0x1) == 1) {//k is odd
      r = k;
      break;
    }
  }

  //g will give us the prime factors with high probability over 100 iterations
  for(int i = 1; i < 100; i++){
    bool goToNext = false;

    mpz_class g = rand()%n; //Pseudo random numbers are enough for our purposes
    mpz_class y;
    mpz_powm(y.get_mpz_t(),g.get_mpz_t(),r.get_mpz_t(),n.get_mpz_t());//y = g^r mod n

    if (y == 1 || y == n-1){
      continue;
    }

    mpz_class x;
    for(int j = 1; j <= t-1; j++){
      //Calculate x = y^2 mod n
      mpz_class temp = 2;
      mpz_powm(x.get_mpz_t(),y.get_mpz_t(),temp.get_mpz_t(),n.get_mpz_t());

      if (x == 1){
        calculateAns(y, n, outp, outq);
        return;
      }

      if (x == n-1){
        goToNext = true;
        break;
      }

      y = x;
    }

    if(goToNext){
      continue;
    }

    mpz_class temp = 2;
    mpz_powm(x.get_mpz_t(),y.get_mpz_t(),temp.get_mpz_t(),n.get_mpz_t());//x = y^2 mod n

    if (x == 1){
      calculateAns(y, n, outp, outq);
      return;
    }
  }

  //If we still haven't found y such that x=1, consider the corner case y = n-1
  mpz_class y=n-1;
  mpz_class temp = 2;
  mpz_class x;
  mpz_powm(x.get_mpz_t(),y.get_mpz_t(),temp.get_mpz_t(),n.get_mpz_t());//x = y^2 mod n

  if (x == 1){
    calculateAns(y, n, outp, outq);
    return;
  }

  cout << "not found" <<  endl;

}

/*
 * Subfunction for recoverPrimeFactors, calculates the prime factors once x is 1 in recoverPrimeFactors
 * Params:
 *  y - Corresponds to y in recoverPrimeFactors
 *  n - Corresponds to n in recoverPrimeFactors
 *  outp - The output location for the first prime factor
 *  outq - THe output location for the second prime factor
 */
void calculateAns(mpz_class y, mpz_class n, mpz_class *outp, mpz_class *outq){
  y = y-1;
  *outp = gcd(y,n);
  *outq = n/(*outp);
}

/*
 * Outputs the two prime numbers in increasing order
 * Params:
 *  p - First prime factor
 *  q - Second prime factor
 */
void outputAns(mpz_class p, mpz_class q){
  if (p < q){
    cout << p << " " << q << endl;
  }
  else{
    cout << q << " " << p << endl;
  }
}
