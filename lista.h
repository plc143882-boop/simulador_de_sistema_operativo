/*
 lista.h - Definicion del pool de hilos (lista enlazada)
 */

#ifndef LISTA_H
#define LISTA_H

#include "sistema.h"

/* 
 NodoHilo
 Cada nodo guarda un Hilo y apunta al siguiente.
 */
typedef struct NodoHilo {
    Hilo            hilo;
    struct NodoHilo *siguiente;
} NodoHilo;

/*ListaHilos
 Solo necesita un puntero a la cabeza.
  */
typedef struct {
    NodoHilo *cabeza;
} ListaHilos;

// Prototipos
void  initPool        (ListaHilos *pool, int n);
Hilo *buscarHiloLibre (NodoHilo *nodo);     /* recursiva */
void  imprimirPool    (ListaHilos *pool);
void  liberarPool     (ListaHilos *pool);

#endif

