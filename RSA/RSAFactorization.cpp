
/*
 * Factorization of RSA
 * Author: Thomas Peterson
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <algorithm>    /* gcd */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <gmpxx.h>      /* mpz_class */

using namespace std;

void recoverPrimeFactors(mpz_class n, mpz_class e, mpz_class d, mpz_class *p, mpz_class *q);

int main(){
  srand(time(NULL));

  mpz_class n,e,d;
  mpz_class p = 0;
  mpz_class q = 0;
  while(cin >> n){
    cin >> e;
    cin >> d;

    recoverPrimeFactors(n, e, d, &p, &q);
  }

}


//Theory: //http://www.ams.org/notices/199902/boneh.pdf
//Good reference: https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-56Br1.pdf
void recoverPrimeFactors(mpz_class n, mpz_class e, mpz_class d, mpz_class *pout, mpz_class *qout) {
  mpz_class p,q;
  mpz_class k = (d*e)-1;
  mpz_class r,t;

  if((k&0x1) == 1){//k is odd
    cout << "Can not compute primefactors for " << k << endl;
    return;
  }

  mpz_class oldk = k;

  t = 0;
  while(true){
    k = k/2;
    t++;
    if((k&0x1) == 1) {//k is odd
      r = k;
      break;
    }
  }

  cout << "k = " << oldk << " = 2^" << t << "*" << r << endl;
/*
  if (oldk !=  mpz_ui_pow_ui(2,t)*r){//Sanity check
    cout << "Calculations went terribly wrong.." << endl;
    cout << "k = " << oldk << " != 2^" << t << "*" << r << endl;
  }
*/
  //cout << "n is " << n << endl;
  cout << n <<" < RAND_MAX: " << RAND_MAX << endl;
  for(int i = 0; i < 100; i++){
    mpz_class g = rand()%n;
    //cout << "g:" << g << endl;
    mpz_class y;
    mpz_powm(y.get_mpz_t(),g.get_mpz_t(),r.get_mpz_t(),n.get_mpz_t());//y = g^r mod n
    //cout << y << " = " << g << "^" << r << " mod " << n << endl;

    if (y == 1 || y == n-1){
      //continue;
    }

    mpz_class x;
    for(int j = 1; j < t; j++){
      mpz_class temp = 2;
      mpz_powm(x.get_mpz_t(),y.get_mpz_t(),temp.get_mpz_t(),n.get_mpz_t());//x = y^2 mod n

      if (x == 1){
        y = y-1;
        p = gcd(y,n);
        q = n/p;
        cout << "p=" << p << " q=" << q << endl;
        return;
      }

      if (x == n-1){
        continue;
      }

      y = x;
    }
    if (x == 1){
      y = y-1;
      p = gcd(y,n);
      q = n/p;
      cout << "p=" << p << " q=" << q << endl;
      return;
    }
  }

  cout << "not found" <<  endl;
}
