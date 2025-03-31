#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

// Include de archivos del programa.
#include "../include/administrativas.h"
#include "../include/familia.h"
#include "../include/producto.h"
#include "../include/cargaStock.h"
#include "../include/db.h"
#include "../include/manipularArchivos.h"
#include "../include/estadisticas.h"
#include "../include/consultaFacturas.h"


// opcion != 's' && opcion != 'S'
//         leerPrimerCaracter(&opcion);



/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void incluir_familia_de_productos() {

    MYSQL *conexion = NULL;

    if (conectar(&conexion) != 0) {
        return; 
    }

    // Lista de familias.
    NodoFamilia *listaDeFamilias = NULL;

    // Solicitar la ruta y el nombre del archivo que se quiere usar.
    char *rutasArchivo;
    printf("\nIngresa la ruta y el nombre del archivo a usar, solo se pueden usar archivos .txt (E.J: data/familias.txt): ");
    leerCaracteresDeFormadinamica(&rutasArchivo);
    printf("\n");

    // Aqui se harian validaciones para lo del archivo.

    // Primero se cargan los datos desde el archivo y se almacenan en la lista.
    int estado = cargarFamiliasDesdeArchivo(rutasArchivo, &listaDeFamilias);

    if (estado == 1) {
        printf("\n Error en el archivo, por favor verifique la ruta y el nombre del archivo ingresado antes de continuar. \n");
        return;
    }

    //Imprimimos los datos guardados en la lista.
    //imprimirListaNodosFamilia(listaDeFamilias);
  
    // Los intentamos almacenar en la base de datos.
    guardarFamiliasEnDB(conexion,listaDeFamilias);

    // Liberamos la memoria asignada a la lista.
    liberarListaFamilia(listaDeFamilias);

    
    printf("\n Fin del proceso de cargas de datos desde archivos, volvinedo al menu de la seccion.\n");
    return;
}


/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void menu_agregar_eliminar_productos() {
    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    char opcion;
    do {
        printf("\nRegistro de productos... \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Registrar nuevo lote. \n");
        printf(">> B) Eliminar producto.\n");
        printf(">> S) Volver al menu de opciones generales.\n");


        printf("Ingrese la letra de la consulta a realizar.: ");
        scanf(" %c", &opcion); 
        getchar(); 

        switch (opcion) {
            // ========== Agregar lote de productos.
            case 'a' :
                char *rutasArchivo1;
                printf("\nIngresa la ruta y el nombre del archivo a usar, solo se pueden usar archivos .txt (E.J: data/familias.txt): ");
                leerCaracteresDeFormadinamica(&rutasArchivo1);
                printf("\n");
                
                NodoProducto *listaProductos1 = NULL;

                // Primero se cargan los datos desde el archivo y se almacenan en la lista.
                int estado1 = cargarProductosDesdeArchivo(rutasArchivo1, &listaProductos1);
            
                if (estado1 == 1) {
                    printf("\n Error en el archivo, por favor verifique la ruta y el nombre del archivo ingresado antes de continuar. \n");
                    return;
                }
                //Imprimimos los datos guardados en la lista.
                //imprimirListaNodosProducto(listaProductos1);
            
                // Los intentamos almacenar en la base de datos.
                guardarProductosEnDB(conexion, listaProductos1);
            
                // Liberamos la memoria asignada a la lista.
                liberarListaProducto(listaProductos1);

                printf("\n -- Fin del proceso de cargas de datos desde archivos, volvinedo al menu de la seccion.\n");

                break;
            case 'A':
                char *rutasArchivo2;
                printf("\nIngresa la ruta y el nombre del archivo a usar, solo se pueden usar archivos .txt (E.J: data/familias.txt): ");
                leerCaracteresDeFormadinamica(&rutasArchivo1);
                printf("\n");

                NodoProducto *listaProductos2 = NULL;

                // Primero se cargan los datos desde el archivo y se almacenan en la lista.
                int estado2 = cargarProductosDesdeArchivo(rutasArchivo2, &listaProductos2);
                if (estado2 == 1) {
                    printf("\n Error en el archivo, por favor verifique la ruta y el nombre del archivo ingresado antes de continuar. \n");
                    return;
                }
                //Imprimimos los datos guardados en la lista.
                //imprimirListaNodosProducto(listaProductos2);
            
                // Los intentamos almacenar en la base de datos.
                guardarProductosEnDB(conexion, listaProductos2);
            
                // Liberamos la memoria asignada a la lista.
                liberarListaProducto(listaProductos2);

                printf("\n -- Fin del proceso de cargas de datos desde archivos, volvinedo al menu de la seccion.\n");

                break;

            // ========== Eliminar producto..
            case 'b':
                 
                // Solicitar 
                char *id_producto1;
                printf("\nIngresa el id del producto a eliminar: ");
                leerCaracteresDeFormadinamica(&id_producto1);
                printf("\n");
                eliminarProducto(conexion, id_producto1);
                free(id_producto1);
                break;

            case 'B':

                char *id_producto2;
                printf("\nIngresa el id del producto a eliminar: ");
                leerCaracteresDeFormadinamica(&id_producto2);
                printf("\n");
                eliminarProducto(conexion, id_producto2);
                free(id_producto2);

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

    cerrarConexion(conexion);
    
    return;
}


/**
 * Nombre:
 * 
 * Descripcion:
 * 
 * Entradas:
 * 
 * Salidas:
 * 
 */
void menu_modificar_stock() {

    MYSQL *conexion = NULL;

    if (conectar(&conexion) != 0) {
        return; 
    }

    // Lista de familias.
    NodoFamilia *listaDeFamilias = NULL;

    // Solicitar la ruta y el nombre del archivo que se quiere usar.
    char *rutasArchivo;
    printf("\nIngresa la ruta y el nombre del archivo a usar, solo se pueden usar archivos .txt (E.J: data/stock.txt): ");
    leerCaracteresDeFormadinamica(&rutasArchivo);
    printf("\n");
    
    // Aqui va la validacion y el porceso de guardar los datos en la base de datos.

    NodoCargaStock *lista_carga_stock = NULL;

    int estado = cargarStockDesdeArchivo("data/stock.txt", &lista_carga_stock);
    if (estado == 1) {
        printf("\n Error en el archivo, por favor verifique la ruta y el nombre del archivo ingresado antes de continuar. \n");
        return;
    }
    //  imprimirListaNodosCargaStock(lista_carga_stock);

    //  guardarStockEnDB(conexion,lista_carga_stock);

    liberarListaCargaStock(lista_carga_stock);


    // Cerrar la conexion con la base de datos.
    cerrarConexion(conexion);
    printf("\n -- Fin del proceso de cargas de datos desde archivos, volvinedo al menu de la seccion.\n");

    return;
}

/**
 * Nombre: menu_principal_consulta_facturas
 * 
 * Descripcion: Funcion que actua como menu de la seccion de consulta de facturas, consta de un do while para ir mostrando las opciones y reccionando a lo que deses el usaurio.
 * 
 * Entradas: No posee.
 * 
 * Salidas: No posee.
 * 
 */
void menu_principal_consulta_facturas() {
    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    char opcion;
    do {
        printf("\nRegistro de productos... \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Ver facturas realizadas. \n");
        printf(">> B) Ver detalle de una factura.\n");
        printf(">> S) Volver al menu de opciones generales.\n");


        printf("Ingrese la letra de la consulta a realizar.: ");
        // scanf(" %c", &opcion); 
        // getchar(); 
        leerPrimerCaracter(&opcion);

        switch (opcion) {
            // ========== Ver facturas realizadas.
            case 'a' :
                mostrar_facturas(conexion);
                break;
            case 'A':
                mostrar_facturas(conexion);
                break;

            // ==========  Ver detalle de una factura.
            case 'b':
                 
                // Solicitar 
                
                printf("\nIngresa el id de la factura a consultar: ");
                int id_factura1 = leeNumeroDinamicoV2();

                if (id_factura1 == -21) {
                    break;
                }

                mostrar_detalles_factura(conexion, id_factura1);
                break;

            case 'B':

                printf("\nIngresa el id de la factura a consultar: ");
                int id_factura2 = leeNumeroDinamicoV2();

                if (id_factura2 == -21) {
                    break;
                }

                mostrar_detalles_factura(conexion, id_factura2);
                break;

      

            // ========== Salir del menu.
            case 's':
                printf("Saliendo de la seccion de consulta de facturas...\n");
                break;
            case 'S':
                printf("Saliendo de la seccion de consulta de facturas...\n");
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's' && opcion != 'S');

    cerrarConexion(conexion);
    
    return;

}

/**
 * Nombre: menu_principal_estadisticas
 * 
 * Descripcion: Funcion que actua como menu de la seccion de estadisticas, consta de un do while para ir mostrando las opciones y reccionando a lo que deses el usaurio.
 * 
 * Entradas: No posee.
 * 
 * Salidas: No posee.
 * 
 */
void menu_principal_estadisticas() {
    MYSQL *conexion = NULL;

    if (conectar(&conexion) != 0) {
        
        return; 
    }

    char opcion;
    do {
        printf("\nBienvenido al apartado de estadisticas. \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Cantidad de cotizaciones pendientes. \n");
        printf(">> B) Cantidad de cotizaciones facturadas.\n");
        printf(">> C) Promedio de compra (promedio de total).\n");
        printf(">> D) Top 5 de productos más vendidos.\n");
        printf(">> E) Producto más vendido por familia .\n");
        printf(">> F) Monto vendido por familia.\n");

        printf(">> S) Volver al menu principal.\n");

        printf("Ingrese la letra de las seccion a la que desea ingresar: ");
        // scanf(" %c", &opcion); 
        // getchar(); 
        leerPrimerCaracter(&opcion);

        switch (opcion) {
            // ========== Cantidad de cotizaciones pendientes.
            case 'a' :
                PrimerEstadistica(conexion);
                break;
            case 'A':
                PrimerEstadistica(conexion);

                
                break;

            // ========== Cantidad de cotizaciones facturadas.
            case 'b':
                SegundaEstadistica(conexion);
                break;

            case 'B':
                SegundaEstadistica(conexion);
                break;

            // ========== Promedio de compra (promedio de total).
            case 'c' :
                TerceraEstadistica(conexion);
                break;
            case 'C':
                TerceraEstadistica(conexion);
                
                break;

            // ========== Top 5 de productos más vendidos.
            case 'd':
                CuartaEstadistica(conexion);
                break;

            case 'D':
                break;
                
             // ========== Producto más vendido por familia.
             case 'e' :
                QuintaEstadistica(conexion);
             break;

            case 'E':
                QuintaEstadistica(conexion);
                
                break;

             // ========== Monto vendido por familia.
             case 'f' :
                SextaEstadistica(conexion);
                
             break;

            case 'F':
                SextaEstadistica(conexion);
                
                break;
             
            // ======== Salir de la seccion:
            case 's':
                printf("Saliendo de la seccion de estadisticas...\n");
                break;

            case 'S':
                printf("Saliendo de la seccion de estadisticas...\n");
                break;


            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's' && opcion != 'S');
    cerrarConexion(conexion);

    return;
}


/**
 * Nombre: menu_principal_administrativos
 * 
 * Descripcion: Funcion que actua como menu de la seccion de administrativos, consta de un do while para ir mostrando las opciones y reccionando a lo que deses el usaurio.
 * 
 * Entradas: No posee.
 * 
 * Salidas: No posee.
 * 
 */
void menu_principal_administrativos() {

    char opcion;
    do {
        printf("\nBienvenido al apartado de Opciones Administrativas. \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Registro de familias de productos. \n");
        printf(">> B) Registro de productos.\n");
        printf(">> C) Cargar inventario.\n");
        printf(">> D) Consulta de facturas.\n");
        printf(">> E) Estadísticas .\n");

        printf(">> S) Volver al menu principal.\n");

        printf("Ingrese la letra de las seccion a la que desea ingresar: ");
        // scanf(" %c", &opcion); 
        // getchar(); 
        leerPrimerCaracter(&opcion);


        switch (opcion) {
            // ========== Registro de familias de productos.
            case 'a' :
                incluir_familia_de_productos();
                break;
            case 'A':
                incluir_familia_de_productos();
                
                break;

            // ========== Registro de productos.
            case 'b':
                menu_agregar_eliminar_productos();
                break;

            case 'B':
                menu_agregar_eliminar_productos();

                break;

            // ========== Cargar inventario.
            case 'c' :
                menu_modificar_stock();
                break;
            case 'C':
                menu_modificar_stock();
                
                break;

            // ========== Consulta de facturas.
            case 'd':
                menu_principal_consulta_facturas();
                break;

            case 'D':
                menu_principal_consulta_facturas();
                break;
                
             // ========== Estadísticas.
             case 'e' :
                menu_principal_estadisticas();
             break;

            case 'E':
                menu_principal_estadisticas();
                break;

             
            // ======== Salir de la seccion:
            case 's':
                printf("Saliendo de la seccion de opciones administrativas...\n");
                break;

            case 'S':
                printf("Saliendo de la seccion de opciones administrativas...\n");
                break;


            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's' && opcion != 'S');

    return;
}

