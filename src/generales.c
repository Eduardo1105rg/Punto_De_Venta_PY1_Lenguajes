#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>


// Include de archivos del programa.
#include "../include/generales.h"
#include "../include/consultaCatalogo.h"
#include "../include/manipularArchivos.h"
#include "../include/db.h"
#include "../include/cotizacion.h"
#include "../include/facturacion.h"


int cantidadCotizaciones = 1;
// const char *estadoCotizacion = "Pago pendiente";


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
                mostrar_cotizacion(lista_productos_en_cotizacion);

                char *nombre_cliente1;
                printf("\nIngresa el nombre del cliente para la cotizacion: ");
                leerCaracteresDeFormadinamica(&nombre_cliente1);
                printf("\n");
                int idCotizacion;
                crearCotizacion(conexion, nombre_cliente1, &idCotizacion);
                enviarCotizacionDB(conexion,lista_productos_en_cotizacion,idCotizacion );
                //printf("Cotización creada, tu ID de cotizacion es: %d", cantidadCotizaciones);  
                break;

            case 'D':
                mostrar_cotizacion(lista_productos_en_cotizacion);

                char *nombre_cliente2;
                printf("\nIngresa el nombre del cliente para la cotizacion: ");
                leerCaracteresDeFormadinamica(&nombre_cliente2);
                printf("\n");
                int idCotizacion2;
                crearCotizacion(conexion, nombre_cliente2,&idCotizacion2);
                enviarCotizacionDB(conexion,lista_productos_en_cotizacion,idCotizacion2);
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
void menu_modificar_cotizacion() {

    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    NodoCotizacionDetalle *lista_productos_en_cotizacion = NULL;

    printf("Por favor escriba el identificador de la cotizacion a modificar:");
    int identificadorCotizacion = 0;
    scanf("%d", &identificadorCotizacion);
    printf("\n");
    optener_datos_cotizacion_por_id(conexion, &lista_productos_en_cotizacion, identificadorCotizacion);

    char opcion;
    do {
        printf("\nModificar cotizacion... \n");

        mostrar_cotizacion(lista_productos_en_cotizacion);


        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Ver catalogo de productos. \n");
        printf(">> B) Agregar productos.\n");
        printf(">> C) Eliminar producto.\n");
        printf(">> D) Guardar cambios.\n");
        printf(">> S) Volver al menu anterior (No se guardan los cambios).\n");


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
                int guarda = 1;



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

                free(id_producto2);

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

            // ========== Guardar la cotizacion creada.
            case 'd':
                if(guarda == 1) {
                    char * consulta3 = NULL;
                    NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                    while (actual != NULL) { 
                        char *IdProd = actual->detallesCotizacion.IdProducto;
                        int cantidadProdu = actual->detallesCotizacion.cantidad;
                        float precio = actual->detallesCotizacion.precio;

                        printf("%d",identificadorCotizacion);                    
                        printf("ID Producto: %s, Nombre Producto: %d\n", IdProd, identificadorCotizacion);
                        
                        int largoConsulta = asprintf(&consulta3, 
                            "INSERT INTO CotizacionDetalle (IdCotizacion, IdProducto,Cantidad,PrecioXunidad) VALUES (%d,'%s',%d,%f);", 
                            identificadorCotizacion, IdProd, cantidadProdu, precio);
                    
                        if (mysql_query(conexion, consulta3)) {
                            printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
                            free(consulta3);
                            return;
                        }
                        free(consulta3);                     
                        actual = actual->siguiente; 
                    }
                

                }else { 
                    NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                    while (actual != NULL) { 
                        char *IdProd = actual->detallesCotizacion.IdProducto;  
                        printf("%d",identificadorCotizacion);                    
                        printf("ID Producto: %s, Nombre Producto: %d\n", IdProd, identificadorCotizacion);
                        
                        eliminarFilaBD(conexion, IdProd, identificadorCotizacion);
                        
                        actual = actual->siguiente; 
                    }
                }
                break;

            case 'D':
            if(guarda == 1) {
                char * consulta3 = NULL;
                NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                while (actual != NULL) { 
                    char *IdProd = actual->detallesCotizacion.IdProducto;
                    int cantidadProdu = actual->detallesCotizacion.cantidad;
                    float precio = actual->detallesCotizacion.precio;                   
                    int largoConsulta = asprintf(&consulta3, 
                        "INSERT INTO CotizacionDetalle (IdCotizacion, IdProducto,Cantidad,PrecioXunidad) VALUES (%d,'%s',%d,%f);", 
                        identificadorCotizacion, IdProd, cantidadProdu, precio);
                
                    if (mysql_query(conexion, consulta3)) {
                        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
                        free(consulta3);
                        return;
                    }
                    free(consulta3);                    
                    actual = actual->siguiente; 
                }
            

            }else { 
                NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                while (actual != NULL) { 
                    char *IdProd = actual->detallesCotizacion.IdProducto;  
                    printf("%d",identificadorCotizacion);                    
                    printf("ID Producto: %s, Nombre Producto: %d\n", IdProd, identificadorCotizacion);
                    
                    eliminarFilaBD(conexion, IdProd, identificadorCotizacion);
                    
                    actual = actual->siguiente; 
                }
            }
                break;

            // ========== Salir del menu.
            case 's': 
                char op1;
                printf("Advertencia: Si sales en este momento no se guardaran los cambios en la cotizacion.\n");
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
void menu_crear_factura() {
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);

    //Funcion que habiamos hecho en el portafolio para obtener la fecha y hora actual
    char fechaHora[70];
    char *formato = "%Y-%m-%d %H:%M:%S";
    int escritos = strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);
    if (escritos != 0) {
        printf("Fecha y hora: %s", fechaHora);
    
    } else {
        printf("No se formateo bien la fecha");
    }

    char *nombreClienteF = NULL;
    int numCotizacionF = 0;
    MYSQL *conexion = NULL;

    if (conectar(&conexion) != 0) {
        return;
    }

    nombreClienteF = (char *)malloc(100 * sizeof(char));
    if (nombreClienteF == NULL) {
        printf("Error al asignar memoria.\n");
        return;
    }

    printf("Antes de darle su factura por favor escribanos su numero de cotizacion\n");
    printf("y si es muy amable el nombre de la persona que cotizo\n");
    scanf("%d", &numCotizacionF);
    getchar(); 
    scanf("%s", nombreClienteF);

    char *temp = realloc(nombreClienteF, (strlen(nombreClienteF) + 1) * sizeof(char));
    if (temp == NULL) {
        printf("Error al reasignar memoria para nombreClienteF.\n");
        free(nombreClienteF);
        return;
    }
    nombreClienteF = temp;


    char *consultaFC = NULL;
    int largoConsultaFC = asprintf(&consultaFC, "update Cotizacion set EstadoCotizacion = '%s' where IdCotizacion = '%d'", "Facturado",numCotizacionF);
    if (mysql_query(conexion, consultaFC)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaFC);
        return;
    }
    free(consultaFC);



    //Aqui recien creamos la factura antes de mostrarla
    int resID = crearFactura(conexion, numCotizacionF, nombreClienteF,fechaHora);
    printf("DEBUG: Valor de resID = %d\n", resID);



    char *consultaF = NULL;
    int largoConsultaF = asprintf(&consultaF, "select NumSecuencial, NombreLocal, CedulaJuridica, Telefono from Negocio");

    if (mysql_query(conexion, consultaF)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaF);
        return;
    }
    free(consultaF);

    MYSQL_RES *resultado2 = mysql_store_result(conexion);
    if (resultado2 == NULL) {
        printf("No se obtuvieron resultados: %s\n", mysql_error(conexion));
        return;
    }

    MYSQL_ROW fila;

    while ((fila = mysql_fetch_row(resultado2)) != NULL) {
        char *empresaNombre = fila[1];   
        char *cedulaJuridica = fila[2];  
        char *telefonoEmpresa = fila[3]; 

        printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
        printf("|                                       %-57s |\n", empresaNombre); 
        printf("| Fecha de emisión: %-77s |\n", fechaHora);
        printf("| Identificador: %-80d |\n", resID);
        printf("| Cédula jurídica: %-78s |\n", cedulaJuridica);
        printf("| Teléfono: %-85s |\n", telefonoEmpresa);
        printf("| Cliente: %-86s |\n", nombreClienteF);
        printf("|                                       %-57s |\n", "Productos"); 

        break;
    }

    char *consultaF2 = NULL;
    int largoConsultaF2 = asprintf(&consultaF2, "call facturaFin('%d', '%s')", numCotizacionF, nombreClienteF);

    if (mysql_query(conexion, consultaF2)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaF2);
        return;
    }
    free(consultaF2);

    MYSQL_RES *resultado3 = mysql_store_result(conexion);
    if (resultado3 == NULL) {
        printf("No se obtuvieron resultados: %s\n", mysql_error(conexion));
        return;
    }

    MYSQL_ROW fila2;
    while ((fila2 = mysql_fetch_row(resultado3)) != NULL) {
        char *Producto = fila2[0];   
        char *Cantidad = fila2[1];    
        char *PrecioXUnidad = fila2[2];
        char *subtotal = fila2[3];  
        char *Impuesto = fila2[4]; 
        char *Total = fila2[5];
        printf("| Producto: %-85s |\n", Producto);
        printf("| Cantidad: %-85s |\n", Cantidad);
        printf("| Precio X Unidad: %-78s |\n", PrecioXUnidad);
        printf("| subtotal: %-85s |\n", subtotal);
        printf("| Impuesto: %-85s |\n", Impuesto);
        printf("| Total: %-88s |\n", Total);
        printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
    }


    while (mysql_next_result(conexion) == 0) {
        MYSQL_RES *res = mysql_store_result(conexion);
        mysql_free_result(res);
    }

    char *consultaF3 = NULL;
    int largoConsultaF3 = asprintf(&consultaF3, "call facturaFinDinero('%d', '%s')", numCotizacionF, nombreClienteF);

    if (mysql_query(conexion, consultaF3)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaF3);
        return;
    }
    free(consultaF3);

    MYSQL_RES *resultado4 = mysql_store_result(conexion);
    if (resultado4 == NULL) {
        printf("No se obtuvieron resultados: %s\n", mysql_error(conexion));
        return;
    }

    MYSQL_ROW fila3;
    while ((fila3 = mysql_fetch_row(resultado4)) != NULL) {
        char *subototalF = fila3[0];   
        char *impuestoF = fila3[1];    
        char *TotalF = fila3[2];
        printf("|                                       %-57s |\n", "Final factura"); 
        printf("| Subtotal final: %-79s |\n", subototalF);
        printf("| Impuesto final: %-79s |\n", impuestoF);
        printf("| Total final: %-82s |\n", TotalF);
        printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
        break;
    }
    free(nombreClienteF);
    return;
}

// void menu_crear_factura() {

//     while (1) {
//         printf("\nIngresa el codigo de la cotizacion a facturar: ");
//         int cantidad_producto1 = leerNumeroDinamico();
//         printf("\n");


//         char *nombre_cliente;
//         printf("\nIngresa el nombre del cliente para la facturacion: ");
//         leerCaracteresDeFormadinamica(&nombre_cliente);
//         printf("\n");

//     }

// }


/* ============================ Menu principal de la seccion ==========================*/
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

                menu_modificar_cotizacion();
                break;

            case 'C':
                menu_modificar_cotizacion();
                break;

            // ========== Facturar.
            case 'd':
                menu_crear_factura();
                
                break;

            case 'D':
                menu_crear_factura();
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

