#include <iostream>
#include "readerh.h"
#include "Tag.h"
#include "readerh.h"
#include <string>
#include "Server.h"
#include "conio.h"
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;
//ESTE ES EL ULTIMOOO Y MEJORADOOO CON HAMMING
int iterations=0;


void  initialize_matrix(float matrixpos[31][3], int r, int c)
{
    for(int i=0; i<r; i++)
        for(int j=0; j<c; j++)
            matrixpos[i][j]=0;
}
void  print_matrix(float matrixpos[31][3], int r, int c)
{
    cout<<"-------------IMPRESION MATRIZ-----------"<<endl;
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++)
            cout<<matrixpos[i][j]<<tab;
        cout<<endl;
    }
}


void RFID( Creader reader, Cserver server, Ctag tag, bitset<8> arrayX[31] , float  matrix_accum[31][3],float  matrix_result[31][3], int row, int colum )
{ //MIS VARIABLES NO CAMBIAN EN CADA ITERACION ABDEF :( PORQUEEEEEEEEEEEEEE
    char tecla = 'a';//getch();
    cout<<"presione 'x' para terminar y 'a' para continuar  "<<endl;
    while(cin>>tecla){
        if(tecla == 'a' ){
            iterations+=1;
            //PASO 1
            cout<<"***Paso 1: verificar autorizacion del LECTOR"<<endl;
            if(reader.request_authorization(server))
            {
                cout<<"El lector esta autorizado por el servidor"<<endl;
                //PASO 2: Lector ya autOrizado
                cout<<"***Paso 2: "<<endl;
                reader.IDrequestPID2(tag);
                //PASO 3
                cout<<"***Paso 3"<<endl;
                if(!reader.send_PID2_C(server))
                {
                    reader.IDrequestPID(tag);
                    if(!reader.send_PID_C(server))
                        cout<<"No permitido "<<endl; //TERMINAR
                }
                //PASO 4
                cout<<"***PASO 4"<<endl;
                reader.send_ABD();
                tag.set_a(reader.get_a());
                tag.set_b(reader.get_b());
                tag.set_d(reader.get_d());
                //PASO 5
                cout<<"***PASO 5"<<endl;

                if(tag.infer_n1_n2())
                {
                    reader.set_e(tag.get_e());
                    reader.set_f(tag.get_f());
                    tag.updates_values();
                          //  PASO 6
                    cout<<"***PASO 6"<<endl;
                    reader.get_ID();
                    reader.send_PID_PID2_C(server);
                }
                    //por cada iteraccion va cambiando TABLA HAMMING
                    tag.get_arrayX(arrayX);
                    tag.iteration_TABLE_HAMMING(arrayX , matrix_accum, row );
                    print_matrix(matrix_accum, row, colum);

                    tag.finish_TABLE_HAMMING(matrix_accum, matrix_result, row, iterations);
                    cout<<"RESULT"<<endl;
                    print_matrix(matrix_result, row, colum);

                   // hacer funcion ponde pase float  matrix_result[31][3] a un vector y ese le paso
                    cout<<"GOOD APROXIMATION"<<endl;
                    tag.good_aproximations( arrayX, matrix_result, row,colum,iterations);
            }
            else{
                cout<<"El lector No tiene autorizacion del servidor"<<endl;
            }
        }
        else break;
    }
}

int main()
{
    Creader reader;
    Cserver server;
    Ctag tag;

    int row=31;
    int colum=3;
    bitset<8> arrayX[row];

    float matrix_accum[31][3];
    float matrix_result[31][3];

    initialize_matrix(matrix_accum, row, colum);
   // print_matrix(matrix_accum, row, colum);
    RFID(reader,server,tag, arrayX , matrix_accum, matrix_result, row, colum);  //ITERACIONEEEEEEES :)



    return 0;
}
