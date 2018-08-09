#include<stdio.h>
#include<mpi.h>
#include<fstream>
#include <iostream>
using namespace std;


int contarlargo(char *direct)
{
	int cont=-1,numero[2];
	string linea;
	ifstream fixero(direct);
	MPI_Recv(numero, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   //cuenta cada procesador
    fixero.seekg(numero[0]);
	while(numero[1]>=fixero.tellg() && fixero.tellg()!=-1)
		{
			getline(fixero,linea);
			cont++;
		}

	fixero.close();
	return cont;
}

void repartiendo(char *direct,int tamano)
{
	int cont=-1, numero[2], cadauno,final,conta;
	string linea;
	ifstream fixero(direct);
	fixero.seekg(0, fixero.end); 
	final = fixero.tellg();
	cadauno=final/tamano;
	    
	if(final%tamano==0) ////////////////////////////////cuando la division es exacta se debe restar un numero para dar con el resultado exacto
	    cont--;
////////////////////////////////////////////////////////////////////

	fixero.seekg(0);
	for(int i=1;i<tamano;i++) //envia inicio y final a cada procesador
	{
	    numero[0]=cadauno*i;
	    numero[1]=cadauno*(i+1);
	    MPI_Send(numero, 2, MPI_INT, i, 0, MPI_COMM_WORLD);

	}
	/////////////////////////////////////////////////////////////////////////////////////////

    while(cadauno>=fixero.tellg())                       //cuenta las lineas de procesador 0
	{
		getline(fixero,linea);
		cont++;
	}
	cout<<"Lineas Procesador 0"<<" "<<cont<<endl;

	////////////////////////////////////////////////////////////////////////////////////////
	for(int i=1;i<tamano;i++)
	{
		 MPI_Recv(&conta, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		 cout<<"Lineas Procesador "<<i<<" "<<conta<<endl;
		 cont=conta+cont;
	}
	cout<<"Total lineas "<<cont<<endl;

    fixero.close();
}

int main(int argc, char* argv[]){

  	int tamano, procesador, cont=-1,final,cadauno,conta,numero[2];
  	MPI_Init(&argc,&argv);
  	MPI_Comm_size(MPI_COMM_WORLD, &tamano); // obtenems el tamaño o np
  	MPI_Comm_rank(MPI_COMM_WORLD, &procesador);// captura el procesador

	
	if(procesador==0)
	{
		repartiendo(argv[1],tamano);
	}
	else
	{
		conta=contarlargo(argv[1]);
		MPI_Send(&conta, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}


  MPI_Finalize();
}