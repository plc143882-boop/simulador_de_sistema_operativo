/*
 pila.c - Implementacion de la pila de historial
 */

#include <stdio.h>
#include <stdlib.h>
#include "pila.h"

/*
initPila
Inicializa la pila con la cima en NULL
*/
void initPila(Pila *pila) {
    pila->cima = NULL;
}

/* 
estaVaciaPila
Debuelve 1 si la pila esta vacia, 0 si no
 */
int estaVaciaPila(Pila *pila) {
    return pila->cima == NULL;
}

/* 
push
Inserta un Resultado en la cima de la pila
 */
void push(Pila *pila, Resultado r) {
    NodoPila *nuevo = (NodoPila *)malloc(sizeof(NodoPila));
    if (nuevo == NULL) {
        printf("Error: no hay memoria para la pila.\n");
        return;
    }
    nuevo->resultado  = r;
    nuevo->siguiente  = pila->cima;
    pila->cima        = nuevo;
}

/* 
pop
Saca el Resultado de la cima y libera el nodo.
*/
Resultado pop(Pila *pila) {
    NodoPila  *temp = pila->cima;
    Resultado  r    = temp->resultado;
    pila->cima      = temp->siguiente;
    free(temp);
    return r;
}

/* 
imprimirPilaRec
Funcion auxiliar recursiva. Llega hasta el fondo
de la pila e imprime de abajo hacia arriba, asi
se ve el orden real de ejecucion.
*/
static void imprimirPilaRec(NodoPila *nodo, int nivel) {
    if (nodo == NULL) return;                      /* caso base */
    imprimirPilaRec(nodo->siguiente, nivel + 1);   /* ir al fondo */
    printf("  [%d] %s\n", nivel, nodo->resultado.descripcion);
}

/* 
imprimirPila
Muestra el historial La cima es la ultima tarea
completada (la mas reciente)
*/
void imprimirPila(Pila *pila) {
    if (estaVaciaPila(pila)) {
        printf("  Historial vacio.\n");
        return;
    }
    printf("  (cima = ultima tarea completada)\n");
    imprimirPilaRec(pila->cima, 1);
}

/*
liberarPila
Hace pop de todos los nodos para liberar la memoria.
*/
void liberarPila(Pila *pila) {
    while (!estaVaciaPila(pila)) {
        pop(pila);
    }
}

/* 
guardarPilaRec  (RECURSIVA)
Llega hasta el fondo de la pila y escribe de abajo
hacia arriba, para que al cargar quede el mismo orden
Caso base: nodo == NULL -> no hay mas nodos
Caso rec.: ir al fondo primero, luego escribir
*/
static void guardarPilaRec(NodoPila *nodo, FILE *archivo) {
    if (nodo == NULL) return;                        //caso base
    guardarPilaRec(nodo->siguiente, archivo);        //ir al fondo 
    fprintf(archivo, "%d|%s\n", nodo->resultado.tareaId, nodo->resultado.descripcion);
}

/* 
 guardarHistorial
Sobreescribe historial.txt con el contenido actual
de la pila. Se llama justo antes de salir.
*/
void guardarHistorial(Pila *pila) {
    FILE *archivo = fopen("historial.txt", "w");
    if (archivo == NULL) {
        printf("Error: no se pudo guardar el historial.\n");
        return;
    }
    guardarPilaRec(pila->cima, archivo);
    fclose(archivo);
}

/*
cargarHistorial
 Lee historial.txt y reconstruye la pila con los
 resultados de sesiones anteriores. Se llama al inicio.
 */
void cargarHistorial(Pila *pila) {
    FILE *archivo = fopen("historial.txt", "r");
    if (archivo == NULL) return;   /* no existe historial previo, no es error */

    Resultado r;
    while (fscanf(archivo, "%d|%49[^\n]\n", &r.tareaId, r.descripcion) == 2) {
        push(pila, r);
    }

    fclose(archivo);
    printf("[Sistema] Historial previo cargado.\n");
}
