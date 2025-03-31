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

/**
 * Nombre: conectar
 * 
 * Descripcion: Esta funcion incia una instancia de MySQL, es decir se establce una conexion con la base de datos, esta funcion utiliza las credenciales definidas,
 * en las directivas preprocesador para usarlas posteriormente cuando se vaya a crear una nueva conexion.
 * 
 * Entradas: MYSQL **conexion: Un puntero a una instacia de MySQL.
 * 
 * Salidas: int error: Un valor que indica la falla o el exito en la coneccion con la base de datos; 0 (Conectado), -1 (No se conecto)
 * 
 */
int conectar(MYSQL **conexion) {
    int error;

    *conexion = mysql_init(NULL);  // Inicializamos la estructura de conexión
    //printf("hola");
    if (mysql_real_connect(*conexion, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL, 0) != NULL) {
        printf("Se estableció la conexión con la base de datos\n");
        error = 0;
    } else {
        printf("Error al conectarse a la base de datos: %s\n", mysql_error(*conexion));
        error = 1;
    }

    return error;
}


/**
 * Nombre: cerrarConexion
 * 
 * Descripcion: Una funcon encargada de cerrar la conexion con la base de datos.
 * 
 * Entradas: MYSQL *conexion: Un puntero a una instacia de MySQL.
 * 
 * Salidas: No posee.
 * 
 */
void cerrarConexion(MYSQL *conexion) {
    mysql_close(conexion);
    printf("Conexión cerrada.\n");
}

