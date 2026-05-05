/*
Autor: Saul Ascencion Cruz
1 de mayo 2026
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "BST_TAD.h"
#include "tiempo.h"


//Struct con los argumentos necesarios para el procesamiento de rangos
typedef struct RangoArgs
{
    arbolBinario *raiz;
    int *arreglo;
    int inicio;
    int fin;
} RangoArgs;

//Funcion para procesar rangos
void *InsertarRango(void *arg)
{
    //Recuperamos los valores de los rangos pasados por argumento haciendoles un casteo
    //al tipode dato struct RangoArgs
    RangoArgs *rango = (RangoArgs *)arg;
    //Calculamos la cantidad de elementos en ese rango
    int elementos = rango->fin - rango->inicio; 
    //Creamos un arreglo de argumentos con suficientes elementos para cada insercion
    ThreadArgs *D = malloc(sizeof(ThreadArgs)*elementos); 
    //Inicializamos cada insercion y lanzamos al hilo a insertarlo
    for (int i = 0; i < elementos; i++)  
    {
        D[i].raiz = rango->raiz;
        D[i].dato.numero = rango->arreglo[rango->inicio+i];
        D[i].dato.RelativeIdx = rango->inicio+i;
        Insert((void *)&D[i]); 
    }
}
int main(int argc, char *argv[])
{
    //Leer el tamaño del arreglo
    int n = atoi(argv[1]);
    //Leer el umero a buscar
    int number = atoi(argv[2]);
    //Crear arreglo
    int *A = malloc(sizeof(int) * n);
    double utime0, stime0, wtime0,utime1, stime1, wtime1; //Variables para medición de tiempos 
    //Leer arreglo
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &A[i]);
    }
    //Crear ABB
    arbolBinario B;
    //Inicializar ABB
    Inicializar(&B);
 //******************************************************************	
	//Iniciar el conteo del tiempo para las evaluaciones de rendimiento
	//******************************************************************	
	uswtime(&utime0, &stime0, &wtime0);

    //Crear dos hilos para el procesamiento por hilos
    pthread_t hilos[2];
    //Crear un arreglo de structs en donde se guardan los argumentos para cada hilo necesarios
    //para el procesamiento de rangos
    RangoArgs rangos[2];
    //Crear un arreglo de struct para los argumentos de la primera inserción (raiz)
    ThreadArgs raiz_args;
    //Inicializar raíz del ABB haciendo referencia al ABB
    raiz_args.raiz = &B;
    //La raiz llevara el numero A[0] del arreglo
    raiz_args.dato.numero = A[0];
    //Guardamos la psocicion que le corresponde al A[0]
    raiz_args.dato.RelativeIdx = 0;
    //Insertamos con la funcioon de la libreria BTS_TAD usando los parametros solamente de la raiz
    Insert((void *)&raiz_args);
    //Calculamos el rango que tendra cada hilo
    int rango = n / 2;
    //Inicializamos los structs en donde se guardan los argumentos para cada hilo necesarios
    //para el procesamiento de rangos e inmediatamente lanzamos a ejecucion el hilo
    for (int i = 0; i < 2; i++)
    {
        rangos[i].raiz = &B;
        rangos[i].arreglo = A;
        if (i == 0)
        {
            rangos[i].inicio = 1;
            rangos[i].fin = rango;
        }
        else
        {
            rangos[i].inicio = i * rango;
            rangos[i].fin = n;
        }
        //Lanzamos los hilos
        pthread_create(&hilos[i], NULL, InsertarRango, (void *)&rangos[i]);
    }

    //Esperamos a que ambos hilos regresen
    for (int i = 0; i < 2; i++)
    {
        pthread_join(hilos[i], NULL);
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
/*    //Si la funcion encontro al numero entonces imprime su ubicacion en el arreglo
    if (Idx != -1)
    {
        printf("El numero %d fue encontrado en la posicion %d\n", number, Idx);
    } 
    //Si no se encontro el numero entonces nos lo indica
    else
    {
        printf("El numero %d no se encuentra en el arreglo\n", number);
    }
*/
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
