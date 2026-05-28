/*
 archivos.h - Definicion del sistema de archivos simulado
 Adiel Lopez Cruz

 Estructura:
   Directorio raiz (/)
   └── Directorio hijo  (ej: /docs)
       └── Archivo      (ej: /docs/nota.txt)

 Todo se guarda en memoria (arreglos estaticos).
 Se puede persistir con guardarSistemaArchivos / cargarSistemaArchivos.
 */

#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "sistema.h"

/* ── Limites ────────────────────────────────────────────── */
#define MAX_NOMBRE_ARCHIVO  30
#define MAX_CONTENIDO      200
#define MAX_ARCHIVOS        20   /* archivos por directorio  */
#define MAX_DIRECTORIOS     10   /* directorios en el sistema */

/* ── Archivo simulado ───────────────────────────────────── */
/*
 ArchivoSim
 Representa un archivo dentro de un directorio.
 Guarda nombre, contenido de texto y si esta activo.
 */
typedef struct {
    char nombre   [MAX_NOMBRE_ARCHIVO];
    char contenido[MAX_CONTENIDO];
    int  activo;   /* 1 = existe, 0 = eliminado */
} ArchivoSim;

/* ── Directorio ─────────────────────────────────────────── */
/*
 Directorio
 Tiene un nombre y un arreglo de hasta MAX_ARCHIVOS archivos.
 activo = 0 significa que el slot esta libre.
 */
typedef struct {
    char       nombre   [MAX_NOMBRE_ARCHIVO];
    ArchivoSim archivos [MAX_ARCHIVOS];
    int        numArchivos;
    int        activo;   /* 1 = existe, 0 = slot libre */
} Directorio;

/* ── Sistema de archivos ────────────────────────────────── */
/*
 SistemaArchivos
 Contenedor principal: arreglo de directorios.
 */
typedef struct {
    Directorio dirs[MAX_DIRECTORIOS];
    int        numDirs;
} SistemaArchivos;

/* ── Prototipos ─────────────────────────────────────────── */

/* Inicializacion */
void initSistemaArchivos (SistemaArchivos *sa);

/* Directorios */
int  crearDirectorio     (SistemaArchivos *sa, const char *nombre);
int  buscarDirectorio    (SistemaArchivos *sa, const char *nombre);
void listarDirectorios   (SistemaArchivos *sa);

/* Archivos */
int  crearArchivo        (SistemaArchivos *sa, const char *dir,
                          const char *nombre, const char *contenido);
int  leerArchivo         (SistemaArchivos *sa, const char *dir,
                          const char *nombre);
int  eliminarArchivo     (SistemaArchivos *sa, const char *dir,
                          const char *nombre);
void listarArchivos      (SistemaArchivos *sa, const char *dir);

/* Vista del arbol completo */
void imprimirArbol       (SistemaArchivos *sa);

/* Persistencia */
void guardarSistemaArchivos (SistemaArchivos *sa);
void cargarSistemaArchivos  (SistemaArchivos *sa);

#endif /* ARCHIVOS_H */
