#ifndef FACTURACION_H
#define FACTURACION_H

#include <mysql/mysql.h>

int crearFactura(MYSQL *conexion, int numCotizacion, char *nombreCliente, char *fechaHora);

#endif
