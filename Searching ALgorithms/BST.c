/*
Autor: Saul Ascencion Cruz
1 de mayo 2026
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "BST_TAD.h"
#include "tiempo.h"

int main(int argc, char *argv[])
{
    //Leer el tamaño del arreglo
    int n = atoi(argv[1]);
    //Leer el umero a buscar
    int number = atoi(argv[2]);
    //Crear arreglo
    elemento *A = malloc(sizeof(int) * n);
    double utime0, stime0, wtime0,utime1, stime1, wtime1; //Variables para medición de tiempos 
    //Leer arreglo
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &A[i].numero);
        A[i].RelativeIdx = i;
    }
    //Crear ABB
    arbolBinario B;
    //Inicializar ABB
    Inicializar(&B);
 //******************************************************************	
	//Iniciar el conteo del tiempo para las evaluaciones de rendimiento
	//******************************************************************	
	uswtime(&utime0, &stime0, &wtime0);
  for(int i = 0; i <n; i++)
  {
    Insert(&B, A[i]);
  }
  //Creamos un dato elemento necesario segun la BTS_TAD
  elemento e;
  //Asignamos a este elemento el numero que queremos buscar
  e.numero = number;
  //Buscamos el indice e donde se encuentra el numero deseado
  int Idx = Buscar(&B, e);
  //******************************************************************	
	//Evaluar los tiempos de ejecución 
	//******************************************************************
	uswtime(&utime1, &stime1, &wtime1);
    //Si la funcion encontro al numero entonces imprime su ubicacion en el arreglo
    if (Idx != -1)
    {
        printf("El numero %d fue encontrado en la posicion %d\n", number, Idx);
    } 
    //Si no se encontro el numero entonces nos lo indica
    else
    {
        printf("El numero %d no se encuentra en el arreglo\n", number);
    }

    //Liberacion de memoria
    free(A);
    Destruir(&B);
 //Mostrar los tiempos en formato exponecial
	printf("\n");
	printf("real (Tiempo total)  %.10e s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10e s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10e s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));

    return 0;
}
