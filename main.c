/*
 
 main.c - Simulador de sistema operativo (procesador de tareas)
 Estructuras de Datos
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
#include "archivos.h"
 
#define NUM_HILOS 5
//Prototipos
void limpiarpantalla();
int leerEntero();
int leerPrioridad();
void simularLiberacion(ListaHilos *pool, Pila *historial, FILE *log);
void menuArchivos(SistemaArchivos *sa);
 
//main
int main(void) {
    srand((unsigned int)time(NULL));
 
    //Inicializar estructuras
    Cola       cola;
    ListaHilos pool;
    Pila       historial;
    SistemaArchivos sa;
 
    initCola(&cola);
    initPool(&pool, NUM_HILOS);
    initPila(&historial);
    initSistemaArchivos(&sa);
    cargarHistorial(&historial);   /* reconstruir pila desde sesion anterior */
    cargarSistemaArchivos(&sa);    /* recuperar directorios y archivos */
 
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
        printf("4. Sistema de archivos\n");
        printf("5. Salir\n");
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
            menuArchivos(&sa);
 
        } else if (opcion == 5) {
            guardarHistorial(&historial);   /* persistir pila antes de salir */
            guardarSistemaArchivos(&sa);    /* persistir sistema de archivos */
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
 
void menuArchivos(SistemaArchivos *sa) {
    int  op;
    char dir[30];
    char nombre[30];
    char contenido[200];
 
    while (1) {
        printf("\n----- SISTEMA DE ARCHIVOS -----\n");
        printf("1. Crear directorio\n");
        printf("2. Crear archivo\n");
        printf("3. Leer archivo\n");
        printf("4. Eliminar archivo\n");
        printf("5. Listar archivos de un directorio\n");
        printf("6. Ver arbol completo\n");
        printf("7. Volver\n");
        printf("Opcion: ");
        op = leerEntero();
 
        if (op == 1) {
            printf("Nombre del directorio: ");
            if (fgets(dir, sizeof(dir), stdin) != NULL)
                dir[strcspn(dir, "\n")] = '\0';
            crearDirectorio(sa, dir);
 
        } else if (op == 2) {
            printf("Directorio destino: ");
            if (fgets(dir, sizeof(dir), stdin) != NULL)
                dir[strcspn(dir, "\n")] = '\0';
            printf("Nombre del archivo: ");
            if (fgets(nombre, sizeof(nombre), stdin) != NULL)
                nombre[strcspn(nombre, "\n")] = '\0';
            printf("Contenido: ");
            if (fgets(contenido, sizeof(contenido), stdin) != NULL)
                contenido[strcspn(contenido, "\n")] = '\0';
            crearArchivo(sa, dir, nombre, contenido);
 
        } else if (op == 3) {
            printf("Directorio: ");
            if (fgets(dir, sizeof(dir), stdin) != NULL)
                dir[strcspn(dir, "\n")] = '\0';
            printf("Nombre del archivo: ");
            if (fgets(nombre, sizeof(nombre), stdin) != NULL)
                nombre[strcspn(nombre, "\n")] = '\0';
            leerArchivo(sa, dir, nombre);
 
        } else if (op == 4) {
            printf("Directorio: ");
            if (fgets(dir, sizeof(dir), stdin) != NULL)
                dir[strcspn(dir, "\n")] = '\0';
            printf("Nombre del archivo: ");
            if (fgets(nombre, sizeof(nombre), stdin) != NULL)
                nombre[strcspn(nombre, "\n")] = '\0';
            eliminarArchivo(sa, dir, nombre);
 
        } else if (op == 5) {
            printf("Directorio: ");
            if (fgets(dir, sizeof(dir), stdin) != NULL)
                dir[strcspn(dir, "\n")] = '\0';
            listarArchivos(sa, dir);
 
        } else if (op == 6) {
            imprimirArbol(sa);
 
        } else if (op == 7) {
            break;
 
        } else {
            printf("Opcion no valida.\n");
        }
    }
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
