/*
 cola.h - Definicion de la cola de prioridad
 */

#ifndef COLA_H
#define COLA_H

#include "sistema.h"

/*
NodoCola
Cada nodo guarda una Tarea y apunta al siguiente
*/
typedef struct NodoCola {
    Tarea           tarea;
    struct NodoCola *siguiente;
} NodoCola;

/* 
 Cola
 Frente = primer elemento a procesar
 Final  = donde se insertan tareas normales
 */
typedef struct {
    NodoCola *frente;
    NodoCola *final;
} Cola;

/* 
Prototipos
 */
void  initCola     (Cola *cola);
void  encolar      (Cola *cola, Tarea t);
Tarea desencolar   (Cola *cola);
int   estaVaciaCola(Cola *cola);
void  imprimirCola (Cola *cola);
void  liberarCola  (Cola *cola);

#endif//fin
