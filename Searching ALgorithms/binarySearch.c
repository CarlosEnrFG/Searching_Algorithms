#include <stdio.h>
#include <stdlib.h>
#include "tiempo.h"

/*
***************************************************************
FUNCIÓN: swap
***************************************************************
    Intercambia los valores de dos variables enteras usando
    una variable temporal.

Entrada:
    int *a -> puntero al primer entero a intercambiar
    int *b -> puntero al segundo entero a intercambiar

Salida:
    No retorna ningún valor, pero los valores apuntados
    por a y b quedan intercambiados en memoria.
***************************************************************
*/
void swap(int *a, int *b){
    int temp = *a;  // Guarda el valor de a en una variable temporal
                    // para no perderlo al momento de sobreescribir a
    *a = *b;        // Copia el valor de b dentro de a
    *b = temp;      // Copia el valor temporal (valor original de a) dentro de b
}

/*
***************************************************************
FUNCIÓN: medianaDeTres
***************************************************************
    Elige el pivote del quicksort usando la estrategia de
    "mediana de tres". Compara el elemento izquierdo, el
    central y el derecho del subarreglo, los ordena entre
    sí, y coloca la mediana (el valor intermedio) en la
    posición high para usarla como pivot.

    Esto evita el peor caso del quicksort (O(n²)) que ocurre
    cuando el pivot elegido es siempre el mayor o el menor,
    lo cual es común cuando el arreglo ya está ordenado.

Entrada:
    int A[]  -> arreglo de enteros
    int low  -> índice del elemento más a la izquierda del subarreglo
    int high -> índice del elemento más a la derecha del subarreglo

Salida:
    Retorna el valor del pivote (la mediana de los tres elementos)
    y lo deja colocado en la posición high del arreglo.
***************************************************************
*/
int medianaDeTres(int A[], int low, int high){
    // Calcula el índice del elemento central del subarreglo
    // Se usa low + (high-low)/2 en lugar de (low+high)/2 para evitar desbordamiento de enteros con índices muy grandes
    int mid = low + (high - low) / 2;

    // Ordena los tres elementos (low, mid, high) entre sí con tres comparaciones e intercambios:

    // Si el elemento izquierdo es mayor que el central, los intercambia para garantizar que A[low] <= A[mid]
    if (A[low] > A[mid])  swap(&A[low], &A[mid]);

    // Si el elemento izquierdo es mayor que el derecho, los intercambia para garantizar que A[low] <= A[high]
    if (A[low] > A[high]) swap(&A[low], &A[high]);

    // Si el elemento central es mayor que el derecho, los intercambia para garantizar que A[mid] <= A[high]
    // En este punto: A[low] <= A[mid] <= A[high] por lo tanto A[mid] es la mediana
    if (A[mid] > A[high]) swap(&A[mid], &A[high]);

    // Mueve el pivot (la mediana) a la posición high para que la función de partición pueda trabajar con él
    swap(&A[mid], &A[high]);

    // Retorna el valor del pivot para usarlo en la partición
    return A[high];
}

/*
***************************************************************
FUNCIÓN: insercion
***************************************************************
    Se usa en lugar de quicksort para subarreglos pequeños
    (menos de 20 elementos) porque para pocos elementos,
    inserción es más eficiente al no tener el costo de
    calcular pivots ni hacer particiones.

Entrada:
    int A[]  -> arreglo de enteros
    int low  -> índice donde inicia la porción a ordenar
    int high -> índice donde termina la porción a ordenar

Salida:
    No retorna ningún valor. El subarreglo A[low..high]
    queda ordenado de menor a mayor.
***************************************************************
*/
void insercion(int A[], int low, int high){
    // Recorre el subarreglo desde el segundo elemento (low+1) hasta el final (high), tomando cada elemento como "key"
    for(int i = low + 1; i <= high; i++){

        int key = A[i]; // Guarda el elemento actual que se va a insertar en su posición correcta entre los anteriores

        int j = i - 1;  // j apunta al elemento inmediatamente anterior a key desde aquí se compara hacia la izquierda

        // Mientras j no salga del subarreglo (j >= low) y el elemento en j sea mayor que key, desplaza ese elemento una posición a la derecha para hacer espacio donde insertar key
        while(j >= low && A[j] > key){
            A[j + 1] = A[j]; // Mueve el elemento mayor una posición a la derecha
            j--;              // Retrocede un lugar para seguir comparando
        }

        // Inserta key en la posición correcta:
        // j+1 porque el while decrementó j una vez de más
        A[j + 1] = key;
    }
}

/*
***************************************************************
FUNCIÓN: quickSort
***************************************************************
Descripción:
    Implementación iterativa del algoritmo quicksort con tres
    optimizaciones clave:

    1. MEDIANA DE TRES: Elige el pivot como la mediana entre
       el elemento izquierdo, central y derecho, evitando el
       peor caso O(n²) de quicksort.

    2. INSERCIÓN PARA SUBARREGLOS PEQUEÑOS: Cuando un subarreglo
       tiene menos de 20 elementos, usa inserción directa que
       es más eficiente para pocos elementos.

    3. ITERATIVO CON PILA MANUAL: En lugar de usar recursión,
       simula la pila de llamadas con un arreglo manual. Esto
       evita el desbordamiento de pila (stack overflow) que
       ocurriría con 10 millones de elementos en recursión profunda.

Entrada:
    int A[]  -> arreglo de enteros a ordenar
    int low  -> índice inicial del subarreglo (0 para el arreglo completo)
    int high -> índice final del subarreglo (n-1 para el arreglo completo)

Salida:
    No retorna ningún valor. El arreglo A queda ordenado
    de menor a mayor entre los índices low y high.
***************************************************************
*/
void quickSort(int A[], int low, int high){
    // Pila manual que almacena pares de índices (low, high) de los subarreglos pendientes por ordenar.
    // Tamaño 64 es suficiente para 2^32 elementos (más que 10 millones)
    int pila[64];
    int tope = -1; // Inicia vacía: tope = -1 significa pila vacía

    // Empuja el subarreglo inicial (el arreglo completo) a la pila
    pila[++tope] = low;   // Guarda el índice izquierdo
    pila[++tope] = high;  // Guarda el índice derecho

    // Mientras haya subarreglos pendientes en la pila
    while(tope >= 0){
        // Saca el subarreglo más reciente de la pila
        high = pila[tope--]; // Recupera el índice derecho
        low  = pila[tope--]; // Recupera el índice izquierdo

        // Si el subarreglo tiene menos de 20 elementos, usa inserción directa que es más eficiente para pocos datos
        if(high - low < 20){
            insercion(A, low, high);
            continue; // Pasa al siguiente subarreglo en la pila
        }

        // Elige el pivot usando mediana de tres y lo coloca en la posición high
        int pivot = medianaDeTres(A, low, high);

        // PARTICIÓN: reorganiza el subarreglo dejando elementos menores al pivot a la izquierda y elementos mayores al pivot a la derecha
        int i = low - 1;  // i recorre de izquierda a derecha buscando elementos mayores al pivot
        int j = high;     // j recorre de derecha a izquierda buscando elementos menores al pivot

        while(1){
            // Avanza i hacia la derecha hasta encontrar un elemento mayor o igual al pivot
            while(A[++i] < pivot);

            // Avanza j hacia la izquierda hasta encontrar un elemento menor o igual al pivot
            while(A[--j] > pivot);

            // Si i y j se cruzaron, la partición terminó
            if(i >= j) break;

            // Si no se cruzaron, intercambia los elementos para que el mayor quede a la derecha y el menor a la izquierda
            swap(&A[i], &A[j]);
        }

        // Coloca el pivot en su posición final correcta (entre los elementos menores y los mayores)
        swap(&A[i], &A[high]);

        // Empuja los dos subarreglos resultantes a la pila.
        // El subarreglo más grande se empuja primero para que el más pequeño se procese antes, minimizando el uso de la pila
        if((i - 1 - low) > (high - i - 1)){
            // El subarreglo izquierdo es más grande: empuja izquierdo primero
            pila[++tope] = low;      // Índice izquierdo del subarreglo izquierdo
            pila[++tope] = i - 1;    // Índice derecho del subarreglo izquierdo
            pila[++tope] = i + 1;    // Índice izquierdo del subarreglo derecho
            pila[++tope] = high;     // Índice derecho del subarreglo derecho
        } else {
            // El subarreglo derecho es más grande: empuja derecho primero
            pila[++tope] = i + 1;    // Índice izquierdo del subarreglo derecho
            pila[++tope] = high;     // Índice derecho del subarreglo derecho
            pila[++tope] = low;      // Índice izquierdo del subarreglo izquierdo
            pila[++tope] = i - 1;    // Índice derecho del subarreglo izquierdo
        }
    }
}

/*
***************************************************************
FUNCIÓN: main
***************************************************************
    Función principal del programa. Recibe por argumentos el
    tamaño del arreglo y el número a buscar, lee los datos
    desde la entrada estándar, los ordena con quicksort y
    aplica búsqueda binaria para encontrar el número.
    Mide y reporta los tiempos de ejecución del ordenamiento
    y la búsqueda usando la librería tiempo.h.

Entrada (argumentos de terminal):
    argv[1] -> cantidad de elementos del arreglo (n)
    argv[2] -> número entero a buscar

Salida:
    Imprime si el número fue encontrado y en qué posición,
    además de los tiempos de ejecución del algoritmo.
***************************************************************
*/
int main(int argc, char *argv[])
{
    // Convierte los argumentos de texto recibidos en la terminal a enteros usando atoi
    int n = atoi(argv[1]);              // Tamaño del arreglo
    int numeritoABuscar = atoi(argv[2]); // Número que se desea encontrar
    int *arreglito;                      // Puntero al arreglo dinámico

    // Variables para almacenar los tiempos medidos por tiempo.h:
    // utime -> tiempo de procesamiento en CPU (user time)
    // stime -> tiempo en operaciones de entrada/salida (system time)
    // wtime -> tiempo real transcurrido (wall time)
    double utime0, stime0, wtime0, utime1, stime1, wtime1;

    // Reserva n espacios de memoria para enteros en el heap
    // sizeof(int) garantiza el tamaño correcto en cualquier arquitectura
    arreglito = malloc(n * sizeof(int));

    // Verifica que la reserva de memoria fue exitosa
    // malloc retorna NULL si no hay memoria suficiente disponible
    if(arreglito == NULL)
    {
        printf("\nError al intentar reservar memoria para %d elementos\n", n);
        exit(1); // Termina el programa con código de error 1
    }

    // Lee n números enteros desde la entrada estándar (el archivo redirigido con <) y los guarda uno por uno en el arreglo
    for(int i = 0; i < n; i++)
        scanf("%d", &arreglito[i]); // &arreglito[i] es la dirección de memoria donde guardar cada número

    // Marca el inicio de la medición de tiempo
    // Captura los tiempos actuales del proceso antes de ordenar y buscar
    uswtime(&utime0, &stime0, &wtime0);

    // Ordena el arreglo completo de menor a mayor usando quicksort optimizado
    // Es necesario ordenar antes de aplicar búsqueda binaria, ya que esta requiere que el arreglo esté ordenado para funcionar
    quickSort(arreglito, 0, n - 1);

    // Inicializa las variables de control de la búsqueda binaria:
    int mid, left, right;
    int posicionEncontrado = -1; // -1 indica que el número aún no fue encontrado

    left = 0;                    // Índice del primer elemento del arreglo
    right = n - 1;               // Índice del último elemento del arreglo
    mid = (left + right) / 2;    // Índice del elemento central inicial

    /*
    BÚSQUEDA BINARIA:
    Divide repetidamente el arreglo a la mitad comparando el elemento
    central (arreglito[mid]) con el número buscado:
    - Si son iguales: encontró el número
    - Si el buscado es menor: descarta la mitad derecha (right = mid-1)
    - Si el buscado es mayor: descarta la mitad izquierda (left = mid+1)
    Continúa hasta encontrarlo o hasta que left > right (no existe)
    */
    while(left <= right){

        if(numeritoABuscar == arreglito[mid]){
            // El número buscado coincide con el elemento central
            // Guarda la posición sumando 1 para que sea natural (desde 1, no desde 0)
            posicionEncontrado = mid + 1;
            break; // Termina la búsqueda, ya encontró el número
        }
        else if(numeritoABuscar < arreglito[mid]){
            // El número buscado es menor que el elemento central, por lo tanto solo puede estar en la mitad izquierda.
            // Descarta la mitad derecha moviendo right a mid-1
            right = mid - 1;
            mid = (left + right) / 2; // Recalcula el nuevo centro
        }
        else if(numeritoABuscar > arreglito[mid]){
            // El número buscado es mayor que el elemento central, por lo tanto solo puede estar en la mitad derecha.
            // Descarta la mitad izquierda moviendo left a mid+1
            left = mid + 1;
            right = n - 1;
            mid = (left + right) / 2; // Recalcula el nuevo centro
        }
    }

    // Marca el fin de la medición de tiempo
    // Captura los tiempos actuales del proceso después de ordenar y buscar
    uswtime(&utime1, &stime1, &wtime1);

    // Imprime el resultado de la búsqueda
    if(posicionEncontrado != -1)
        // El número fue encontrado: imprime en qué posición está
        printf("Se encontro el numero en la posicion %d\n", posicionEncontrado);
    else
        // El número no existe en el arreglo
        printf("No se encontro el numero\n");

    // Imprime los tiempos de ejecución en formato exponencial
    // para mayor precisión con números muy pequeños
    printf("\n");
    printf("real (Tiempo total)                   %.10e s\n", wtime1 - wtime0);
    printf("user (Tiempo de procesamiento en CPU) %.10e s\n", utime1 - utime0);
    printf("sys  (Tiempo en acciones de E/S)      %.10e s\n", stime1 - stime0);
    // CPU/Wall indica qué porcentaje del tiempo real fue usado por la CPU
    printf("CPU/Wall                              %.10f %%\n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    printf("\n");

    // Libera la memoria reservada con malloc
    // Es importante hacerlo siempre para evitar fugas de memoria
    free(arreglito);

    return 0; // Retorna 0 indicando que el programa terminó exitosamente
}