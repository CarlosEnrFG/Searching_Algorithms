#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

int comparar(const void *left, const void *right){
    return(*(int*)left - *(int*)right);
}

int main(int argc, char *argv[]){
    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    int n = atoi(argv[1]);

    int *arreglito = malloc(n * sizeof(int));
    if(arreglito == NULL){
        printf("\nError al intentar reservar memoria para %d elementos\n", n);
        exit(1);
    }

    for(int i = 0; i < n; i++)
        scanf("%d", &arreglito[i]);

    int numeritoABuscar; scanf("%d", &numeritoABuscar);

    qsort(arreglito, n, sizeof(int), comparar);

    int mid, left, right;
    left = 0;
    right = n - 1;
    mid = (left + right) / 2;

    uswtime(&utime0, &stime0, &wtime0);

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
    if(left > right) printf("Numero no encontrado");

    uswtime(&utime1, &stime1, &wtime1);

    printf("real (Tiempo total)                   %.10e s\n", wtime1 - wtime0);
    printf("user (Tiempo de procesamiento en CPU) %.10e s\n", utime1 - utime0);
    printf("sys  (Tiempo en acciones de E/S)      %.10e s\n", stime1 - stime0);
    printf("CPU/Wall                              %.10f %%\n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    printf("\n");

    free(arreglito);
    return 0;
}