//SHA-256 implementation
//Author: Thomas Peterson
//For more info, see https://ws680.nist.gov/publication/get_pdf.cfm?pub_id=910977
//For operation precedence, see https://en.cppreference.com/w/cpp/language/operator_precedence
//For testing, see: http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA256.pdf

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>

#include "sha256.h"

using namespace std;

bool padMessage(string& str);
void getVector(vector<vector<uint32_t>>& v,string str);
void sha256(vector<vector<uint32_t>> blocks, uint32_t* K, uint32_t* hash);

uint32_t ROTR(uint32_t x, uint8_t n);
uint32_t ROTL(uint32_t x, uint8_t n);
uint32_t SHR(uint32_t x,uint8_t n);
uint32_t addModulo32bit(uint32_t x, uint32_t y);

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t capitalSigma0(uint32_t x);
uint32_t capitalSigma1(uint32_t x);
uint32_t minusculeSignma0(uint32_t x);
uint32_t minusculeSignma1(uint32_t x);

void testOperations();
void testInternalFunctions();

int main() {

    testOperations();
    testInternalFunctions();

    vector<vector<uint32_t>> blocks;

    string str;
    while (getline(cin, str))
    {

        padMessage(str);

        getVector(blocks,str);
    }

#ifdef DEBUG_OUTPUT
    for (vector<uint32_t> block : blocks) {
        cout << "---" << endl;

        for (int i = 0; i < 16; i++){
            cout << block[i] << endl;
        }
    }
#endif

    uint32_t Hash[8];
    sha256(blocks, K, Hash);

    for(int i = 0; i < 8; i++){
        printf("%08x", Hash[i]);
    }
    cout << endl;

}

bool padMessage(string& str){
    int appended = 0;
    bool hadToPad = false;
    unsigned long len = str.size()*4;

    while((len+appended*4) % 512 != 448){
        if (appended == 0){
            //Add a 1 followed by 0:s
            str.append("8");
        }
        else{
            str.append("0");
        }
        appended++;
        hadToPad = true;
    }

    //Add length field which consists of 64 bits
    std::stringstream lenS;
    lenS << std::hex << len;
    string leading0s = string( 16 - lenS.str().size(), '0');
    str.append(leading0s);
    str.append(lenS.str());

    return hadToPad;
}

//Returns a hexstring as a vector of 512-bit blocks
void getVector(vector<vector<uint32_t>>& v, string str){
    unsigned long len = str.length()/(128);
    for(uint32_t i = 0; i < len; i++) {
        vector<uint32_t> block;
        v.push_back(block);
        for(uint32_t j = 0; j < 16; j++){
            std::istringstream strm(str.substr(128*i+8*j, 8));
            uint32_t x;
            strm >> std::hex >> x;
            v[i].push_back(x);
        }
    }
}

void sha256(vector<vector<uint32_t>> blocks, uint32_t* K, uint32_t* hash){

    uint32_t H[] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    for (uint32_t i = 0; i < blocks.size(); i++) {
        vector<uint32_t> m = blocks[i];
        uint32_t W[64];

        //Calculate W with dynamic programming
        for (int t = 0; t < 64; t++) {
            if (t < 16) {
                W[t] = m[t];
            } else {
                W[t] = minusculeSignma1(W[t - 2]) + W[t - 7] + minusculeSignma0(W[t - 15]) + W[t - 16];
            }
        }

#ifdef DEBUG_OUTPUT
        cout << "--------" << endl;
        for (int t = 0; t < 64; t++) {
            cout << "W" << dec <<  t << ": ";
            cout << hex << W[t] << endl;
        }
        cout << "--------" << endl;
#endif

        //Set variables for the calculations
        uint32_t a = H[0];
        uint32_t b = H[1];
        uint32_t c = H[2];
        uint32_t d = H[3];
        uint32_t e = H[4];
        uint32_t f = H[5];
        uint32_t g = H[6];
        uint32_t h = H[7];

        //Calculate new values for the 8 previous variables
        for (int t = 0; t < 64; t++) {
            uint32_t T1 = h + capitalSigma1(e) + Ch(e,f,g) + K[t] + W[t];
            uint32_t T2 = capitalSigma0(a) + Maj(a,b,c);
#ifdef DEBUG_OUTPUT
            if (t == 0){
                cout << "h=" << h << endl;
                cout << "EP1(e)=" << EP1(e) << endl;
                cout << CH(e,f,g) << endl;
                cout << K[t] << endl;
                cout << W[t] << endl;
                cout << "--------" << endl;
                cout << "t=" << dec << t << endl << hex;
                cout << "K[t]=" << K[t] << endl;
                cout << "T1=" << T1 << endl;
                cout << "a=" << a << endl;
                cout << "b=" << b << endl;
                cout << "c=" << c << endl;
                cout << "d=" << d << endl;
                cout << "e=" << e << endl;
                cout << "f=" << f << endl;
                cout << "g=" << g << endl;
                cout << "h=" << h << endl;
                cout << "--------" << endl;
            }
#endif
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;

        }

        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];

    }

    //Copy the calculated hash to the provided buffer
    for(int i = 0; i < 8; i++){
        hash[i] = H[i];
    }

}

//----Operations-----

//Implements the circular right shift
uint32_t ROTR(uint32_t x, uint8_t n){
    return x >> n | x << 32-n;
}

//Implements the circular right shift
uint32_t ROTL(uint32_t x, uint8_t n){
    return x << n | x >> 32-n;
}

//Implements the shift right operation
uint32_t SHR(uint32_t x,uint8_t n){
    return x >> n;
}

//Implements addition modulo 32-bit
uint32_t addModulo32bit(uint32_t x, uint32_t y){
    return x+y;
}

//----Internal Functions-----
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
    return x & y ^ ~x & z;
}

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z){
    return x & y ^ x & z ^ y & z;
}

uint32_t capitalSigma0(uint32_t x){
    return ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22);
}

uint32_t capitalSigma1(uint32_t x){
    return ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25);
}

uint32_t  minusculeSignma0(uint32_t x){
    return ROTR(x,7) ^ ROTR(x,18) ^ SHR(x,3);
}

uint32_t  minusculeSignma1(uint32_t x){
    return ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10);
}

//-----Tests------
void testOperations(){
    assert(ROTR(1,1) == 0x80000000);
    assert(ROTL(0x80000000,1) == 1);
    assert(SHR(2,1) == 1);
    assert(addModulo32bit(0x80000001,0x80000000) == 1);
}

void testInternalFunctions(){
    assert(Ch(1,1,2) == 3);
    assert(Maj(3,1,0) == 1);
}
