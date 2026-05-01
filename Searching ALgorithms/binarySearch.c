#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

int comparar(const void *left, const void *right){
    return(*(int*)left - *(int*)right);
}

/*
Función principal del programa
*/
int main(int argc, char *argv[])
{
    // Convierte el primer argumento recibido a entero para definir el tamaño del arreglo
    int n = atoi(argv[1]);
    int numeritoABuscar = atoi(argv[2]);
    int *arreglito;
    double utime0, stime0, wtime0,utime1, stime1, wtime1; //Variables para medición de tiempos
    // Reserva memoria dinámica para el arreglo principal
    arreglito = malloc(n * sizeof(int));
    if(arreglito == NULL)
    {
        printf("\nError al intentar reservar memoria para %d elementos\n", n);
        exit(1);
    }

    // Lee los n números desde la entrada estándar y los guarda en arreglito
    for(int i = 0; i < n; i++)
        scanf("%d", &arreglito[i]);
  //--------------------------------------------------------------------
    //inicio de medicion
    uswtime(&utime0, &stime0, &wtime0);
    // Ordena el arreglo antes de aplicar la búsqueda binaria
    qsort(arreglito, n, sizeof(int), comparar);

    // Inicializa los índices para la búsqueda binaria
    int mid, left, right;
    left = 0;
    right = n - 1;
    mid = (left + right) / 2;

    // Búsqueda binaria
    while(left <= right){
        if(numeritoABuscar == arreglito[mid]){
            printf("Numerito encontrado");
            break;
        } else if(numeritoABuscar < arreglito[mid]){
            right = mid - 1;
            mid = (left + right) / 2;
        } else if(numeritoABuscar > arreglito[mid]){
            left = mid + 1;
            right = n - 1;
            mid = (left + right) / 2;
        }
    }
    //Evaluar los tiempos de ejecución 
	//******************************************************************
	uswtime(&utime1, &stime1, &wtime1);
    if(left > right) printf("Numero no encontrado");
    //Mostrar los tiempos en formato exponecial
	printf("\n");
	printf("real (Tiempo total)  %.10e s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10e s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10e s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
	printf("\n");
    free(arreglito);

    return 0;
}
