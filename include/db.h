#ifndef DB_H
#define DB_H
#include <mysql/mysql.h>

int conectar(MYSQL **conexion);

void cerrarConexion(MYSQL *conexion);

#endif