#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/cotizacion.h"

/**
 * Nombre: mostrar_facturas
 * 
 * Descripcion: Esta funcion se encarga de mostrar los datos de todas las facturas registradas en la base de datos, muestra (el id de la factura, 
 * La fecha de la factura, el subtotal y el total), para esto se llama a una vista que ya contiene esos datos.
 * 
 * Entradas: MYSQL *conexion: un puntero a un tipo de datos mysql.
 * 
 * Salidas: No posee.
 * 
 */
void mostrar_facturas(MYSQL *conexion) {

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM verFacturas";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    // Guarda los datos recibidos
    datos_recibidos = mysql_store_result(conexion);
    if (datos_recibidos == NULL) {
        printf("Error al obtener los datos de las facturas: %s\n", mysql_error(conexion));
        return;
    }

    // Formato de impresion...
    printf("Facturas registradas en el sistema:\n");
    
        // esta seria para lo que es la cotizacion

    printf("+--------------+----------------------+-------------------+-------------+\n");
    printf("| ID Factura   | Fecha de la factura  | Subtotal          | Total       |\n");
    printf("+--------------+----------------------+-------------------+-------------+\n");

    // Recorrer los resultados e imprimir cada fila
    while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
        //printf("Datos: %s\n", fila[0]);
        printf("| %-12s | %-20s | %-17.2f | %-11.2f |\n", fila[0], fila[1], atof(fila[2]), atof(fila[3]));

    }

    printf("+--------------+----------------------+-------------------+-------------+\n\n");

    mysql_free_result(datos_recibidos);

    return;
}


/**
 * Nombre: mostrar_detalles_factura
 * 
 * Descripcion: Funcion para mostrar los detalles especificos de una factura, para esto se recibe el id de la factura y se extraen los datos del encabezado y del detalle,
 * de dicha factura registrados en la base de datos.
 * 
 * Entradas: MYSQL *conexion: un puntero a un tipo de datos mysql.
 * const int id_factura: El id de la factura con la que se consultaran los datos a usar.
 * 
 * Salidas: No posee.
 * 
 */
void mostrar_detalles_factura(MYSQL *conexion, const int id_factura) {
    // Para esta se usaria, el procedure de ver datos factura (Se excluyen los datos de los productos.)y el procedure de mostrarDetalleCotizacion.

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;
    char consulta[128]; // Esto de aqui es por que no se dabe el tamaño del id del proucto y como se va a ocupar espacio, se tiene que reservar.

    snprintf(consulta, sizeof(consulta), "CALL verDatosFacturaEspecifica('%i')", id_factura);
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    int countFactura = 0;
    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {

            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    //printf("+--------------+----------------------+-------------------+\n\n");
                    printf("\n+--------------+ Detalles de la factura: %i +--------------+\n\n", id_factura);

                    printf(">> ID Factura: %s\n", fila[0]);
                    printf(">> Fecha de la factura: %s\n", fila[2]);
                    printf(">> Subtotal: %.2f\n", atof(fila[3]));
                    printf(">> Impuestos: %.2f\n", atof(fila[4]));
                    printf(">> Total: %.2f\n", atof(fila[4]));

                    printf("+--------------+----------------------+-------------------+\n\n");
                    countFactura = atoi(fila[1]);

                }

            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    if (countFactura != 0) {

        printf("Datos de los prodcutos de esta factura:\n");

        mostrar_detalle_cotizacion_facturada(conexion, countFactura);
    } else {
        printf("La factura con el id ingresado no fue encontrado...\n");
    }
    //printf("\nDato id:%s\n", fila[1]);

    return;
}

