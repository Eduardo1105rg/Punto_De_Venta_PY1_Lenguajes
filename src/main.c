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
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c src/producto.c src/cargaStock.c src/db.c src/menu.c src/administrativas.c src/generales.c src/consultaCatalogo.c src/cotizacion.c src/estadisticas.c src/facturacion.c src/consultaFacturas.c -lmysqlclient -lcrypto
//./punto_de_venta

int main() {
    // Pruebas del menu..
    menu_principal();


    return 0;
}