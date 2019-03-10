/*
 * Aes 128-bit implementation
 * Author: Thomas Peterson
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>

#include "AES.h"

// Good references:
// http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
// https://en.wikipedia.org/wiki/Rijndael_S-box
// https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
// https://en.wikipedia.org/wiki/Rijndael_key_schedule
//Testing: https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_Core128.pdf

using namespace std;

void cipher(BLOCK in, uint8_t out[4*Nb], uint32_t word[Nb*(Nr+1)]);

void keyExpansion(uint8_t *key, uint32_t *w);
uint32_t subWord(uint32_t word);
uint32_t rotWord(uint32_t word);

void addRoundKey(uint32_t *key, uint8_t state[][4]);
void subBytes(uint8_t state[][4]);
void shiftRows(uint8_t state[STATE_DIM][STATE_DIM]);
void mixColumns(uint8_t state[STATE_DIM][STATE_DIM]);

void printByte(uint8_t byte);
void printWord(uint32_t byte);
void printByteArray(uint8_t *array, int size);
void printBlock(BLOCK block);
void printState(uint8_t state[][4]);

int main(){

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

  cout << "empty block" << endl;
  printBlock(block);

  while(!cin.eof()){
    byte = cin.get();

    printWord(byte);
    cout << endl;

    block.set(row,col,(uint8_t)byte);

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
  cout << "pushing empty block" << endl;
  printBlock(block);
  blocklist.push_back(block);

  uint32_t keys[Nb*(Nr+1)];
  keyExpansion(key,keys);


  cout << "Starting encryption" << endl;
  for(int b = 0; b < blocklist.size(); b++){
    uint8_t out[4*Nb];
    cipher(blocklist[b], out, keys);
    printBlock(blocklist[b]);
    printByteArray(out,4*Nb);
  }



  //blocklist.erase(blocklist.begin());

  return 0;
}

void cipher(BLOCK in, uint8_t out[4*Nb], uint32_t word[Nb*(Nr+1)]){
  uint8_t state[STATE_DIM][STATE_DIM];
  memset(state, 0, sizeof state);

  //Copy first block to state
  for(int r = 0; r < STATE_DIM; r++){
    for(int c = 0; c < STATE_DIM; c++){
     state[r][c] = in.get(r,c);
    }
  }

  //cout << "Addroundkey:" << endl;
  addRoundKey(word,state);
  //printState(state);

  for(int round = 1; round < Nr; round++){
    //cout << "round " << dec << round << ", FIGHT!" << endl;
    //cout << "subBytes:" << endl;
    subBytes(state);
    //printState(state);

    //cout << "shiftRows:" << endl;
    shiftRows(state);
    //printState(state);

    //cout << "mixColumns:" << endl;
    mixColumns(state);
    //printState(state);

    //cout << "Addroundkey:" << endl;
    addRoundKey(word+Nb*round,state);
    //printState(state);
  }

  //cout << "subBytes:" << endl;
  subBytes(state);
  //printState(state);

  //cout << "shiftRows:" << endl;
  shiftRows(state);
  //printState(state);

  //cout << "Addroundkey:" << endl;
  addRoundKey(word+Nb*Nr,state);
  //printState(state);



  //Extract state into out
  for(int r = 0; r < STATE_DIM; r++){
    for(int c = 0; c < STATE_DIM; c++){
      out[STATE_DIM*c+r] = state[r][c];
    }
  }


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
void addRoundKey(uint32_t *key, uint8_t state[][4]){
  for(int c = 0; c < STATE_DIM; c++){
    state[0][c] ^= (key[c] & 0xff000000) >> 24;
    state[1][c] ^= (key[c] & 0xff0000) >> 16;
    state[2][c] ^= (key[c] & 0xff00) >> 8;
    state[3][c] ^= (key[c] & 0xff);
  }
}

void subBytes(uint8_t state[STATE_DIM][STATE_DIM]){
  for(int r = 0; r < STATE_DIM; r++){
    for(int c = 0; c < STATE_DIM; c++){
      state[r][c] = sbox[state[r][c]];
    }
  }
}

void shiftRows(uint8_t state[STATE_DIM][STATE_DIM]){
  for(int r = 0; r < STATE_DIM; r++){
    uint8_t temp[STATE_DIM];
    for(int c = 0; c < STATE_DIM; c++){
      temp[c] = state[r][(c+r) % Nb];
    }

    for(int c = 0; c < STATE_DIM; c++) {
      state[r][c] = temp[c];
    }
  }
}

void mixColumns(uint8_t state[STATE_DIM][STATE_DIM]){
  for(int c = 0; c < STATE_DIM; c++){
    uint8_t temp[STATE_DIM];
    temp[0] = GF2[state[0][c]] ^ GF3[state[1][c]] ^ state[2][c] ^ state[3][c];
    temp[1] = state[0][c] ^ GF2[state[1][c]] ^ GF3[state[2][c]] ^ state[3][c];
    temp[2] = state[0][c] ^ state[1][c] ^ GF2[state[2][c]] ^ GF3[state[3][c]];
    temp[3] = GF3[state[0][c]] ^ state[1][c] ^ state[2][c] ^ GF2[state[3][c]];

    for(int r = 0; r < STATE_DIM; r++) {
      state[r][c] = temp[r];
    }
  }
}

//-----Debug-----
void printByte(uint8_t byte){
  cout << hex << setfill('0') << setw(2) << uppercase << static_cast<unsigned>(byte);
}

void printWord(uint32_t word){
  cout << hex << setfill('0') << setw(8) << uppercase << static_cast<unsigned>(word) << endl;
}

void printByteArray(uint8_t *array, int size){
  for(int i = 0; i < size; i++){
    cout << hex << setfill('0') << setw(2) << uppercase << static_cast<unsigned>(array[i]);
  }
  cout << endl;
}

void printBlock(BLOCK block){
  for(int r = 0; r < STATE_DIM; r++) {
    for (int c = 0; c < STATE_DIM; c++) {
      cout << hex << setfill('0') << setw(2) << uppercase << static_cast<unsigned>(block.get(r,c));
    }
    cout << endl;
  }
  cout << endl;
}

void printState(uint8_t state[][4]){
  for(int r = 0; r < STATE_DIM; r++) {
    for (int c = 0; c < STATE_DIM; c++) {
      cout << hex << setfill('0') << setw(2) << uppercase << static_cast<unsigned>(state[r][c]);
    }
    cout << endl;
  }
  cout << endl;
}