/*TAD Arbol Binario de Búsqueda ABB
Autor: Saul Ascencion Cruz
Fecha: 1 de Mayo del 2026
*/
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
typedef unsigned char boolean;

typedef struct elemento
{
    int numero;
    int RelativeIdx;
} elemento;

typedef struct nodo
{
    elemento valor;
    struct nodo* hijoIzq;
    struct nodo* hijoDer;
} nodo;

typedef nodo* arbolBinario;
typedef nodo* posicion;

void Inicializar(arbolBinario *a);
/*Libera la memoria necesaria para crear el ABB*/
void Destruir(arbolBinario *a);
/*Regresa el nodo raíz del subárbol*/
posicion Raiz(arbolBinario *a);
/*Regresa el hijo derecho del nodo actual*/
posicion HijoDerecho(arbolBinario *a,posicion p); 
/*Regresa el hijo izquierdo del nodo actual*/
posicion HijoIzquierdo(arbolBinario *a,posicion p);
/*Verifica si un nodo está vacío. Si el nodo está vacío regresa FALSE, por el contrario si el nodo NO
está vacío regresa TRUE*/
boolean Vacio(arbolBinario *a);
/*Regresa el elemento del nodo dado*/
elemento LeerNodo(arbolBinario *a,posicion p);
/*Inserta un elemento en un árbol siguiendo las reglas del ABB. Recibe un arbol binario y un elemento*/
void Insert(arbolBinario *a, elemento e);
/*Retornar el indice del nodo dado*/
int Idx(arbolBinario *a);
/*Recorre un arbol hasta encontrar o*/
int Buscar(arbolBinario *a, elemento n);
