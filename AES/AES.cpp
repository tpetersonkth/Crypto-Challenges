/*
 * Aes 128-bit implementation
 * Author: Thomas Peterson
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "AES.h"

// Good references:
// http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
// https://en.wikipedia.org/wiki/Rijndael_S-box
// https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
// https://en.wikipedia.org/wiki/Rijndael_key_schedule

using namespace std;

void keyExpansion(uint8_t *key, uint32_t *w);
uint32_t subWord(uint32_t word);
uint32_t rotWord(uint32_t word);

void addRoundKey(uint8_t key, BLOCK state);
void subBytes(BLOCK block);

void printByte(uint8_t byte);
void printWord(uint32_t byte);
void printByteArray(uint8_t *array, int size);
void printBlock(BLOCK block);

int main(){
  uint8_t state[STATE_DIM][STATE_DIM];

  // Get key
  uint8_t key[16];
  for(int i = 0; i < 16; i++){
    key[i] = cin.get();
  }

  cout << "key: ";
  printByteArray(key,16);

  //Get blocks
  vector<BLOCK> blocklist;

  int row = 0;
  int col = 0;
  BLOCK block;
  uint8_t byte;
  while(!cin.eof()){
    byte = cin.get();
    block.set(row,col,byte);

    if (col == 3 && row == 3){
      blocklist.push_back(block);
      printBlock(block);
      block = *(new BLOCK);
    }

    row++;
    if (row == 4){
      row = 0;
      col++;
      if (col == 4){
        col = 0;
      }
    }
  }
  blocklist.push_back(block);

  /*
  //Copy first block to state
  for(int r; r < STATE_DIM; r++){
    for(int c; c < STATE_DIM; c++){
      state[r][c] = blocklist[0].get(r,c);
    }
  }
  */

  //blocklist.erase(blocklist.begin());

  uint32_t keys[Nb*(Nr+1)];
  keyExpansion(key,keys);


  return 0;
}

//-----Key schedule-----
void keyExpansion(uint8_t *key, uint32_t *w){
  uint32_t temp;

  int i = 0;


  while(i < Nk){
    w[i] = 0;
    for(int j = 0; j < 4; j++){
      w[i] |= key[4*i+j] << (3-j)*8;
    }
    i++;
  }

  i = Nk;
  while (i < Nb * (Nr+1)){
    uint32_t temp = w[i-1];

    if (i % Nk == 0){
      temp = rotWord(temp);
      temp = subWord(temp);
      temp ^= rcon[i/Nk] << 24;
    }
    else if (Nk > 6 and i % Nk == 4){//Not used when Nk=4
      temp = subWord(temp);
    }
    w[i] = w[i-Nk] ^ temp;
    i++;
  }

}

uint32_t subWord(uint32_t word){
  uint32_t temp = 0;
  for(int i = 0; i < STATE_DIM; i++){
    int offset = i*8;
    int index = (word >> offset) & 0xff;
    temp |= sbox[index] << offset;
  }
  return temp;
}

uint32_t rotWord(uint32_t word){
  return word << 8 | word >> 24;
}


//-----State manipulation-----

void addRoundKey(uint8_t key, BLOCK state){

}


void subBytes(BLOCK block){
  for(int r; r < STATE_DIM; r++){
    for(int c; c < STATE_DIM; c++){
      block.set(r,c, sbox[block.get(r,c)]);
    }
  }
}

//-----Debug-----
void printByte(uint8_t byte){
  cout << hex << setfill('0') << setw(2) << static_cast<unsigned>(byte) << endl;
}

void printWord(uint32_t word){
  cout << hex << setfill('0') << setw(8) << static_cast<unsigned>(word) << endl;
}

void printByteArray(uint8_t *array, int size){
  for(int i = 0; i < size; i++){
    cout << hex << setfill('0') << setw(2) << static_cast<unsigned>(array[i]);
  }
  cout << endl;
}

void printBlock(BLOCK block){
  for(int r = 0; r < STATE_DIM; r++) {
    for (int c = 0; c < STATE_DIM; c++) {
      cout << hex << setfill('0') << setw(2) << static_cast<unsigned>(block.get(r,c));
    }
    cout << endl;
  }
  cout << endl;
}