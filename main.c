/*

 main.c - Simulador de sistema operativo (procesador de tareas)
 Estructuras de Datos
 Adiel Lopez Cruz
 Flujo: Cola de prioridad -> Pool de hilos (lista) -> Pila de historial -> Log
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sistema.h"
#include "cola.h"
#include "lista.h"
#include "pila.h"

#define NUM_HILOS 5
//Prototipos
void limpiarpantalla();
int leerEntero();
int leerPrioridad();
void simularLiberacion(ListaHilos *pool, Pila *historial, FILE *log);

//main
int main(void) {
    srand((unsigned int)time(NULL));

    //Inicializar estructuras
    Cola       cola;
    ListaHilos pool;
    Pila       historial;

    initCola(&cola);
    initPool(&pool, NUM_HILOS);
    initPila(&historial);
    cargarHistorial(&historial);   /* reconstruir pila desde sesion anterior */

    //Abrir log para registrar eventos de esta sesion 
    FILE *log = fopen("log_sesion.txt", "a");
    if (log == NULL) {
        printf("Error: no se pudo abrir el archivo de log.\n");
        return 1;
    }
    fprintf(log, "\n=== Nueva sesion ===\n");

    int contadorTareas = 1;
    int opcion;

    printf("=== Simulador de Sistema Operativo ===\n");

    while (1) {

        // Simular si algun hilo termino su tarea
        simularLiberacion(&pool, &historial, log);

        //Asignar tareas pendientes a hilos libres 
        while (!estaVaciaCola(&cola)) {
            Hilo *libre = buscarHiloLibre(pool.cabeza); //recursiva en lista.c
            if (libre == NULL) break;

            Tarea t = desencolar(&cola);
            libre->tarea_actual = t;
            libre->estado = 1;

            printf("[Sistema] '%s' asignada a Hilo_%d\n", t.nombre, libre->id);
            fprintf(log, "Asignada: %s -> Hilo_%d\n", t.nombre, libre->id);
        }

        // Menu
        printf("\n1. Agregar tarea\n");
        printf("2. Ver estado\n");
        printf("3. Deshacer ultima tarea\n");
        printf("4. Salir\n");
        printf("Opcion: ");
        opcion = leerEntero();

        if (opcion == 1) {
            Tarea nueva;
            nueva.id = contadorTareas;
            sprintf(nueva.nombre, "Proceso_%d", contadorTareas);

            nueva.prioridad = leerPrioridad();

            encolar(&cola, nueva);
            contadorTareas++;
            printf("[Cola] Tarea '%s' agregada.\n", nueva.nombre);

        } else if (opcion == 2) {
            printf("\n--- Cola de tareas ---\n");
            imprimirCola(&cola);

            printf("\n--- Pool de hilos ---\n");
            imprimirPool(&pool);

            printf("\n--- Historial (pila) ---\n");
            imprimirPila(&historial);

        } else if (opcion == 3) {
            if (estaVaciaPila(&historial)) {
                printf("No hay tareas en el historial.\n");
            } else {
                Resultado r = pop(&historial);
                printf("[Deshacer] Se revierto: '%s'\n", r.descripcion);
            }

        } else if (opcion == 4) {
            guardarHistorial(&historial);   /* persistir pila antes de salir */
            break;

        } else {
            printf("Opcion no valida.\n");
        }
    }

    //Liberar memoria y cerrar log
    fprintf(log, "=== Sesion terminada ===\n");
    fclose(log);
    liberarCola(&cola);
    liberarPool(&pool);
    liberarPila(&historial);

    printf("Hasta luego.\n");
    return 0;
}

//limpia pantalla en cualquier sistema :)
void limpiarpantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
//soloo enteros
int leerEntero(void) {
    int valor;
    char buffer[50];

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);

        char *endptr;
        valor = (int)strtol(buffer, &endptr, 10);

        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("Entrada invalida, ingresa un numero: ");
        } else {
            break;
        }
    }
    limpiarpantalla();
    return valor;
}
int leerPrioridad(void) {
    int valor;
    do {
        printf("Prioridad (0=normal, 1=urgente): ");
        valor = leerEntero();
        if (valor != 0 && valor != 1) {
            printf("Solo puedes ingresar 0 o 1.\n");
        }
    } while (valor != 0 && valor != 1);
    return valor;
}
void simularLiberacion(ListaHilos *pool, Pila *historial, FILE *log) {
    NodoHilo *actual = pool->cabeza;

    while (actual != NULL) {
        Hilo *h = &(actual->hilo);

        if (h->estado == 1 && (rand() % 100) < 20 ) {
            //Guardar resultado en la pila 
            Resultado r;
            r.tareaId = h->tarea_actual.id;
            strcpy(r.descripcion, h->tarea_actual.nombre);

            push(historial, r);
            fprintf(log, "Completada: %s por Hilo_%d\n", h->tarea_actual.nombre, h->id);
            printf("[Hilo_%d] termino la tarea '%s'\n", h->id, h->tarea_actual.nombre);

            // Liberar el hilo
            h->estado = 0;
        }

        actual = actual->siguiente;
    }
}

