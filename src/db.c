#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/db.h"

#include <mysql/mysql.h>
#define HOST "172.22.112.1"
#define PORT 3307
#define USERNAME "root"
#define PASSWORD "root"
#define DATABASE "puntoVenta"


int conectar(MYSQL **conexion) {
    int error;

    *conexion = mysql_init(NULL);  // Inicializamos la estructura de conexión
    printf("hola");
    if (mysql_real_connect(*conexion, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL, 0) != NULL) {
        printf("Se estableció la conexión con la base de datos\n");
        error = 0;
    } else {
        printf("Error al conectarse a la base de datos: %s\n", mysql_error(*conexion));
        error = 1;
    }

    return error;
}

