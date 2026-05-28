/*
 archivos.c - Implementacion del sistema de archivos simulado
 Adiel Lopez Cruz

 Flujo:
   initSistemaArchivos -> crearDirectorio -> crearArchivo
                       -> leerArchivo
                       -> eliminarArchivo
                       -> listarArchivos / imprimirArbol
   guardarSistemaArchivos / cargarSistemaArchivos  (persistencia)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archivos.h"

/* ── Inicializacion ─────────────────────────────────────── */

/*
 initSistemaArchivos
 Marca todos los slots de directorios como libres
 y crea el directorio raiz "/" automaticamente.
 */
void initSistemaArchivos(SistemaArchivos *sa) {
    int i, j;

    sa->numDirs = 0;

    for (i = 0; i < MAX_DIRECTORIOS; i++) {
        sa->dirs[i].activo      = 0;
        sa->dirs[i].numArchivos = 0;

        for (j = 0; j < MAX_ARCHIVOS; j++) {
            sa->dirs[i].archivos[j].activo = 0;
        }
    }

    /* Crear directorio raiz por defecto */
    crearDirectorio(sa, "raiz");
}

/* ── Directorios ────────────────────────────────────────── */

/*
 buscarDirectorio
 Recorre el arreglo buscando un directorio activo con ese nombre.
 Devuelve el indice o -1 si no existe.
 */
int buscarDirectorio(SistemaArchivos *sa, const char *nombre) {
    int i;
    for (i = 0; i < MAX_DIRECTORIOS; i++) {
        if (sa->dirs[i].activo &&
            strcmp(sa->dirs[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 crearDirectorio
 Busca un slot libre y crea un nuevo directorio.
 Devuelve el indice creado o -1 si no hay espacio o ya existe.
 */
int crearDirectorio(SistemaArchivos *sa, const char *nombre) {
    int i;

    if (buscarDirectorio(sa, nombre) != -1) {
        printf("  [FS] Error: el directorio '%s' ya existe.\n", nombre);
        return -1;
    }

    if (sa->numDirs >= MAX_DIRECTORIOS) {
        printf("  [FS] Error: no hay espacio para mas directorios.\n");
        return -1;
    }

    /* Buscar primer slot libre */
    for (i = 0; i < MAX_DIRECTORIOS; i++) {
        if (!sa->dirs[i].activo) {
            strncpy(sa->dirs[i].nombre, nombre, MAX_NOMBRE_ARCHIVO - 1);
            sa->dirs[i].nombre[MAX_NOMBRE_ARCHIVO - 1] = '\0';
            sa->dirs[i].activo      = 1;
            sa->dirs[i].numArchivos = 0;
            sa->numDirs++;
            printf("  [FS] Directorio '/%s' creado.\n", nombre);
            return i;
        }
    }

    return -1;
}

/*
 listarDirectorios
 Muestra todos los directorios activos y cuantos archivos tienen.
 */
void listarDirectorios(SistemaArchivos *sa) {
    int i;
    int hay = 0;

    printf("  Directorios en el sistema:\n");

    for (i = 0; i < MAX_DIRECTORIOS; i++) {
        if (sa->dirs[i].activo) {
            printf("    [DIR] /%s  (%d archivo(s))\n",
                   sa->dirs[i].nombre,
                   sa->dirs[i].numArchivos);
            hay = 1;
        }
    }

    if (!hay) {
        printf("    (sin directorios)\n");
    }
}

/* ── Archivos ───────────────────────────────────────────── */

/*
 crearArchivo
 Busca el directorio indicado y agrega un archivo con el
 contenido dado. Devuelve 1 si tuvo exito, 0 si no.
 */
int crearArchivo(SistemaArchivos *sa, const char *dir,
                 const char *nombre, const char *contenido) {
    int idx = buscarDirectorio(sa, dir);
    int j;

    if (idx == -1) {
        printf("  [FS] Error: directorio '%s' no encontrado.\n", dir);
        return 0;
    }

    if (sa->dirs[idx].numArchivos >= MAX_ARCHIVOS) {
        printf("  [FS] Error: directorio '/%s' lleno.\n", dir);
        return 0;
    }

    /* Verificar que no exista ya un archivo con ese nombre */
    for (j = 0; j < MAX_ARCHIVOS; j++) {
        if (sa->dirs[idx].archivos[j].activo &&
            strcmp(sa->dirs[idx].archivos[j].nombre, nombre) == 0) {
            printf("  [FS] Error: '%s' ya existe en /%s.\n", nombre, dir);
            return 0;
        }
    }

    /* Buscar slot libre dentro del directorio */
    for (j = 0; j < MAX_ARCHIVOS; j++) {
        if (!sa->dirs[idx].archivos[j].activo) {
            strncpy(sa->dirs[idx].archivos[j].nombre,
                    nombre, MAX_NOMBRE_ARCHIVO - 1);
            sa->dirs[idx].archivos[j].nombre[MAX_NOMBRE_ARCHIVO - 1] = '\0';

            strncpy(sa->dirs[idx].archivos[j].contenido,
                    contenido, MAX_CONTENIDO - 1);
            sa->dirs[idx].archivos[j].contenido[MAX_CONTENIDO - 1] = '\0';

            sa->dirs[idx].archivos[j].activo = 1;
            sa->dirs[idx].numArchivos++;

            printf("  [FS] Archivo '/%s/%s' creado.\n", dir, nombre);
            return 1;
        }
    }

    return 0;
}

/*
 leerArchivo
 Busca el archivo y muestra su contenido.
 Devuelve 1 si lo encontro, 0 si no.
 */
int leerArchivo(SistemaArchivos *sa, const char *dir,
                const char *nombre) {
    int idx = buscarDirectorio(sa, dir);
    int j;

    if (idx == -1) {
        printf("  [FS] Error: directorio '%s' no encontrado.\n", dir);
        return 0;
    }

    for (j = 0; j < MAX_ARCHIVOS; j++) {
        if (sa->dirs[idx].archivos[j].activo &&
            strcmp(sa->dirs[idx].archivos[j].nombre, nombre) == 0) {

            printf("  [FS] Contenido de /%s/%s:\n", dir, nombre);
            printf("  ----------------------------------------\n");
            printf("  %s\n", sa->dirs[idx].archivos[j].contenido);
            printf("  ----------------------------------------\n");
            return 1;
        }
    }

    printf("  [FS] Error: archivo '%s' no encontrado en /%s.\n",
           nombre, dir);
    return 0;
}

/*
 eliminarArchivo
 Marca el archivo como inactivo (borrado logico).
 Devuelve 1 si tuvo exito, 0 si no.
 */
int eliminarArchivo(SistemaArchivos *sa, const char *dir,
                    const char *nombre) {
    int idx = buscarDirectorio(sa, dir);
    int j;

    if (idx == -1) {
        printf("  [FS] Error: directorio '%s' no encontrado.\n", dir);
        return 0;
    }

    for (j = 0; j < MAX_ARCHIVOS; j++) {
        if (sa->dirs[idx].archivos[j].activo &&
            strcmp(sa->dirs[idx].archivos[j].nombre, nombre) == 0) {

            sa->dirs[idx].archivos[j].activo = 0;
            sa->dirs[idx].numArchivos--;
            printf("  [FS] Archivo '/%s/%s' eliminado.\n", dir, nombre);
            return 1;
        }
    }

    printf("  [FS] Error: archivo '%s' no encontrado en /%s.\n",
           nombre, dir);
    return 0;
}

/*
 listarArchivos
 Muestra todos los archivos activos dentro de un directorio.
 */
void listarArchivos(SistemaArchivos *sa, const char *dir) {
    int idx = buscarDirectorio(sa, dir);
    int j;
    int hay = 0;

    if (idx == -1) {
        printf("  [FS] Error: directorio '%s' no encontrado.\n", dir);
        return;
    }

    printf("  Archivos en /%s:\n", dir);

    for (j = 0; j < MAX_ARCHIVOS; j++) {
        if (sa->dirs[idx].archivos[j].activo) {
            printf("    [ARC] %s\n", sa->dirs[idx].archivos[j].nombre);
            hay = 1;
        }
    }

    if (!hay) {
        printf("    (directorio vacio)\n");
    }
}

/*
 imprimirArbol
 Muestra el arbol completo: directorios y sus archivos.
 Similar a un 'ls -R' basico.
 */
void imprimirArbol(SistemaArchivos *sa) {
    int i, j;
    int hayAlgo = 0;

    printf("  Sistema de archivos:\n");
    printf("  /\n");

    for (i = 0; i < MAX_DIRECTORIOS; i++) {
        if (!sa->dirs[i].activo) continue;

        printf("  |-- %s/\n", sa->dirs[i].nombre);
        hayAlgo = 1;

        for (j = 0; j < MAX_ARCHIVOS; j++) {
            if (sa->dirs[i].archivos[j].activo) {
                printf("  |   |-- %s\n",
                       sa->dirs[i].archivos[j].nombre);
            }
        }
    }

    if (!hayAlgo) {
        printf("  (sistema vacio)\n");
    }
}

/* ── Persistencia ───────────────────────────────────────── */

/*
 guardarSistemaArchivos
 Escribe todos los directorios y archivos activos en
 "fs_datos.txt" para recuperarlos en la siguiente sesion.
 Formato por linea:
   DIR|nombre_directorio
   ARC|dir|nombre|contenido
 */
void guardarSistemaArchivos(SistemaArchivos *sa) {
    FILE *f = fopen("fs_datos.txt", "w");
    int   i, j;

    if (f == NULL) {
        printf("  [FS] Error: no se pudo guardar el sistema de archivos.\n");
        return;
    }

    for (i = 0; i < MAX_DIRECTORIOS; i++) {
        if (!sa->dirs[i].activo) continue;

        fprintf(f, "DIR|%s\n", sa->dirs[i].nombre);

        for (j = 0; j < MAX_ARCHIVOS; j++) {
            if (sa->dirs[i].archivos[j].activo) {
                fprintf(f, "ARC|%s|%s|%s\n",
                        sa->dirs[i].nombre,
                        sa->dirs[i].archivos[j].nombre,
                        sa->dirs[i].archivos[j].contenido);
            }
        }
    }

    fclose(f);
    printf("  [FS] Sistema de archivos guardado.\n");
}

/*
 cargarSistemaArchivos
 Lee "fs_datos.txt" y reconstruye directorios y archivos.
 Si el archivo no existe, no es un error (primera sesion).
 */
void cargarSistemaArchivos(SistemaArchivos *sa) {
    FILE *f = fopen("fs_datos.txt", "r");
    char  linea[300];
    char  tipo[4];
    char  campo1[MAX_NOMBRE_ARCHIVO];
    char  campo2[MAX_NOMBRE_ARCHIVO];
    char  contenido[MAX_CONTENIDO];

    if (f == NULL) return;   /* primera vez, no hay datos previos */

    while (fgets(linea, sizeof(linea), f) != NULL) {

        /* Quitar salto de linea al final */
        linea[strcspn(linea, "\n")] = '\0';

        if (sscanf(linea, "DIR|%29s", campo1) == 1) {
            /* Crear el directorio si no existe ya */
            if (buscarDirectorio(sa, campo1) == -1) {
                crearDirectorio(sa, campo1);
            }

        } else if (sscanf(linea, "ARC|%29[^|]|%29[^|]|%199[^\n]",
                          campo1, campo2, contenido) == 3) {
            /* campo1 = dir, campo2 = nombre, contenido = texto */
            crearArchivo(sa, campo1, campo2, contenido);
        }
    }

    fclose(f);
    printf("  [FS] Sistema de archivos cargado desde sesion anterior.\n");
}
