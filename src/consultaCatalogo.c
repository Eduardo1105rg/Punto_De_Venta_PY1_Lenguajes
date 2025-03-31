#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>


/**
 * Nombre: consultarCatalogo
 * 
 * Descripcion: Funcion para mostrar los datos de los prodcutos del catalogo de prodcutos, esta funcion cuenta con dos modos, el modo 0, es para mostrar los datos totales de los producto, 
 * el modo 1 es para mostrar los datos de los porductos omitiendo algunas partes, ambas funciones optienen sus datos de la vista verCatalogo.
 * 
 * Entradas: MYSQL *conexion: Un puntero a una instacia de MySQL.
 *  int modo: El modo con el que se usara la funcion.
 * 
 * Salidas:
 * 
 */ 
void consultarCatalogo(MYSQL *conexion, int modo) {
    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM verCatalogo";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    // Guarda los datos recibidos
    datos_recibidos = mysql_store_result(conexion);
    if (datos_recibidos == NULL) {
        printf("Error al obtener los resultados de los productos: %s\n", mysql_error(conexion));
        return;
    }

    // Formato de impresion...
    printf("Catálogo de Productos:\n");


    if (modo == 0) {
        // este seria para una consulta normal del catalogo.

        printf("+--------------+----------------------+-------------------+----------+----------------------+\n");
        printf("| ID Producto  | Nombre               | Precio sin IVA    | Cantidad | Familia              |\n");
        printf("+--------------+----------------------+-------------------+----------+----------------------+\n");
    
        // Recorrer los resultados e imprimir cada fila
        while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {

            printf("| %-12s | %-20s | %-17.2f | %-8s | %-20s |\n", fila[0], fila[1], atof(fila[2]), fila[3], fila[4]);

        }
    
        // Imprimir línea final de tabla
        printf("+--------------+----------------------+-------------------+-------------+\n\n");

    } else { 
        // esta seria para lo que es la cotizacion

        printf("+--------------+----------------------+-------------------+-------------+\n");
        printf("| ID Producto  | Nombre               | Descripcion       | Precio      |\n");
        printf("+--------------+----------------------+-------------------+-------------+\n");
    
        // Recorrer los resultados e imprimir cada fila
        while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
            //printf("Datos: %s\n", fila[0]);
            printf("| %-12s | %-20s | %-19s | %-15.2f |\n", fila[0], fila[1],fila[4], atof(fila[2]));

    
        }
    
        // Imprimir línea final de tabla
        printf("+--------------+----------------------+-------------------+---------------+\n\n");

    }
 
    mysql_free_result(datos_recibidos);
    return;
}


/**
 * Nombre: consultarCatalogoPorFamilia
 * 
 * Descripcion: Funcion para ver los datos de los procutos filtrando por una familia especifica. Esta funcion cuenta con dos modos, el modo 0, es para mostrar los datos totales de los producto, 
 * el modo 1 es para mostrar los datos de los porductos omitiendo algunas partes.
 * 
 * Entradas: MYSQL *conexion: Un puntero a una instacia de MySQL.
 *  int modo: El modo con el que se usara la funcion.
 * const char* descripcion: La descripcion de la familia que se busacara.
 * 
 * Salidas: No posee.
 * 
 */
void consultarCatalogoPorFamilia(MYSQL *conexion, const char* descripcion, int modo) {

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;
    char consulta[128]; // Esto de aqui es por que no se dabe el tamaño del id del proucto y como se va a ocupar espacio, se tiene que reservar.

    snprintf(consulta, sizeof(consulta), "CALL verCatalogoFiltro('%s')", descripcion);
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    // datos_recibidos = mysql_store_result(conexion);
    // if (datos_recibidos == NULL) {
    //     printf("Error al obtener los resultados: %s\n", mysql_error(conexion));
    //     return;
    // }

    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            if (modo == 0) {
                // Consulta normal del catálogo
                printf("+--------------+----------------------+-------------------+----------+----------------------+\n");
                printf("| ID Producto  | Nombre               | Precio sin IVA    | Cantidad | Familia              |\n");
                printf("+--------------+----------------------+-------------------+----------+----------------------+\n");

                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("| %-12s | %-20s | %-17.2f | %-8s | %-20s |\n",
                           fila[0], fila[1], atof(fila[2]), fila[3], fila[4]);
                }

                // Línea final de tabla
                printf("+--------------+----------------------+-------------------+-------------+\n\n");
            } else {
                // Consulta para cotización
                printf("+--------------+----------------------+-------------------+-------------+\n");
                printf("| ID Producto  | Nombre               | Descripcion       | Precio      |\n");
                printf("+--------------+----------------------+-------------------+-------------+\n");

                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("| %-12s | %-20s | %-19s | %-15.2f |\n",
                           fila[0], fila[1], fila[4], atof(fila[2]));
                }

                // Línea final de tabla
                printf("+--------------+----------------------+-------------------+---------------+\n\n");
            }
            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    return;
}

