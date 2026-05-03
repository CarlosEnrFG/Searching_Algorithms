#include "BST_TAD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//Funcion para inicializar el ABB
void Inicializar(arbolBinario *a) 
{ 
    //Ponemos el apuntador de la raiz en NULL
    *a = NULL; 
}

//Funcion para destruir el ABB y liberar la memoria ocupada
void Destruir(arbolBinario *a) 
{
    //Si el arbol no esta vacio procedemos a borrar sus nodos
    if (*a != NULL) 
    {
        //Si el hijo izquierdo existe lo borramos llamando a la misma funcion
        if ((*a)->hijoIzq != NULL) 
        {
            Destruir(&(*a)->hijoIzq);
        }
        //Si el hijo derecho existe lo borramos llamando a la misma funcion
        if ((*a)->hijoDer != NULL) 
        {
            Destruir(&(*a)->hijoDer);
        }
        //Eliminamos el mutex que usamos para el control de hilos en ese nodo
        pthread_mutex_destroy(&((*a)->ocupado));
        //Liberamos el espacio de memoria que ocupaba el nodo
        free(*a);
        //Ponemos el apuntador en NULL para dejarlo limpio
        *a = NULL;
    }
}

//Funcion para obtener la posicion de la raiz del arbol
posicion Raiz(arbolBinario *a) 
{ 
    //Regresamos el valor que tiene el apuntador al arbol
    return *a; 
}

//Funcion para obtener la posicion del hijo derecho de un nodo p
posicion HijoDerecho(arbolBinario *a, posicion p) 
{
    nodo *ret;
    //Si la posicion que nos dan es valida buscamos su hijo
    if (p != NULL) 
    {
        //Asignamos el hijo derecho a nuestra variable de retorno
        ret = p->hijoDer;
    } 
    else 
    {
        //Sino encontramos nada regresamos NULL
        ret = NULL;
    }
    return ret;
}

//Funcion para obtener la posicion del hijo izquierdo de un nodo p
posicion HijoIzquierdo(arbolBinario *a, posicion p) 
{
    nodo *ret;
    //Si la posicion que nos dan es valida buscamos su hijo
    if (p != NULL) 
    {
        //Asignamos el hijo izquierdo a nuestra variable de retorno
        ret = p->hijoIzq;
    } 
    else 
    {
        //Sino encontramos nada regresamos NULL
        ret = NULL;
    }
    return ret;
}

//Funcion para saber si el arbol no tiene ningun nodo
boolean Vacio(arbolBinario *a) 
{ 
    //Regresamos verdadero si el contenido del arbol es NULL
    return ((*a) == NULL); 
}

//Funcion para leer los datos que tiene un nodo en especifico
elemento LeerNodo(arbolBinario *a, posicion p) 
{ 
    //Regresamos la estructura valor que esta guardada en esa posicion
    return p->valor; 
}

//Funcion para insertar elementos en el ABB usando exclusion mutua para hilos
void *Insert(void *a)
{
    //Recuperamos los argumentos pasados por el hilo haciendo el casteo a ThreadArgs
    ThreadArgs *args = (ThreadArgs*)a;
    //Recuperamos el elemento
    elemento e = args->dato;
    arbolBinario *raiz = args->raiz;
    
    //Si el arbol esta vacio entonces este elemento sera la nueva raiz
    if (*raiz == NULL)
    {
        //Creamos una nueva posicion
        posicion nuevo = (posicion)malloc(sizeof(nodo));
        //Guardamos las cosas dentro de este nueva posicion
        nuevo->valor = e;
        nuevo->hijoIzq = nuevo->hijoDer = NULL;
        //Inicializamos el mutex para que otros hilos puedan interactuar con el
        pthread_mutex_init(&(nuevo->ocupado), NULL);
        //Indicamos que el nodo nuevo sera la raiz
        *raiz = nuevo;
        //NO sé bien por qué necesita return NULL pero si lo quito se rompe
        return NULL;
    }
    
    //Si ya hay raiz empezamos a buscar su lugar desde arriba
    posicion actual = *raiz;
    //Buscamos hasta llegar a una posicion vacia en donde podamos meter un nuevo elemento
    while (actual != NULL)
    {
        //Bloqueamos el nodo actual para que ningun otro hilo lo mueva mientras decidimos
        pthread_mutex_lock(&(actual->ocupado));
        //Si el numero que traemos es menor que el del nodo actual vamos a la izquierda
        if (e.numero < actual->valor.numero)
        {
            //Si el lugar de la izquierda esta libre lo insertamos ahi
            if (actual->hijoIzq == NULL)
            {
                posicion nuevo = (posicion)malloc(sizeof(nodo));
                nuevo->valor = e;
                nuevo->hijoIzq = NULL;
                nuevo->hijoDer = NULL;
                pthread_mutex_init((&(nuevo->ocupado)), NULL);
                actual->hijoIzq = nuevo;
                //Desbloqueamos el nodo y nos salimos
                pthread_mutex_unlock(&(actual->ocupado));
                break;
            }
            else
            {
                //Si ya hay alguien nos movemos a ese nodo
                posicion siguiente = actual->hijoIzq;
                //Desbloqueamos el nodo en el que estamos
                pthread_mutex_unlock(&(actual->ocupado));
                //Nos movemos a ese nodo NO vacio 
                actual = siguiente;
            }
        }
        //Si el numero es mas grande que el del nodo actual vamos a la derecha
        else if (e.numero > actual->valor.numero)
        {
            //Si el lugar de la derecha esta libre lo insertamos ahi
            if (actual->hijoDer == NULL)
            {
                posicion nuevo = (posicion)malloc(sizeof(nodo));
                nuevo->valor = e;
                nuevo->hijoIzq = NULL;
                nuevo->hijoDer = NULL;
                pthread_mutex_init((&(nuevo->ocupado)), NULL);
                actual->hijoDer = nuevo;
                //Desbloqueamos el nodo y nos salimos
                pthread_mutex_unlock(&(actual->ocupado));
                break;
            }
            else
            {
                //Si ya hay alguien nos movemos a ese nodo y soltamos el candado del anterior
                posicion siguiente = actual->hijoDer;
                //Desbloqueamos el nodo en el que estamos
                pthread_mutex_unlock(&(actual->ocupado));
                //NOs movemos a ese nodo NO vacio
                actual = siguiente;
            }
        }
        //Si el numero es igual entonces ya existe y no hacemos nada
        else
        {
            pthread_mutex_unlock(&(actual->ocupado));
            break;
        }
    }
    return NULL;
}

//Funcion para buscar un numero dentro del arbol
int Buscar(arbolBinario *a, elemento n)
{
    //Variable que nos va a indicar el indice en que se encuentra el numero que deseamos 
    int Idx = -1;
    posicion actual = *a;
    //Si el arbol no tiene nada pues no hay nada que buscar
    if(actual == NULL)
    {
        return Idx;
    }
    //Recorremos el arbol comparando los valores
    while(actual != NULL)
    {
        //Si encontramos el numero guardamos el indice que traia desde el arreglo
        if(actual->valor.numero == n.numero)
        {
            Idx = actual->valor.RelativeIdx;
            break;
        }
        //Si el que buscamos es menor nos vamos por la rama izquierda
        if(n.numero < actual->valor.numero)
        {
            actual = actual->hijoIzq;
        }
        //Sino nos vamos por la rama derecha
        else
        {
            actual = actual->hijoDer;
        }
    }
    //Regresamos el indice o el -1 segun si se encontro o no
    return Idx;
}
