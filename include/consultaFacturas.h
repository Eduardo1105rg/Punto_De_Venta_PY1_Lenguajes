#ifndef CONSULTA_FACTURAS_H
#define CONSULTA_FACTURAS_H
#include <mysql/mysql.h>

#include "../include/consultaFacturas.h"


void mostrar_facturas(MYSQL *conexion);

void mostrar_detalles_factura(MYSQL *conexion, const int id_cotizacion);

#endif