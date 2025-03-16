#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #include "../include/familia.h"
#include "../include/manipularArchivos.h"


// ================ Funciones para lo que seria la creacion de la lista de familias.


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2
NodoFamilia* crearNodo(const char *id, const char *descripcion) {
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


void insertarelementoAlInicio(NodoFamilia** head, const char *id, const char *descripcion) {
    NodoFamilia* nuevoNodo = crearNodo(id, descripcion);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}

void insertarElementoAlFinal(NodoFamilia** head, const char *id, const char *descripcion) {

    NodoFamilia * nuevoNddo = crearNodo(id, descripcion);
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


void eliminarPorId(NodoFamilia** head, char * id) {

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


int buscarPorId(NodoFamilia* head, const char* id) {
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



void imprimirListaNodos(NodoFamilia* head) {
    NodoFamilia *actual = head;

    while (actual != NULL)
    {
        printf("Datos del elementos de la lista:");
        printf("Id: %s, \nDescripcion: %s \n\n", actual->familia.IdFamilia, actual->familia.Descripcion);
        actual = actual->siguiente;
    }
    return;
}



void liberarLista(NodoFamilia* head) {
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

        insertarElementoAlFinal(listaDeFamilias, lista_palabras[0], lista_palabras[1]);

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


void guardarFamiliasEnDB(NodoFamilia** head) {

    NodoFamilia *actul = head;
    while (actul != NULL)
    {

        char *id_familia;
        char *descripcion;

        id_familia = (char *)malloc((strlen(actul->familia.IdFamilia) + 1) * sizeof(char));
        descripcion = (char *)malloc((strlen(actul->familia.Descripcion) + 1) * sizeof(char));

        if (id_familia == NULL || descripcion == NULL) {
            printf("Error al asignar memoria para las cadenas.\n");
            //free(nuevoNodo); // Liberar la memoria del nodo si falla
            exit(1);
        }
        strcpy(id_familia, actul->familia.IdFamilia);
        strcpy(descripcion, actul->familia.Descripcion);    
        //actual->familia.IdFamilia
        //actual->familia.Descripcion
    }
    return;
}