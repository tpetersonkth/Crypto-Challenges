//
// Created by user on 3/17/19.
//

#ifndef ELLCURVE_ELLCURVE_H
#define ELLCURVE_ELLCURVE_H

//#include <gmpxx.h> /* mpz_class */


struct point {
    mpz_class x,y;
    bool idEl = false;

    point(){

    }

    point(mpz_class newX, mpz_class newY){
        x = newX;
        y = newY;
        idEl = false;
    }

    void setToIdEl(){
      idEl = true;
    }

    void set(mpz_class newX, mpz_class newY){
      x = newX;
      y = newY;
      idEl = false;
    }

    void print(){
      std::cout << std::dec << x << ":" << y << std::endl;
    }

};


#endif //ELLCURVE_ELLCURVE_H
