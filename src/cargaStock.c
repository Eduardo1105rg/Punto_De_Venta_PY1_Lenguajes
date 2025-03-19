#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/cargaStock.h"
#include "../include/manipularArchivos.h"


// ================ Funciones para lo que seria la creacion de la lista de familias.


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2
NodoCargaStock* crearNodoCargaStock(const char *idProducto, const int cantidad) {
    NodoCargaStock* nuevoNodo = (NodoCargaStock*)malloc(sizeof(NodoCargaStock));

    if (nuevoNodo == NULL) {
        printf("Error a asignar memoria al nodo.");
        exit(1);
    }

    // Asignar memoria para IdFamilia y Descripcion
    nuevoNodo->stockCargado.IdProducto = (char *)malloc((strlen(idProducto) + 1) * sizeof(char));
    //nuevoNodo->stockCargado = (char *)malloc((strlen(can) + 1) * sizeof(char));

    if (nuevoNodo->stockCargado.IdProducto == NULL) {
        printf("Error al asignar memoria para las cadenas.\n");
        free(nuevoNodo); // Liberar la memoria del nodo si falla
        exit(1);
    }

    // En esta parte de copiarian los datos y se le asignarian a lo de 
    // este guardado en el nodo.
    strcpy(nuevoNodo->stockCargado.IdProducto, idProducto);
    //strcpy(nuevoNodo->familia.Descripcion, descripcion);

    nuevoNodo->stockCargado.Cantidad = cantidad;

    //newNodo->familia.IdFamilia[alFinalDelString] = '\0';
    nuevoNodo->siguiente = NULL; //cambio realizado
    return nuevoNodo;
}

void insertarElementoAlInicioCargaStock(NodoCargaStock** head, const char *idProducto, const int cantidad) {
    NodoCargaStock* nuevoNodo = crearNodoCargaStock(idProducto, cantidad);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}

void insertarElementoAlFinalCargaStock(NodoCargaStock** head, const char *idProducto, const int cantidad) {

    NodoCargaStock * nuevoNddo = crearNodoCargaStock(idProducto, cantidad);
    if (*head == NULL) {
        *head = nuevoNddo;
        return;
    }

    NodoCargaStock* actual = *head;
    while (actual->siguiente != NULL)
    {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevoNddo;
    return;
}

void eliminarPorIdNodoCargaStock(NodoCargaStock** head, char * idProducto) {

    NodoCargaStock* actual = *head;
    NodoCargaStock* anterior = NULL;

    while (actual != NULL && strcmp(actual->stockCargado.IdProducto, idProducto) != 0)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("La familia con el indice %s no ha sido encontrada.", idProducto);
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

int buscarPorIdCargaStock(NodoCargaStock* head, const char* id) {
    NodoCargaStock* actual = head;

    while (actual != NULL)
    {
        if (strcmp(actual->stockCargado.IdProducto, id) == 0) {
            return 1;
        }
        actual = actual->siguiente;
    }

    return 0;
}

void imprimirListaNodosCargaStock(NodoCargaStock* head) {
    NodoCargaStock *actual = head;

    while (actual != NULL)
    {
        printf("Datos del elementos de la lista:\n");
        printf("Id: %s, \nCantidad: %i \n\n", actual->stockCargado.IdProducto, actual->stockCargado.Cantidad);
        actual = actual->siguiente;
    }
    return;
}

void liberarListaCargaStock(NodoCargaStock* head) {
    NodoCargaStock *actual;
    while (head != NULL)
    {
        actual = head;
        head = head->siguiente;

        free(actual->stockCargado.IdProducto);
        free(actual);
    }
    return;
}


// ==============Este seria el apartado para cargar los datos de una familia des un archivo y su guardado en la base de datos.
int cargarStockDesdeArchivo(char * nombreArchivo, NodoCargaStock** listaCargaStock) {
    
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
        int cantidad = atoi(lista_palabras[1]);
        
        //printf("Elementos de la lista: Id: %s ---- Stock: %i\n", lista_palabras[0], cantidad);

        insertarElementoAlFinalCargaStock(listaCargaStock, lista_palabras[0], cantidad);

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

void agregarStock(MYSQL *conexion, const char *id_producto, int cantidad){

    printf("%d\n",cantidad);
    printf("%s\n",id_producto);
    printf("SOY YO");
    if (id_producto == NULL || cantidad == 0) {
        printf("Error: Por favor ingrese datos distintos de nullo o una cantidad mayor a cero\n");
        return;
    }
    char *consulta;
    int largo = asprintf(&consulta, "update Productos set Cantidad = %d where IdProducto = '%s'", cantidad, id_producto);

    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
}

void guardarStockEnDB(MYSQL *conexion, NodoCargaStock* head) {

    NodoCargaStock *actual = head;
    while (actual != NULL)
    {

        printf("Pass...");

        char *id_producto;
        int cantidad;
        printf("Soy la cantidad %d:" ,cantidad );

        id_producto = (char *)malloc((strlen(actual->stockCargado.IdProducto) + 1) * sizeof(char));

        if (id_producto == NULL) {
            printf("Error al asignar memoria para las cadenas.\n");
            //free(nuevoNodo); // Liberar la memoria del nodo si falla
            free(id_producto);
            exit(1);
        }
        strcpy(id_producto, actual->stockCargado.IdProducto);

        agregarStock(conexion,id_producto,cantidad);
        //actual->familia.IdFamilia
        //actual->familia.Descripcion

        free(id_producto);

        actual = actual->siguiente;

    }
    return;
}