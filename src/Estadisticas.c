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
    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM CantidadCotizacionesPendientes";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Esta es la cantidad de cotizaciones pendientes: %s\n", fila[0]);
                }

            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    //free(consulta);

    //    int largoConsultaF = asprintf(&consulta, "select * from CantidadCotizacionesPendientes");

    //printf("Esta es la cantidad de cotizaciones pendientes %s\n", fila[0]);
    return;

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

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM CantidadCotizacionesFacturadas";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Esta es la cantidad de cotizaciones facturadas: %s\n", fila[0]);
                }

            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    return;

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

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM PromedioTotal";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Este es el promedio de compra: %s\n", fila[0]);
                }

            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen


    return;

    
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

    // char *consulta = NULL;
    // int largoConsultaF = asprintf(&consulta, "select * from Top5ProductosVendidos");
    // if(mysql_query(conexion,consulta)){
    //     printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
    //     free(consulta);
    //     return;
    // }
    // free(consulta);
    // MYSQL_RES *resultado = mysql_store_result(conexion);
    // MYSQL_ROW fila;
    // for (int i = 0; i < mysql_num_rows(resultado); i++) {
    //     fila = mysql_fetch_row(resultado);
    //     if (fila) {
    //         printf("Este es el producto #%i, su nombre es %s y su cantidad es %s\n", i + 1, fila[0], fila[1]);
    //     }
    // }
    // mysql_free_result(resultado);

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM Top5ProductosVendidos";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    int count = 1;
    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Este es el producto #%i, su nombre es %s y su cantidad es %s\n", count, fila[0], fila[1]);
                    count++;
                }

            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    return;


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

    // char *consulta = NULL;
    // int largoConsultaF = asprintf(&consulta, "select * from TopProductoFamilia");
    // if(mysql_query(conexion,consulta)){
    //     printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
    //     free(consulta);
    //     return;
    // }
    // free(consulta);
    // MYSQL_RES *resultado = mysql_store_result(conexion);
    // MYSQL_ROW fila;
    // for (int i = 0; i < mysql_num_rows(resultado); i++) {
    //     fila = mysql_fetch_row(resultado);
    //     if (fila) {
    //         printf("Este es el nombre de la familia %s y su producto mas vendido es %s\n", fila[0], fila[1]);
    //     }
    // }
    // mysql_free_result(resultado);

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM TopProductoFamilia";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    int count = 1;
    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Este es el nombre de la familia %s y su producto mas vendido es %s\n", fila[0], fila[1]);
                    count++;
                }

            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    return;

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

    // char *consulta = NULL;
    // int largoConsultaF = asprintf(&consulta, "select * from MontoTopFamilia");
    // if(mysql_query(conexion,consulta)){
    //     printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
    //     free(consulta);
    //     return;
    // }
    // free(consulta);
    // MYSQL_RES *resultado = mysql_store_result(conexion);
    // MYSQL_ROW fila;
    // for (int i = 0; i < mysql_num_rows(resultado); i++) {
    //     fila = mysql_fetch_row(resultado);
    //     if (fila) {
    //         printf("Este es el nombre de la familia %s y su monto es de %s\n", fila[0], fila[1]);
    //     }
    // }
    // mysql_free_result(resultado);


    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    // Se crea la consulta...
    const char* consulta = "SELECT * FROM MontoTopFamilia";
    
    //Validamos si se puede hacer .
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Este es el nombre de la familia %s y su monto es de %s\n", fila[0], fila[1]);
                }

            mysql_free_result(datos_recibidos); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    return;

}