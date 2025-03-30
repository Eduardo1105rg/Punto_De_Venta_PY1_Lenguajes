#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>

#include "../include/estadisticas.h"

/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Funcionamiento:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void PrimerEstadistica(MYSQL *conexion) {

    char *consulta = NULL;
    int largoConsultaF = asprintf(&consulta, "select * from CantidadCotizacionesPendientes");
    if(mysql_query(conexion,consulta)){
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    free(consulta);
    MYSQL_RES *resultado = mysql_store_result(conexion);
    MYSQL_ROW fila;

    printf("Esta es la cantidad de cotizaciones pendientes %s\n", fila[0]);
    mysql_free_result(resultado);

}


/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Funcionamiento:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void SegundaEstadistica(MYSQL *conexion) {

    char *consulta = NULL;
    int largoConsultaF = asprintf(&consulta, "select * from CantidadCotizacionesFacturadas");
    if(mysql_query(conexion,consulta)){
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    free(consulta);
    MYSQL_RES *resultado = mysql_store_result(conexion);
    MYSQL_ROW fila;

    printf("Esta es la cantidad de cotizaciones facturadas %s\n", fila[0]);
    mysql_free_result(resultado);

}


/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Funcionamiento:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void TerceraEstadistica(MYSQL *conexion) {

    char *consulta = NULL;
    int largoConsultaF = asprintf(&consulta, "select * from PromedioTotal");
    if(mysql_query(conexion,consulta)){
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    free(consulta);
    MYSQL_RES *resultado = mysql_store_result(conexion);
    MYSQL_ROW fila;

    printf("Este es el promedio de compra %s\n", fila[0]);
    mysql_free_result(resultado);

}


/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Funcionamiento:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void CuartaEstadistica(MYSQL *conexion) {

    char *consulta = NULL;
    int largoConsultaF = asprintf(&consulta, "select * from Top5ProductosVendidos");
    if(mysql_query(conexion,consulta)){
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    free(consulta);
    MYSQL_RES *resultado = mysql_store_result(conexion);
    MYSQL_ROW fila;
    for (int i = 0; i < mysql_num_rows(resultado); i++) {
        fila = mysql_fetch_row(resultado);
        if (fila) {
            printf("Este es el producto #%i, su nombre es %s y su cantidad es %s\n", i + 1, fila[0], fila[1]);
        }
    }
    mysql_free_result(resultado);

}


/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Funcionamiento:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void QuintaEstadistica(MYSQL *conexion) {

    char *consulta = NULL;
    int largoConsultaF = asprintf(&consulta, "select * from TopProductoFamilia");
    if(mysql_query(conexion,consulta)){
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    free(consulta);
    MYSQL_RES *resultado = mysql_store_result(conexion);
    MYSQL_ROW fila;
    for (int i = 0; i < mysql_num_rows(resultado); i++) {
        fila = mysql_fetch_row(resultado);
        if (fila) {
            printf("Este es el nombre de la familia %s y su producto mas vendido es %s\n", fila[0], fila[1]);
        }
    }
    mysql_free_result(resultado);

}


/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Funcionamiento:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void SextaEstadistica(MYSQL *conexion) {

    char *consulta = NULL;
    int largoConsultaF = asprintf(&consulta, "select * from MontoTopFamilia");
    if(mysql_query(conexion,consulta)){
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    free(consulta);
    MYSQL_RES *resultado = mysql_store_result(conexion);
    MYSQL_ROW fila;
    for (int i = 0; i < mysql_num_rows(resultado); i++) {
        fila = mysql_fetch_row(resultado);
        if (fila) {
            printf("Este es el nombre de la familia %s y su monto es de %s\n", fila[0], fila[1]);
        }
    }
    mysql_free_result(resultado);

}