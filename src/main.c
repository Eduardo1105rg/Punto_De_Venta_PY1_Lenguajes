#include <stdio.h>
#include "../include/familia.h"
#include "../include/producto.h"
#include "../include/cargaStock.h"




// Comando para ejecutar desde c, se deben de compilar todos los archivos .c
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c src/producto.c
//./punto_de_venta

int main() {

    // Lista de familias.
    NodoFamilia *listaDeFamilias = NULL;

    // Primero se cargan los datos desde el archivo y se almacenan en la lista.
    cargarFamiliasDesdeArchivo("data/familias.txt", &listaDeFamilias);

    //Imprimimos los datos guardados en la lista.
    imprimirListaNodosFamilia(listaDeFamilias);

    // Los intentamos almacenar en la base de datos.
    //guardarFamiliasEnDB(listaDeFamilias);

    // Liberamos la memoria asignada a la lista.
    liberarListaFamilia(listaDeFamilias);

    // Lista de productos.
    NodoProducto *listaProductos = NULL;

    // Primero se cargan los datos desde el archivo y se almacenan en la lista.
    cargarProductosDesdeArchivo("data/producto.txt", &listaProductos);

    //Imprimimos los datos guardados en la lista.
    imprimirListaNodosProducto(listaProductos);

    // Los intentamos almacenar en la base de datos.
    //guardarProductosEnDB(listaProductos);

    // Liberamos la memoria asignada a la lista.
    liberarListaProducto(listaProductos);



    // Cargar estock de productos desde txt
    NodoCargaStock *lista_carga_stock = NULL;

    cargarStockDesdeArchivo("data/stock.txt", &lista_carga_stock);

    imprimirListaNodosCargaStock(lista_carga_stock);

    liberarListaCargaStock(lista_carga_stock);


    return 0;
}