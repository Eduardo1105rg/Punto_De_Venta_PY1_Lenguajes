#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/cotizacion.h"


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2
NodoCotizacionDetalle* crearNodoCotizacionDetalle(const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad) {
    NodoCotizacionDetalle* nuevoNodo = (NodoCotizacionDetalle*)malloc(sizeof(NodoCotizacionDetalle));

    if (nuevoNodo == NULL) {
        printf("Error a asignar memoria al nodo.");
        exit(1);
    }

    // Asignar memoria para IdFamilia y Descripcion
    nuevoNodo->detallesCotizacion.IdProducto = (char *)malloc((strlen(id) + 1) * sizeof(char));
    nuevoNodo->detallesCotizacion.NombreProducto = (char *)malloc((strlen(nombreProducto) + 1) * sizeof(char));
    nuevoNodo->detallesCotizacion.Descripcion = (char *)malloc((strlen(descripcion) + 1) * sizeof(char));

    if (nuevoNodo->detallesCotizacion.IdProducto == NULL || nuevoNodo->detallesCotizacion.NombreProducto == NULL || nuevoNodo->detallesCotizacion.Descripcion == NULL) {
        printf("Error al asignar memoria para las cadenas.\n");
        free(nuevoNodo); // Liberar la memoria del nodo si falla
        exit(1);
    }

    // En esta parte de copiarian los datos y se le asignarian a lo de 
    // este guardado en el nodo.
    strcpy(nuevoNodo->detallesCotizacion.IdProducto, id);
    strcpy(nuevoNodo->detallesCotizacion.NombreProducto, nombreProducto);
    strcpy(nuevoNodo->detallesCotizacion.Descripcion, descripcion);
    nuevoNodo->detallesCotizacion.precio = precio;
    nuevoNodo->detallesCotizacion.cantidad = cantidad;


    nuevoNodo->siguiente = NULL; //cambio realizado
    return nuevoNodo;
}

void insertarelementoAlInicioCotizacionDetalle(NodoCotizacionDetalle** head, const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad) {
    NodoCotizacionDetalle* nuevoNodo = crearNodoCotizacionDetalle(id, nombreProducto, descripcion, precio, cantidad);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}

void insertarElementoAlFinalCotizacionDetalle(NodoCotizacionDetalle** head, const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad) {

    //Cambio 
    if (head == NULL) {
        printf("Error: El puntero 'head' es NULL.\n");
        return;
    }

    NodoCotizacionDetalle* nuevoNodo = crearNodoCotizacionDetalle(id, nombreProducto, descripcion, precio, cantidad);

    if (nuevoNodo == NULL) {
        printf("Error al crear un nuevo nodo.\n");
        return;
    }    

    if (*head == NULL) {

        nuevoNodo->detallesCotizacion.NumeroFila = 1;
        *head = nuevoNodo;
        return;
    }

    int numeroFila = 2;
    NodoCotizacionDetalle* actual = *head;
    while (actual->siguiente != NULL)
    {
        actual = actual->siguiente;
        numeroFila++;
    }
    nuevoNodo->detallesCotizacion.NumeroFila = numeroFila;

    actual->siguiente = nuevoNodo;
    return;
}

void eliminarPorIdCotizacionDetalle(NodoCotizacionDetalle** head, char * id) {

    NodoCotizacionDetalle* actual = *head;
    NodoCotizacionDetalle* anterior = NULL;

    while (actual != NULL && strcmp(actual->detallesCotizacion.IdProducto, id) != 0)
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

void eliminarCotizacionPorNumFila(NodoCotizacionDetalle** head, const int numFila) {

    NodoCotizacionDetalle* actual = *head;
    NodoCotizacionDetalle* anterior = NULL;

    while (actual != NULL && actual->detallesCotizacion.NumeroFila != numFila)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("El producto con el indice %i no ha sido encontrado.", numFila);
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


int buscarPorIdCotizacionDetalle(NodoCotizacionDetalle* head, const char* id) {

    if (head == NULL) {
        return 0;
    }

    NodoCotizacionDetalle* actual = head;

    while (actual != NULL)
    {
        if (strcmp(actual->detallesCotizacion.IdProducto, id) == 0) {
            return actual->detallesCotizacion.cantidad;
        }
        actual = actual->siguiente;
    }

    return 0;
}

void imprimirListaNodosCotizacionDetalle(NodoCotizacionDetalle* head) {
    NodoCotizacionDetalle *actual = head;

    while (actual != NULL)
    {
        printf("Datos del elementos de la lista:\n");
        printf("Id: %s, \nDescripcion: %s \n\n", actual->detallesCotizacion.IdProducto, actual->detallesCotizacion.Descripcion);
        actual = actual->siguiente;
    }
    return;
}

void modificarValoresNodoCotizacionDetalle(NodoCotizacionDetalle** head, const char * id, const int cantidad) {

    if (head == NULL) {
        return;
    }
    NodoCotizacionDetalle* actual = *head;
    while (actual != NULL)
    {

        // Buscamos el id que coincida y modificamos la cantidad.
        if (strcmp(actual->detallesCotizacion.IdProducto, id) == 0 ) {

            actual->detallesCotizacion.cantidad += cantidad;
            return;
        }
        actual = actual->siguiente;
    }

    return;

}


void liberarListaCotizacionDetalle(NodoCotizacionDetalle* head) {
    NodoCotizacionDetalle *actual;
    while (head != NULL)
    {
        actual = head;
        head = head->siguiente;

        free(actual->detallesCotizacion.IdProducto);
        free(actual->detallesCotizacion.NombreProducto);
        free(actual->detallesCotizacion.Descripcion);
        free(actual);
    }
    return;
}




void mostrar_cotizacion(NodoCotizacionDetalle* head) {

    if (head == NULL) {
        printf("Actualemente esta cotizacion, no tiene productos para mostrar.\n");
        return;
    }


    NodoCotizacionDetalle *actual = head;

    printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
    printf("| Fila       | ID Producto  | Nombre               | Descripcion       | Precio      | Cantidad   |\n");
    printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");

    while (actual != NULL)
    {
        printf("| %-10i | %-12s | %-20s | %-17s | %-11.2f | %-10i |\n", actual->detallesCotizacion.NumeroFila, actual->detallesCotizacion.IdProducto, actual->detallesCotizacion.NombreProducto, actual->detallesCotizacion.Descripcion, actual->detallesCotizacion.precio, actual->detallesCotizacion.cantidad);
        
        actual = actual->siguiente;
    }

    printf("+------------+--------------+----------------------+-------------------+-------------+-------------+\n\n");

    return;
}


void agregar_nuevo_producto(MYSQL *conexion, NodoCotizacionDetalle** head, const char * idProducto, const int cantidad) {

    MYSQL_RES *resultado;
    MYSQL_ROW fila;
    char consulta[256]; // Tamaño que se podria ocupar por la cuestion del id

    // Crear la consulta al procedimiento almacenado
    snprintf(consulta, sizeof(consulta),"CALL obtenerProductoPorID('%s')", idProducto);

    // Ejecutar la consulta
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    // Obtener los resultados
    // resultado = mysql_store_result(conexion);
    // if (resultado == NULL) {
    //     printf("Error al obtener los resultados: %s\n", mysql_error(conexion));
    //     return;
    // }

    // Procesar los resultados
    // fila = mysql_fetch_row(resultado);

    // if (fila) {
    //     const char *nombreProducto = fila[1];
    //     const char *descripcion = fila[4];
    //     int precio = atoi(fila[2]); 


    //     // Revisar si no hay otro elementos con este id

    //     // if ( buscarPorIdCotizacionDetalle(coti) )


    //     // Validar que la cantidad del producto no sea menor a la solicitada... Pendiente

        
    //     // Agregar el producto a la cotización
    //     insertarElementoAlFinalCotizacionDetalle(head, idProducto, nombreProducto, descripcion, precio, cantidad);

    //     printf("Producto agregado a la cotización: %s\n", idProducto);
    // } else {
    //     printf("El producto con ID '%s' no fue encontrado.\n", idProducto);
    // }

    do {
        resultado = mysql_store_result(conexion);
        if (resultado) {
            while ((fila = mysql_fetch_row(resultado)) != NULL) {



                const char *nombreProducto = fila[1];
                const char *descripcion = fila[4];
                int precio = atoi(fila[2]);
                printf("Pass1");
                // Revisar si no hay otro elemento con este ID
                int cantProducto = buscarPorIdCotizacionDetalle(*head, idProducto);
                printf("Pass2");

                if (cantProducto != 0) {
                    printf("Pass4");

                    // Este es para combinar las cantodades de producto exitentes...
                    modificarValoresNodoCotizacionDetalle(head, idProducto, cantidad);

                } else {
                    printf("Pass5");

                    // Agregar el producto a la cotización
                    insertarElementoAlFinalCotizacionDetalle(head, idProducto, nombreProducto, descripcion, precio, cantidad);

                    printf("Producto agregado a la cotización: %s\n", idProducto);
                }

            }
            mysql_free_result(resultado); // Liberar este conjunto de resultados
        }
    } while (mysql_next_result(conexion) == 0); // Mover al siguiente conjunto de resultados


    // Liberar memoria
    //mysql_free_result(resultado);

    return;
}


void buscar_cotizacion(MYSQL *conexion, NodoCotizacionDetalle** head, const char * idCotizacion) {

    return;
}



