#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/familia.h"
#include "../include/manipularArchivos.h"


// ================ Funciones para lo que seria la creacion de la lista de familias.


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2
NodoFamilia* crearNodoFamilia(const char *id, const char *descripcion) {
    NodoFamilia* nuevoNodo = (NodoFamilia*)malloc(sizeof(NodoFamilia));

    if (nuevoNodo == NULL) {
        printf("Error a asignar memoria al nodo.");
        exit(1);
    }

    // Asignar memoria para IdFamilia y Descripcion
    nuevoNodo->familia.IdFamilia = (char *)malloc((strlen(id) + 1) * sizeof(char));
    nuevoNodo->familia.Descripcion = (char *)malloc((strlen(descripcion) + 1) * sizeof(char));

    if (nuevoNodo->familia.IdFamilia == NULL || nuevoNodo->familia.Descripcion == NULL) {
        printf("Error al asignar memoria para las cadenas.\n");
        free(nuevoNodo); // Liberar la memoria del nodo si falla
        exit(1);
    }

    // En esta parte de copiarian los datos y se le asignarian a lo de 
    // este guardado en el nodo.
    strcpy(nuevoNodo->familia.IdFamilia, id);
    strcpy(nuevoNodo->familia.Descripcion, descripcion);


    //newNodo->familia.IdFamilia[alFinalDelString] = '\0';
    return nuevoNodo;
}

void insertarelementoAlInicioFamilia(NodoFamilia** head, const char *id, const char *descripcion) {
    NodoFamilia* nuevoNodo = crearNodoFamilia(id, descripcion);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}

void insertarElementoAlFinalFamilia(NodoFamilia** head, const char *id, const char *descripcion) {

    NodoFamilia * nuevoNddo = crearNodoFamilia(id, descripcion);
    if (*head == NULL) {
        *head = nuevoNddo;
        return;
    }

    NodoFamilia* actual = *head;
    while (actual->siguiente != NULL)
    {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevoNddo;
    return;
}

void eliminarPorIdFamilia(NodoFamilia** head, char * id) {

    NodoFamilia* actual = *head;
    NodoFamilia* anterior = NULL;

    while (actual != NULL && strcmp(actual->familia.IdFamilia, id) != 0)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("La familia con el indice %s no ha sido encontrada.", id);
        return;
    }

    if (anterior == NULL)
    {
        *head = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    
    free(actual);
    return;
}

int buscarPorIdFamilia(NodoFamilia* head, const char* id) {
    NodoFamilia* actual = head;

    while (actual != NULL)
    {
        if (strcmp(actual->familia.IdFamilia, id) == 0) {
            return 1;
        }
        actual = actual->siguiente;
    }

    return 0;
}

void imprimirListaNodosFamilia(NodoFamilia* head) {
    NodoFamilia *actual = head;

    while (actual != NULL)
    {
        printf("Datos del elementos de la lista:\n");
        printf("Id: %s, \nDescripcion: %s \n\n", actual->familia.IdFamilia, actual->familia.Descripcion);
        actual = actual->siguiente;
    }
    return;
}

void liberarListaFamilia(NodoFamilia* head) {
    NodoFamilia *actual;
    while (head != NULL)
    {
        actual = head;
        head = head->siguiente;

        free(actual->familia.IdFamilia);
        free(actual->familia.Descripcion);
        free(actual);
    }
    return;
}


// ==============Este seria el apartado para cargar los datos de una familia des un archivo y su guardado en la base de datos.
int cargarFamiliasDesdeArchivo(char * nombreArchivo, NodoFamilia** listaDeFamilias) {
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return -1;
    }

    int num_linea = 0;

    while (!feof(archivo))
    {
        num_linea++;
        char *linea = leer_lineas(archivo, num_linea);

        int num_palabras;
        char** lista_palabras = separar_cadenas(linea, &num_palabras);
        
        //printf("Elementos de la lista: %s ---- %s", lista_palabras[0], lista_palabras[1]);

        insertarElementoAlFinalFamilia(listaDeFamilias, lista_palabras[0], lista_palabras[1]);

        //imprimir_lista_palabras(lista_palabras, num_palabras, num_linea);

        for (int i = 0; i < num_palabras; i++) {
            free(lista_palabras[i]);
        }
        free(lista_palabras);


        free(linea);
    }
    

    fclose(archivo);
    return 0;
}


void guardarFamiliasEnDB(NodoFamilia* head) {

    NodoFamilia *actual = head;
    while (actual != NULL)
    {

        printf("Pass...");

        char *id_familia;
        char *descripcion;

        id_familia = (char *)malloc((strlen(actual->familia.IdFamilia) + 1) * sizeof(char));
        descripcion = (char *)malloc((strlen(actual->familia.Descripcion) + 1) * sizeof(char));

        if (id_familia == NULL || descripcion == NULL) {
            printf("Error al asignar memoria para las cadenas.\n");
            //free(nuevoNodo); // Liberar la memoria del nodo si falla
            free(id_familia);
            free(descripcion);
            exit(1);
        }
        strcpy(id_familia, actual->familia.IdFamilia);
        strcpy(descripcion, actual->familia.Descripcion);    
        //actual->familia.IdFamilia
        //actual->familia.Descripcion

        free(id_familia);
        free(descripcion);

        actual = actual->siguiente;

    }
    return;
}