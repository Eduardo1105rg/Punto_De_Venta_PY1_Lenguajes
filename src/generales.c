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


int cantidadCotizaciones = 1;
const char *estadoCotizacion = "Pago pendiente";

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

void imprimirListaCotizacionDetalle(NodoCotizacionDetalle *lista) {
    NodoCotizacionDetalle *actual = lista;
    while (actual != NULL) {
        printf("IdProducto: %s\n", actual->detallesCotizacion.IdProducto);
        printf("NombreProducto: %s\n", actual->detallesCotizacion.NombreProducto);
        printf("Descripcion: %s\n", actual->detallesCotizacion.Descripcion);
        printf("NumeroFila: %d\n", actual->detallesCotizacion.NumeroFila);
        printf("Precio: %.2f\n", actual->detallesCotizacion.precio);
        printf("Cantidad: %d\n", actual->detallesCotizacion.cantidad);
        printf("-------------------------\n");
        actual = actual->siguiente;
    }
}

void crearCotizacion(MYSQL *conexion) {
    char *consulta = NULL;
    int largoConsulta = asprintf(&consulta, "INSERT INTO Cotizacion(IdCotizacion, EstadoCotizacion) VALUES ('%d', '%s');",
        cantidadCotizaciones, estadoCotizacion);
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
}


void enviarCotizacionDB(MYSQL *conexion, NodoCotizacionDetalle *lista) {
    NodoCotizacionDetalle *actual = lista;
    char *consulta = NULL;
    while(actual != NULL) {
        const char *idProdu = actual->detallesCotizacion.IdProducto;
        int cantidadProductos = actual->detallesCotizacion.cantidad;
        float precioProducto = actual->detallesCotizacion.precio;
        int largoConsulta = asprintf(&consulta, "INSERT INTO CotizacionDetalle(IdCotizacion, IdProducto, Cantidad, PrecioXunidad) VALUES ('%d', '%s', '%d', '%f');",
            cantidadCotizaciones, idProdu, cantidadProductos, precioProducto);
        if (mysql_query(conexion,consulta)) {
            printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
            free(consulta);
            return;
        }
        free(consulta);
        actual = actual->siguiente;
    }
    
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
                //printf("Pass0");
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

                //char *id_producto_a_eliminar1;
                printf("\nIngresa el numero de la fila del producto que desea eliminar: ");
                // leerCaracteresDeFormadinamica(&id_producto_a_eliminar1);
                int num_fila_eliminar1 = leerNumeroDinamico();

                printf("\n");

                eliminarCotizacionPorNumFila(&lista_productos_en_cotizacion, num_fila_eliminar1);
                //eliminarPorIdCotizacionDetalle(&lista_productos_en_cotizacion, id_producto_a_eliminar1);

                //free(id_producto_a_eliminar1);

                break;
            case 'C':

                printf("\nIngresa el numero de la fila del producto que desea eliminar: ");
                int num_fila_eliminar2 = leerNumeroDinamico();
                printf("\n");

                eliminarCotizacionPorNumFila(&lista_productos_en_cotizacion, num_fila_eliminar2);

                break;

            // ========== Guardar la cotizacion realizada.
            case 'd':
                imprimirListaCotizacionDetalle(lista_productos_en_cotizacion);
                crearCotizacion(conexion);
                enviarCotizacionDB(conexion,lista_productos_en_cotizacion);
                printf("Cotización creada, tu ID de cotizacion es: %d", cantidadCotizaciones);  
                break;
                
            case 'D':
                imprimirListaCotizacionDetalle(lista_productos_en_cotizacion);
                crearCotizacion(conexion);
                enviarCotizacionDB(conexion,lista_productos_en_cotizacion);
                printf("Cotización creada, tu ID de cotizacion es: %d", cantidadCotizaciones);  
                break;

            // ========== Salir del menu.
            case 's': 
                char op1;
                printf("Advertencia: Si sales en este momento no se guardara la cotización si no la haz guardado\n");
                printf("Si desea salir vuelva a escribir S\n");
                scanf("%c",&op1);
                if(op1 == 'S' || op1 == 's'){
                    printf("Volviendo a la seccion anterior...\n");
                    return;
                }
                break;
            case 'S':
            char op2;
            printf("Advertencia: Si sales en este momento no se guardara la cotización si no la haz guardado\n");
            printf("Si desea salir vuelva a escribir S\n");
            scanf("%c",&op2);
            if(op2 == 'S' || op2 == 's'){
                printf("Volviendo a la seccion anterior...\n");
                return;
            }
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
    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

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
                printf("Por favor escriba el identificador de la cotizacion a modificar:\n");
                int modifica = 0;
                scanf("%d", &modifica);
                mostrar_cotizacionID(conexion,modifica);
                printf("Si desea agregar productos escriba 1 si quiere eliminar 2");
                int modifica2 = 0;
                scanf("%d", &modifica2);
                if (modifica2 == 1) {
                    // podriamos mostrar catalogo
                    printf("Estas agregando productos, escribe el nombre del producto\n");
                    //agregar_nuevo_producto(conexion, &lista_productos_en_cotizacion, id_producto1, cantidad_producto1);


                } else {
                    printf("Estas eliminando productos, escriba el nombre del producto a eliminar\n");

                }

                //Ok necesito ver si a esa lista de productos como es que queda guardada
                //mostrar_cotizacion(lista_productos_en_cotizacion);
                
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