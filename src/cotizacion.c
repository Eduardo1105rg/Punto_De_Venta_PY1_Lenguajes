#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

//Podriamos hacer una variable aqui para manejar el ID

int CotizacionesCreadas = 0;


void AgregarCotizacion(MYSQL *conexion, char * idProducto, int cantidad, int precio) {
    const char *Estado = ("Pendiente de pago");

    if (idProducto == NULL) {
        printf("Error: Por favor ingrese datos distintos de nullo\n");
        return;
    }
    char *consulta;
    int largo = asprintf(&consulta, "insert into Cotizacion(IdCotizacion, EstadoCotizacion) values ('%d','%S')", CotizacionesCreadas,Estado);
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    CotizacionesCreadas++;

    char *consulta2;
    int largo2 = asprintf(&consulta2, "insert into CotizacionDetalle(IdCotizacion, IdProducto, Cantidad,PrecioXunidad) values
        ('%d','%s','%d','%f')", CotizacionesCreadas-1);

    if (mysql_query(conexion, consulta2)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta2);
        return;
        }
    
}