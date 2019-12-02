#ifndef READER_H
#define READER_H
#include <string>
#include "server.h"
#include <bitset>
#include "Tag.h"
#include <iostream>
using namespace std;

class Creader{
    bitset<8>mPID;
    bitset<8>mPID2;
    string mMessage;
    bitset<8>mK1;
    bitset<8>mK2;
    string mC;
    bitset<8>mID;

    bitset<8>mn1;
    bitset<8>mn2;
    bitset<8>me;
    bitset<8>mf;


public:
    Creader()
    {
        mMessage="Hola";
    }
    //PASO 1
    bool request_authorization(Cserver server)
    {
        mC=server.verify_authorization(mMessage);
        if(mC=="unauthorized")
            return false;
        cout<<"Certificado de acceso de un dia es: "<< mC<<endl;
        return true;
    }
    //PASO 2
    void IDrequestPID2(Ctag tag){
      mPID2=tag.get_PID2();
      cout<<"PID2 : "<<mPID2<<endl;
    }
    //paso 2 y 3
    void IDrequestPID(Ctag tag){
      mPID=tag.get_PID();
      cout<<"PID : "<<mPID<<endl;
    }

    //paso 3
    bool send_PID2_C(Cserver server){
        if(server.verify_PID2_C(mPID2,mC))
        {
            mK1= server.get_k1();
            mK2= server.get_k2();
            cout<<"K1 recibido "<<mK1<<endl;
            cout<<"K2 recibido "<<mK2<<endl;
            return true ;
        }
        else{
            cout<<"Invalido PID2 " <<endl;
            return false;
        }
    }
    //paso 3
    bool send_PID_C(Cserver server){
        if(server.verify_PID_C(mPID,mC))
        {
            mK1= server.get_k1();
            mK2= server.get_k2();
            cout<<"K1 recibido "<<mK1<<endl;
            cout<<"K2 recibido "<<mK2<<endl;
            return true ;
        }
        else{
            cout<<"Invalido PID "<<endl;
            return false;
        }
    }
    //PASO 4
    void send_ABD(Ctag tag)
    {
         mn1=18; //random
         mn2=16; //random

         bitset<8>a=(mPID2 & mK1 & mK2) ^ mn1;
         bitset<8>b=(~mPID2 & mK2 & mK1) ^ mn2;
         bitset<8>d=(mK1 & mn2)^(mK2 & mn1);

         tag.set_a(a);
         tag.set_b(b);
         tag.set_d(d);

         cout<<"A "<<a<<endl;
         cout<<"B "<<b<<endl;
         cout<<"D "<<d<<endl;
    }

    void set_e(bitset<8> e){
    	me=e;
    }
    void set_f(bitset<8> f){
    	mf=f;
    }
    //PASO 6
    void get_ID()
    {
        bitset<8>f=(mK1&mn1)^(mK2&mn2);
        if(mf==f)
        {
            cout<<"Etiqueta autenticada"<<endl;
            mID=me^(mK2&mn2)^mK1^mn1;
            mPID=mPID2;
            mPID2=mPID2^mn1^mn2;
        }
    }

    void send_PID_PID2_C(Cserver server)
    {
        if(server.verify_C(mC))
        {
            server.set_PID(mPID);
            server.set_PID2(mPID2);
        }
    }
};
#endif // READER_H
