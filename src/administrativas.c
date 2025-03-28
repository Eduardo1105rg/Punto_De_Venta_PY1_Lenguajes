#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

// Include de archivos del programa.
#include "../include/administrativas.h"
#include "../include/familia.h"
#include "../include/producto.h"
#include "../include/db.h"
#include "../include/manipularArchivos.h"
#include "../include/estadisticas.h"



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
    cargarFamiliasDesdeArchivo(rutasArchivo, &listaDeFamilias);

    //Imprimimos los datos guardados en la lista.
    imprimirListaNodosFamilia(listaDeFamilias);
  
    // Los intentamos almacenar en la base de datos.
    guardarFamiliasEnDB(conexion,listaDeFamilias);

    // Liberamos la memoria asignada a la lista.
    liberarListaFamilia(listaDeFamilias);

    return;
}

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
            // ========== Consulta de catalogo.
            case 'a' :
                char *rutasArchivo1;
                printf("\nIngresa la ruta y el nombre del archivo a usar, solo se pueden usar archivos .txt (E.J: data/familias.txt): ");
                leerCaracteresDeFormadinamica(&rutasArchivo1);
                printf("\n");
                
                NodoProducto *listaProductos1 = NULL;

                // Primero se cargan los datos desde el archivo y se almacenan en la lista.
                cargarProductosDesdeArchivo(rutasArchivo1, &listaProductos1);
            
                //Imprimimos los datos guardados en la lista.
                imprimirListaNodosProducto(listaProductos1);
            
                // Los intentamos almacenar en la base de datos.
                //guardarProductosEnDB(conexion,listaProductos);
            
                // Liberamos la memoria asignada a la lista.
                liberarListaProducto(listaProductos1);

                break;
            case 'A':
                char *rutasArchivo2;
                printf("\nIngresa la ruta y el nombre del archivo a usar, solo se pueden usar archivos .txt (E.J: data/familias.txt): ");
                leerCaracteresDeFormadinamica(&rutasArchivo1);
                printf("\n");
                
                NodoProducto *listaProductos2 = NULL;

                // Primero se cargan los datos desde el archivo y se almacenan en la lista.
                cargarProductosDesdeArchivo(rutasArchivo2, &listaProductos2);
            
                //Imprimimos los datos guardados en la lista.
                imprimirListaNodosProducto(listaProductos2);
            
                // Los intentamos almacenar en la base de datos.
                //guardarProductosEnDB(conexion,listaProductos);
            
                // Liberamos la memoria asignada a la lista.
                liberarListaProducto(listaProductos2);


                break;

            // ========== Consulta de catalogo por la descripcion de la familia.
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

void menu_modificar_stock() {

    

}


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
        scanf(" %c", &opcion); 
        getchar(); 

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
    } while (opcion != 's');
    cerrarConexion(conexion);

    return;
}


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
        scanf(" %c", &opcion); 
        getchar(); 

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
                
                break;
            case 'C':
                
                break;

            // ========== Consulta de facturas.
            case 'd':
                
                break;

            case 'D':
                break;
                
             // ========== Estadísticas.
             case 'e' :
                
             break;

            case 'E':
                
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
    } while (opcion != 's');

    return;
}

