/*
 * Modular Exponentitation
 * Author: Thomas Peterson
 */

#include <iostream>
#include <gmpxx.h>

using namespace std;

mpz_class binExpNoRec(mpz_class b, mpz_class e, mpz_class m);

int main(){

  mpz_class a, e, n;

  while(cin >> a){
    cin >> e;
    cin >> n;

    cout << binExpNoRec(a,e,n) << endl;
  }

}

mpz_class binExpNoRec(mpz_class b, mpz_class e, mpz_class m){

  mpz_class r = 1;
  while (e > 0){
    if (e % 2 == 1){
      r = (r* b) % m;
    }

    e = e >> 1;
    b = (b * b) % m;
  }

  return r;
}