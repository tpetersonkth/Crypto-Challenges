// io/read-file-sum.cpp - Read integers from file and print sum.
// Fred Swartz 2003-08-20

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

bool getVector(vector<uint32_t>& vector,string str);

int main() {

    vector<uint32_t> blocks;

    cout << "starting\n";

    vector <std::string>Data;

    string str;
    while (getline(cin, str))
    {
        cout << "Line: " << str << endl;
        getVector(blocks,str);
        cout << "vector len: " << blocks.size() << endl;
    }

    for (uint32_t block : blocks) {
        cout << block << endl;
    }

    cout << "Tot size: " << Data.size() << endl;

    return 0;
}

//Returns a hexstring as a vector of 32-bit blocks
bool getVector(vector<uint32_t>& vector,string str){
    uint32_t len = str.length();
    bool hadToPad = false;
    for(uint32_t i = 0; i < len; i += 8) {
        int zeroes = 0;
        while(len-i+zeroes < 8){
            if (zeroes == 0){
                cout << "Appending x80";
                str.append("8");
            }
            else{
                str.append("0");
            }
            zeroes++;
            hadToPad = true;
        }
        std::istringstream strm(str.substr(i, 8));
        uint32_t x;
        strm >> std::hex >> x;
        vector.push_back(x);
    }

    return hadToPad;
}