#ifndef SERVER_H
#define SERVER_H
#include "readerh.h"
#include <string>
#include "Tag.h"
#include <bitset>
#include <iostream>
using namespace std;

class Cserver{
private:
    string mKey_authorization;
    bitset<8> mPID;
	bitset<8> mPID2;
	bitset<8> mK1;
    bitset<8> mK2;
    string mC;

public:
    Cserver(){
        mK1=17; //ESTABLE
        mK2=13; //ESTABLE
        mPID=32; //ALEATORIO
        mPID2=45; //ALEATORIO
        mKey_authorization="Hola";
        mC="authorization1xyz"; //Aleatorio por dia
    }
  //PASO 1
    string verify_authorization(string message){
        if(mKey_authorization == message)
            return mC;
        else
            return "unauthorized";
    }

    bool verify_C(string C)
    {
        return (mC==C);
    }

    bool verify_PID2_C(bitset<8> pid2, string C)
    {
        cout<< " mPID2 SERVER: " << mPID2 <<" PID2 PASO"<< pid2 << endl;
        return mC==C && mPID2 == pid2;

    }

    bool verify_PID_C(bitset<8> PID, string C)
    {
        cout<< "  " << mPID << "  "<< PID << endl;
        return mC==C && mPID == PID;
    }

    bitset<8>get_k1(){
			return mK1;
    }
    bitset<8>get_k2(){
			return mK2;
    }
    void set_PID( bitset<8> PID){
        mPID=PID;
    }
    void set_PID2( bitset<8> PID2){
        mPID2=PID2;
    }
    bitset<8>get_PID(){
        return mPID;
    }
    bitset<8>get_PID2(){
        return mPID2;
    }
};

#endif // SERVER_H
