/*
Autor: Saul Ascencion Cruz
1 de Mayo del 2026
Parallel Merge Sort & Binary Search Optimization

Run: ./Binary_Search [Array Length] [Number to find] <[Array]
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

typedef struct Data
{
  //Numero en esa posicion especifica del arreglo
  int numero;
  //Indice en el arreglo original
  int Idx;
} Data;

typedef struct
{
  // Puntero al arreglo de estructuras a ordenar
  Data *A;    
  // Indice inicial del sub-arreglo (Límite izquierdo)
  int p;
  //Indice final del sub-arreglo (Limite deecho)
  int r;
  // Nivel de profundidad para limitar la creación de hilos
  int depth;
} Args;

//Ordena en un nuevo arreglo para mezclar y luego lo copia al arreglo original
void Sort(Data A[], int p, int q, int r);
//Crea tatos hilos como el sistema y la computadora pueden, luego ordena secuencialmente
void *MergeSort(void *args);
//Realiza una busqueda binaria basica
int Bin(Data A[], int n, int number);

int main(int argc, char *argv[])
{
  // Lectura del tamaño de numeros a ordenar
  int n = atoi(argv[1]);
  // Lectura del numero a buscar
  int number = atoi(argv[2]);
  //Indice original del numero en el arreglo
  int fIdx;
  double utime0, stime0, wtime0,utime1, stime1, wtime1; //Variables para medición de tiempos

  // Asignacion de memoria dinamica
  Data *A = malloc(sizeof(Data) * n);
  // Lectura del contenido del arreglo dinamico
  for (int i = 0; i < n; i++)
  {
    //Numero de esa posicion del arreglo
    scanf("%d", &A[i].numero);
    //Indice de esa posicion en el arreglo antes de hacerle algun cambio
    A[i].Idx = i;
  }

  // Inicializacion de argumentos
  Args B;
  //Arreglo original
  B.A = A;
  //Lado izquierdo
  B.p = 0;
  //Lado derecho
  B.r = n - 1;
  //Que tantos hilos queremos dado por 2^n, en este caso seran a lo mas 2 hilos por
  //las capacidades de la computadora
  B.depth = 1;

  //******************************************************************	
	//Iniciar el conteo del tiempo para las evaluaciones de rendimiento
	//******************************************************************	
	uswtime(&utime0, &stime0, &wtime0);

  // Llamada al algoritmo de ordenamiento paralelo
  MergeSort(&B);

  // Busqueda binaria sobre el arreglo ya ordenado
  fIdx = Bin(A, n, number);

	//******************************************************************	
	//Evaluar los tiempos de ejecución 
	//******************************************************************
	uswtime(&utime1, &stime1, &wtime1);

/*  if(fIdx != -1)
  {
    printf("%d fue encontrado en el indice %d\n", number, fIdx);
  }
  else
  {
    printf("%d no se encuentra en el arreglo\n", number);
  }
*/
  //Liberar memoria
  free(A);

  //Mostrar los tiempos en formato exponencial
  printf("\n");
  printf("real (Tiempo total) %.10e s\n", wtime1 - wtime0);
  printf("user (Tiempo de procesamiento en CPU) %.10e s\n", utime1 - utime0);
  printf("sys (Tiempo en acciones de E/S) %.10e s\n", stime1 - stime0);
  printf("CPU/Wall %.10f %% \n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));}

void *MergeSort(void *args)
{
  // Castear el puntero genérico void* a la estructura Args para acceder a los datos
  Args *Argm = (Args *)args;
  int p = Argm->p;
  int r = Argm->r;

  if (p < r)
  {
    // Calcular el punto medio
    int q = p + (r - p) / 2;

    //Si depth > 0 se crean nuevos hilos
    if (Argm->depth > 0)
    {
      pthread_t nuevo;
      // Preparar argumentos para la mitad izquierda y derecha reduciendo la profundidad
      Args left = {Argm->A, p, q, Argm->depth - 1};
      Args right = {Argm->A, q + 1, r, Argm->depth - 1};
      // Crear un hilo hijo para que procese la mitad izquierda del arreglo
      pthread_create(&nuevo, NULL, MergeSort, &left);
      
      // El hilo actual procesa la mitad derecha recursivamente
      MergeSort(&right);
      //Esperamos a que termine el otro nuevo hilo
      pthread_join(nuevo, NULL);
    }
    else
    {
      // Si ya no se permiten más hilos, se procede de forma secuencial
      Args left = {Argm->A, p, q, 0};
      Args right = {Argm->A, q + 1, r, 0};
      MergeSort(&left);
      MergeSort(&right);
    }

    // Una vez que ambas mitades están ordenadas, se mezclan en orden ascendente
    Sort(Argm->A, p, q, r);
  }
  return NULL;
}

void Sort(Data A[], int p, int q, int r)
{
  // Calcular el tamaño necesario para el arreglo temporal
  int l = r - p + 1;
  // Reservar memoria para el buffer temporal de tipo Data (mantiene numero e Idx)
  Data *C = malloc(sizeof(Data) * l);
  
  // i: puntero mitad izquierda, j: puntero mitad derecha, k: índice del temporal
  int i = p, j = q + 1, k = 0;

  // Comparar elementos de ambas mitades y mover el menor al arreglo temporal C
  while (i <= q && j <= r)
  {
    if (A[i].numero <= A[j].numero)
      C[k++] = A[i++]; // El elemento de la izquierda es menor o igual
    else
      C[k++] = A[j++]; // El elemento de la derecha es menor
  }

  // Si quedaron elementos en la mitad izquierda, se copian al final de C
  while (i <= q)
    C[k++] = A[i++];

  // Si quedaron elementos en la mitad derecha, se copian al final de C
  while (j <= r)
    C[k++] = A[j++];

  // Copiar los elementos ya ordenados del buffer C de vuelta al arreglo original A
  for (i = 0; i < l; i++)
  {
    A[p + i] = C[i];
  }
  
  // Liberar la memoria del buffer temporal para evitar fugas (memory leaks)
  free(C);
}
int Bin(Data A[], int n, int number)
{
  int fIdx = -1;
  int l = 0;
  int r = n - 1;
  while (l <= r)
  {
    int mid = l + (r - l) / 2;
    if (A[mid].numero == number)
    {
      fIdx = A[mid].Idx;
    }
    if (number > A[mid].numero)
    {
      l = mid + 1;
    }
    else
    {
      r = mid - 1;
    }
  }
  return fIdx;
}
