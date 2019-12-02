#ifndef READERH_H_INCLUDED
#define READERH_H_INCLUDED
#include <string>
#include "Server.h"
#include <bitset>
#include "Tag.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
using namespace std;

class Creader{
private:
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

    bitset<8>ma;
    bitset<8>mb;
    bitset<8>md;


public:
    Creader()
    {
        mMessage="Hola";
        srand (time(NULL));
    }
    //PASO 1
    bool request_authorization(Cserver& server)
    {
        mC=server.verify_authorization(mMessage);
        if(mC=="unauthorized")
            return false;
        cout<<"Certificado de acceso de un dia es: "<< mC<<endl;
        return true;
    }
    //PASO 2
    void IDrequestPID2(Ctag& tag){
      mPID2=tag.get_PID2();
      cout<<"PID2 : "<<mPID2<<endl;
    }
    //paso 2 y 3
    void IDrequestPID(Ctag& tag){
      mPID=tag.get_PID();
      cout<<"PID : "<<mPID<<endl;
    }

    //paso 3
    bool send_PID2_C(Cserver& server){
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
    bool send_PID_C(Cserver& server){
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
    void send_ABD()
    {
         mn1=rand() % 254 + 1; //random
         mn2=rand() % 200 << 2; //random

         cout<<"n1 "<<mn1<<endl;
         cout<<"n2 "<<mn2<<endl;
         cout<<"mPID2 "<<mPID2<<endl;
         cout<<"mk1 "<<mK1<<endl;
         cout<<"mk2 "<<mK2<<endl;

         ma=(mPID2 & mK1 & mK2) ^ mn1;
         mb=(~mPID2 & mK2 & mK1) ^ mn2;
         md=(mK1 & mn2)^(mK2 & mn1);

         cout<<"A "<<ma<<endl;
         cout<<"B "<<mb<<endl;
         cout<<"D "<<md<<endl;
    }
    bitset<8> get_a(){return ma;}
    bitset<8> get_b(){return mb;}
    bitset<8> get_d(){return md;}
    //PASO5
    void set_e(bitset<8> e){
    	me=e;
        cout<<"E "<<me<<endl;
    }
    void set_f(bitset<8> f){
    	mf=f;
        cout<<"F "<<mf<<endl;
    }
    //PASO 6
    void get_ID()
    {
        bitset<8>f;
        cout<<"READER"<<endl;
        f=(mK1&mn1)^(mK2&mn2);

        if(mf==f)
        {
            cout<<"Etiqueta autenticada"<<endl;
            mID=me^(mK2&mn2)^mK1^mn1;
            mPID=mPID2;
            mPID2=mPID2^mn1^mn2;

            cout<<"reader ID "<<mID<<endl;
            cout<<"reader PID "<<mPID<<endl;
            cout<<"reader PID2 "<<mPID2<<endl;

        }
    }
    void send_PID_PID2_C(Cserver& server)
    {
        if(server.verify_C(mC))
        {
            cout<<"SEND A SERVER "<<endl;
            cout<<"reader PID "<<mPID<<endl;
            cout<<"reader PID2 "<<mPID2<<endl;
            server.set_PID(mPID);
            server.set_PID2(mPID2);
        }
    }
};

#endif // READERH_H_INCLUDED
