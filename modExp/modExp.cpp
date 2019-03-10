
/*
 * Modular Exponentitation
 * Author: Thomas Peterson
 */

#include <iostream>
#include <gmpxx.h>
#include <string>

using namespace std;

uint64_t modexpNaive(uint64_t a, uint64_t e, uint64_t n);
uint64_t binaryExp(uint64_t x, uint64_t n, uint64_t m);
long long binExpNoRec(long long b, long long e, long long m);

int main2(){

  string a;
  string e;
  string n;

  while(cin >> a){
    cin >> e;
    cin >> n;


    cout << a << endl;
    cout << e << endl;
    cout << n << endl;



    //uint64_t r = modexpNaive(a,e,n);
    //uint64_t r = binaryExp(a,e,n) % n;
    //long long r = binExpNoRec(a,e,n);

    //cout << r << endl;

  }

}

int main(){

  uint64_t a;
  uint64_t e;
  uint64_t n;

  while(cin >> a){
    cin >> e;
    cin >> n;

    //uint64_t r = modexpNaive(a,e,n);
    //uint64_t r = binaryExp(a,e,n) % n;
    long long r = binExpNoRec(a,e,n);

    cout << r << endl;

  }

}

uint64_t modexpNaive(uint64_t a, uint64_t e, uint64_t n){//Way to slow ;P as expected.. :P
  uint64_t r = a;
  while (e > 1){
    r *= a;
    e--;
    r = r % n;
  }
  return r;
}

//https://simple.wikipedia.org/wiki/Exponentiation_by_squaring
uint64_t binaryExp(uint64_t x, uint64_t n, uint64_t m){
  if (n == 1){
    return x;
  }

  if ((n&0x1) == 0){//n is even
    return binaryExp(x*x,n/2,m);
  }
  else{
    return (x * binaryExp(x*x,(n-1)/2,m));
  }
}

long long binExpNoRec(long long b, long long e, long long m){

  long long r = 1;
  while (e > 0){
    if (e % 2 == 1){
      r = (r* b) % m;
    }

    e = e >> 1;
    b = (b * b) % m;
  }

  return r;
}

void test(){
  uint64_t r;
  uint64_t r2;
}