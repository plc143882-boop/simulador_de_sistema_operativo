/*
 pila.h - Definicion de la pila de historial
 */

#ifndef PILA_H
#define PILA_H

#include "sistema.h"

/* 
NodoPila
Cada nodo guarda un Resultado y apunta al siguiente
*/
typedef struct NodoPila {
    Resultado       resultado;
    struct NodoPila *siguiente;
} NodoPila;

/* 
Pila
Solo necesita un puntero a la cima.
*/
typedef struct {
    NodoPila *cima;
} Pila;

/* 
Prototipos
 */
void      initPila        (Pila *pila);
void      push            (Pila *pila, Resultado r);
Resultado pop             (Pila *pila);
int       estaVaciaPila   (Pila *pila);
void      imprimirPila    (Pila *pila);
void      liberarPila     (Pila *pila);
void      guardarHistorial(Pila *pila);              //guarda pila en archivo  
void      cargarHistorial (Pila *pila);              //carga archivo a la pila

#endif
