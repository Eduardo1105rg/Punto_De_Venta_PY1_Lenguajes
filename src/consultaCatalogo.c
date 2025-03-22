#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

void consultarCatalogo(MYSQL *conexion) {
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
    // printf("ID Producto | Nombre | Descripcion | Precio sin IVA | Cantidad\n");

    // // recorrer todas las filas para imprimir.
    // while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
    //     printf("%s | %s | %s | %.2f | %s\n", fila[0], fila[1], fila[4] , atof(fila[2]), fila[3]);
    // }

    printf("+--------------+----------------------+-------------------+----------+----------------------+\n");
    printf("| ID Producto  | Nombre               | Precio sin IVA    | Cantidad | Familia              |\n");
    printf("+--------------+----------------------+-------------------+----------+----------------------+\n");

    // Recorrer los resultados e imprimir cada fila
    while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
        //printf("Datos: %s\n", fila[0]);
        printf("| %-12s | %-20s | %-17.2f | %-8s | %-20s |\n", fila[0], fila[1], atof(fila[2]), fila[3], fila[4]);
        //printf("| %-12s | %-20s | %-17.2f | %-8s | %-20s |\n", fila[0], fila[1], atof(fila[2]), fila[3], fila[4]);
     
        // printf("| %-15s | %-20s | %-17.2f | %-8s | %-20s |\n",
        //     (fila[0] != NULL) ? fila[0] : "Sin ID",
        //     (fila[1] != NULL) ? fila[1] : "Sin Nombre",
        //     (fila[2] != NULL) ? atof(fila[2]) : 0.0,
        //     (fila[3] != NULL) ? fila[3] : "0",
        //     (fila[4] != NULL) ? fila[4] : "Sin Familia");
        // printf("| %-12s | %-20s | %-17.2f | %-8s |\n", fila[0], fila[1], atof(fila[2]), fila[3]);
        // printf("Datos: %s\n", fila[4]);

        // printf("| %s\t|", fila[0]);
        // printf(" %s\t|", fila[1]);
        // printf(" %2f\t|", atof(fila[2]));
        // printf(" %s\t|", fila[3]);
        // printf(" %s\t|\n", fila[4]);

    }

    // Imprimir línea final de tabla
    printf("+--------------+----------------------+-------------------+----------+----------------------+\n\n");


    mysql_free_result(datos_recibidos);
    return;
}

void consultarCatalogoPorFamilia(MYSQL *conexion, const char* descripcion) {

    MYSQL_RES *datos_recibidos;
    MYSQL_ROW fila;
    char query[128];

    snprintf(query, sizeof(query), "CALL verCatalogoFiltro('%s')", descripcion);
    
    if (mysql_query(conexion, query)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    datos_recibidos = mysql_store_result(conexion);
    if (datos_recibidos == NULL) {
        printf("Error al obtener los resultados: %s\n", mysql_error(conexion));
        return;
    }

    printf("Catálogo de Productos de la Familia '%s':\n", descripcion);
    printf("+--------------+----------------------+-------------------+----------+----------------------+\n");
    printf("| ID Producto  | Nombre               | Precio sin IVA    | Cantidad | Familia              |\n");
    printf("+--------------+----------------------+-------------------+----------+----------------------+\n");

    // Recorrer los resultados e imprimir cada fila
    while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
        //printf("Datos: %s\n", fila[0]);
        printf("| %-12s | %-20s | %-17.2f | %-8s | %-20s |\n", fila[0], fila[1], atof(fila[2]), fila[3], fila[4]);

    }

    // Imprimir línea final de tabla
    printf("+--------------+----------------------+-------------------+----------+----------------------+\n\n");

    mysql_free_result(datos_recibidos); // Por alguna razon dicen que hay que liberarlo.
}

