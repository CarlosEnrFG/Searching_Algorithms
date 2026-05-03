#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

void QuickSort(int A[], int p, int r);
void Intercambio(int A[], int i, int j);
int Pivot(int A[], int p, int r);

/*
***************************************************************
FUNCIÓN: Intercambio
***************************************************************
    Intercambia de posición dos elementos dentro del arreglo
    usando una variable temporal para no perder ningún valor.

Entrada:
    int A[] -> arreglo de números enteros
    int i   -> índice del primer elemento a intercambiar
    int j   -> índice del segundo elemento a intercambiar

Salida:
    No retorna ningún valor. Los elementos en las posiciones
    i y j quedan intercambiados dentro del arreglo A.
***************************************************************
*/
void Intercambio(int A[], int i, int j){
    int temp = A[j];    // Guarda temporalmente el valor de A[j] para no perderlo al sobreescribir
    A[j] = A[i];        // Asigna a A[j] el valor de A[i]
    A[i] = temp;        // Asigna a A[i] el valor temporal guardado
}

/*
***************************************************************
FUNCIÓN: Pivot
***************************************************************
    Selecciona el primer elemento de la sublista como pivote
    y reorganiza los elementos del subarreglo de forma que:
    - Los elementos menores o iguales al pivote queden a su izquierda
    - Los elementos mayores al pivote queden a su derecha
    Al finalizar, coloca el pivote en su posición definitiva.

Entrada:
    int A[] -> arreglo de números enteros
    int p   -> índice inicial de la sublista
    int r   -> índice final de la sublista

Salida:
    Retorna el índice donde quedó colocado el pivote
    en su posición final definitiva.
***************************************************************
*/
int Pivot(int A[], int p, int r){
    int piv = A[p];     // Toma el primer elemento de la sublista como pivote
    int i = p + 1;      // i recorre de izquierda a derecha buscando elementos mayores al pivote para intercambiarlos
    int j = r;          // j recorre de derecha a izquierda buscando elementos menores al pivote para intercambiarlos

    // Continúa mientras los índices i y j no se hayan cruzado
    while (i < j)
    {
        // Avanza i hacia la derecha mientras encuentre elementos menores o iguales al pivote (estos ya están en su lado correcto)
        while (A[i] <= piv && i <= r){
            i++;
        }

        // Retrocede j hacia la izquierda mientras encuentre elementos mayores al pivote (estos ya están en su lado correcto)
        while (A[j] > piv && j >= p){
            j--;
        }

        // Si i y j no se han cruzado, significa que encontramos un elemento mayor al pivote a la izquierda (en i) y uno menor a la derecha (en j), los intercambiamos
        if (i < j)
        {
            Intercambio(A, i, j);
        }
    }

    // Coloca el pivote en su posición final definitiva intercambiándolo con el elemento en j, que es el último elemento menor o igual al pivote
    Intercambio(A, p, j);

    // Retorna la posición donde quedó el pivote
    return j;
}

/*
***************************************************************
FUNCIÓN: QuickSort
***************************************************************
    Ordena los elementos del arreglo usando el algoritmo
    QuickSort. Aplica la estrategia "divide y vencerás":
    elige un pivote, particiona el arreglo en dos mitades
    (menores y mayores al pivote) y se llama recursivamente
    para ordenar cada mitad.

Entrada:
    int A[] -> arreglo de números enteros a ordenar
    int p   -> índice inicial del subarreglo a ordenar
    int r   -> índice final del subarreglo a ordenar

Salida:
    No retorna ningún valor. El subarreglo A[p..r]
    queda ordenado de menor a mayor.
***************************************************************
*/
void QuickSort(int A[], int p, int r){
    int j;
    // Condición base: verifica que la sublista tenga al menos dos elementos
    // Si p >= r el subarreglo tiene 0 o 1 elementos y ya está ordenado
    if (p < r)
    {
        // Obtiene la posición definitiva del pivote y particiona el arreglo
        j = Pivot(A, p, r);

        // Ordena recursivamente la sublista izquierda (elementos menores al pivote)
        QuickSort(A, p, j - 1);

        // Ordena recursivamente la sublista derecha (elementos mayores al pivote)
        QuickSort(A, j + 1, r);
    }
}

/*
***************************************************************
FUNCIÓN: main
***************************************************************
    Función principal del programa. Recibe por argumentos el
    tamaño del arreglo y el número a buscar, lee los datos
    desde la entrada estándar, los ordena con QuickSort y
    aplica búsqueda exponencial seguida de búsqueda binaria
    para encontrar el número buscado.
    Mide y reporta los tiempos de ejecución usando tiempo.h.

Entrada (argumentos de terminal):
    argv[1] -> cantidad de elementos del arreglo (n)
    argv[2] -> número entero a buscar

Salida:
    Imprime si el número fue encontrado y en qué posición,
    además de los tiempos de ejecución del algoritmo.
***************************************************************
*/
int main(int argc, char *argv[]){
    // Convierte los argumentos de texto recibidos en la terminal a enteros usando atoi (ascii to integer)
    int n = atoi(argv[1]);               // Tamaño del arreglo
    int numeritoABuscar = atoi(argv[2]); // Número que se desea encontrar
    int *arreglito;                       // Puntero al arreglo dinámico

    // Variables para almacenar los tiempos medidos por tiempo.h:
    // utime -> tiempo de procesamiento en CPU (user time)
    // stime -> tiempo en operaciones de entrada/salida (system time)
    // wtime -> tiempo real transcurrido (wall time)
    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    // Reserva n espacios de memoria para enteros en el heap
    // sizeof(int) garantiza el tamaño correcto en cualquier arquitectura
    arreglito = malloc(n * sizeof(int));

    // Verifica que la reserva de memoria fue exitosa, malloc retorna NULL si no hay memoria suficiente disponible
    if(arreglito == NULL){
        printf("Error al intentar reservar memoria para %d elementos.\n", n);
        exit(1); // Termina el programa con código de error 1
    }

    // Lee n números enteros desde la entrada estándar (el archivo redirigido con <) y los guarda uno por uno en el arreglo
    for(int i = 0; i < n; i++){
        scanf("%d", &arreglito[i]); // &arreglito[i] es la dirección de memoria donde guardar cada número
    }

    // Marca el inicio de la medición de tiempo
    // Captura los tiempos actuales del proceso antes de ordenar y buscar
    uswtime(&utime0, &stime0, &wtime0);

    // Ordena el arreglo completo de menor a mayor usando QuickSort
    // Es necesario ordenar antes de aplicar la búsqueda exponencial, ya que esta requiere que el arreglo esté ordenado para funcionar
    QuickSort(arreglito, 0, n - 1);

    /*
    FASE 1 - BÚSQUEDA EXPONENCIAL:
    Salta por índices que son potencias de 2 (1, 2, 4, 8, 16...)
    comparando el valor en cada posición con el número buscado.
    Continúa mientras el índice no se salga del arreglo y el
    valor en esa posición sea menor al número buscado.
    Al terminar, acota el rango donde debe estar el número buscado
    entre inicio (2^(i-1)) y min (2^i o n-1).
    */
    int inicio = 0;      // Índice donde comenzará la búsqueda binaria, inicia en 0 por si el número está al principio
    int posicionActual;  // Guarda el índice del salto exponencial actual
    int i = 0;           // Exponente que controla el tamaño del salto (2^i)

    // Salta exponencialmente mientras el índice sea válido y el valor en esa posición sea menor al buscado
    while(1 << i < n - 1 && arreglito[1 << i] < numeritoABuscar){
        posicionActual = 1 << i;  // Calcula el índice actual (2^i)
        inicio = posicionActual;  // Guarda este índice como nuevo punto de inicio
        i++;                      // Incrementa el exponente para el siguiente salto
    }

    /*
    FASE 2 - BÚSQUEDA BINARIA:
    Usa el rango acotado por la fase exponencial para hacer
    una búsqueda binaria entre inicio (2^(i-1)) y min (2^i o n-1).
    Divide repetidamente el rango a la mitad comparando el elemento
    central con el número buscado hasta encontrarlo o agotar el rango.
    */
    int left, right, mid;
    int posicionEncontrado = -1; // -1 indica que el número aún no fue encontrado

    // Calcula el límite derecho de la búsqueda binaria: si 2^i se sale del arreglo, usa n-1 como límite
    int limiteDerecho = (1 << i) < (n - 1) ? (1 << i) : (n - 1);

    left = inicio;         // Límite izquierdo: donde terminó la fase exponencial
    right = limiteDerecho; // Límite derecho: 2^i o n-1, lo que sea menor
    mid = (left + right) / 2; // Índice central inicial del rango de búsqueda

    // Búsqueda binaria dentro del rango acotado
    while(left <= right){
        if(numeritoABuscar == arreglito[mid]){
            // El número buscado coincide con el elemento central
            // Suma 1 para que la posición sea natural (desde 1, no desde 0)
            posicionEncontrado = mid + 1;
            break; // Termina la búsqueda
        }
        else if(numeritoABuscar < arreglito[mid]){
            // El número está en la mitad izquierda
            // Descarta la mitad derecha moviendo right a mid-1
            right = mid - 1;
            mid = (left + right) / 2;
        }
        else if(numeritoABuscar > arreglito[mid]){
            // El número está en la mitad derecha
            // Descarta la mitad izquierda moviendo left a mid+1
            left = mid + 1;
            right = limiteDerecho;
            mid = (left + right) / 2;
        }
    }

    // Marca el fin de la medición de tiempo
    uswtime(&utime1, &stime1, &wtime1);

    // Imprime el resultado de la búsqueda
    if(posicionEncontrado != -1)
        printf("Se encontro el numero en la posicion %d\n", posicionEncontrado);
    else
        printf("No se encontro el numero\n");

    // Imprime los tiempos de ejecución en formato exponencial
    printf("\n");
    printf("real (Tiempo total)                   %.10e s\n", wtime1 - wtime0);
    printf("user (Tiempo de procesamiento en CPU) %.10e s\n", utime1 - utime0);
    printf("sys  (Tiempo en acciones de E/S)      %.10e s\n", stime1 - stime0);
    printf("CPU/Wall                              %.10f %%\n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    printf("\n");

    // Libera la memoria reservada con malloc
    free(arreglito);

    return 0; // Retorna 0 indicando que el programa terminó exitosamente
}