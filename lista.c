/*
 lista.c - Implementacion del pool de hilos (lista enlazada)
 */

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/* 
initPool
Crea n hilos, todos en estado libre, y los inserta
en la lista El ultimo en insertarse queda de cabeza.
*/
void initPool(ListaHilos *pool, int n) {
    pool->cabeza = NULL;

    int i;
    for (i = 1; i <= n; i++) {
        NodoHilo *nuevo = (NodoHilo *)malloc(sizeof(NodoHilo));
        if (nuevo == NULL) {
            printf("Error: no hay memoria para el pool.\n");
            return;
        }
        nuevo->hilo.id     = i;
        nuevo->hilo.estado = 0;   /* libre */
        nuevo->siguiente   = pool->cabeza;
        pool->cabeza       = nuevo;
    }
}

/*
 buscarHiloLibre  (RECURSIVA)
Recorre la lista buscando el primer hilo con estado 0
Devuelve un puntero al hilo libre, o NULL si no hay
 */
Hilo *buscarHiloLibre(NodoHilo *nodo) {
    if (nodo == NULL) return NULL;              //caso base: lista terminada 
    if (nodo->hilo.estado == 0) return &(nodo->hilo); // caso base: hilo libre
    return buscarHiloLibre(nodo->siguiente);    // llamada recursiva  
}

/*
 imprimirPool
 Muestra el id y estado de cada hilo en la lista.
Si el hilo esta ocupado, muestra la tarea asignada.
*/
void imprimirPool(ListaHilos *pool) {
    NodoHilo *actual = pool->cabeza;

    if (actual == NULL) {
        printf("  Pool vacio.\n");
        return;
    }

    while (actual != NULL) {
        if (actual->hilo.estado == 0) {
            printf("  Hilo_%d -> libre\n", actual->hilo.id);
        } else {
            printf("  Hilo_%d -> ocupado (tarea: %s)\n",
                   actual->hilo.id,
                   actual->hilo.tarea_actual.nombre);
        }
        actual = actual->siguiente;
    }
}

/*liberarPool
 Recorre la lista liberando cada nodo.
*/
void liberarPool(ListaHilos *pool) {
    NodoHilo *actual = pool->cabeza;

    while (actual != NULL) {
        NodoHilo *temp = actual;
        actual         = actual->siguiente;
        free(temp);
    }

    pool->cabeza = NULL;
}
