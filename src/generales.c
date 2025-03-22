#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

// Include de archivos del programa.
#include "../include/generales.h"
#include "../include/consultaCatalogo.h"
#include "../include/manipularArchivos.h"
#include "../include/db.h"
#include "../include/cotizacion.h"




/**
 * 
 */
void menu_consulta_catalogo() {

    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    char opcion;
    do {
        printf("\nConsultal del catalogo... \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Consulta normal \n");
        printf(">> B) Consulta por familia.\n");
        printf(">> S) Volver al menu anterior.\n");


        printf("Ingrese la letra de la consulta a realizar.: ");
        scanf(" %c", &opcion); 
        getchar(); 

        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
            
                consultarCatalogo(conexion, 1);
                break;
            case 'A':
                consultarCatalogo(conexion, 1);
                break;

            // ========== Consulta de catalogo por la descripcion de la familia.
            case 'b':
                 
                // Solicitar 
                char *descripcion_familia1;
                printf("\nIngresa la descripcion de la familia a buscar: ");
                leerCaracteresDeFormadinamica(&descripcion_familia1);
                printf("\n");
                consultarCatalogoPorFamilia(conexion, descripcion_familia1, 1);
                free(descripcion_familia1);
                break;

            case 'B':

                char *descripcion_familia2;
                printf("\nIngresa la descripcion de la familia a buscar: ");
                leerCaracteresDeFormadinamica(&descripcion_familia2);
                printf("\n");
                consultarCatalogoPorFamilia(conexion, descripcion_familia2, 1);
                free(descripcion_familia2);

                break;

      

            // ========== Salir del menu.
            case 's':
                printf("Volviendo a la seccion anterior...\n");
                break;
            case 'S':
                printf("Volviendo a la seccion anterior...\n");
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's');

    cerrarConexion(conexion);
    
    return;
}


void menu_cotizacion() {

    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    NodoCotizacionDetalle *lista_productos_en_cotizacion = NULL;

    char opcion;
    do {
        printf("\nCotizacion... \n");

        mostrar_cotizacion(lista_productos_en_cotizacion);


        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Ver catalogo de productos. \n");
        printf(">> B) Agregar productos.\n");
        printf(">> C) Eliminar producto.\n");
        printf(">> D) Registrar cotizacion.\n");
        printf(">> S) Volver al menu anterior.\n");


        printf("Ingrese la letra de la accion a realizar: ");
        scanf(" %c", &opcion); 
        getchar(); 

        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
            
                menu_consulta_catalogo();
                break;
            case 'A':
                menu_consulta_catalogo();
                break;

            // ========== Agregar un nuevo producto a la cotizacion.
            case 'b':
                 
                // Solicitar 
                char *id_producto1;
                printf("\nIngresa el id del producto a agregar: ");
                leerCaracteresDeFormadinamica(&id_producto1);
                printf("\n");


                printf("\nIngresa la cantidad del producto que desea agregar: ");
                int cantidad_producto1 = leerNumeroDinamico();
                printf("\n");

                agregar_nuevo_producto(conexion, &lista_productos_en_cotizacion, id_producto1, cantidad_producto1);

                free(id_producto1);
                break;

            case 'B':

                // Solicitar el id del producto.
                char *id_producto2;
                printf("\nIngresa el id del producto a agregar: ");
                leerCaracteresDeFormadinamica(&id_producto2);
                printf("\n");

                //Solicitar la cantidad del producto.
                printf("\nIngresa la cantidad del producto que desea agregar: ");
                int cantidad_producto2 = leerNumeroDinamico();
                printf("\n");

                agregar_nuevo_producto(conexion, &lista_productos_en_cotizacion, id_producto2, cantidad_producto2);

                free(id_producto1);

                break;

            // ========== Eliminar un producto de la cotizacion.
            case 'c':

                char *id_producto_a_eliminar1;
                printf("\nIngresa el id del producto que desea eliminar: ");
                leerCaracteresDeFormadinamica(&id_producto_a_eliminar1);
                printf("\n");


                eliminarPorIdCotizacionDetalle(&lista_productos_en_cotizacion, id_producto_a_eliminar1);

                free(id_producto_a_eliminar1);

                break;
            case 'C':
                char *id_producto_a_eliminar2;
                printf("\nIngresa el id del producto que desea eliminar: ");
                leerCaracteresDeFormadinamica(&id_producto_a_eliminar2);
                printf("\n");

                eliminarPorIdCotizacionDetalle(&lista_productos_en_cotizacion, id_producto_a_eliminar2);

                free(id_producto_a_eliminar2);

                break;

            // ========== Guardar la cotizacion realizada.
            case 'd':
                break;
            case 'D':
                break;
            // ========== Salir del menu.
            case 's': // En esta parte hay que agregar algo para que  no se salga de una vez. hay que avisarle al usuario que si sale con S no se guarda la cotizacion.
                printf("Volviendo a la seccion anterior...\n");
                break;
            case 'S':
                printf("Volviendo a la seccion anterior...\n");
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's');

    cerrarConexion(conexion);
    
    liberarListaCotizacionDetalle(lista_productos_en_cotizacion);

    return;
}



/* ============================ Menu principal de la seccion ==========================*/
void menu_principal_generales() {

    char opcion;
    do {
        printf("\nBienvenido al apartado de Opciones generales. \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Consulta de catalogo. \n");
        printf(">> B) Cotizar.\n");
        printf(">> C) Modificar cotización.\n");
        printf(">> D) Facturar.\n");
        printf(">> S) Volver al menu principal.\n");

        printf("Ingrese la letra de las seccion a la que desea ingresar: ");
        scanf(" %c", &opcion); 
        getchar(); 

        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
                menu_consulta_catalogo();
                break;
            case 'A':
                menu_consulta_catalogo();
                break;

            // ========== Cotizar..
            case 'b':
                menu_cotizacion();
                break;

            case 'B':
                menu_cotizacion();

                break;

            // ========== Modificar cotización.
            case 'c' :
                
                break;
            case 'C':
                
                break;

            // ========== Facturar.
            case 'd':
                
                break;

            case 'D':
                break;      

            // ========== Salir del menu.
            case 's':
                printf("Saliendo de la seccion de opciones generales...\n");
                break;
            case 'S':
                printf("Saliendo de la seccion de opciones generales...\n");
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's');

    return;
}