#ifndef COTIZACION_H
#define COTIZACION_H

#include <mysql/mysql.h>


struct NodoCotizacionDetalle;

typedef struct CotizacionDetalle
{
    char *IdProducto;
    char *NombreProducto;
    char *Descripcion;  // Esto es de la familia, la descripcion de esta.
    float precio;
    int cantidad;

} CotizacionDestalle;


typedef struct NodoCotizacionDetalle {

    CotizacionDestalle detallesCotizacion;

    struct NodoCotizacionDetalle * siguiente;

} NodoCotizacionDetalle;


NodoCotizacionDetalle* crearNodoCotizacionDetalle(const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad);

void imprimirListaNodosCotizacionDetalle(NodoCotizacionDetalle* head);

void insertarElementoAlFinalCotizacionDetalle(NodoCotizacionDetalle** head, const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad);

void liberarListaCotizacionDetalle(NodoCotizacionDetalle* head);

void eliminarPorIdCotizacionDetalle(NodoCotizacionDetalle** head, char * id);



// Apartados especificos..
void mostrar_cotizacion(NodoCotizacionDetalle* head);

void agregar_nuevo_producto(MYSQL *conexion, NodoCotizacionDetalle** head, const char * idProducto, const int cantidad);

void buscar_cotizacion(MYSQL *conexion, NodoCotizacionDetalle** head, const char * idCotizacion);

#endif