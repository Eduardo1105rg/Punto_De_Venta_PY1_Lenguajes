#include <stdio.h>
#include <mysql/mysql.h>
#include <time.h>

#include "../include/familia.h"
#include "../include/producto.h"
#include "../include/cargaStock.h"
#include "../include/db.h"
#include "../include/menu.h"
#include "../include/consultaCatalogo.h"
#include "../include/consultaFacturas.h"
#include "../include/estadisticas.h"





// Comando para ejecutar desde c, se deben de compilar todos los archivos .c
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c src/producto.c
//./punto_de_venta


// Comando de compilacion actual con lo de la base de datos:
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c src/producto.c src/cargaStock.c src/db.c -lmysqlclient
//./punto_de_venta


// Anterio.
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c src/producto.c src/cargaStock.c src/db.c src/menu.c src/administrativas.c src/generales.c src/consultaCatalogo.c src/cotizacion.c  -lmysqlclient


// Actual.
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c src/producto.c src/cargaStock.c src/db.c src/menu.c src/administrativas.c src/generales.c src/consultaCatalogo.c src/cotizacion.c src/estadisticas.c src/facturacion.c src/consultaFacturas.c  -lmysqlclient


//Final finalito
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c src/producto.c src/cargaStock.c src/db.c src/menu.c src/administrativas.c src/generales.c src/consultaCatalogo.c src/cotizacion.c src/estadisticas.c src/facturacion.c src/consultaFacturas.c -lmysqlclient -lcrypto

int main() {

    MYSQL *conexion = NULL;

    //printf("Aqui\n");
    if (conectar(&conexion) != 0) {
        
        return 1; 
    }
    //printf("Se logro conectar.\n");

    // Lista de familias.
    // NodoFamilia *listaDeFamilias = NULL;

    // // // Primero se cargan los datos desde el archivo y se almacenan en la lista.
    // cargarFamiliasDesdeArchivo("data/familias.txt", &listaDeFamilias);

    // //Imprimimos los datos guardados en la lista.
    //  imprimirListaNodosFamilia(listaDeFamilias);
  
    //  guardarFamiliasEnDB(conexion, listaDeFamilias);

    // // // Los intentamos almacenar en la base de datos.
    // guardarFamiliasEnDB(conexion,listaDeFamilias);

    // // // Liberamos la memoria asignada a la lista.
    // liberarListaFamilia(listaDeFamilias);

    // // Lista de productos.
    // NodoProducto *listaProductos = NULL;

    // // // Primero se cargan los datos desde el archivo y se almacenan en la lista.
    // cargarProductosDesdeArchivo("data/producto.txt", &listaProductos);

    // // //Imprimimos los datos guardados en la lista.
    //  imprimirListaNodosProducto(listaProductos);

    // // // Los intentamos almacenar en la base de datos.
    //  guardarProductosEnDB(conexion,listaProductos);

    // // // Liberamos la memoria asignada a la lista.
    // liberarListaProducto(listaProductos);



    // // Cargar estock de productos desde txt
    //   NodoCargaStock *lista_carga_stock = NULL;

    //   cargarStockDesdeArchivo("data/stock.txt", &lista_carga_stock);

    //   imprimirListaNodosCargaStock(lista_carga_stock);

    //   guardarStockEnDB(conexion,lista_carga_stock);

    //   liberarListaCargaStock(lista_carga_stock);



    // Para lo de ver el inventario...

    //consultarCatalogo(conexion, 2);

    // consultarCatalogoPorFamilia(conexion, "Enlatados");


    // Pruebas del menu..
    menu_principal();

    //mostrar_detalles_factura(conexion, 0);

    //eliminarProducto(conexion, "Prod1");


    // ============= Estadisticas del programa...
    //SextaEstadistica(conexion);

    return 0;
}