
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>

#include "../include/facturacion.h"

/**
 * Nombre: crearFactura
 * 
 * Descripcion: Funciona para registrar una factura en la base de datos, se toman los datos del encabezado de la factura y se registran esos datos, ademas se vincula 
 * con la tabla de la cotizacion con la cual se esta creando esta factura.
 * 
 * Entradas: MYSQL *conexion: Un puntero a una instancia de MySQL.
 * int numCotizacion: El numero de la cotizacion con las cual se va a crear esta factura?
 * char *nombreCliente, char *fechaHora: La hora y fecha actual en la que se esta creando la factura.
 * 
 * Salidas: int idFactura: Un valor entero que sera -1 en caso de que ocurra un error, 
 * en caso de que se registre bien los datos de la factura, devolvera el id de la cotizacion.
 * 
 */
int crearFactura(MYSQL *conexion, int numCotizacion, char *nombreCliente, char *fechaHora) {
    char *consulta = NULL;
    int largoConsultaF2 = asprintf(&consulta, "call facturaFinDinero('%d', '%s')", numCotizacion, nombreCliente);
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return -1; 
    }
    free(consulta);

    MYSQL_RES *resultado2 = mysql_store_result(conexion);
    if (resultado2 == NULL) {
        printf("Error al obtener el resultado: %s\n", mysql_error(conexion));
        return -1; 
    }
    MYSQL_ROW fila = mysql_fetch_row(resultado2);


    while (mysql_next_result(conexion) == 0) {
        MYSQL_RES *res = mysql_store_result(conexion);
        if (res) mysql_free_result(res);
    }
    


    char *consulta2 = NULL;
    int largoConsultaF3 = asprintf(&consulta2, "insert into Factura(IdCotizacion, fechaHora, SubTotal, Impuesto, Total) values(%d, '%s', %f, %f, %f)", numCotizacion, fechaHora, atof(fila[0]), atof(fila[1]), atof(fila[2]));
    if (mysql_query(conexion, consulta2)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta2);
        mysql_free_result(resultado2);
        return -1;
    }
    free(consulta2);
    mysql_free_result(resultado2);

    char *consulta3 = NULL;
    int largoConsultaF4 = asprintf(&consulta3, "SELECT IdFactura FROM Factura WHERE IdCotizacion = '%d'", numCotizacion);
    if (mysql_query(conexion, consulta3)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta3);
        return -1; 
    }
    MYSQL_RES *resultado3 = mysql_store_result(conexion);
    if (resultado3 == NULL) {
        printf("Error al obtener el resultado: %s\n", mysql_error(conexion));
        free(consulta3);
        return -1; 
    }
    MYSQL_ROW fila2 = mysql_fetch_row(resultado3);
    int idFactura = -1;
    if (fila2) {
        idFactura = atoi(fila2[0]); 
    }
    mysql_free_result(resultado3);
    free(consulta3);

    return idFactura;
}

