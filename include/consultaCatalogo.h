#ifndef CONSULTA_CATALOGO_H
#define CONSULTA_CATALOGO_H


#include <mysql/mysql.h>

void consultarCatalogo(MYSQL *conexion, int modo); 

void consultarCatalogoPorFamilia(MYSQL *conexion, const char* descripcion, int modo);

#endif