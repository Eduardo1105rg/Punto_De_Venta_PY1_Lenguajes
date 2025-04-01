#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>

#include "../include/estadisticas.h"

/**
 * Nombre:PrimerEstadistica
 * 
 * Descripcion: Lo que hace es por medio de una consulta a la base de datos se le retorna la cantida de cotizaciones pendientes
 el cual lo muestra con un mensaje
 * 
 * Entradas:Un puntero a un tipo de dato MYSQL
 * 
 * Salidas:No tiene
 * 
 */
void PrimerEstadistica(MYSQL *conexion) {
    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    const char* consulta = "SELECT * FROM CantidadCotizacionesPendientes";
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Esta es la cantidad de cotizaciones pendientes: %s\n", fila[0]);
                }

            mysql_free_result(datos_recibidos); 
        }
    } while (mysql_next_result(conexion) == 0); 


    return;

}


/**
 * Nombre:SegundaEstadistica
 * 
 * Descripcion:Lo que hace es por medio de una consulta a la base de datos se le retorna la cantida de cotizaciones facturadas
 el cual lo muestra con un mensaje
 * 
 * Entradas: Un puntero a un tipo de dato MYSQL
 * 
 * Salidas: No tiene
 * 
 */
void SegundaEstadistica(MYSQL *conexion) {

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    const char* consulta = "SELECT * FROM CantidadCotizacionesFacturadas";
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    do {
        datos_recibidos = mysql_store_result(conexion);
        if (datos_recibidos) {
            
                while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
                    printf("Esta es la cantidad de cotizaciones facturadas: %s\n", fila[0]);
                }

            mysql_free_result(datos_recibidos); 
        }
    } while (mysql_next_result(conexion) == 0); 

    return;

}


/**
 * Nombre:TerceraEstadistica
 * 
 * Descripcion: Por medio de una consulta a la base de datos recibimos el promedio total de todas las facturas generadas
 * Esta se mostrara por medio de un print
 * Entradas: Un puntero a un tipo de dato MYSQL
 * 
 * Salidas:No tienes
 * 
 */
 void TerceraEstadistica(MYSQL *conexion) {
    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    const char* consulta = "SELECT * FROM PromedioTotal";
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    datos_recibidos = mysql_store_result(conexion);
    
    if (!datos_recibidos) {
        printf("Error al obtener los datos: %s\n", mysql_error(conexion));
        return;
    }

    if (mysql_num_rows(datos_recibidos) == 0) {
        printf("Actualmente no han habido compras realizadas\n");
    } else {
        while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
            printf("Este es el promedio de compra: %s\n", fila[0]);
        }
    }

    mysql_free_result(datos_recibidos);
    return;
}



/**
 * Nombre:CuartaEstadistica
 * 
 * Descripcion: Generamos una consulta con la cual sacamos el top5 de los productos más vendidos estos se mostraran
 * por medio de un print
 * Entradas: un puntero a un tipo de dato MYSQL
 * 
 * Salidas:No tiene
 * 
 */
 void CuartaEstadistica(MYSQL *conexion) {
    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    const char* consulta = "SELECT * FROM Top5ProductosVendidos";
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    datos_recibidos = mysql_store_result(conexion);
    
    if (!datos_recibidos) {
        printf("Actualmente no tenemos datos para esta estadística\n");
        return;
    }

    int count = 1;
    if (mysql_num_rows(datos_recibidos) == 0) {
        printf("Actualmente no tenemos datos para esta estadística\n");
    } else {
        while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
            printf("Este es el producto #%i, su nombre es %s y su cantidad es %s\n", count, fila[0], fila[1]);
            count++;
        }
    }

    mysql_free_result(datos_recibidos);
    return;
}



/**
 * Nombre:QuintaEstadistica
 * 
 * Descripcion: Como tal lo que hace es una consulta con la cual mostraremos el producto top por familia
 * esto lo haremos por medio de un while el cual realice el proceso de los resultados
 * Entradas: Un puntero a un tipo de dato MYSQL
 * 
 * Salidas: No tiene
 * 
 */
 void QuintaEstadistica(MYSQL *conexion) {
    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    const char* consulta = "SELECT * FROM TopProductoFamilia";
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    datos_recibidos = mysql_store_result(conexion);

    if (!datos_recibidos) {
        printf("Error al obtener los datos: %s\n", mysql_error(conexion));
        return;
    }

    if (mysql_num_rows(datos_recibidos) == 0) {
        printf("Actualmente no tenemos datos para esta estadística\n");
    } else {
        while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
            printf("Este es el nombre de la familia %s y su producto más vendido es %s\n", fila[0], fila[1]);
        }
    }

    mysql_free_result(datos_recibidos);
    return;
}



/**
 * Nombre:SextaEstadistica
 * 
 * Descripcion: Esta lo que hace es por medio de una consulta mostrar los montos mas altos generados por familia
 * todos estos se hacen igual por medio de un while en caso de que hayan muchos resultados
 * Entradas:un puntero a un tipo de datos MYSQL
 * 
 * Salidas:No tiene
 * 
 */
 void SextaEstadistica(MYSQL *conexion) {
    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;

    const char* consulta = "SELECT * FROM MontoTopFamilia";
    
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    datos_recibidos = mysql_store_result(conexion);

    if (!datos_recibidos) {
        printf("Error al obtener los datos: %s\n", mysql_error(conexion));
        return;
    }

    if (mysql_num_rows(datos_recibidos) == 0) {
        printf("Actualmente no tenemos datos para esta estadística\n");
    } else {
        while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
            printf("Este es el nombre de la familia %s y su monto es de %s\n", fila[0], fila[1]);
        }
    }

    mysql_free_result(datos_recibidos);
    return;
}

