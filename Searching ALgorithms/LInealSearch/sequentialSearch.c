/*
===============================================================
PROGRAMA: Búsqueda Lineal con Medición de Tiempos
AUTOR:    Devin Landeros Chávez  4 de mayo de 2026
DESCRIPCIÓN:
    Este programa implementa el algoritmo de búsqueda secuencial
    en un arreglo dinámico de enteros.
===============================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

/*
***************************************************************
FUNCIÓN:  Search
***************************************************************
    Realiza una búsqueda lineal, recorre el arreglo uno a uno 
    para encontrar el valor x. Si lo halla, devuelve su 
    índice; de lo contrario, devuelve -1.

Entrada:
    int arreglo[] -> arreglo de números enteros.
    int x   -> Número entero que se desea encontrar en el 
               arreglo.
    int n   -> Tamaño del arreglo.

Salida:
    int posicion -> int posicion -> Índice del elemento 
    hallado (0 a n-1) o -1 si el elemento es inexistente.
***************************************************************
*/

int Search (int arreglo[],int x,int n){
    
    // Inicializa la variable posicion de retorno en -1 (se asume que no existe)
    int posicion = -1; 

    // Ciclo que recorre el arreglo desde el índice 0 hasta n-1
    for (int i = 0; i < n; i++) {
            // Compara si el valor en la posición actual es igual al buscado
            if (arreglo[i] == x){
                posicion = i; // Si coincide, guarda el índice actual en la variable posicion
                break; // Detiene el ciclo porque ya encontro x
            }    
        }
        //retorna el resultado que puede ser la posicion donde se encontro x o -1
        return posicion; 
    }

/*
***************************************************************
FUNCIÓN:  main
***************************************************************
    Punto de entrada del programa. Coordina la gestión de memoria
    dinámica, la lectura de datos del usuario y la ejecución de
    la búsqueda lineal. Además, utiliza la librería "tiempo.h" 
    para medir el rendimiento del algoritmo en términos de 
    tiempo de CPU y tiempo real (wall time).

Entrada (Argumentos de terminal):
    int argc      -> Cantidad de argumentos recibidos.
    char *argv[]  -> Arreglo de cadenas:
                     argv[1]: Tamaño del arreglo (n).
                     argv[2]: Valor entero a buscar.

Salida:
    Imprime, dependiendo lo que de la ejecución de la función 
    search, si se sncontro o no el valor a buscar en el arreglo.

    int -> Retorna 0 si la ejecución fue exitosa o termina el
           programa con error (exit 1) si falla la memoria.
***************************************************************
*/

int main (int argc, char *argv[]){
     // Convierte los argumentos de texto recibidos en la terminal a enteros usando atoi (ascii to integer)
    int n = atoi(argv[1]); // Tamaño del arreglo
    int numbertofind = atoi(argv[2]); // Número que se desea encontrar
    int *A; // Puntero al arreglo dinámico

    /* Variables para almacenar los tiempos medidos por tiempo.h:
     utime -> tiempo de procesamiento en CPU (user time)
     stime -> tiempo en operaciones de entrada/salida (system time)
     wtime -> tiempo real transcurrido (wall time)*/
    double utime0, stime0, wtime0,utime1, stime1, wtime1;

    // Reserva n espacios de memoria para enteros en el heap
    // sizeof(int) garantiza el tamaño correcto en cualquier arquitectura
    A = malloc(sizeof(int) * n);

    // Verifica que la reserva de memoria fue exitosa malloc retorna NULL si no hay memoria suficiente disponible
    if(A == NULL){
        printf("\nError al intentar reservar memoria para %d elementos\n", n);
        exit(1); // Termina el programa con código de error 1
    }

    // Lee n números enteros desde la entrada estándar (el archivo redirigido con <) y los guarda uno por uno en el arreglo
    for(int i = 0; i < n; i++){
        scanf("%d", &A[i]); // &A[i] es la dirección de memoria donde guardar cada número
    } 

    // Marca el inicio de la medición de tiempo
    // Captura los tiempos actuales del proceso antes de ordenar y buscar
    uswtime(&utime0, &stime0, &wtime0);

    //Llama a la función Search que realiza la busqueda lineal con los argumentos proporcionados
    int resultado = Search(A,numbertofind,n);

    // Marca el fin de la medición de tiempo
    uswtime(&utime1, &stime1, &wtime1);

    //Si la funcion no encontro al numero entonces nos lo dice
    if (resultado == -1){
            printf("\nEl elemento no esta presente en el arreglo\n");
        }else{
            //Si se encontro el numero entonces nos indica su posición
            printf("\nEl elemento %d se encuentra en el indice %d\n",numbertofind,resultado);
        }
    
    
    //Liberacion de memoria
    free(A);


    // Imprime los tiempos de ejecución en formato exponencial para mayor precisión con números muy pequeños
	printf("\n");
	printf("real (Tiempo total)  %.10e s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10e s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10e s\n",  stime1 - stime0);
    // CPU/Wall indica qué porcentaje del tiempo real fue usado por la CPU
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    printf("\n");


    return 0; // Retorna 0 indicando que el programa terminó exitosamente
}