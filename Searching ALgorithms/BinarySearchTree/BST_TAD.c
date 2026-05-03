#include "BST_TAD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void Insert(arbolBinario *a, elemento e)
{
  posicion *actual = a;
  // Busca la posición adecuada para insertar
  while (*actual != NULL)
  {
    if (e.numero < (*actual)->valor.numero)
    {
      actual = &((*actual)->hijoIzq);
    }
    else if (e.numero > (*actual)->valor.numero)
    {
      actual = &((*actual)->hijoDer);
    }
    else
    {
      return; // No inserta elementos duplicados
    }
  }
 // Reserva memoria para el nuevo nodo
  *actual = malloc(sizeof(nodo));
  if (*actual == NULL)
  {
    printf("No se pudo reservar memoria");
    exit(1);
  }
 // Inicializa el nodo
  (*actual)->hijoIzq = NULL;
  (*actual)->hijoDer = NULL;
  (*actual)->valor = e;
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
