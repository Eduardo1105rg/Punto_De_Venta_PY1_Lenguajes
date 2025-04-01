#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/cargaStock.h"
#include "../include/manipularArchivos.h"


// ================ Funciones para lo que seria la creacion de la lista de familias.


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2

/**
 * Nombre: crearNodoCargaStock
 * 
 * Descripcion: Funcion que crea un nuevo nodo de un elemento, este nodo contendra los valores que define el struct que lo representa.
 * 
 * Entradas: const char *idProducto, const int cantidad: Datos del struct que lo representa.
 * 
 * Salidas:  NodoCargaStock: Devuelve el nodo creado.
 * 
 */
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


/**
 * Nombre: insertarElementoAlInicioCargaStock
 * 
 * Descripcion: Funcion para insertar un nuevo nodo al inicio de la lista de nodos, tambien llama a la funcion encargada de crear el nodo.
 * 
 * Entradas: NodoCargaStock** head: Puntero a la lista de nodos.
 * const char *idProducto, const int cantidad: Datos del nodo.
 * 
 * Salidas: No posee.
 * 
 */
void insertarElementoAlInicioCargaStock(NodoCargaStock** head, const char *idProducto, const int cantidad) {
    NodoCargaStock* nuevoNodo = crearNodoCargaStock(idProducto, cantidad);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}


/**
 * Nombre: insertarElementoAlFinalCargaStock
 * 
 * Descripcion: Funcion para insertar un nuevo nodo al final de la lista de nodos, tambien llama a la funcion encargada de crear el nodo.
 * 
 * Entradas: NodoCargaStock** head: Puntero a la lista de nodos.
 * const char *idProducto, const int cantidad: Datos del nodo.
 * Salidas: No posee.
 * 
 */
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


/**
 * Nombre: eliminarPorIdNodoCargaStock
 * 
 * Descripcion: Funcion para eliminar un elementos especifico de la lista de nodos.
 * 
 * Entradas:  NodoCargaStock** head: Puntero a la lista de nodos.
 *  char * idProducto: Id del NodoCargaStock a eliminar.
 * 
 * Salidas: No posee.
 * 
 */
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


/**
 * Nombre: buscarPorIdCargaStock
 * 
 * Descripcion: Funcion para buscar la exitencia de un elementos en la lista de nodos, retorna un entero indicando la exitencia del producto en la lista.
 * 
 * Entradas: NodoCargaStock* head: Puntero a la lista de nodos.
 *  const char* idProducto: Id del NodoCargaStock a buscar.
 * 
 * Salidas: Un entero que representa la exitencia del elemento buscado, 0 (No existe), 1 (Existe).
 * 
 */
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


/**
 * Nombre: imprimirListaNodosCargaStock
 * 
 * Descripcion: Funcion para imprimir los datos de una lista de nodos.
 * 
 * Entradas: NodoCargaStock* head: Puntero a la lista de nodos.
 * 
 * Salidas: No posee.
 * 
 */
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


/**
 * Nombre: liberarListaCargaStock
 * 
 * Descripcion: Funcion que se encarga de liberar la memoria asignada a los nodos y datos de la lista.
 * 
 * Entradas: NodoCargaStock* head: Puntero a la lista de nodos.
 * 
 * Salidas: No posee.
 * 
 */
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


/**
 * Nombre: cargarStockDesdeArchivo
 * 
 * Descripcion: Esta funcion se encarga de cargar los datos desde un archivo, validarlo y posteriormente los alamacena en una lista de punteros de nodos que almacenan la 
 * informacion del archivo.
 * 
 * Entradas: char * nombreArchivo: Nombre del archivo del que se estan leyendo lo datos.
 *  NodoCargaStock** listaCargaStock: Lista de punteros para la lista de nodos en la que se estan guardando los datos.
 * 
 * Salidas: El estado de la carga de datos.
 * 
 */
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


/**
 * Nombre: agregarStock
 * 
 * Descripcion: Funcion para alamacenar en la base de datos los producto en la base de datos.
 * 
 * Entradas: MYSQL *conexion: conexion un puntero a un tipo de datos mysql.
 *  const char *id_producto: I del producto a agregar en stock.
 *  int cantidad: Cantidad del prodcuto a agregar.
 * 
 * Salidas:No posee.
 * 
 */
void agregarStock(MYSQL *conexion, const char *id_producto, int cantidad){
    if (id_producto == NULL || cantidad == 0) {
        printf("Error: Por favor ingrese datos distintos de nullo o una cantidad mayor a cero\n");
        return;
    }
    char *consulta;
    int largo = asprintf(&consulta, "call actualizaStockC('%s', %d);", id_producto, cantidad);

    if (mysql_query(conexion, consulta)) {
        //printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        printf("No se pudo agregar el %s pues su cantidad %d excede el stock dejandolo en negativo", id_producto, cantidad);
        free(consulta);
        return;
    }
    printf("Se ha modificado la cantidad del producto con id:%s.", id_producto);
    return;
}


/**
 * Nombre: guardarStockEnDB
 * 
 * Descripcion: Funcion encargada de registrar los datos del stock cargado desde un archivo en la base de datos.
 * 
 * Entradas: MYSQL *conexion un puntero a un tipo de datos mysql.
 *  NodoCargaStock* head: Punteroa a la lista de nodos.
 * 
 * Salidas: No posee.
 * 
 */
void guardarStockEnDB(MYSQL *conexion, NodoCargaStock* head) {

    NodoCargaStock *actual = head;
    while (actual != NULL)
    {


        char *id_producto;
        int cantidad = 0;

        id_producto = (char *)malloc((strlen(actual->stockCargado.IdProducto) + 1) * sizeof(char));

        if (id_producto == NULL) {
            printf("Error al asignar memoria para las cadenas.\n");
            //free(nuevoNodo); // Liberar la memoria del nodo si falla
            free(id_producto);
            exit(1);
        }
        cantidad = actual->stockCargado.Cantidad;
        strcpy(id_producto, actual->stockCargado.IdProducto);
        agregarStock(conexion,id_producto,cantidad);
        //actual->familia.IdFamilia
        //actual->familia.Descripcion

        free(id_producto);

        actual = actual->siguiente;

    }
    return;
}
