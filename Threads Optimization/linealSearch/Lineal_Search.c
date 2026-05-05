/*
Autor: Saul Ascencion Cruz
30 de Abril del 2026
Lineal_Search Threads Optimization
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"
pthread_t Threads[2];
int FoundIdx = -1;

void *Lineal_Search(void *B);

typedef struct
{
  int **A;
  int startIdx;
  int endIdx;
  int number;
  int id;
} Args;

int main(int argc, char *argv[])
{
  // Lectura del tamaño de numeros a ordenar
  int n = atoi(argv[1]);
  // Lectura del numero a buscar
  int number = atoi(argv[2]);
  double utime0, stime0, wtime0,utime1, stime1, wtime1; //Variables para medición de tiempos
  // Calculo de la mitad del arreglo
  int midIdx = ((n - 1) / 2);
  // Asignacion de memoria dinamica
  int *A = malloc(sizeof(int) * n);
  if (A == NULL)
  {
    printf("Memory asign error\n");
    exit(-1);
  }
  // Lectura del contenido del arreglo dinamico
  for (int i = 0; i < n; i++)
  {
    scanf("%d", &A[i]);
  }
  // Inicializacion de hilos
  Args B[2];
  B[0].A = &A;
  B[0].startIdx = 0;
  B[0].endIdx = midIdx;
  B[0].number = number;
  B[0].id = 0;
  B[1].A = &A;
  B[1].startIdx = midIdx;
  B[1].endIdx = n - 1;
  B[1].number = number;
  B[1].id = 1;
  //******************************************************************	
	//Iniciar el conteo del tiempo para las evaluaciones de rendimiento
	//******************************************************************	
	uswtime(&utime0, &stime0, &wtime0);
  // Llamada de hilos
  for (int i = 0; i < 2; i++)
  {
    pthread_create(&Threads[i], NULL, Lineal_Search, &B[i]);
  }
  //Espera a que todos los hilos terminen
  for (int i = 0; i < 2; i++)
  {
    pthread_join(Threads[i], NULL);
  }
	//******************************************************************	
	//Evaluar los tiempos de ejecución 
	//******************************************************************
	uswtime(&utime1, &stime1, &wtime1);
/*  if(FoundIdx != -1)
  {
    printf("%d fue encontrado en el indice %d\n", number,FoundIdx);
  }
  else
  {
    printf("%d no se encuentra en el arreglo\n",number);
  }
*/
  //Liberar memoria
  free(A);
  //Mostrar los tiempos en formato exponecial
	printf("\n");
	printf("real (Tiempo total)  %.10e s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10e s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10e s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
}
void *Lineal_Search(void *B)
{
  //Castear los argumentos a su tipo original
  Args *Argm = (Args *)B;
  //Variable de parada
  char Found = 0;
  int i = 0;
  for (i = Argm->startIdx; i <= Argm->endIdx; i++)
  {
    if ((*Argm->A)[i] == Argm->number)
    {
      Found = 1;
      break;
    }
  }
  if (Found == 1)
  {
    FoundIdx = i;
    //Terminar el segundo hilo
    pthread_cancel(Threads[1 - Argm->id]);
  }
  return NULL;
}
