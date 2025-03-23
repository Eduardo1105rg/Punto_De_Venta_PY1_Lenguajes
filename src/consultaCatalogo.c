#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

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
    // printf("ID Producto | Nombre | Descripcion | Precio sin IVA | Cantidad\n");

    // // recorrer todas las filas para imprimir.
    // while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
    //     printf("%s | %s | %s | %.2f | %s\n", fila[0], fila[1], fila[4] , atof(fila[2]), fila[3]);
    // }

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
 
    // printf("+--------------+----------------------+-------------------+----------+----------------------+\n");
    // printf("| ID Producto  | Nombre               | Precio sin IVA    | Cantidad | Familia              |\n");
    // printf("+--------------+----------------------+-------------------+----------+----------------------+\n");

    // // Recorrer los resultados e imprimir cada fila
    // while ((fila = mysql_fetch_row(datos_recibidos)) != NULL) {
    //     //printf("Datos: %s\n", fila[0]);
    //     printf("| %-12s | %-20s | %-17.2f | %-8s | %-20s |\n", fila[0], fila[1], atof(fila[2]), fila[3], fila[4]);
    //     //printf("| %-12s | %-20s | %-17.2f | %-8s | %-20s |\n", fila[0], fila[1], atof(fila[2]), fila[3], fila[4]);
     
    //     // printf("| %-15s | %-20s | %-17.2f | %-8s | %-20s |\n",
    //     //     (fila[0] != NULL) ? fila[0] : "Sin ID",
    //     //     (fila[1] != NULL) ? fila[1] : "Sin Nombre",
    //     //     (fila[2] != NULL) ? atof(fila[2]) : 0.0,
    //     //     (fila[3] != NULL) ? fila[3] : "0",
    //     //     (fila[4] != NULL) ? fila[4] : "Sin Familia");
    //     // printf("| %-12s | %-20s | %-17.2f | %-8s |\n", fila[0], fila[1], atof(fila[2]), fila[3]);
    //     // printf("Datos: %s\n", fila[4]);

    //     // printf("| %s\t|", fila[0]);
    //     // printf(" %s\t|", fila[1]);
    //     // printf(" %2f\t|", atof(fila[2]));
    //     // printf(" %s\t|", fila[3]);
    //     // printf(" %s\t|\n", fila[4]);

    // }

    // // Imprimir línea final de tabla
    // printf("+--------------+----------------------+-------------------+----------+----------------------+\n\n");


    mysql_free_result(datos_recibidos);
    return;
}

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

