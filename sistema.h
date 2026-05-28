/*
 sistema.h - Definicion de estructuras principales
Todos los archivos del proyecto incluyen este header
 */

#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdio.h>

/* 
Tarea
Representa un proceso que entra al sistema.
*/
typedef struct {
    int  id;
    char nombre[50];
    int  prioridad;   // 0 = normal, 1 = urgente 
} Tarea;

/*
Hilo
Representa un hilo de ejecucion en el pool.
 */
typedef struct {
    int   id;
    int   estado;        //0 = libre, 1 = ocupado 
    Tarea tarea_actual;  // tarea asignada al hilo  
} Hilo;

/* 
Resultado
Lo que se guarda en la pila al completar una tarea.
*/
typedef struct {
    int  tareaId;
    char descripcion[50];
} Resultado;

/* 
Prototipo de leerEntero
Se implementa en main.c y se usa en todo el programa.
*/
int leerEntero  (void);
int leerPrioridad(void);

#endif
