#include<stdio.h>
#include<mpi.h>
#include<fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){

  int tamano, procesador, cont=0,final,cadauno,conta,numero[2];
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &tamano); // obtenems el tamaño o np
  MPI_Comm_rank(MPI_COMM_WORLD, &procesador);// captura el procesador
		string linea;
	ifstream fixero(argv[1]);
	
	if(procesador==0)
	{

		fixero.seekg(0, fixero.end); 
	    final = fixero.tellg();
	    cadauno=final/tamano;
	    fixero.seekg(0);
	    for(int i=1;i<tamano;i++)
	    {
	    	numero[0]=cadauno*i;
	    	numero[1]=cadauno*(i+1);
	    	MPI_Send(numero, 2, MPI_INT, i, 0, MPI_COMM_WORLD);

	    }

		 while(cadauno> fixero.tellg())
		{
			getline(fixero,linea);
			cont++;
		}
		cout<<"Lineas Procesador 0"<<" "<<cont<<endl;
		for(int i=1;i<tamano;i++)
		 {
		 	MPI_Recv(&conta, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		 	cout<<"Lineas Procesador "<<i<<" "<<conta<<endl;
		 	cont=conta+cont;
		 }
		 cout<<"Total lineas "<<cont<<endl;

	}
	else
	{
		 MPI_Recv(numero, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		 fixero.seekg(numero[0]);
		 while(numero[1]>fixero.tellg())
		{
			getline(fixero,linea);
			cont++;
		}
		MPI_Send(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	fixero.close();

  MPI_Finalize();
}