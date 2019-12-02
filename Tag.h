#ifndef TAG_H
#define TAG_H
#include <bitset>
#include <string>
#include "readerh.h"
#include "Server.h"
#include <vector>
#include <algorithm>
using namespace std;

namespace std {
  template <typename _CharT, typename _Traits>
  inline basic_ostream<_CharT, _Traits> &
  tab(basic_ostream<_CharT, _Traits> &__os) {
    return __os.put(__os.widen('\t'));
  }
}

class Ctag{
private:
  bitset<8>mPID;
  bitset<8>mPID2;
  bitset<8>mK1;
  bitset<8>mK2;
  bitset<8>mID;

  bitset<8>ma;
  bitset<8>mb;
  bitset<8>md;
  bitset<8>mn1;
  bitset<8>mn2;

  bitset<8>me;
  bitset<8>mf;
public:
    Ctag(){
        mK1=17; //ESTABLE
        mK2=13; //ESTABLE
        mPID=32; //ALEATORIO
        mPID2=45; //ALEATORIO
        mID=7; //ALEATORIO al inicio luego estable
    }
//PASO 2
    bitset<8> get_PID2(){
        return  mPID2;
    }
//paso 2 y 3
    bitset<8> get_PID(){
        return  mPID2;
    }
//paso 4
    void set_a(bitset<8>a){
        ma=a;
    }
    void set_b(bitset<8>b){
        mb=b;
    }
    void set_d(bitset<8>d){
        md=d;
    }
    //PASO 5
    bool infer_n1_n2()
    {
        mn1=(mPID2 & mK1 & mK2) ^ ma;
        mn2=(~mPID2 & mK2 & mK1) ^ mb;
        bitset<8>d= (mK1 & mn2)^(mK2 & mn1);
        cout<<"md "<<md<<endl;
        cout<<"d "<<d<<endl;
        if(d==md)
        {
            cout<<"TAG"<<endl;
            cout<<"K1 "<<mK1<<endl;
            cout<<"k2 "<<mK2<<endl;
            cout<<"n1 "<<mn1<<endl;
            cout<<"n2 "<<mn2<<endl;
            me= (mK1^mn1^mID)^(mK2&mn2);
            mf= (mK1&mn1)^(mK2&mn2);
            cout<<"Lector autentificado"<<endl;
            return 1;
        }
        else
        {
            cout<<"Lector NO autentificado"<<endl;
            return 0;

        }
    }
    bitset<8> get_e()
    {
        return me;
    }
    bitset<8> get_f()
    {
        return mf;
    }
    void updates_values()
    {
        cout<<"TAG"<<endl;
        mPID=mPID2;
        mPID2=mPID2^mn1^mn2;
        cout << "update PID: "<< mPID<<endl;
        cout << "update PID2: "<< mPID2<<endl;
    }

    void get_arrayX(bitset<8> arrayX[31])
    {
        cout<< " tabla hamming X "<<endl;
        arrayX[0]=ma;
        arrayX[1]=mb;
        arrayX[2]=md;
        arrayX[3]=me;
        arrayX[4]=mf;
        arrayX[5]=ma^mb;
        arrayX[6]=ma^md;
        arrayX[7]=ma^me;
        arrayX[8]=ma^mf;
        arrayX[9]=mb^md;
        arrayX[10]=mb^me;
        arrayX[11]=mb^mf;
        arrayX[12]=md^me;
        arrayX[13]=md^mf;
        arrayX[14]=me^mf;
        arrayX[15]=ma^mb^md;
        arrayX[16]=ma^mb^me;
        arrayX[17]=ma^mb^mf;
        arrayX[18]=ma^md^me;
        arrayX[19]=ma^md^mf;
        arrayX[20]=ma^me^mf;
        arrayX[21]=mb^md^me;
        arrayX[22]=mb^md^mf;
        arrayX[23]=mb^me^mf;
        arrayX[24]=md^me^mf;
        arrayX[25]=ma^mb^md^me;
        arrayX[26]=ma^mb^md^mf;
        arrayX[27]=ma^mb^me^mf;
        arrayX[28]=ma^md^me^mf;
        arrayX[29]=mb^md^me^me;
        arrayX[30]=ma^mb^md^me^mf;

        for(int i=0;i<31;i++)
            cout<< i<<": "<<arrayX[i]<<endl;
    }
  //*******************************DESCENCRIPTAR*****************
    int dist(bitset<8> x, bitset<8> k1k2id){
        bitset<8> temp=(x^k1k2id);
        return temp.count();
    }
    void iteration_TABLE_HAMMING( bitset<8> arrayX[31] , float matrixpos[31][3], int size )
    {
        bitset<8> arrayk1k2id[3]={mK1,mK2,mID};
        for(int i=0; i<size; i++)
            for(int j=0; j<3; j++)
                matrixpos[i][j] += dist(arrayX[i], arrayk1k2id[j]);   //ARMAR FUNCION DIST que son los difrentes la suma
    }
    void finish_TABLE_HAMMING( float matrixpos[31][3],float matrix_result[31][3], int size, int iterations )
    {
        for(int i=0; i<size; i++)
            for(int j=0; j<3; j++){
                matrix_result[i][j] = matrixpos[i][j]/iterations + 0.0 ;   //ARMAR FUNCION DIST que son los difrentes la suma
            }
    }
    void  good_aproximations(bitset<8> arrayX[31], float  matrix_result[31][3], int row, int colum, int iterations){
        vector<float> k1;
        vector<float> k2;
        vector<float> id;
        int min_pos;
        vector<bitset<8> > k1_ga;
        vector<bitset<8> > k2_ga;
        vector<bitset<8> > id_ga;
        bitset<8> temp;

        for(int i=0; i<row; i++){
            k1.push_back(matrix_result[i][0]);
            k2.push_back(matrix_result[i][1]);
            id.push_back(matrix_result[i][2]);
        }

        for(int i=0; i<row; i++){
           cout<< k1[i] <<tab ;
           cout<< k2[i] <<tab;
           cout<< id[i] <<endl;
        }
        for(int i=0;i<4;i++)
        {
            min_pos = distance( k1.begin(),min_element(k1.begin(),k1.end()));
            k1.erase (k1.begin()+min_pos);
            temp=arrayX[min_pos];
            k1_ga.push_back(temp);

            min_pos = distance( k2.begin(),min_element(k2.begin(),k2.end()));
            k2.erase (k2.begin()+min_pos);
            temp=arrayX[min_pos];
            k2_ga.push_back(temp);

            min_pos = distance( id.begin(),min_element(id.begin(),id.end()));
            id.erase (id.begin()+min_pos);
            temp=arrayX[min_pos];
            id_ga.push_back(temp);
        }
        for(int i=0;i<4;i++)
        {
            min_pos = distance( k1.begin(),max_element(k1.begin(),k1.end()));
            k1.erase (k1.begin()+min_pos);
            temp=arrayX[min_pos];
            k1_ga.push_back(temp);

            min_pos = distance( k2.begin(),max_element(k2.begin(),k2.end()));
            k2.erase (k2.begin()+min_pos);
            temp=arrayX[min_pos];
            k2_ga.push_back(temp);

            min_pos = distance( id.begin(),max_element(id.begin(),id.end()));
            id.erase (id.begin()+min_pos);
            temp=arrayX[min_pos];
            id_ga.push_back(temp);
        }
        int new_row=8*iterations;
        for(int i=0; i<new_row; i++)
        {
            cout<<k1_ga[i]<<tab;
            cout<<k2_ga[i]<<tab;
            cout<<id_ga[i]<<endl;
        }

        int k1_accum[8];
        int k2_accum[8];
        int id_accum[8];

        for(int i=0;i<8;i++)
        {
            k1_accum[i]=0;
            k2_accum[i]=0;
            id_accum[i]=0;
        }

        for(int i=0;i<8;i++)
        {
            for(int j=0;j<new_row;j++){
                k1_accum[i]+=(k1_ga[j])[7-i];
                k2_accum[i]+=(k2_ga[j])[7-i];
                id_accum[i]+=(id_ga[j])[7-i];
            }
        }
        bool result[3][8];

        int umbral=4*iterations;

        cout<<"K1 ";
        for(int i=0;i<8;i++){
            result[0][i]= (k1_accum[i]<umbral)?0:1;
            cout<<k1_accum[i]<<" ";
        }cout<<endl;
        cout<<"K2 ";
        for(int i=0;i<8;i++){
            result[1][i]= (k2_accum[i]<umbral)?0:1;
            cout<<k2_accum[i]<<" ";
        }cout<<endl;
        cout<<"ID ";
        for(int i=0;i<8;i++){
            result[2][i]= (id_accum[i]<umbral)?0:1;
            cout<<id_accum[i]<<" ";
        }cout<<endl;
        //resultados
        cout<<"UMBRAL: "<<umbral<<endl;
        for(int i=0;i<3;i++){
            for(int j=0;j<8;j++)
                cout<<result[i][j]<<" ";
            cout<<endl;
        }
    }

};


#endif // TAG_H
