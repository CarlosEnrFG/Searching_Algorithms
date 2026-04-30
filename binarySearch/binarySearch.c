#include <stdio.h>
#include <stdlib.h>

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
    int *arreglito;

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

    // Lee el número a buscar
    int numeritoABuscar; scanf("%d", &numeritoABuscar);

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
    if(left > right) printf("Numero no encontrado");

    free(arreglito);

    return 0;
}
