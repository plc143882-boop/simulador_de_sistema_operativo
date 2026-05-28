/*
cola.c - Implementacion de la cola de prioridad
 
 * Prioridad:
Tarea normal  (prioridad = 0)se inserta al final
Tarea urgente (prioridad = 1) se inserta al frente
 */

#include <stdio.h>
#include <stdlib.h>
#include "cola.h"

/* 
 initCola
 Inicializa frente y final en NULL
 */
void initCola(Cola *cola) {
    cola->frente = NULL;
    cola->final  = NULL;
}

/*
  estaVaciaCola
Devuelve 1 si la cola esta vacia, 0 si no
*/
int estaVaciaCola(Cola *cola) {
    return cola->frente == NULL;
}

/*
encolar
Inserta una tarea en la cola
Si es urgente va al frente si es normal va al final
*/
void encolar(Cola *cola, Tarea t) {
    NodoCola *nuevo = (NodoCola *)malloc(sizeof(NodoCola));
    if (nuevo == NULL) {
        printf("Error: no hay memoria para la cola.\n");
        return;
    }
    nuevo->tarea     = t;
    nuevo->siguiente = NULL;

    if (t.prioridad == 1) {
        /* Urgente va al frente */
        nuevo->siguiente = cola->frente;
        cola->frente     = nuevo;

        /* Si la cola estaba vacia, el final tambien apunta aqui */
        if (cola->final == NULL) {
            cola->final = nuevo;
        }
    } else {
        /* Normal: va al final */
        if (estaVaciaCola(cola)) {
            cola->frente = nuevo;
            cola->final  = nuevo;
        } else {
            cola->final->siguiente = nuevo;
            cola->final            = nuevo;
        }
    }
}

/*
 desencolar
 Saca la tarea del frente y libera el nodo
*/
Tarea desencolar(Cola *cola) {
    NodoCola *temp = cola->frente;
    Tarea     t    = temp->tarea;

    cola->frente = temp->siguiente;

    /* Si la cola quedo vacia, final tambien debe ser NULL */
    if (cola->frente == NULL) {
        cola->final = NULL;
    }

    free(temp);
    return t;
}

/* 
 Muestra todas las tareas en orden de frente a final
*/
void imprimirCola(Cola *cola) {
    if (estaVaciaCola(cola)) {
        printf("  Cola vacia.\n");
        return;
    }

    NodoCola *actual = cola->frente;
    int posicion = 1;

    while (actual != NULL) {
        printf("  [%d] %s - %s\n",
               posicion,
               actual->tarea.nombre,
               actual->tarea.prioridad == 1 ? "URGENTE" : "normal");
        actual = actual->siguiente;
        posicion++;
    }
}

/*
 Desencola todos los nodos para liberar la memoria
*/
void liberarCola(Cola *cola) {
    while (!estaVaciaCola(cola)) {
        desencolar(cola);
    }
}
