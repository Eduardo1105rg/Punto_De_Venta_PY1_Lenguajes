#ifndef COTIZACION_H
#define COTIZACION_H

#include <mysql/mysql.h>


struct NodoCotizacionDetalle;

typedef struct CotizacionDetalle
{
    char *IdProducto;
    char *NombreProducto;
    char *Descripcion;  // Esto es de la familia, la descripcion de esta.
    int NumeroFila;
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

void eliminarCotizacionPorNumFila(NodoCotizacionDetalle** head, const int numFila);

void modificarValoresNodoCotizacionDetalle(NodoCotizacionDetalle** head, const char * id, const int cantidad);



// Apartados especificos..
void mostrar_cotizacion(NodoCotizacionDetalle* head);

void mostrar_cotizacionID(MYSQL *conexion, int id);

void agregar_nuevo_producto(MYSQL *conexion, NodoCotizacionDetalle** head, const char * idProducto, const int cantidad);

void optener_datos_cotizacion_por_id(MYSQL *conexion, NodoCotizacionDetalle** head, const int idCotizacion);

void eliminarFilaBD(MYSQL *conexion, char *IdProd, int idCot);
#endif